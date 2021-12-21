#ifndef __H_STYLE__
#define __H_STYLE__

#include "lvgl.h"
#include "lv_conf.h"
#include "quickjs-libc.h"

extern lv_style_prop_t LV_STYLE_JSVALUE ;
extern JSClassID js_lv_style_class_id ;

JSValue lv_style_wrapper(JSContext * ctx, lv_style_t * style) ;

JSValue lv_style_value_to_js(JSContext * ctx, lv_style_prop_t prop, lv_style_value_t value) ;
bool lv_style_js_to_value(JSContext * ctx, lv_style_prop_t prop, JSValue jsval, lv_style_value_t * value) ;

void init_lvgl_style() ;

void require_vlgl_js_style(JSContext *ctx, JSValue lvgl) ;

#endif