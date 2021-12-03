#ifndef _MODULE_LVGL_WIDGETS_H
#define _MODULE_LVGL_WIDGETS_H

#include "quickjs-libc.h"
#include "lvgl.h"

JSClassID lv_obj_js_class_id() ;


JSValue js_lv_obj_wrapper(JSContext *ctx, lv_obj_t * cobj, JSClassID clzid) ;

void vlgl_js_widgets_init() ;
void require_vlgl_js_widgets(JSContext *ctx, JSValue lvgl) ;

#endif