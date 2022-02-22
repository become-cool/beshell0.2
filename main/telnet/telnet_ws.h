#ifndef _TELNET_WS_H
#define _TELNET_WS_H

#include "quickjs-libc.h"
#include "lvgl.h"
#include "lv_conf.h"

void ws_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) ;
void ws_driver_input_read(lv_indev_drv_t *drv, lv_indev_data_t *data) ;

void be_telnet_ws_init() ;

void telnet_ws_output(uint8_t cmd, int pkgid, const char * data, size_t datalen) ;

#endif