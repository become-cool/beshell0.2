#ifndef _VLGL_JS_DISPLAY
#define _VLGL_JS_DISPLAY

#include "lvgl.h"
#include "quickjs-libc.h"
#include <stdbool.h>

#define DISP_BUFF_LINES_POOR 5
#define DISP_BUFF_LINES_ENOUGH 20
// #define DISP_BUFF_LINES ( (getPsramTotal()>1024)? DISP_BUFF_LINES_ENOUGH: DISP_BUFF_LINES_POOR)
#define DISP_BUFF_LINES 15
#define DISP_BUFF_AUX_SIZE 16        // 附加数据，记录 area(x1,y1,x2,y2)，放在 DMA_BUFF 前面

typedef struct {
    uint8_t id ;
    void * spi_dev ;
    void * jsobj ; // JS_MKPTR(JS_TAG_OBJECT, <JSObject *>)
    void * buff1 ;
    void * buff2 ;
    lv_coord_t buff_lines ;

    bool enable_input_event ;
    JSContext * ctx ;
} disp_drv_spec_t ;



JSClassID lv_disp_js_class_id() ;

// bool be_lv_fake_indev(lv_indev_data_t *data) ;
lv_disp_t * find_disp_by_id(uint8_t id) ;
disp_drv_spec_t * find_disp_spec_by_id(uint8_t id) ;

disp_drv_spec_t * default_disp_drv_spec() ;


void be_lv_display_init() ;

void be_lv_display_require(JSContext *ctx, JSValue lvgl) ;

void be_lv_display_reset(JSContext * ctx) ;

JSValue js_lv_disp_wrapper(JSContext *ctx, lv_disp_t * disp) ;

#endif