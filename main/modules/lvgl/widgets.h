#ifndef __H_WIDGETS_EXTRA__
#define __H_WIDGETS_EXTRA__

#include "widgets_gen.h"

void require_vlgl_js_widgets(JSContext *ctx, JSValue lvgl) ;

void js_lv_obj_init(JSContext *ctx, JSValue jsobj) ;

inline void lv_obj_set_coord_x(lv_obj_t * obj, lv_coord_t x) ;
inline void lv_obj_set_coord_y(lv_obj_t * obj, lv_coord_t y) ;

JSValue js_lv_obj_wrapper(JSContext *ctx, lv_obj_t * cobj, JSValue cotr, JSClassID clsid) ;

JSValue js_lv_obj_as(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
JSValue js_lv_obj_ptr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
JSValue js_lv_obj_from_ptr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_enable_event(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
JSValue js_lv_obj_disable_event(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
JSValue js_lvgl_is_event_name(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_is_screen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_set_coord_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
JSValue js_lv_obj_set_coord_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
JSValue js_lv_obj_set_coords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_get_coord_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
JSValue js_lv_obj_get_coord_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
JSValue js_lv_obj_get_coords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_move(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
JSValue js_lv_obj_move_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
JSValue js_lv_obj_move_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_get_all_style_values(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_set_style(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
JSValue js_lv_obj_get_style(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_refresh_style(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_get_local_style(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv)  ;

JSValue js_lv_obj_get_font_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_label_set_font(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_img_set_src(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
JSValue js_lv_img_set_symbol(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_list_add_btn(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

JSValue js_lv_msgbox_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv) ;

JSValue js_lv_canvas_malloc(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv) ;
// JSValue js_lv_canvas_free(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv) ;

JSValue js_lv_obj_abort_scroll(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;

#endif