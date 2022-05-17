#ifndef _VLGL_JS_DISPLAY
#define _VLGL_JS_DISPLAY

#include "lvgl.h"
#include "quickjs-libc.h"
#include <stdbool.h>



typedef struct {
    void * spi_dev ;
    JSValue jsobj ;
    void * buff1 ;
    void * buff2 ;
} disp_drv_data_t ;


JSClassID lv_disp_js_class_id() ;

bool be_lv_fake_indev(lv_indev_data_t *data) ;

void be_lv_display_init() ;

void be_lv_display_require(JSContext *ctx, JSValue lvgl) ;

void be_lv_display_reset(JSContext * ctx) ;

#endif