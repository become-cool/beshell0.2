#ifndef __H_WIDGETS_EXTRA__
#define __H_WIDGETS_EXTRA__

#include "widgets.h"


void js_lv_obj_init(JSContext *ctx, JSValue jsobj) ;

JSValue js_lv_obj_wrapper(JSContext *ctx, lv_obj_t * cobj, JSClassID clsid) ;

JSValue js_lv_obj_enable_event(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
JSValue js_lv_obj_disable_event(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_is_screen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_ptr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_set_coords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_get_coords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_move(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
JSValue js_lv_obj_move_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
JSValue js_lv_obj_move_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_get_all_style_values(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_set_style(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
JSValue js_lv_obj_get_style(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_refresh_style(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_get_local_style(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)  ;

#endif