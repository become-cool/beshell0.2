#ifndef _VLGL_JS_DISPLAY
#define _VLGL_JS_DISPLAY

#include "lvgl.h"
#include "quickjs-libc.h"
#include <stdbool.h>


#define DISP_BUFF_LINES 20
#define DISP_BUFF_AUX_SIZE 16        // 附加数据，记录 area(x1,y1,x2,y2)，放在 DMA_BUFF 前面

typedef struct {
    void * spi_dev ;
    void * jsobj ; // JS_MKPTR(JS_TAG_OBJECT, <JSObject *>)
    void * buff1 ;
    void * buff2 ;
} disp_drv_spec_t ;


JSClassID lv_disp_js_class_id() ;

bool be_lv_fake_indev(lv_indev_data_t *data) ;

void be_lv_display_init() ;

void be_lv_display_require(JSContext *ctx, JSValue lvgl) ;

void be_lv_display_reset(JSContext * ctx) ;

#endif