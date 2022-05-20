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

#define THIS_OBJ(clzname, mehtodname, thisobj, ctype)                                                                        \
    CHECK_INSOF_LVOBJ(clzname, this_val, "lvgl."clzname"."mehtodname"() must be called as a lvgl."clzname" method")     \
    ctype * thisobj = JS_GetOpaqueInternal(this_val) ;                                                               \
    if(!thisobj) {                                                                                                      \
        THROW_EXCEPTION("lvgl."clzname"."mehtodname"() must be called as a lvgl."clzname" method")                      \
    }

#define THIS_LVOBJ(clzname, mehtodname, thisobj) THIS_OBJ(clzname, mehtodname, thisobj, lv_obj_t)



inline JSClassID lv_obj_js_class_id() ;
inline JSClassID lv_label_js_class_id() ;
inline JSClassID lv_btn_js_class_id() ;

void be_lv_widgets_gen_init() ;
void be_lv_widgets_gen_require(JSContext *ctx, JSValue lvgl) ;

JSClassID be_lv_type_to_js_class(lv_obj_t * obj) ;

// AUTO GENERATE CODE START [CONST CONVERT] --------

bool lv_flex_flow_str_to_const(const char * str, lv_flex_flow_t* out) ;
bool lv_flex_flow_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_flex_flow_t* out) ;
const char *  lv_flex_flow_const_to_str(lv_flex_flow_t code) ;
JSValue lv_flex_flow_const_to_jsstr(JSContext *ctx, lv_flex_flow_t code) ;

bool lv_flex_align_str_to_const(const char * str, lv_flex_align_t* out) ;
bool lv_flex_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_flex_align_t* out) ;
const char *  lv_flex_align_const_to_str(lv_flex_align_t code) ;
JSValue lv_flex_align_const_to_jsstr(JSContext *ctx, lv_flex_align_t code) ;

bool lv_event_code_str_to_const(const char * str, lv_event_code_t* out) ;
bool lv_event_code_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_event_code_t* out) ;
const char *  lv_event_code_const_to_str(lv_event_code_t code) ;
JSValue lv_event_code_const_to_jsstr(JSContext *ctx, lv_event_code_t code) ;

bool lv_dir_str_to_const(const char * str, lv_dir_t* out) ;
bool lv_dir_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_dir_t* out) ;
const char *  lv_dir_const_to_str(lv_dir_t code) ;
JSValue lv_dir_const_to_jsstr(JSContext *ctx, lv_dir_t code) ;

bool lv_align_str_to_const(const char * str, lv_align_t* out) ;
bool lv_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_align_t* out) ;
const char *  lv_align_const_to_str(lv_align_t code) ;
JSValue lv_align_const_to_jsstr(JSContext *ctx, lv_align_t code) ;

bool lv_border_side_str_to_const(const char * str, lv_border_side_t* out) ;
bool lv_border_side_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_border_side_t* out) ;
const char *  lv_border_side_const_to_str(lv_border_side_t code) ;
JSValue lv_border_side_const_to_jsstr(JSContext *ctx, lv_border_side_t code) ;

bool lv_base_dir_str_to_const(const char * str, lv_base_dir_t* out) ;
bool lv_base_dir_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_base_dir_t* out) ;
const char *  lv_base_dir_const_to_str(lv_base_dir_t code) ;
JSValue lv_base_dir_const_to_jsstr(JSContext *ctx, lv_base_dir_t code) ;

bool lv_style_prop_str_to_const(const char * str, lv_style_prop_t* out) ;
bool lv_style_prop_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_style_prop_t* out) ;
const char *  lv_style_prop_const_to_str(lv_style_prop_t code) ;
JSValue lv_style_prop_const_to_jsstr(JSContext *ctx, lv_style_prop_t code) ;

bool lv_scr_load_anim_str_to_const(const char * str, lv_scr_load_anim_t* out) ;
bool lv_scr_load_anim_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_scr_load_anim_t* out) ;
const char *  lv_scr_load_anim_const_to_str(lv_scr_load_anim_t code) ;
JSValue lv_scr_load_anim_const_to_jsstr(JSContext *ctx, lv_scr_load_anim_t code) ;

bool lv_state_str_to_const(const char * str, lv_state_t* out) ;
bool lv_state_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_state_t* out) ;
const char *  lv_state_const_to_str(lv_state_t code) ;
JSValue lv_state_const_to_jsstr(JSContext *ctx, lv_state_t code) ;

bool lv_obj_flag_str_to_const(const char * str, lv_obj_flag_t* out) ;
bool lv_obj_flag_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_obj_flag_t* out) ;
const char *  lv_obj_flag_const_to_str(lv_obj_flag_t code) ;
JSValue lv_obj_flag_const_to_jsstr(JSContext *ctx, lv_obj_flag_t code) ;

bool lv_text_align_str_to_const(const char * str, lv_text_align_t* out) ;
bool lv_text_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_text_align_t* out) ;
const char *  lv_text_align_const_to_str(lv_text_align_t code) ;
JSValue lv_text_align_const_to_jsstr(JSContext *ctx, lv_text_align_t code) ;

bool lv_palette_str_to_const(const char * str, lv_palette_t* out) ;
bool lv_palette_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_palette_t* out) ;
const char *  lv_palette_const_to_str(lv_palette_t code) ;
JSValue lv_palette_const_to_jsstr(JSContext *ctx, lv_palette_t code) ;

bool lv_label_long_mode_str_to_const(const char * str, lv_label_long_mode_t* out) ;
bool lv_label_long_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_label_long_mode_t* out) ;
const char *  lv_label_long_mode_const_to_str(lv_label_long_mode_t code) ;
JSValue lv_label_long_mode_const_to_jsstr(JSContext *ctx, lv_label_long_mode_t code) ;

bool lv_keyboard_mode_str_to_const(const char * str, lv_keyboard_mode_t* out) ;
bool lv_keyboard_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_keyboard_mode_t* out) ;
const char *  lv_keyboard_mode_const_to_str(lv_keyboard_mode_t code) ;
JSValue lv_keyboard_mode_const_to_jsstr(JSContext *ctx, lv_keyboard_mode_t code) ;

bool lv_img_cf_str_to_const(const char * str, lv_img_cf_t* out) ;
bool lv_img_cf_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_img_cf_t* out) ;
const char *  lv_img_cf_const_to_str(lv_img_cf_t code) ;
JSValue lv_img_cf_const_to_jsstr(JSContext *ctx, lv_img_cf_t code) ;

bool lv_scroll_snap_str_to_const(const char * str, lv_scroll_snap_t* out) ;
bool lv_scroll_snap_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_scroll_snap_t* out) ;
const char *  lv_scroll_snap_const_to_str(lv_scroll_snap_t code) ;
JSValue lv_scroll_snap_const_to_jsstr(JSContext *ctx, lv_scroll_snap_t code) ;

bool lv_indev_type_str_to_const(const char * str, lv_indev_type_t* out) ;
bool lv_indev_type_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_indev_type_t* out) ;
const char *  lv_indev_type_const_to_str(lv_indev_type_t code) ;
JSValue lv_indev_type_const_to_jsstr(JSContext *ctx, lv_indev_type_t code) ;
// AUTO GENERATE CODE END [CONST CONVERT] --------


#endif