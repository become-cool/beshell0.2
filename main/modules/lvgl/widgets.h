#ifndef _MODULE_LVGL_WIDGETS_H
#define _MODULE_LVGL_WIDGETS_H

#include "quickjs-libc.h"
#include "lvgl.h"


#define CHECK_INSOF_LVOBJ(className, var, msg)                                  \
    {                                                                           \
        JSValue ObjCotr = js_get_glob_prop(ctx, 3, "beapi", "lvgl", className) ;\
        if(!JS_IsInstanceOf(ctx, var, ObjCotr)) {                               \
            JS_FreeValue(ctx,ObjCotr) ;                                         \
            THROW_EXCEPTION(msg) ;                                              \
        }                                                                       \
        JS_FreeValue(ctx,ObjCotr) ;                                             \
    }

#define THIS_LBOBJ(clzname, mehtodname, thisobj)                                                                        \
    CHECK_INSOF_LVOBJ(clzname, this_val, "lvgl."clzname"."mehtodname"() must be called as a lvgl."clzname" method")     \
    lv_obj_t * thisobj = JS_GetOpaqueInternal(this_val) ;                                                               \
    if(!thisobj) {                                                                                                      \
        THROW_EXCEPTION("lvgl."clzname"."mehtodname"() must be called as a lvgl."clzname" method")                      \
    }



JSClassID lv_obj_js_class_id() ;

JSValue js_lv_obj_wrapper(JSContext *ctx, lv_obj_t * cobj, JSClassID clzid) ;

void vlgl_js_widgets_init() ;
void require_vlgl_js_widgets(JSContext *ctx, JSValue lvgl) ;


// AUTO GENERATE CODE START [CONST CONVERT] --------

bool lv_flex_flow_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_flex_flow_t* out) ;
const char *  lv_flex_flow_const_to_str(lv_flex_flow_t code) ;
JSValue lv_flex_flow_const_to_jsstr(JSContext *ctx, lv_flex_flow_t code) ;

bool lv_flex_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_flex_align_t* out) ;
const char *  lv_flex_align_const_to_str(lv_flex_align_t code) ;
JSValue lv_flex_align_const_to_jsstr(JSContext *ctx, lv_flex_align_t code) ;

bool lv_event_code_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_event_code_t* out) ;
const char *  lv_event_code_const_to_str(lv_event_code_t code) ;
JSValue lv_event_code_const_to_jsstr(JSContext *ctx, lv_event_code_t code) ;

bool lv_dir_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_dir_t* out) ;
const char *  lv_dir_const_to_str(lv_dir_t code) ;
JSValue lv_dir_const_to_jsstr(JSContext *ctx, lv_dir_t code) ;

bool lv_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_align_t* out) ;
const char *  lv_align_const_to_str(lv_align_t code) ;
JSValue lv_align_const_to_jsstr(JSContext *ctx, lv_align_t code) ;

bool lv_border_side_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_border_side_t* out) ;
const char *  lv_border_side_const_to_str(lv_border_side_t code) ;
JSValue lv_border_side_const_to_jsstr(JSContext *ctx, lv_border_side_t code) ;

bool lv_base_dir_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_base_dir_t* out) ;
const char *  lv_base_dir_const_to_str(lv_base_dir_t code) ;
JSValue lv_base_dir_const_to_jsstr(JSContext *ctx, lv_base_dir_t code) ;

bool lv_style_prop_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_style_prop_t* out) ;
const char *  lv_style_prop_const_to_str(lv_style_prop_t code) ;
JSValue lv_style_prop_const_to_jsstr(JSContext *ctx, lv_style_prop_t code) ;

bool lv_scr_load_anim_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_scr_load_anim_t* out) ;
const char *  lv_scr_load_anim_const_to_str(lv_scr_load_anim_t code) ;
JSValue lv_scr_load_anim_const_to_jsstr(JSContext *ctx, lv_scr_load_anim_t code) ;

bool lv_obj_flag_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_obj_flag_t* out) ;
const char *  lv_obj_flag_const_to_str(lv_obj_flag_t code) ;
JSValue lv_obj_flag_const_to_jsstr(JSContext *ctx, lv_obj_flag_t code) ;
// AUTO GENERATE CODE END [CONST CONVERT] --------

#endif