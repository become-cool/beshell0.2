#ifndef _MODULE_LVGL_H
#define _MODULE_LVGL_H

#include "quickjs-libc.h"

void init_lvgl() ;
void lvgl_loop(JSContext *ctx)  ;

void require_module_lvgl(JSContext *ctx)  ;

#endif