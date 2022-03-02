#include "module_process.h"
#include "task_js.h"
#include "beshell.h"
#include "utils.h"
#include <string.h>

#include "telnet.h"

#ifndef SIMULATION
#include "esp32_perfmon.h"
#include "psram.h"

#include "esp_system.h"
#include "soc/efuse_reg.h"
#include "esp_heap_caps.h"
#include <sys/types.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

static int cpu0_idle_cnt = 0;
static int cpu1_idle_cnt = 0;
static void idle_task(int *parm)
{
    while(1==1)
    {
        int64_t now = esp_timer_get_time();     // time anchor
        vTaskDelay(0 / portTICK_RATE_MS);
        int64_t now2 = esp_timer_get_time();
        (*parm) += (now2 - now) / 1000;        // diff
    }
}
static void mon_task(int *parm)
{
    while(1==1)
    {
        // Note the trick of saving it on entry, so print time
        // is not added to our timing.
        
        // float new_cnt =  (float)(*parm);    // Save the count for printing it ...
        
        // // Compensate for the 100 ms delay artifact: 900 ms = 100%
        // float cpu_percent = ((99.9 / 90.) * new_cnt) / 10;
        // printf("%.0f%%  ", 100 - cpu_percent); fflush(stdout);
        (*parm) = 0;                        // Reset variable
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}


#endif

static JSValue js_process_cpu_usage(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0,cpu)

#ifndef SIMULATION

    // float new_cnt =  (float)idle_cnt;    // Save the count for printing it ...
    // // Compensate for the 100 ms delay artifact: 900 ms = 100%
    // float cpu_percent = ((99.9 / 90.) * new_cnt) / 10;
    // printf("%.0f%%\n", 100 - cpu_percent); fflush(stdout);


    if(cpu==0) {
        return JS_NewUint32(ctx, cpu0_usage()) ;
    }
    else if(cpu==1) {
        return JS_NewUint32(ctx, cpu1_usage()) ;
    }
    else {
        THROW_EXCEPTION("arv cpu core must be 0 or 1")
    }
#else
    return JS_NewUint32(ctx, 50) ;
#endif

}


JSValue js_process_reset(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    int level = -1 ;
    if(argc>0 && JS_IsNumber(argv[0])) {
        if(JS_ToInt32(ctx, &level, argv[0])!=0) {
            level = -1 ;
        }
    }
    char * script = NULL ;
    if(argc>1) {
        script = JS_ToCString(ctx, argv[1]) ;
    }

    task_reset(level, script) ;

    if(script) {
        JS_FreeCString(ctx, script) ;
    }
    return JS_UNDEFINED ;
}
JSValue js_process_reboot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
#ifndef SIMULATION
    esp_restart() ;
#endif
    return JS_UNDEFINED ;
}


#ifdef SIMULATION
JSValue js_process_exit(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    int code = 0 ;
    if(argc>1) {
        JS_ToInt32(ctx, &code, argv[0]) ;
    }

    exit(code) ;
    return JS_UNDEFINED ;
}
#endif

JSValue js_process_memory_usage(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    (void) argc ;
    (void) argv ;

    int heap_total = 0 ;
    int heap_used = 0 ;
    int psram_total = 0 ;
    int psram_used = 0 ;
    int dma_total = 0 ;
    int dma_used = 0 ;

#ifndef SIMULATION
    multi_heap_info_t info;

    heap_caps_get_info(&info, MALLOC_CAP_DMA);
    dma_total = info.total_free_bytes + info.total_allocated_bytes;
    dma_used = info.total_allocated_bytes;

    heap_caps_get_info(&info, MALLOC_CAP_INTERNAL);
    heap_total = info.total_free_bytes + info.total_allocated_bytes;
    heap_used = info.total_allocated_bytes;

    heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);
    psram_total = info.total_free_bytes + info.total_allocated_bytes;
    psram_used = info.total_allocated_bytes;
#endif

    JSValue obj = JS_NewObject(ctx) ;
    JS_SetPropertyStr(ctx, obj, "heap_total", JS_NewUint32(ctx, heap_total)) ;
    JS_SetPropertyStr(ctx, obj, "heap_used", JS_NewUint32(ctx, heap_used)) ;
    JS_SetPropertyStr(ctx, obj, "psram_total", JS_NewUint32(ctx, psram_total)) ;
    JS_SetPropertyStr(ctx, obj, "psram_used", JS_NewUint32(ctx, psram_used)) ;
    JS_SetPropertyStr(ctx, obj, "dma_total", JS_NewUint32(ctx, dma_total)) ;
    JS_SetPropertyStr(ctx, obj, "dma_used", JS_NewUint32(ctx, dma_used)) ;

    return obj ;
}


