#include "js_main_loop.h"
#include <string.h>
#include <stdbool.h>
#include "utils.h"
#include "eventloop.h"
#include <sys/stat.h>

#include "module_telnet.h"
#include "module_fs.h"
#include "module_utils.h"

#ifndef SIMULATION
#include "malloc_funcs.h"
#include "psram.h"
#include "sniffer.h"
#include "module_bt.h"
#include "module_wifi.h"
#include "module_gpio.h"
#include "module_serial.h"
#include "module_socks.h"
#include "module_gameplayer.h"
#include "module_media.h"
#include "module_driver.h"
#include "driver_camera.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <nvs_flash.h>
#include "esp_vfs_fat.h"

#else

#include "simulate.h"
#include "_module_gameplayer.h"
#endif

#include "rawfs.h"
#include "module_mg.h"
#include "module_lvgl.h"
#include "module_process.h"
#include "module_metadata.h"
#include "module_nvs.h"

JSRuntime *rt;
JSContext *ctx;
static uint8_t boot_level = 5 ;
static bool requst_reset = false ;
static bool nowifi = false ;

uint8_t task_boot_level() {
    return boot_level ;
}

void task_reset(int level) {
    if(level>-1 && level<256)
        boot_level = (uint8_t)level ;
    requst_reset = true ;
}

JSContext * task_current_context() {
    return ctx ;
}

#define InitScriptTpl "require('/etc/rc%d.d.js')"

static void eval_rc_script(JSContext *ctx, const char * path) {
    char * fullpath = vfspath_to_fs(path) ;

#ifndef SIMULATION
    char * binpath = mallocf("%s.bin", fullpath) ;
    struct stat statbuf;
    if(stat(binpath,&statbuf)<0) {
        evalScript(ctx, fullpath, false) ;
    }
    else {
        evalScript(ctx, binpath, true) ;
    }
    free(binpath) ;
#else
    evalScript(ctx, fullpath, false) ;
#endif

    free(fullpath) ;
}

static JSModuleDef * be_js_module_loader(JSContext *ctx, const char *module_name, void *opaque) {
    IS_NULL(opaque)
    return NULL ;
}

static JSContext * init_custom_context(JSRuntime *rt) {
    JS_SetModuleLoaderFunc(rt, NULL, be_js_module_loader, NULL);
    JSContext *ctx;
    ctx = JS_NewContextRaw(rt);
    if (!ctx)
        return NULL;

    JS_AddIntrinsicBaseObjects(ctx);
    JS_AddIntrinsicDate(ctx);
    JS_AddIntrinsicEval(ctx);
    JS_AddIntrinsicStringNormalize(ctx);
    JS_AddIntrinsicRegExp(ctx);
    JS_AddIntrinsicJSON(ctx);
    JS_AddIntrinsicProxy(ctx);
    JS_AddIntrinsicMapSet(ctx);
    JS_AddIntrinsicTypedArrays(ctx);
    JS_AddIntrinsicPromise(ctx);
#ifdef CONFIG_BIGNUM
    JS_AddIntrinsicBigInt(ctx);
#endif

    // global 对象
    JSValue global = JS_GetGlobalObject(ctx);
    JS_SetPropertyStr(ctx, global, "global", global);
    
    // beapi 对象
    JSValue beapi = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, global, "beapi", beapi);
    JS_FreeValue(ctx, global);
    
    be_module_fs_require(ctx) ;
    be_module_utils_require(ctx) ;
    be_module_process_require(ctx) ;

    // base 函数
    eval_rc_script(ctx, "/lib/base/base.js") ;
    eval_rc_script(ctx, "/lib/base/console.js") ;
    eval_rc_script(ctx, "/lib/base/events.js") ;
    eval_rc_script(ctx, "/lib/base/require.js") ;

#ifndef SIMULATION

#ifdef BT_ENABLED
    be_module_bt_require(ctx) ;
#endif
    be_module_wifi_require(ctx) ;
    be_module_gpio_require(ctx) ;  
    // be_module_telnet_require(ctx) ;
    be_module_serial_require(ctx) ;
    be_module_socks_require(ctx) ;
    be_module_driver_require(ctx) ;
#endif
    be_module_gameplayer_require(ctx) ;
    be_module_media_require(ctx) ;
    be_module_mg_require(ctx) ;
    be_telnet_require(ctx) ;
    be_module_lvgl_require(ctx) ;
    module_metadata_require(ctx) ;
    be_module_nvs_require(ctx) ;

#ifdef SIMULATION
    be_simulate_require(ctx) ;
#endif

    return ctx;
}

#ifndef SIMULATION
static const JSMallocFunctions def_malloc_funcs = {
    js_def_malloc,
    js_def_free,
    js_def_realloc,
    malloc_usable_size,
};
#endif


