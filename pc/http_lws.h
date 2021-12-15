#ifndef __H_HTTP_LWS__
#define __H_HTTP_LWS__

#include "quickjs-libc.h"
#include "lvgl.h"
#include "lv_conf.h"

void ws_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) ;
void ws_driver_input_read(lv_indev_drv_t *drv, lv_indev_data_t *data) ;

void be_module_httplws_init() ;
void be_module_httplws_loop(JSContext * ctx) ;

#endif