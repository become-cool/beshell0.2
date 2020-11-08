#include "task_js.h"
#include "utils.h"
#include "telnet.h"
#include "sniffer.h"
#include "eventloop.h"
#include "module_wifi.h"
#include "module_fs.h"
#include "module_utils.h"
#include "module_gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <nvs_flash.h>

#include "esp_vfs_fat.h"
#include "beshell.h"


JSRuntime *rt;
JSContext *ctx;
int boot_level = 5 ;
bool requst_reset = false ;

JSContext * task_current_context() {
    return ctx ;
}

JSValue js_process_reset(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(argc>0 && JS_IsNumber(argv[0])) {
        int level = 0;
        if(JS_ToInt32(ctx, &level, argv[0])>=0) {
            if(level>=0 && level<1000) {
                boot_level = level ;
            }
        }
    }
    requst_reset = true ;
    return JS_UNDEFINED ;
}
JSValue js_process_reboot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    esp_restart() ;
    return JS_UNDEFINED ;
}

void require_module_process(JSContext *ctx) {
    
    JSValue global = JS_GetGlobalObject(ctx);
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
    JS_SetPropertyStr(ctx, versions, "quickjs", JS_NewString(ctx, QUICKJS_VERSION));
    JS_SetPropertyStr(ctx, process, "versions", versions);
    
    char buff[32] ;
    sprintf(buff, "%s %s", __DATE__, __TIME__) ;
    JS_SetPropertyStr(ctx, process, "build", JS_NewString(ctx, buff));

	JS_FreeValue(ctx, global);
}

static JSContext *JS_NewCustomContext(JSRuntime *rt)
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
    // for `console.log`
    js_std_add_helpers(ctx, 0, NULL);

    require_module_fs(ctx) ;
    require_module_utils(ctx) ;
    require_module_wifi(ctx) ;
    require_module_gpio(ctx) ;
    require_module_process(ctx) ;

    return ctx;
}

#define InitScriptTpl "require('/etc/rc%d.d.js')"

void init_quickjs() {
    if(rt!=NULL) {
        return ;
    }
    rt = JS_NewRuntime();
    js_std_set_worker_new_context_func(JS_NewCustomContext);
    js_std_init_handlers(rt);
    ctx = JS_NewCustomContext(rt);

    // require 函数
    evalScript(ctx, "/fs/lib/base/require.js") ;

    // 0等级，不加载任何启动脚本，作为安全模式
    if(boot_level>0) {  
        printf("init level: %d\n", boot_level) ;
        char initScriptCodeBuff[ sizeof(InitScriptTpl) + 1 ] ;   // %d -> 999
        sprintf(initScriptCodeBuff, InitScriptTpl, boot_level) ;
        EVAL_STR(initScriptCodeBuff, ":init.d")
    }
}

void deinit_quickjs() {
    js_std_free_handlers(rt);
    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
    rt = NULL ;
    ctx = NULL ;
}

void task_js_main(){

    nvs_flash_init();

    fs_init() ;
    wifi_init() ;
    telnet_init() ;
    sniffer_init() ;

    init_quickjs() ;

    // printf("xxxxxxxxxx\n") ;
    // JSValue n = JS_NewInt32(ctx, 100) ;
    // JSValue s = JS_NewString(ctx, "xxxxxxxx") ;
    // JSValue o = JS_NewObject(ctx) ;
    // printf("number has ref? %d\n", JS_VALUE_HAS_REF_COUNT(n)) ;
    // printf("string has ref? %d\n", JS_VALUE_HAS_REF_COUNT(s)) ;
    // printf("object has ref? %d\n", JS_VALUE_HAS_REF_COUNT(o)) ;
    // printf("o ref: %d\n", VAR_REFCNT(o)) ;
    // JS_FreeValue(ctx, o) ;
    // printf("o ref: %d\n", VAR_REFCNT(o)) ;

    while(1) {

        if(requst_reset) {

            telnet_on_before_reset(ctx) ;
            eventloop_on_before_reset(ctx) ;

            deinit_quickjs() ;
            init_quickjs() ;
            
            requst_reset = false ;
        }

        js_std_loop(ctx) ;
        telnet_loop(ctx) ;
        sniffer_loop() ;
        eventloop_punp(ctx) ;

        js_std_loop(ctx) ;
        
        vTaskDelay(1) ;
    }
}
