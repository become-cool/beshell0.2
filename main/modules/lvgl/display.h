#ifndef _VLGL_JS_DISPLAY
#define _VLGL_JS_DISPLAY

#include "quickjs-libc.h"

void vlgl_js_display_init() ;

void require_vlgl_js_display(JSContext *ctx, JSValue lvgl) ;

#endif