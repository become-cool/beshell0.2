#include "module_serial_uart.h"
#include "utils.h"
#include <string.h>


void be_module_serial_uart_init() {
}

void be_module_serial_uart_require(JSContext *ctx, JSValue pkg) {

    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue serial_uart = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "serial_uart", serial_uart);

    // JS_SetPropertyStr(ctx, beapi, "xxxOoo", JS_NewCFunction(ctx, js_xxx_ooo, "xxxOoo", 1));


    JS_FreeValue(ctx, beapi);
}

void be_module_serial_uart_loop(JSContext *ctx) {
}

void be_module_serial_uart_reset(JSContext *ctx) {
}

