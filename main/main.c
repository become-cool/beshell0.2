/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#define CONFIG_LOG_DEFAULT_LEVEL ESP_LOG_VERBOSE

#include <stdio.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <sys/unistd.h>
#include <sys/select.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "esp_vfs_dev.h"
#include "driver/uart.h"
#include "esp_system.h"
#include <stdatomic.h>
#include <nvs_flash.h>
#include <esp_event.h>
#include <esp_event_loop.h>


#include "quickjs-libc.h"
#include "telnet.h"
#include "eventloop.h"
#include "module_wifi.h"
#include "module_fs.h"
#include "module_utils.h"
#include "module_gpio.h"


// #include "esp_vfs.h"
#include "esp_vfs_fat.h"
// #include "esp_system.h"



/*
 * Log a logo to the console.
 */
// static void showLogo() {
// 	// Print a console logo.
// 	printf(
		
// 		"\n"
// 		" ____      ____                                     ____                       \n"
// 		"|    ~.   |                       ..''''|         ||           |       |       \n"
// 		"|____.'_  |______              .''      |_________||______     |       |       \n"
// 		"|       ~.|                 ..'         |         ||           |       |       \n"
// 		"|_______.'|___________....''            |         ||___________|_______|_______\n"
// 		"https://become.cool  (c) 2020"
// 		"\n"
// 		"version BeShell: " BESHELL_VERSION "; Duktape: " STR2(DUK_VERSION) "\n"
// 		"build: " __DATE__ " " __TIME__ "\n"
// 		"BeShell is ready\n"
// 		"\n"
// 	);
// } // showLogo



// JSModuleDef * JSModuleLoaderFunc(JSContext *ctx, const char *module_name, void *opaque){

// }

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

    require_module_utils(ctx) ;
    require_module_fs(ctx) ;
    require_module_wifi(ctx) ;
    require_module_gpio(ctx) ;

    return ctx;
}


void app_main(void)
{
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    nvs_flash_init();

    fs_init() ;
    wifi_init() ;
    telnet_init() ;

    JSRuntime *rt;
    JSContext *ctx;
    rt = JS_NewRuntime();
    js_std_set_worker_new_context_func(JS_NewCustomContext);
    js_std_init_handlers(rt);
    ctx = JS_NewCustomContext(rt);

    while(1) {
        js_std_loop(ctx) ;
        telnet_loop(ctx) ;
        eventloop_punp(ctx) ;

        vTaskDelay(1) ;
    }
}






