#ifndef _MODULE_LVGL_H
#define _MODULE_LVGL_H

#include "quickjs-libc.h"
#include <stdbool.h>


extern int16_t indev_input_x ;
extern int16_t indev_input_y ;
extern bool indev_input_pressed ;

void be_module_lvgl_malloc_buffer() ;
void be_module_lvgl_init() ;
void be_module_lvgl_loop(JSContext *ctx)  ;

void be_module_lvgl_require(JSContext *ctx) ;

#endif