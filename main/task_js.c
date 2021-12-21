#include "task_js.h"
#include <string.h>
#include <stdbool.h>
#include "utils.h"
#include "eventloop.h"

#ifndef SIMULATION
#include "telnet.h"
#include "sniffer.h"
#include "webtelnet.h"
#else
#include "repl.h"
#include "http_lws.h"
#endif

#include "module_events.h"
#include "module_fs.h"
#include "module_utils.h"
#ifndef SIMULATION
#include "module_wifi.h"
#include "module_gpio.h"
#include "module_serial.h"
#include "module_socks.h"
#include "module_http.h"
#include "module_mg.h"
#endif
#include "module_lvgl.h"
#include "module_process.h"

#ifndef SIMULATION
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <nvs_flash.h>
#include "esp_vfs_fat.h"
#endif


JSRuntime *rt;
JSContext *ctx;
uint8_t boot_level = 5 ;
bool requst_reset = false ;


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



static JSContext * JS_NewCustomContext(JSRuntime *rt)
{
    JS_SetModuleLoaderFunc(rt, NULL, js_module_loader, NULL);

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

    // js_init_module_std(ctx, "std") ;
    // js_init_module_os(ctx, "os") ;

    // global 对象
    JSValue global = JS_GetGlobalObject(ctx);
    JS_SetPropertyStr(ctx, global, "global", global);
    
    // beapi 对象
    JSValue beapi = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, global, "beapi", beapi);
    JS_FreeValue(ctx, global);

    be_module_fs_require(ctx) ;
    be_module_utils_require(ctx) ;
    be_module_events_require(ctx) ;
    be_module_process_require(ctx) ;  
#ifndef SIMULATION
    be_module_wifi_require(ctx) ;
    be_module_gpio_require(ctx) ;  
    be_module_telnet_require(ctx) ;
    be_module_serial_require(ctx) ;
    be_module_socks_require(ctx) ;
    require_module_http(ctx) ;
#else
    be_module_mg_require(ctx) ;
    be_module_repl_require(ctx) ;
#endif
    be_module_lvgl_require(ctx) ;

    return ctx;
}

#define InitScriptTpl "require('/etc/rc%d.d.js')"

void eval_rc_script(JSContext *ctx, const char * path) {
    char * fullpath = vfspath_to_fs(path) ;
    evalScript(ctx, fullpath) ;
    free(fullpath) ;
}

void quickjs_init() {
    if(rt!=NULL) {
        return ;
    }
    rt = JS_NewRuntime();
    js_std_set_worker_new_context_func(JS_NewCustomContext);
    js_std_init_handlers(rt);
    ctx = JS_NewCustomContext(rt);

    // base 函数
    eval_rc_script(ctx, "/lib/base/console.js") ;
    eval_rc_script(ctx, "/lib/base/require.js") ;

    // 0等级，不加载任何启动脚本，作为安全模式
    if(boot_level>0) { 
#ifndef SIMULATION    
        echof("init level: %d\n", boot_level) ;
#else
        printf("init level: %d\n", boot_level) ;
#endif
        char * initScriptCodeBuff = mallocf(InitScriptTpl, boot_level) ;
        EVAL_CODE(initScriptCodeBuff, ":init.d")
        free(initScriptCodeBuff) ;
    }

#ifndef SIMULATION
    // ready 包
    telnet_send_ready() ;
#endif
}

void quickjs_deinit() {
    js_std_free_handlers(rt);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    rt = NULL ;
    ctx = NULL ;
}

void task_js_main(){

    be_module_process_init() ;

#ifndef SIMULATION
    nvs_flash_init();

    be_module_fs_init() ;
    be_module_wifi_init() ;
    be_module_telweb_init() ;
#endif
    be_module_mg_init() ;
    be_module_init_lvgl() ;

#ifndef SIMULATION
    be_module_socks_init() ;
    be_module_telnet_init() ;
    be_module_sniffer_init() ;
    be_module_gpio_init() ;
    be_module_http_init() ;
#else
    be_module_repl_init() ;
    be_module_httplws_init() ;
#endif

    quickjs_init() ;

    while(1) {

        if(requst_reset) {

            be_module_eventloop_reset(ctx) ;
#ifndef SIMULATION
            be_module_telnet_reset(ctx) ;
            be_module_gpio_reset(ctx) ;
            be_module_serial_reset(ctx) ;
            be_module_socks_reset(ctx) ;
            be_module_http_reset(ctx) ;
            be_module_wifi_reset(ctx) ;
#else
            be_module_repl_reset(ctx) ;
#endif
            be_module_mg_reset(ctx) ;

            quickjs_deinit() ;
            quickjs_init() ;
            
            requst_reset = false ;
        }

        js_std_loop(ctx) ;
#ifndef SIMULATION
        be_module_telnet_loop(ctx) ;
        be_module_sniffer_loop() ;
        be_module_socks_udp_loop(ctx) ;
        be_module_gpio_loop(ctx) ;
#else
        be_module_repl_loop(ctx) ;
        // be_module_httplws_loop(ctx) ;
#endif
        be_module_mg_loop(ctx) ;
        be_module_lvgl_loop(ctx) ;
        be_module_eventloop_loop(ctx) ;

        js_std_loop(ctx) ;
#ifndef SIMULATION
        vTaskDelay(1) ;
#endif
    }
}