#ifndef SIMULATION
#define HOLD_MEM(p, size)                               \
    p = heap_caps_malloc(size, MALLOC_CAP_DMA) ;        \
    vTaskDelay(1) ;                                     \
    if(!p) {                                            \
        echo_DMA("retain memory failed") ;              \
    }                                                   \
    else {                                              \
        printf("remain memory successed, %d\n", size) ; \
    }                                                   \

#define FREE_MEM(p) if(p) { free(p) ; p=NULL; }
#else
#define HOLD_MEM(p, size)
#define FREE_MEM(p) free(p)
#endif

static void quickjs_init() {
    if(rt!=NULL) {
        return ;
    }

#ifndef SIMULATION
    // esp32 平台优先使用 PSRAM内存
    if( getPsramTotal()>1024 ) {
        rt = JS_NewRuntime2(&def_malloc_funcs, NULL);
    }
    else {
        rt = JS_NewRuntime();
    }
#else
    rt = JS_NewRuntime();
#endif

    js_std_set_worker_new_context_func(init_custom_context);
    js_std_init_handlers(rt);
    ctx = init_custom_context(rt);
}

static void rc_init() {

    // 0等级，不加载任何启动脚本，作为安全模式
    if(boot_level>0) { 
        echof("init level: %d\n", boot_level) ;

        char * initScriptCodeBuff = mallocf(InitScriptTpl, boot_level) ;
        EVAL_CODE(initScriptCodeBuff, ":init.d")
        free(initScriptCodeBuff) ;
    }

    // ready 包
    telnet_send_ready() ;
}

static void quickjs_deinit() {

#ifndef SIMULATION
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);
    printf("quickjs_deinit()\n") ;
    dn2(info.total_free_bytes, info.total_allocated_bytes)
#endif

    js_std_free_handlers(rt);

    // JS_FreeLeaks(rt,ctx) ;
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    rt = NULL ;
    ctx = NULL ;

#ifndef SIMULATION
    heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);
    dn2(info.total_free_bytes, info.total_allocated_bytes)
#endif
}


void js_main_loop(const char * script){

    boot_level = 5 ;
    nvs_read_onetime("rst-lv", &boot_level) ;

    nvs_read_onetime("rst-nowifi", (uint8_t*)&nowifi) ;

// nowifi = true ;
    
#ifndef SIMULATION    

    // 在初始化前，先占用整块的 DMA 内存，在初始化完成后释放
    // 迫使各个模块使用 PSRAM
    // void * retain_mem1 = NULL ;
    // void * retain_mem2 = NULL ;
    // if(getPsramTotal()>1024) {
    //     echo_DMA("hold mem") ;
    //     HOLD_MEM(retain_mem1, 60*1024)
    //     // HOLD_MEM(retain_mem2, 50*1024)
    // }
    
    
    if(nowifi) {
        printf("disabled wifi by nvs setting: rst-nowifi\n") ;
    } else {
        be_module_wifi_init() ;
    }
    be_rawfs_mount("/fs") ;
    be_module_fs_init() ;
    vTaskDelay(1) ;
#else
    be_rawfs_mount(NULL) ;
#endif

    be_module_process_init() ;
    be_module_mg_init() ;
    be_module_lvgl_init() ;
    be_telnet_init() ;

#ifndef SIMULATION
    be_module_socks_init() ;
    be_module_sniffer_init() ;
    be_module_gpio_init() ;
    // be_module_http_init() ;
    be_module_driver_init() ;
    be_module_media_init() ;
#endif

    quickjs_init() ;

#ifndef SIMULATION
    // FREE_MEM(retain_mem1)
    // FREE_MEM(retain_mem2)
#endif

    rc_init() ;

    if(script) {
        evalScript(ctx, script, false) ;
    }

#ifndef SIMULATION
    echo_DMA("loop start") ;
#endif
    while(1) {

        if(requst_reset) {

            be_module_eventloop_reset(ctx) ;
            be_module_mg_reset(ctx) ;
            be_module_lvgl_reset(ctx) ;
            be_telnet_reset(ctx) ;
#ifndef SIMULATION
            // be_module_telnet_reset(ctx) ;
            be_module_gpio_reset(ctx) ;
            be_module_serial_reset(ctx) ;
            be_module_socks_reset(ctx) ;
            // be_module_http_reset(ctx) ;
            be_module_driver_reset(ctx) ;
            be_module_wifi_reset(ctx) ;
            be_module_media_reset(ctx) ;
#endif
            quickjs_deinit() ;
            quickjs_init() ;
            rc_init() ;

            requst_reset = false ;
        }

        js_std_loop(ctx) ;
#ifndef SIMULATION
        // be_module_telnet_loop(ctx) ;
        be_module_sniffer_loop() ;
        be_module_socks_udp_loop(ctx) ;
        be_module_gpio_loop(ctx) ;
#endif
        be_telnet_loop(ctx) ;
        be_module_mg_loop(ctx) ;
        be_module_lvgl_loop(ctx) ;
        be_module_eventloop_loop(ctx) ;

        js_std_loop(ctx) ;
#ifndef SIMULATION
        vTaskDelay(0) ;
#endif
    }
}
