#ifndef _MODULE_LVGL_H
#define _MODULE_LVGL_H

#include "quickjs-libc.h"
#include <stdbool.h>
#include "lvgl.h"


extern int16_t indev_input_x ;
extern int16_t indev_input_y ;
extern bool indev_input_pressed ;

void be_lv_pause() ;
void be_lv_resume() ;

void be_lv_disp_inv(lv_disp_t* disp) ;

void be_module_lvgl_malloc_buffer() ;
void be_module_lvgl_init() ;
void be_module_lvgl_loop(JSContext *ctx)  ;

void be_module_lvgl_require(JSContext *ctx) ;
void be_module_lvgl_reset(JSContext *ctx) ;

#endif