JSValue js_console_print(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    size_t len = 0 ;
    const char * str = JS_ToCStringLen(ctx, &len, argv[0]) ;
    if(!str) {
        THROW_EXCEPTION("not a avalid string.") 
    }
    if(len) {
        telnet_output(CMD_OUTPUT, mk_echo_pkgid() , str, len) ;
    }
    JS_FreeCString(ctx, str) ;
    return JS_UNDEFINED ;
}

// JSValue js_console_log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
//     // const char *str;

//     char * strlst[argc] ;
//     size_t lenlst[argc] ;
//     size_t totalLen = 0 ;
    
//     int i;
//     for(i = 0; i < argc; i++) {
//         strlst[i] = JS_ToCStringLen(ctx, &lenlst[i], argv[i]);
//         if (!strlst[i])
//             return JS_EXCEPTION;
//         totalLen+= lenlst[i] + 1 ; // +1 表示间隔空格,和最后\n
//     }
//     totalLen+=1 ; // ending 0字节

//     char * buff = malloc( totalLen ) ;
//     char * writer = buff ;
//     for(i = 0; i < argc; i++) {
//         memcpy(writer, strlst[i], lenlst[i]) ;
//         writer+= lenlst[i] ;
//         if(i<argc-1) {
//             memcpy(writer, " ", 1) ;
//         }
//         else {
//             memcpy(writer, "\n", 1) ;
//         }
//         writer++ ;
//     }
//     buff[totalLen-1] = 0 ;

//     telnet_output(CMD_OUTPUT, mk_echo_pkgid(), buff, totalLen-1) ;

//     for(i = 0; i < argc; i++) {
//         JS_FreeCString(ctx, strlst[i]);
//     }
//     free(buff) ;
    
//     return JS_UNDEFINED;
// }



void be_module_process_init() {
#ifndef SIMULATION
    perfmon_start() ;
    // xTaskCreate(idle_task, "idle_task", 1024 * 2, &cpu0_idle_cnt,  0, NULL);
    // xTaskCreate(mon_task, "mon_task", 1024 * 2, &cpu0_idle_cnt, 10, NULL);
#endif
}

void be_module_process_require(JSContext *ctx) {

    JSValue global = JS_GetGlobalObject(ctx);

    // process
    JSValue process = JS_NewObject(ctx) ;
    JS_SetPropertyStr(ctx, global, "process", process);

    JS_SetPropertyStr(ctx, process, "reset", JS_NewCFunction(ctx, js_process_reset, "reset", 1));
    JS_SetPropertyStr(ctx, process, "reboot", JS_NewCFunction(ctx, js_process_reboot, "reboot", 1));
    JS_SetPropertyStr(ctx, process, "cpuUsage", JS_NewCFunction(ctx, js_process_cpu_usage, "cpuUsage", 1));
    JS_SetPropertyStr(ctx, process, "memoryUsage", JS_NewCFunction(ctx, js_process_memory_usage, "memoryUsage", 1));
    // JS_SetPropertyStr(ctx, process, "global", global);

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
    JS_SetPropertyStr(ctx, process, "exit", JS_NewCFunction(ctx, js_process_exit, "exit", 1));
#endif
    JS_SetPropertyStr(ctx, versions, "quickjs", JS_NewString(ctx, QUICKJS_VERSION));
    JS_SetPropertyStr(ctx, process, "versions", versions);
    
    char buff[32] ;
    sprintf(buff, "%s %s", __DATE__, __TIME__) ;
    JS_SetPropertyStr(ctx, process, "build", JS_NewString(ctx, buff));

    // console
    JSValue console = JS_NewObject(ctx) ;
    JS_SetPropertyStr(ctx, console, "print", JS_NewCFunction(ctx, js_console_print, "print", 1));
    // JS_SetPropertyStr(ctx, console, "log", JS_NewCFunction(ctx, js_console_log, "log", 1));
    // JS_SetPropertyStr(ctx, console, "error", JS_NewCFunction(ctx, js_console_log, "error", 1));
    JS_SetPropertyStr(ctx, global, "console", console);

	JS_FreeValue(ctx, global);
}
