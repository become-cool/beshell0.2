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

bool lv_event_jsstr_to_code(JSContext *ctx, JSValue jsstr, lv_event_code_t* out) ;
JSValue lv_event_code_to_jsstr(JSContext *ctx, lv_event_code_t code) ;

bool lv_flex_flow_jsstr_to_code(JSContext *ctx, JSValue jsstr, lv_flex_flow_t* out) ;
JSValue lv_flex_flow_code_to_jsstr(JSContext *ctx, lv_flex_flow_t code) ;
    
bool lv_flex_align_jsstr_to_code(JSContext *ctx, JSValue jsstr, lv_flex_align_t* out) ;
JSValue lv_flex_align_code_to_jsstr(JSContext *ctx, lv_flex_align_t code) ;

bool lv_align_jsstr_to_code(JSContext *ctx, JSValue jsstr, lv_align_t* out) ;
JSValue lv_align_code_to_jsstr(JSContext *ctx, lv_align_t code) ;

bool lv_dir_jsstr_to_code(JSContext *ctx, JSValue jsstr, lv_dir_t* out) ;
JSValue lv_dir_code_to_jsstr(JSContext *ctx, lv_dir_t code) ;

#endif