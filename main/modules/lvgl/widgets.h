#ifndef _MODULE_LVGL_WIDGETS_H
#define _MODULE_LVGL_WIDGETS_H

#include "quickjs-libc.h"
#include "lvgl.h"

JSClassID lv_obj_js_class_id() ;


JSValue js_lv_obj_wrapper(JSContext *ctx, lv_obj_t * cobj, JSClassID clzid) ;

void vlgl_js_widgets_init() ;
void require_vlgl_js_widgets(JSContext *ctx, JSValue lvgl) ;

bool lv_event_jsstr_to_code(JSContext *ctx, JSValue jsstr, lv_event_code_t* out) ;
JSValue lv_event_code_to_jsstr(JSContext *ctx, lv_event_code_t code) ;

bool lv_flex_flow_jsstr_to_code(JSContext *ctx, JSValue jsstr, lv_flex_flow_t* out) ;
JSValue lv_flex_flow_code_to_jsstr(JSContext *ctx, lv_flex_flow_t code) ;
    
bool lv_flex_align_jsstr_to_code(JSContext *ctx, JSValue jsstr, lv_flex_align_t* out) ;
JSValue lv_flex_align_code_to_jsstr(JSContext *ctx, lv_flex_align_t code) ;

#endif