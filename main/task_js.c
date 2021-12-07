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
#endif
#include "module_lvgl.h"
#include "beshell.h"

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

JSValue js_process_reset(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(argc>0 && JS_IsNumber(argv[0])) {
        int level = 0 ;
        if(JS_ToInt32(ctx, &level, argv[0])>=0) {
            task_reset(level) ;
            return JS_UNDEFINED ;
        }
    }

    task_reset(-1) ;
    return JS_UNDEFINED ;
}
JSValue js_process_reboot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
#ifndef SIMULATION
    esp_restart() ;
#endif
    return JS_UNDEFINED ;
}


JSValue js_console_print(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    size_t len = 0 ;
    const char * str = JS_ToCStringLen(ctx, &len, argv[0]) ;
    if(!str) {
        THROW_EXCEPTION("not a avalid string.") 
    }
    if(len) {
#ifndef SIMULATION
        telnet_send_pkg(echo_pkgid(), CMD_OUTPUT, str, len) ;
#else
        printf(str) ;
        printf("\n") ;
        fflush(stdout) ;
#endif
    }
    JS_FreeCString(ctx, str) ;
    return JS_UNDEFINED ;
}

JSValue js_console_log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    // const char *str;

    char * strlst[argc] ;
    size_t lenlst[argc] ;
    size_t totalLen = 0 ;
    
    int i;
    for(i = 0; i < argc; i++) {
        strlst[i] = JS_ToCStringLen(ctx, &lenlst[i], argv[i]);
        if (!strlst[i])
            return JS_EXCEPTION;
        totalLen+= lenlst[i] + 1 ; // +1 表示间隔空格,和最后的0字节
    }

    char * buff = malloc( totalLen ) ;
    char * writer = buff ;
    for(i = 0; i < argc; i++) {
        memcpy(writer, strlst[i], lenlst[i]) ;
        writer+= lenlst[i] ;
        if(i<argc-1) {
            memcpy(writer, " ", 1) ;
            writer++ ;
        }
    }
    buff[totalLen-1] = 0 ;

#ifndef SIMULATION
    telnet_send_pkg(echo_pkgid(), CMD_OUTPUT, buff, totalLen-1) ;
#else
    printf(buff) ;
    printf("\n") ;
    fflush(stdout) ;
#endif

    for(i = 0; i < argc; i++) {
        JS_FreeCString(ctx, strlst[i]);
    }
    free(buff) ;
    
    return JS_UNDEFINED;
}

void require_module_process(JSContext *ctx) {

    JSValue global = JS_GetGlobalObject(ctx);

    // process
    JSValue process = JS_NewObject(ctx) ;
    JS_SetPropertyStr(ctx, global, "process", process);

    JS_SetPropertyStr(ctx, process, "reset", JS_NewCFunction(ctx, js_process_reset, "reset", 1));
    JS_SetPropertyStr(ctx, process, "reboot", JS_NewCFunction(ctx, js_process_reboot, "reboot", 1));

    JSValue env = JS_NewObject(ctx) ;
    JS_SetPropertyStr(ctx, env, "LOGNAME", JS_NewString(ctx, "become"));
    JS_SetPropertyStr(ctx, env, "HOME", JS_NewString(ctx, "/home/become"));
    JS_SetPropertyStr(ctx, env, "PWD", JS_NewString(ctx, "/home/become"));
    JS_SetPropertyStr(ctx, process, "env", env);
    
    JSValue versions = JS_NewObject(ctx) ;
    JS_SetPropertyStr(ctx, versions, "beshell", JS_NewString(ctx, BESHELL_VERSION));
#ifndef SIMULATION
    JS_SetPropertyStr(ctx, versions, "esp-idf", JS_NewString(ctx, ESPIDF_VERSION));
#else
    JS_SetPropertyStr(ctx, versions, "esp-idf", JS_NewString(ctx, "none"));
    JS_SetPropertyStr(ctx, process, "simulate", JS_NewBool(ctx, true));
#endif
    JS_SetPropertyStr(ctx, versions, "quickjs", JS_NewString(ctx, QUICKJS_VERSION));
    JS_SetPropertyStr(ctx, process, "versions", versions);
    
    char buff[32] ;
    sprintf(buff, "%s %s", __DATE__, __TIME__) ;
    JS_SetPropertyStr(ctx, process, "build", JS_NewString(ctx, buff));

    // console
    JSValue console = JS_NewObject(ctx) ;
    JS_SetPropertyStr(ctx, console, "print", JS_NewCFunction(ctx, js_console_print, "print", 1));
    JS_SetPropertyStr(ctx, console, "log", JS_NewCFunction(ctx, js_console_log, "log", 1));
    JS_SetPropertyStr(ctx, console, "error", JS_NewCFunction(ctx, js_console_log, "error", 1));
    JS_SetPropertyStr(ctx, global, "console", console);

	JS_FreeValue(ctx, global);
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

    require_module_fs(ctx) ;
    require_module_utils(ctx) ;
    require_module_events(ctx) ;
    require_module_process(ctx) ;  
#ifndef SIMULATION
    require_module_wifi(ctx) ;
    require_module_gpio(ctx) ;  
    require_module_telnet(ctx) ;
    require_module_serial(ctx) ;
    require_module_socks(ctx) ;
    require_module_http(ctx) ;
#endif
    require_module_lvgl(ctx) ;

    return ctx;
}

#define InitScriptTpl "require('/etc/rc%d.d.js')"

void eval_rc_script(JSContext *ctx, const char * path) {
    char * fullpath = vfspath_to_fs(path) ;
    evalScript(ctx, fullpath) ;
    free(fullpath) ;
}

void init_quickjs() {
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

void deinit_quickjs() {
    js_std_free_handlers(rt);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    rt = NULL ;
    ctx = NULL ;
}

void task_js_main(){



#ifndef SIMULATION
    nvs_flash_init();

    fs_init() ;
    wifi_init() ;
    webtelnet_init() ;
#endif
    init_lvgl() ;

#ifndef SIMULATION
    socks_init() ;
    telnet_init() ;
    sniffer_init() ;
    gpio_init() ;
    http_init() ;
#else
    repl_init() ;
    httplws_init() ;
#endif

    init_quickjs() ;

    while(1) {

        if(requst_reset) {

            eventloop_on_before_reset(ctx) ;
#ifndef SIMULATION
            telnet_on_before_reset(ctx) ;
            gpio_on_before_reset(ctx) ;
            serial_on_before_reset(ctx) ;
            socks_on_before_reset(ctx) ;
            http_on_before_reset(ctx) ;
            wifi_reset(ctx) ;
#endif

            deinit_quickjs() ;
            init_quickjs() ;
            
            requst_reset = false ;
        }

        js_std_loop(ctx) ;
#ifndef SIMULATION
        telnet_loop(ctx) ;
        sniffer_loop() ;
        socks_udp_loop(ctx) ;
        gpio_loop(ctx) ;
#else
        lvgl_loop(ctx) ;
        repl_loop(ctx) ;
        httplws_loop(ctx) ;
#endif
        eventloop_pump(ctx) ;

        js_std_loop(ctx) ;
#ifndef SIMULATION
        vTaskDelay(1) ;
#endif
    }
}
