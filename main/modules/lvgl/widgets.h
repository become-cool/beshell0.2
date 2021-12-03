#ifndef _MODULE_LVGL_WIDGETS_H
#define _MODULE_LVGL_WIDGETS_H

#include "quickjs-libc.h"
#include "lvgl.h"

JSClassID lv_obj_js_class_id() ;


JSValue js_lv_obj_wrapper(JSContext *ctx, lv_obj_t * cobj, JSClassID clzid) ;

void vlgl_js_widgets_init() ;
void require_vlgl_js_widgets(JSContext *ctx, JSValue lvgl) ;

JSValue lv_event_code_to_jsstr(JSContext *ctx, lv_event_code_t code) ;
lv_event_code_t lv_event_jsstr_to_code(JSContext *ctx, JSValue jsstr) ;

#endif