#ifndef _MODULE_LVGL_H
#define _MODULE_LVGL_H

#include "quickjs-libc.h"

void be_module_init_lvgl() ;
void be_module_lvgl_loop(JSContext *ctx)  ;

void be_module_lvgl_require(JSContext *ctx) ;

#endif