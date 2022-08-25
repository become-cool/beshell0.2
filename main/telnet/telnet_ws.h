#ifndef _TELNET_WS_H
#define _TELNET_WS_H

#include "quickjs-libc.h"
#include "lvgl.h"
#include "lv_conf.h"
#include "mongoose.h"


bool telnet_ws_response(struct mg_connection *c, int ev, void *ev_data, void *fn_data) ;

void telnet_ws_set_repl_client(struct mg_connection * conn) ;
void telnet_ws_output(uint8_t cmd, int pkgid, const char * data, size_t datalen) ;

void ws_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) ;
// void ws_driver_input_read(lv_indev_drv_t *drv, lv_indev_data_t *data) ;

void be_telnet_ws_init() ;
void be_telnet_ws_require(JSContext * ctx, JSValue telnet) ;


#endif