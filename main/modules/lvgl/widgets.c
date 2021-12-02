#include "widgets.h"

#include "utils.h"
#include "cutils.h"
#include "lvgl.h"
#include "lv_conf.h"


// AUTO GENERATE CODE START [DEFINE CLASS] --------
 // beapi.lvgl.Obj --
static JSClassID js_lv_obj_class_id ;
static JSValue js_lv_obj_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1) {
        cparent = JS_GetOpaque(argv[0], js_lv_obj_class_id) ;
    }
    lv_obj_t * cobj = lv_obj_create(cparent) ;
    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_obj_class_id) ;

    lv_obj_set_user_data(cobj, jsobj) ;

    JS_SetOpaque(jsobj, cobj) ;
    JS_DupValue(ctx, jsobj) ;  // 此引用在 c struct 销毁时清除

    return jsobj ;
}
static void js_lv_obj_finalizer(JSRuntime *rt, JSValue val){
    lv_obj_t * thisobj = JS_GetOpaque(val, js_lv_obj_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_obj_class = {
    "lvgl.Obj",
    .finalizer = js_lv_obj_finalizer,
};

 // beapi.lvgl.Arc --
static JSClassID js_lv_arc_class_id ;
static JSValue js_lv_arc_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1) {
        cparent = JS_GetOpaque(argv[0], js_lv_arc_class_id) ;
    }
    lv_arc_t * cobj = lv_arc_create(cparent) ;
    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_arc_class_id) ;

    lv_obj_set_user_data(cobj, jsobj) ;

    JS_SetOpaque(jsobj, cobj) ;
    JS_DupValue(ctx, jsobj) ;  // 此引用在 c struct 销毁时清除

    return jsobj ;
}
static void js_lv_arc_finalizer(JSRuntime *rt, JSValue val){
    lv_arc_t * thisobj = JS_GetOpaque(val, js_lv_arc_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_arc_class = {
    "lvgl.Arc",
    .finalizer = js_lv_arc_finalizer,
};

 // beapi.lvgl.Bar --
static JSClassID js_lv_bar_class_id ;
static JSValue js_lv_bar_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1) {
        cparent = JS_GetOpaque(argv[0], js_lv_bar_class_id) ;
    }
    lv_bar_t * cobj = lv_bar_create(cparent) ;
    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_bar_class_id) ;

    lv_obj_set_user_data(cobj, jsobj) ;

    JS_SetOpaque(jsobj, cobj) ;
    JS_DupValue(ctx, jsobj) ;  // 此引用在 c struct 销毁时清除

    return jsobj ;
}
static void js_lv_bar_finalizer(JSRuntime *rt, JSValue val){
    lv_bar_t * thisobj = JS_GetOpaque(val, js_lv_bar_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_bar_class = {
    "lvgl.Bar",
    .finalizer = js_lv_bar_finalizer,
};

 // beapi.lvgl.Btn --
static JSClassID js_lv_btn_class_id ;
static JSValue js_lv_btn_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1) {
        cparent = JS_GetOpaque(argv[0], js_lv_btn_class_id) ;
    }
    lv_btn_t * cobj = lv_btn_create(cparent) ;
    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_btn_class_id) ;

    lv_obj_set_user_data(cobj, jsobj) ;

    JS_SetOpaque(jsobj, cobj) ;
    JS_DupValue(ctx, jsobj) ;  // 此引用在 c struct 销毁时清除

    return jsobj ;
}
static void js_lv_btn_finalizer(JSRuntime *rt, JSValue val){
    lv_btn_t * thisobj = JS_GetOpaque(val, js_lv_btn_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_btn_class = {
    "lvgl.Btn",
    .finalizer = js_lv_btn_finalizer,
};

 // beapi.lvgl.BtnMatrix --
static JSClassID js_lv_btnmatrix_class_id ;
static JSValue js_lv_btnmatrix_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1) {
        cparent = JS_GetOpaque(argv[0], js_lv_btnmatrix_class_id) ;
    }
    lv_btnmatrix_t * cobj = lv_btnmatrix_create(cparent) ;
    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_btnmatrix_class_id) ;

    lv_obj_set_user_data(cobj, jsobj) ;

    JS_SetOpaque(jsobj, cobj) ;
    JS_DupValue(ctx, jsobj) ;  // 此引用在 c struct 销毁时清除

    return jsobj ;
}
static void js_lv_btnmatrix_finalizer(JSRuntime *rt, JSValue val){
    lv_btnmatrix_t * thisobj = JS_GetOpaque(val, js_lv_btnmatrix_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_btnmatrix_class = {
    "lvgl.BtnMatrix",
    .finalizer = js_lv_btnmatrix_finalizer,
};

 // beapi.lvgl.Canvas --
static JSClassID js_lv_canvas_class_id ;
static JSValue js_lv_canvas_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1) {
        cparent = JS_GetOpaque(argv[0], js_lv_canvas_class_id) ;
    }
    lv_canvas_t * cobj = lv_canvas_create(cparent) ;
    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_canvas_class_id) ;

    lv_obj_set_user_data(cobj, jsobj) ;

    JS_SetOpaque(jsobj, cobj) ;
    JS_DupValue(ctx, jsobj) ;  // 此引用在 c struct 销毁时清除

    return jsobj ;
}
static void js_lv_canvas_finalizer(JSRuntime *rt, JSValue val){
    lv_canvas_t * thisobj = JS_GetOpaque(val, js_lv_canvas_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_canvas_class = {
    "lvgl.Canvas",
    .finalizer = js_lv_canvas_finalizer,
};

 // beapi.lvgl.Checkbox --
static JSClassID js_lv_checkbox_class_id ;
static JSValue js_lv_checkbox_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1) {
        cparent = JS_GetOpaque(argv[0], js_lv_checkbox_class_id) ;
    }
    lv_checkbox_t * cobj = lv_checkbox_create(cparent) ;
    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_checkbox_class_id) ;

    lv_obj_set_user_data(cobj, jsobj) ;

    JS_SetOpaque(jsobj, cobj) ;
    JS_DupValue(ctx, jsobj) ;  // 此引用在 c struct 销毁时清除

    return jsobj ;
}
static void js_lv_checkbox_finalizer(JSRuntime *rt, JSValue val){
    lv_checkbox_t * thisobj = JS_GetOpaque(val, js_lv_checkbox_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_checkbox_class = {
    "lvgl.Checkbox",
    .finalizer = js_lv_checkbox_finalizer,
};

 // beapi.lvgl.Dropdown --
static JSClassID js_lv_dropdown_class_id ;
static JSValue js_lv_dropdown_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1) {
        cparent = JS_GetOpaque(argv[0], js_lv_dropdown_class_id) ;
    }
    lv_dropdown_t * cobj = lv_dropdown_create(cparent) ;
    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_dropdown_class_id) ;

    lv_obj_set_user_data(cobj, jsobj) ;

    JS_SetOpaque(jsobj, cobj) ;
    JS_DupValue(ctx, jsobj) ;  // 此引用在 c struct 销毁时清除

    return jsobj ;
}
static void js_lv_dropdown_finalizer(JSRuntime *rt, JSValue val){
    lv_dropdown_t * thisobj = JS_GetOpaque(val, js_lv_dropdown_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_dropdown_class = {
    "lvgl.Dropdown",
    .finalizer = js_lv_dropdown_finalizer,
};

 // beapi.lvgl.Img --
static JSClassID js_lv_img_class_id ;
static JSValue js_lv_img_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1) {
        cparent = JS_GetOpaque(argv[0], js_lv_img_class_id) ;
    }
    lv_img_t * cobj = lv_img_create(cparent) ;
    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_img_class_id) ;

    lv_obj_set_user_data(cobj, jsobj) ;

    JS_SetOpaque(jsobj, cobj) ;
    JS_DupValue(ctx, jsobj) ;  // 此引用在 c struct 销毁时清除

    return jsobj ;
}
static void js_lv_img_finalizer(JSRuntime *rt, JSValue val){
    lv_img_t * thisobj = JS_GetOpaque(val, js_lv_img_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_img_class = {
    "lvgl.Img",
    .finalizer = js_lv_img_finalizer,
};

 // beapi.lvgl.Label --
static JSClassID js_lv_label_class_id ;
static JSValue js_lv_label_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1) {
        cparent = JS_GetOpaque(argv[0], js_lv_label_class_id) ;
    }
    lv_label_t * cobj = lv_label_create(cparent) ;
    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_label_class_id) ;

    lv_obj_set_user_data(cobj, jsobj) ;

    JS_SetOpaque(jsobj, cobj) ;
    JS_DupValue(ctx, jsobj) ;  // 此引用在 c struct 销毁时清除

    return jsobj ;
}
static void js_lv_label_finalizer(JSRuntime *rt, JSValue val){
    lv_label_t * thisobj = JS_GetOpaque(val, js_lv_label_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_label_class = {
    "lvgl.Label",
    .finalizer = js_lv_label_finalizer,
};

 // beapi.lvgl.Line --
static JSClassID js_lv_line_class_id ;
static JSValue js_lv_line_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1) {
        cparent = JS_GetOpaque(argv[0], js_lv_line_class_id) ;
    }
    lv_line_t * cobj = lv_line_create(cparent) ;
    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_line_class_id) ;

    lv_obj_set_user_data(cobj, jsobj) ;

    JS_SetOpaque(jsobj, cobj) ;
    JS_DupValue(ctx, jsobj) ;  // 此引用在 c struct 销毁时清除

    return jsobj ;
}
static void js_lv_line_finalizer(JSRuntime *rt, JSValue val){
    lv_line_t * thisobj = JS_GetOpaque(val, js_lv_line_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_line_class = {
    "lvgl.Line",
    .finalizer = js_lv_line_finalizer,
};

 // beapi.lvgl.Roller --
static JSClassID js_lv_roller_class_id ;
static JSValue js_lv_roller_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1) {
        cparent = JS_GetOpaque(argv[0], js_lv_roller_class_id) ;
    }
    lv_roller_t * cobj = lv_roller_create(cparent) ;
    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_roller_class_id) ;

    lv_obj_set_user_data(cobj, jsobj) ;

    JS_SetOpaque(jsobj, cobj) ;
    JS_DupValue(ctx, jsobj) ;  // 此引用在 c struct 销毁时清除

    return jsobj ;
}
static void js_lv_roller_finalizer(JSRuntime *rt, JSValue val){
    lv_roller_t * thisobj = JS_GetOpaque(val, js_lv_roller_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_roller_class = {
    "lvgl.Roller",
    .finalizer = js_lv_roller_finalizer,
};

 // beapi.lvgl.Slider --
static JSClassID js_lv_slider_class_id ;
static JSValue js_lv_slider_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1) {
        cparent = JS_GetOpaque(argv[0], js_lv_slider_class_id) ;
    }
    lv_slider_t * cobj = lv_slider_create(cparent) ;
    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_slider_class_id) ;

    lv_obj_set_user_data(cobj, jsobj) ;

    JS_SetOpaque(jsobj, cobj) ;
    JS_DupValue(ctx, jsobj) ;  // 此引用在 c struct 销毁时清除

    return jsobj ;
}
static void js_lv_slider_finalizer(JSRuntime *rt, JSValue val){
    lv_slider_t * thisobj = JS_GetOpaque(val, js_lv_slider_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_slider_class = {
    "lvgl.Slider",
    .finalizer = js_lv_slider_finalizer,
};

 // beapi.lvgl.Switch --
static JSClassID js_lv_switch_class_id ;
static JSValue js_lv_switch_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1) {
        cparent = JS_GetOpaque(argv[0], js_lv_switch_class_id) ;
    }
    lv_switch_t * cobj = lv_switch_create(cparent) ;
    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_switch_class_id) ;

    lv_obj_set_user_data(cobj, jsobj) ;

    JS_SetOpaque(jsobj, cobj) ;
    JS_DupValue(ctx, jsobj) ;  // 此引用在 c struct 销毁时清除

    return jsobj ;
}
static void js_lv_switch_finalizer(JSRuntime *rt, JSValue val){
    lv_switch_t * thisobj = JS_GetOpaque(val, js_lv_switch_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_switch_class = {
    "lvgl.Switch",
    .finalizer = js_lv_switch_finalizer,
};

 // beapi.lvgl.Table --
static JSClassID js_lv_table_class_id ;
static JSValue js_lv_table_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1) {
        cparent = JS_GetOpaque(argv[0], js_lv_table_class_id) ;
    }
    lv_table_t * cobj = lv_table_create(cparent) ;
    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_table_class_id) ;

    lv_obj_set_user_data(cobj, jsobj) ;

    JS_SetOpaque(jsobj, cobj) ;
    JS_DupValue(ctx, jsobj) ;  // 此引用在 c struct 销毁时清除

    return jsobj ;
}
static void js_lv_table_finalizer(JSRuntime *rt, JSValue val){
    lv_table_t * thisobj = JS_GetOpaque(val, js_lv_table_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_table_class = {
    "lvgl.Table",
    .finalizer = js_lv_table_finalizer,
};

 // beapi.lvgl.TextArea --
static JSClassID js_lv_textarea_class_id ;
static JSValue js_lv_textarea_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1) {
        cparent = JS_GetOpaque(argv[0], js_lv_textarea_class_id) ;
    }
    lv_textarea_t * cobj = lv_textarea_create(cparent) ;
    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_textarea_class_id) ;

    lv_obj_set_user_data(cobj, jsobj) ;

    JS_SetOpaque(jsobj, cobj) ;
    JS_DupValue(ctx, jsobj) ;  // 此引用在 c struct 销毁时清除

    return jsobj ;
}
static void js_lv_textarea_finalizer(JSRuntime *rt, JSValue val){
    lv_textarea_t * thisobj = JS_GetOpaque(val, js_lv_textarea_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_textarea_class = {
    "lvgl.TextArea",
    .finalizer = js_lv_textarea_finalizer,
};

// AUTO GENERATE CODE END [DEFINE CLASS] --------




// AUTO GENERATE CODE START [METHOD LIST] --------
static JSValue js_lv_obj_add_flag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.addFlag() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.addFlag() must be called as a lvgl.Obj method")
    }
    uint32_t f ;
    if(!JS_ToUint32(ctx, &f, argv[0])){
        THROW_EXCEPTION("arg f of method lvgl.Obj.addFlag() must be a number")
    }
    lv_obj_add_flag(thisobj, f) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_clear_flag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.clearFlag() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.clearFlag() must be called as a lvgl.Obj method")
    }
    uint32_t f ;
    if(!JS_ToUint32(ctx, &f, argv[0])){
        THROW_EXCEPTION("arg f of method lvgl.Obj.clearFlag() must be a number")
    }
    lv_obj_clear_flag(thisobj, f) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_add_state(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.addState() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.addState() must be called as a lvgl.Obj method")
    }
    uint16_t state ;
    if(!JS_ToUint32(ctx, &state, argv[0])){
        THROW_EXCEPTION("arg state of method lvgl.Obj.addState() must be a number")
    }
    lv_obj_add_state(thisobj, state) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_clear_state(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.clearState() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.clearState() must be called as a lvgl.Obj method")
    }
    uint16_t state ;
    if(!JS_ToUint32(ctx, &state, argv[0])){
        THROW_EXCEPTION("arg state of method lvgl.Obj.clearState() must be a number")
    }
    lv_obj_clear_state(thisobj, state) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_has_flag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.hasFlag() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.hasFlag() must be called as a lvgl.Obj method")
    }
    uint32_t f ;
    if(!JS_ToUint32(ctx, &f, argv[0])){
        THROW_EXCEPTION("arg f of method lvgl.Obj.hasFlag() must be a number")
    }
    return JS_NewBool(ctx,lv_obj_has_flag(thisobj, f)) ;
}

static JSValue js_lv_obj_has_flag_any(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.hasFlagAny() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.hasFlagAny() must be called as a lvgl.Obj method")
    }
    uint32_t f ;
    if(!JS_ToUint32(ctx, &f, argv[0])){
        THROW_EXCEPTION("arg f of method lvgl.Obj.hasFlagAny() must be a number")
    }
    return JS_NewBool(ctx,lv_obj_has_flag_any(thisobj, f)) ;
}

static JSValue js_lv_obj_get_state(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getState() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getState() must be called as a lvgl.Obj method")
    }
    return JS_NewUint32(ctx,lv_obj_get_state(thisobj)) ;
}

static JSValue js_lv_obj_has_state(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.hasState() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.hasState() must be called as a lvgl.Obj method")
    }
    uint16_t state ;
    if(!JS_ToUint32(ctx, &state, argv[0])){
        THROW_EXCEPTION("arg state of method lvgl.Obj.hasState() must be a number")
    }
    return JS_NewBool(ctx,lv_obj_has_state(thisobj, state)) ;
}

static JSValue js_lv_obj_allocate_spec_attr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.allocateSpecAttr() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.allocateSpecAttr() must be called as a lvgl.Obj method")
    }
    lv_obj_allocate_spec_attr(thisobj) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_is_valid(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.isValid() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.isValid() must be called as a lvgl.Obj method")
    }
    return JS_NewBool(ctx,lv_obj_is_valid(thisobj)) ;
}

static JSValue js_lv_obj_dpx(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.dpx() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.dpx() must be called as a lvgl.Obj method")
    }
    int16_t n ;
    if(!JS_ToInt32(ctx, &n, argv[0])){
        THROW_EXCEPTION("arg n of method lvgl.Obj.dpx() must be a number")
    }
    return JS_NewInt32(ctx,lv_obj_dpx(thisobj, n)) ;
}

static JSValue js_lv_obj_is_editable(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.isEditable() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.isEditable() must be called as a lvgl.Obj method")
    }
    return JS_NewBool(ctx,lv_obj_is_editable(thisobj)) ;
}

static JSValue js_lv_obj_set_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Obj.setPos() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.setPos() must be called as a lvgl.Obj method")
    }
    int16_t x ;
    if(!JS_ToInt32(ctx, &x, argv[0])){
        THROW_EXCEPTION("arg x of method lvgl.Obj.setPos() must be a number")
    }
    int16_t y ;
    if(!JS_ToInt32(ctx, &y, argv[1])){
        THROW_EXCEPTION("arg y of method lvgl.Obj.setPos() must be a number")
    }
    lv_obj_set_pos(thisobj, x, y) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_set_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setX() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.setX() must be called as a lvgl.Obj method")
    }
    int16_t x ;
    if(!JS_ToInt32(ctx, &x, argv[0])){
        THROW_EXCEPTION("arg x of method lvgl.Obj.setX() must be a number")
    }
    lv_obj_set_x(thisobj, x) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_set_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setY() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.setY() must be called as a lvgl.Obj method")
    }
    int16_t y ;
    if(!JS_ToInt32(ctx, &y, argv[0])){
        THROW_EXCEPTION("arg y of method lvgl.Obj.setY() must be a number")
    }
    lv_obj_set_y(thisobj, y) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_set_size(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Obj.setSize() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.setSize() must be called as a lvgl.Obj method")
    }
    int16_t w ;
    if(!JS_ToInt32(ctx, &w, argv[0])){
        THROW_EXCEPTION("arg w of method lvgl.Obj.setSize() must be a number")
    }
    int16_t h ;
    if(!JS_ToInt32(ctx, &h, argv[1])){
        THROW_EXCEPTION("arg h of method lvgl.Obj.setSize() must be a number")
    }
    lv_obj_set_size(thisobj, w, h) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_refr_size(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.refrSize() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.refrSize() must be called as a lvgl.Obj method")
    }
    return JS_NewBool(ctx,lv_obj_refr_size(thisobj)) ;
}

static JSValue js_lv_obj_set_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setWidth() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.setWidth() must be called as a lvgl.Obj method")
    }
    int16_t w ;
    if(!JS_ToInt32(ctx, &w, argv[0])){
        THROW_EXCEPTION("arg w of method lvgl.Obj.setWidth() must be a number")
    }
    lv_obj_set_width(thisobj, w) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_set_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setHeight() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.setHeight() must be called as a lvgl.Obj method")
    }
    int16_t h ;
    if(!JS_ToInt32(ctx, &h, argv[0])){
        THROW_EXCEPTION("arg h of method lvgl.Obj.setHeight() must be a number")
    }
    lv_obj_set_height(thisobj, h) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_set_content_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setContentWidth() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.setContentWidth() must be called as a lvgl.Obj method")
    }
    int16_t w ;
    if(!JS_ToInt32(ctx, &w, argv[0])){
        THROW_EXCEPTION("arg w of method lvgl.Obj.setContentWidth() must be a number")
    }
    lv_obj_set_content_width(thisobj, w) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_set_content_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setContentHeight() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.setContentHeight() must be called as a lvgl.Obj method")
    }
    int16_t h ;
    if(!JS_ToInt32(ctx, &h, argv[0])){
        THROW_EXCEPTION("arg h of method lvgl.Obj.setContentHeight() must be a number")
    }
    lv_obj_set_content_height(thisobj, h) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_set_layout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setLayout() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.setLayout() must be called as a lvgl.Obj method")
    }
    uint32_t layout ;
    if(!JS_ToUint32(ctx, &layout, argv[0])){
        THROW_EXCEPTION("arg layout of method lvgl.Obj.setLayout() must be a number")
    }
    lv_obj_set_layout(thisobj, layout) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_is_layout_positioned(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.isLayoutPositioned() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.isLayoutPositioned() must be called as a lvgl.Obj method")
    }
    return JS_NewBool(ctx,lv_obj_is_layout_positioned(thisobj)) ;
}

static JSValue js_lv_obj_mark_layout_as_dirty(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.markLayoutAsDirty() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.markLayoutAsDirty() must be called as a lvgl.Obj method")
    }
    lv_obj_mark_layout_as_dirty(thisobj) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_update_layout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.updateLayout() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.updateLayout() must be called as a lvgl.Obj method")
    }
    lv_obj_update_layout(thisobj) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_set_align(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setAlign() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.setAlign() must be called as a lvgl.Obj method")
    }
    uint8_t align ;
    if(!JS_ToUint32(ctx, &align, argv[0])){
        THROW_EXCEPTION("arg align of method lvgl.Obj.setAlign() must be a number")
    }
    lv_obj_set_align(thisobj, align) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_align(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("lvgl.Obj.align() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.align() must be called as a lvgl.Obj method")
    }
    uint8_t align ;
    if(!JS_ToUint32(ctx, &align, argv[0])){
        THROW_EXCEPTION("arg align of method lvgl.Obj.align() must be a number")
    }
    int16_t x_ofs ;
    if(!JS_ToInt32(ctx, &x_ofs, argv[1])){
        THROW_EXCEPTION("arg x_ofs of method lvgl.Obj.align() must be a number")
    }
    int16_t y_ofs ;
    if(!JS_ToInt32(ctx, &y_ofs, argv[2])){
        THROW_EXCEPTION("arg y_ofs of method lvgl.Obj.align() must be a number")
    }
    lv_obj_align(thisobj, align, x_ofs, y_ofs) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_align_to(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<4) {
        THROW_EXCEPTION("lvgl.Obj.alignTo() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.alignTo() must be called as a lvgl.Obj method")
    }
    lv_obj_t * base = (lv_obj_t *)JS_GetOpaque(argv[0], js_lv_obj_class_id) ;
    if( !base ){
        THROW_EXCEPTION("arg base of method lvgl.Obj.alignTo() must be a beapi.lvgl.Obj")
    }
    uint8_t align ;
    if(!JS_ToUint32(ctx, &align, argv[1])){
        THROW_EXCEPTION("arg align of method lvgl.Obj.alignTo() must be a number")
    }
    int16_t x_ofs ;
    if(!JS_ToInt32(ctx, &x_ofs, argv[2])){
        THROW_EXCEPTION("arg x_ofs of method lvgl.Obj.alignTo() must be a number")
    }
    int16_t y_ofs ;
    if(!JS_ToInt32(ctx, &y_ofs, argv[3])){
        THROW_EXCEPTION("arg y_ofs of method lvgl.Obj.alignTo() must be a number")
    }
    lv_obj_align_to(thisobj, base, align, x_ofs, y_ofs) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_center(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.center() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.center() must be called as a lvgl.Obj method")
    }
    lv_obj_center(thisobj) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_get_coords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.getCoords() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getCoords() must be called as a lvgl.Obj method")
    }
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg coords of method lvgl.Obj.getCoords() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t coords ;
    JSValue jscoords_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jscoords_x1)){
        THROW_EXCEPTION("arg coords of method lvgl.Obj.getCoords() missing property x, or is not a number")
    }
    coords.x1 = jscoords_x1 ;
    JSValue jscoords_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jscoords_y1)){
        THROW_EXCEPTION("arg coords of method lvgl.Obj.getCoords() missing property x, or is not a number")
    }
    coords.y1 = jscoords_y1 ;
    JSValue jscoords_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jscoords_x2)){
        THROW_EXCEPTION("arg coords of method lvgl.Obj.getCoords() missing property x, or is not a number")
    }
    coords.x2 = jscoords_x2 ;
    JSValue jscoords_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jscoords_y2)){
        THROW_EXCEPTION("arg coords of method lvgl.Obj.getCoords() missing property x, or is not a number")
    }
    coords.y2 = jscoords_y2 ;
    lv_obj_get_coords(thisobj, &coords) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_get_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getX() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getX() must be called as a lvgl.Obj method")
    }
    return JS_NewInt32(ctx,lv_obj_get_x(thisobj)) ;
}

static JSValue js_lv_obj_get_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getY() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getY() must be called as a lvgl.Obj method")
    }
    return JS_NewInt32(ctx,lv_obj_get_y(thisobj)) ;
}

static JSValue js_lv_obj_get_x_aligned(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getXAligned() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getXAligned() must be called as a lvgl.Obj method")
    }
    return JS_NewInt32(ctx,lv_obj_get_x_aligned(thisobj)) ;
}

static JSValue js_lv_obj_get_y_aligned(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getYAligned() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getYAligned() must be called as a lvgl.Obj method")
    }
    return JS_NewInt32(ctx,lv_obj_get_y_aligned(thisobj)) ;
}

static JSValue js_lv_obj_get_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getWidth() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getWidth() must be called as a lvgl.Obj method")
    }
    return JS_NewInt32(ctx,lv_obj_get_width(thisobj)) ;
}

static JSValue js_lv_obj_get_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getHeight() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getHeight() must be called as a lvgl.Obj method")
    }
    return JS_NewInt32(ctx,lv_obj_get_height(thisobj)) ;
}

static JSValue js_lv_obj_get_content_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getContentWidth() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getContentWidth() must be called as a lvgl.Obj method")
    }
    return JS_NewInt32(ctx,lv_obj_get_content_width(thisobj)) ;
}

static JSValue js_lv_obj_get_content_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getContentHeight() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getContentHeight() must be called as a lvgl.Obj method")
    }
    return JS_NewInt32(ctx,lv_obj_get_content_height(thisobj)) ;
}

static JSValue js_lv_obj_get_content_coords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.getContentCoords() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getContentCoords() must be called as a lvgl.Obj method")
    }
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg area of method lvgl.Obj.getContentCoords() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t area ;
    JSValue jsarea_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jsarea_x1)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.getContentCoords() missing property x, or is not a number")
    }
    area.x1 = jsarea_x1 ;
    JSValue jsarea_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jsarea_y1)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.getContentCoords() missing property x, or is not a number")
    }
    area.y1 = jsarea_y1 ;
    JSValue jsarea_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jsarea_x2)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.getContentCoords() missing property x, or is not a number")
    }
    area.x2 = jsarea_x2 ;
    JSValue jsarea_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jsarea_y2)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.getContentCoords() missing property x, or is not a number")
    }
    area.y2 = jsarea_y2 ;
    lv_obj_get_content_coords(thisobj, &area) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_get_self_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getSelfWidth() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getSelfWidth() must be called as a lvgl.Obj method")
    }
    return JS_NewInt32(ctx,lv_obj_get_self_width(thisobj)) ;
}

static JSValue js_lv_obj_get_self_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getSelfHeight() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getSelfHeight() must be called as a lvgl.Obj method")
    }
    return JS_NewInt32(ctx,lv_obj_get_self_height(thisobj)) ;
}

static JSValue js_lv_obj_refresh_self_size(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.refreshSelfSize() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.refreshSelfSize() must be called as a lvgl.Obj method")
    }
    return JS_NewBool(ctx,lv_obj_refresh_self_size(thisobj)) ;
}

static JSValue js_lv_obj_refr_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.refrPos() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.refrPos() must be called as a lvgl.Obj method")
    }
    lv_obj_refr_pos(thisobj) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_move_to(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Obj.moveTo() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.moveTo() must be called as a lvgl.Obj method")
    }
    int16_t x ;
    if(!JS_ToInt32(ctx, &x, argv[0])){
        THROW_EXCEPTION("arg x of method lvgl.Obj.moveTo() must be a number")
    }
    int16_t y ;
    if(!JS_ToInt32(ctx, &y, argv[1])){
        THROW_EXCEPTION("arg y of method lvgl.Obj.moveTo() must be a number")
    }
    lv_obj_move_to(thisobj, x, y) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_move_children_by(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("lvgl.Obj.moveChildrenBy() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.moveChildrenBy() must be called as a lvgl.Obj method")
    }
    int16_t x_diff ;
    if(!JS_ToInt32(ctx, &x_diff, argv[0])){
        THROW_EXCEPTION("arg x_diff of method lvgl.Obj.moveChildrenBy() must be a number")
    }
    int16_t y_diff ;
    if(!JS_ToInt32(ctx, &y_diff, argv[1])){
        THROW_EXCEPTION("arg y_diff of method lvgl.Obj.moveChildrenBy() must be a number")
    }
    bool ignore_floating = JS_ToBool(ctx, argv[2]) ;
    lv_obj_move_children_by(thisobj, x_diff, y_diff, ignore_floating) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_invalidate_area(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.invalidateArea() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.invalidateArea() must be called as a lvgl.Obj method")
    }
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg area of method lvgl.Obj.invalidateArea() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t area ;
    JSValue jsarea_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jsarea_x1)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.invalidateArea() missing property x, or is not a number")
    }
    area.x1 = jsarea_x1 ;
    JSValue jsarea_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jsarea_y1)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.invalidateArea() missing property x, or is not a number")
    }
    area.y1 = jsarea_y1 ;
    JSValue jsarea_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jsarea_x2)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.invalidateArea() missing property x, or is not a number")
    }
    area.x2 = jsarea_x2 ;
    JSValue jsarea_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jsarea_y2)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.invalidateArea() missing property x, or is not a number")
    }
    area.y2 = jsarea_y2 ;
    lv_obj_invalidate_area(thisobj, &area) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_invalidate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.invalidate() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.invalidate() must be called as a lvgl.Obj method")
    }
    lv_obj_invalidate(thisobj) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_area_is_visible(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.areaIsVisible() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.areaIsVisible() must be called as a lvgl.Obj method")
    }
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg area of method lvgl.Obj.areaIsVisible() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t area ;
    JSValue jsarea_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jsarea_x1)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.areaIsVisible() missing property x, or is not a number")
    }
    area.x1 = jsarea_x1 ;
    JSValue jsarea_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jsarea_y1)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.areaIsVisible() missing property x, or is not a number")
    }
    area.y1 = jsarea_y1 ;
    JSValue jsarea_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jsarea_x2)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.areaIsVisible() missing property x, or is not a number")
    }
    area.x2 = jsarea_x2 ;
    JSValue jsarea_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jsarea_y2)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.areaIsVisible() missing property x, or is not a number")
    }
    area.y2 = jsarea_y2 ;
    return JS_NewBool(ctx,lv_obj_area_is_visible(thisobj, &area)) ;
}

static JSValue js_lv_obj_is_visible(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.isVisible() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.isVisible() must be called as a lvgl.Obj method")
    }
    return JS_NewBool(ctx,lv_obj_is_visible(thisobj)) ;
}

static JSValue js_lv_obj_set_ext_click_area(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setExtClickArea() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.setExtClickArea() must be called as a lvgl.Obj method")
    }
    int16_t size ;
    if(!JS_ToInt32(ctx, &size, argv[0])){
        THROW_EXCEPTION("arg size of method lvgl.Obj.setExtClickArea() must be a number")
    }
    lv_obj_set_ext_click_area(thisobj, size) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_get_click_area(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.getClickArea() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getClickArea() must be called as a lvgl.Obj method")
    }
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg area of method lvgl.Obj.getClickArea() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t area ;
    JSValue jsarea_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jsarea_x1)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.getClickArea() missing property x, or is not a number")
    }
    area.x1 = jsarea_x1 ;
    JSValue jsarea_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jsarea_y1)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.getClickArea() missing property x, or is not a number")
    }
    area.y1 = jsarea_y1 ;
    JSValue jsarea_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jsarea_x2)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.getClickArea() missing property x, or is not a number")
    }
    area.x2 = jsarea_x2 ;
    JSValue jsarea_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jsarea_y2)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.getClickArea() missing property x, or is not a number")
    }
    area.y2 = jsarea_y2 ;
    lv_obj_get_click_area(thisobj, &area) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_hit_test(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.hitTest() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.hitTest() must be called as a lvgl.Obj method")
    }
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg point of method lvgl.Obj.hitTest() must be a object{x,y}")
    }
    lv_point_t point ;
    JSValue jspoint_x = JS_GetPropertyStr(ctx, argv[0], "x") ;
    if(!JS_IsNumber(jspoint_x)){
        THROW_EXCEPTION("arg point of method lvgl.Obj.hitTest() missing property x, or is not a number")
    }
    point.x = jspoint_x ;
    JSValue jspoint_y = JS_GetPropertyStr(ctx, argv[0], "y") ;
    if(!JS_IsNumber(jspoint_y)){
        THROW_EXCEPTION("arg point of method lvgl.Obj.hitTest() missing property x, or is not a number")
    }
    point.y = jspoint_y ;
    return JS_NewBool(ctx,lv_obj_hit_test(thisobj, &point)) ;
}

static JSValue js_lv_obj_set_scrollbar_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setScrollbarMode() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.setScrollbarMode() must be called as a lvgl.Obj method")
    }
    uint8_t mode ;
    if(!JS_ToUint32(ctx, &mode, argv[0])){
        THROW_EXCEPTION("arg mode of method lvgl.Obj.setScrollbarMode() must be a number")
    }
    lv_obj_set_scrollbar_mode(thisobj, mode) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_set_scroll_dir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setScrollDir() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.setScrollDir() must be called as a lvgl.Obj method")
    }
    uint8_t dir ;
    if(!JS_ToUint32(ctx, &dir, argv[0])){
        THROW_EXCEPTION("arg dir of method lvgl.Obj.setScrollDir() must be a number")
    }
    lv_obj_set_scroll_dir(thisobj, dir) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_set_scroll_snap_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setScrollSnapX() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.setScrollSnapX() must be called as a lvgl.Obj method")
    }
    uint8_t align ;
    if(!JS_ToUint32(ctx, &align, argv[0])){
        THROW_EXCEPTION("arg align of method lvgl.Obj.setScrollSnapX() must be a number")
    }
    lv_obj_set_scroll_snap_x(thisobj, align) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_set_scroll_snap_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setScrollSnapY() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.setScrollSnapY() must be called as a lvgl.Obj method")
    }
    uint8_t align ;
    if(!JS_ToUint32(ctx, &align, argv[0])){
        THROW_EXCEPTION("arg align of method lvgl.Obj.setScrollSnapY() must be a number")
    }
    lv_obj_set_scroll_snap_y(thisobj, align) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_get_scrollbar_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getScrollbarMode() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getScrollbarMode() must be called as a lvgl.Obj method")
    }
    return JS_NewUint32(ctx,lv_obj_get_scrollbar_mode(thisobj)) ;
}

static JSValue js_lv_obj_get_scroll_dir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getScrollDir() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getScrollDir() must be called as a lvgl.Obj method")
    }
    return JS_NewUint32(ctx,lv_obj_get_scroll_dir(thisobj)) ;
}

static JSValue js_lv_obj_get_scroll_snap_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getScrollSnapX() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getScrollSnapX() must be called as a lvgl.Obj method")
    }
    return JS_NewUint32(ctx,lv_obj_get_scroll_snap_x(thisobj)) ;
}

static JSValue js_lv_obj_get_scroll_snap_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getScrollSnapY() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getScrollSnapY() must be called as a lvgl.Obj method")
    }
    return JS_NewUint32(ctx,lv_obj_get_scroll_snap_y(thisobj)) ;
}

static JSValue js_lv_obj_get_scroll_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getScrollX() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getScrollX() must be called as a lvgl.Obj method")
    }
    return JS_NewInt32(ctx,lv_obj_get_scroll_x(thisobj)) ;
}

static JSValue js_lv_obj_get_scroll_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getScrollY() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getScrollY() must be called as a lvgl.Obj method")
    }
    return JS_NewInt32(ctx,lv_obj_get_scroll_y(thisobj)) ;
}

static JSValue js_lv_obj_get_scroll_top(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getScrollTop() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getScrollTop() must be called as a lvgl.Obj method")
    }
    return JS_NewInt32(ctx,lv_obj_get_scroll_top(thisobj)) ;
}

static JSValue js_lv_obj_get_scroll_bottom(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getScrollBottom() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getScrollBottom() must be called as a lvgl.Obj method")
    }
    return JS_NewInt32(ctx,lv_obj_get_scroll_bottom(thisobj)) ;
}

static JSValue js_lv_obj_get_scroll_left(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getScrollLeft() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getScrollLeft() must be called as a lvgl.Obj method")
    }
    return JS_NewInt32(ctx,lv_obj_get_scroll_left(thisobj)) ;
}

static JSValue js_lv_obj_get_scroll_right(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getScrollRight() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getScrollRight() must be called as a lvgl.Obj method")
    }
    return JS_NewInt32(ctx,lv_obj_get_scroll_right(thisobj)) ;
}

static JSValue js_lv_obj_get_scroll_end(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.getScrollEnd() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getScrollEnd() must be called as a lvgl.Obj method")
    }
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg end of method lvgl.Obj.getScrollEnd() must be a object{x,y}")
    }
    lv_point_t end ;
    JSValue jsend_x = JS_GetPropertyStr(ctx, argv[0], "x") ;
    if(!JS_IsNumber(jsend_x)){
        THROW_EXCEPTION("arg end of method lvgl.Obj.getScrollEnd() missing property x, or is not a number")
    }
    end.x = jsend_x ;
    JSValue jsend_y = JS_GetPropertyStr(ctx, argv[0], "y") ;
    if(!JS_IsNumber(jsend_y)){
        THROW_EXCEPTION("arg end of method lvgl.Obj.getScrollEnd() missing property x, or is not a number")
    }
    end.y = jsend_y ;
    lv_obj_get_scroll_end(thisobj, &end) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_scroll_by(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("lvgl.Obj.scrollBy() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.scrollBy() must be called as a lvgl.Obj method")
    }
    int16_t x ;
    if(!JS_ToInt32(ctx, &x, argv[0])){
        THROW_EXCEPTION("arg x of method lvgl.Obj.scrollBy() must be a number")
    }
    int16_t y ;
    if(!JS_ToInt32(ctx, &y, argv[1])){
        THROW_EXCEPTION("arg y of method lvgl.Obj.scrollBy() must be a number")
    }
    bool anim_en = JS_ToBool(ctx, argv[2]) ;
    lv_obj_scroll_by(thisobj, x, y, anim_en) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_scroll_to(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("lvgl.Obj.scrollTo() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.scrollTo() must be called as a lvgl.Obj method")
    }
    int16_t x ;
    if(!JS_ToInt32(ctx, &x, argv[0])){
        THROW_EXCEPTION("arg x of method lvgl.Obj.scrollTo() must be a number")
    }
    int16_t y ;
    if(!JS_ToInt32(ctx, &y, argv[1])){
        THROW_EXCEPTION("arg y of method lvgl.Obj.scrollTo() must be a number")
    }
    bool anim_en = JS_ToBool(ctx, argv[2]) ;
    lv_obj_scroll_to(thisobj, x, y, anim_en) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_scroll_to_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Obj.scrollToX() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.scrollToX() must be called as a lvgl.Obj method")
    }
    int16_t x ;
    if(!JS_ToInt32(ctx, &x, argv[0])){
        THROW_EXCEPTION("arg x of method lvgl.Obj.scrollToX() must be a number")
    }
    bool anim_en = JS_ToBool(ctx, argv[1]) ;
    lv_obj_scroll_to_x(thisobj, x, anim_en) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_scroll_to_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Obj.scrollToY() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.scrollToY() must be called as a lvgl.Obj method")
    }
    int16_t y ;
    if(!JS_ToInt32(ctx, &y, argv[0])){
        THROW_EXCEPTION("arg y of method lvgl.Obj.scrollToY() must be a number")
    }
    bool anim_en = JS_ToBool(ctx, argv[1]) ;
    lv_obj_scroll_to_y(thisobj, y, anim_en) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_scroll_to_view(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.scrollToView() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.scrollToView() must be called as a lvgl.Obj method")
    }
    bool anim_en = JS_ToBool(ctx, argv[0]) ;
    lv_obj_scroll_to_view(thisobj, anim_en) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_scroll_to_view_recursive(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.scrollToViewRecursive() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.scrollToViewRecursive() must be called as a lvgl.Obj method")
    }
    bool anim_en = JS_ToBool(ctx, argv[0]) ;
    lv_obj_scroll_to_view_recursive(thisobj, anim_en) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_is_scrolling(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.isScrolling() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.isScrolling() must be called as a lvgl.Obj method")
    }
    return JS_NewBool(ctx,lv_obj_is_scrolling(thisobj)) ;
}

static JSValue js_lv_obj_update_snap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.updateSnap() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.updateSnap() must be called as a lvgl.Obj method")
    }
    bool anim_en = JS_ToBool(ctx, argv[0]) ;
    lv_obj_update_snap(thisobj, anim_en) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_get_scrollbar_area(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Obj.getScrollbarArea() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getScrollbarArea() must be called as a lvgl.Obj method")
    }
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg hor of method lvgl.Obj.getScrollbarArea() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t hor ;
    JSValue jshor_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jshor_x1)){
        THROW_EXCEPTION("arg hor of method lvgl.Obj.getScrollbarArea() missing property x, or is not a number")
    }
    hor.x1 = jshor_x1 ;
    JSValue jshor_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jshor_y1)){
        THROW_EXCEPTION("arg hor of method lvgl.Obj.getScrollbarArea() missing property x, or is not a number")
    }
    hor.y1 = jshor_y1 ;
    JSValue jshor_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jshor_x2)){
        THROW_EXCEPTION("arg hor of method lvgl.Obj.getScrollbarArea() missing property x, or is not a number")
    }
    hor.x2 = jshor_x2 ;
    JSValue jshor_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jshor_y2)){
        THROW_EXCEPTION("arg hor of method lvgl.Obj.getScrollbarArea() missing property x, or is not a number")
    }
    hor.y2 = jshor_y2 ;
    if(!JS_IsObject(argv[1])){
        THROW_EXCEPTION("arg ver of method lvgl.Obj.getScrollbarArea() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t ver ;
    JSValue jsver_x1 = JS_GetPropertyStr(ctx, argv[1], "x1") ;
    if(!JS_IsNumber(jsver_x1)){
        THROW_EXCEPTION("arg ver of method lvgl.Obj.getScrollbarArea() missing property x, or is not a number")
    }
    ver.x1 = jsver_x1 ;
    JSValue jsver_y1 = JS_GetPropertyStr(ctx, argv[1], "y1") ;
    if(!JS_IsNumber(jsver_y1)){
        THROW_EXCEPTION("arg ver of method lvgl.Obj.getScrollbarArea() missing property x, or is not a number")
    }
    ver.y1 = jsver_y1 ;
    JSValue jsver_x2 = JS_GetPropertyStr(ctx, argv[1], "x2") ;
    if(!JS_IsNumber(jsver_x2)){
        THROW_EXCEPTION("arg ver of method lvgl.Obj.getScrollbarArea() missing property x, or is not a number")
    }
    ver.x2 = jsver_x2 ;
    JSValue jsver_y2 = JS_GetPropertyStr(ctx, argv[1], "y2") ;
    if(!JS_IsNumber(jsver_y2)){
        THROW_EXCEPTION("arg ver of method lvgl.Obj.getScrollbarArea() missing property x, or is not a number")
    }
    ver.y2 = jsver_y2 ;
    lv_obj_get_scrollbar_area(thisobj, &hor, &ver) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_scrollbar_invalidate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.scrollbarInvalidate() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.scrollbarInvalidate() must be called as a lvgl.Obj method")
    }
    lv_obj_scrollbar_invalidate(thisobj) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_readjust_scroll(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.readjustScroll() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.readjustScroll() must be called as a lvgl.Obj method")
    }
    bool anim_en = JS_ToBool(ctx, argv[0]) ;
    lv_obj_readjust_scroll(thisobj, anim_en) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_fade_in(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Obj.fadeIn() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.fadeIn() must be called as a lvgl.Obj method")
    }
    uint32_t time ;
    if(!JS_ToUint32(ctx, &time, argv[0])){
        THROW_EXCEPTION("arg time of method lvgl.Obj.fadeIn() must be a number")
    }
    uint32_t delay ;
    if(!JS_ToUint32(ctx, &delay, argv[1])){
        THROW_EXCEPTION("arg delay of method lvgl.Obj.fadeIn() must be a number")
    }
    lv_obj_fade_in(thisobj, time, delay) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_fade_out(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Obj.fadeOut() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.fadeOut() must be called as a lvgl.Obj method")
    }
    uint32_t time ;
    if(!JS_ToUint32(ctx, &time, argv[0])){
        THROW_EXCEPTION("arg time of method lvgl.Obj.fadeOut() must be a number")
    }
    uint32_t delay ;
    if(!JS_ToUint32(ctx, &delay, argv[1])){
        THROW_EXCEPTION("arg delay of method lvgl.Obj.fadeOut() must be a number")
    }
    lv_obj_fade_out(thisobj, time, delay) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_move_to_index(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.moveToIndex() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.moveToIndex() must be called as a lvgl.Obj method")
    }
    int32_t index ;
    if(!JS_ToInt32(ctx, &index, argv[0])){
        THROW_EXCEPTION("arg index of method lvgl.Obj.moveToIndex() must be a number")
    }
    lv_obj_move_to_index(thisobj, index) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_get_screen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getScreen() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getScreen() must be called as a lvgl.Obj method")
    }
    return lv_obj_get_user_data(lv_obj_get_screen(thisobj)) || JS_NULL ;
}

static JSValue js_lv_obj_get_parent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getParent() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getParent() must be called as a lvgl.Obj method")
    }
    return lv_obj_get_user_data(lv_obj_get_parent(thisobj)) || JS_NULL ;
}

static JSValue js_lv_obj_get_child(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.getChild() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getChild() must be called as a lvgl.Obj method")
    }
    int32_t id ;
    if(!JS_ToInt32(ctx, &id, argv[0])){
        THROW_EXCEPTION("arg id of method lvgl.Obj.getChild() must be a number")
    }
    return lv_obj_get_user_data(lv_obj_get_child(thisobj, id)) || JS_NULL ;
}

static JSValue js_lv_obj_get_child_cnt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getChildCnt() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getChildCnt() must be called as a lvgl.Obj method")
    }
    return JS_NewUint32(ctx,lv_obj_get_child_cnt(thisobj)) ;
}

static JSValue js_lv_obj_get_index(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getIndex() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getIndex() must be called as a lvgl.Obj method")
    }
    return JS_NewUint32(ctx,lv_obj_get_index(thisobj)) ;
}

static JSValue js_lv_obj_set_tile(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Obj.setTile() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.setTile() must be called as a lvgl.Obj method")
    }
    lv_obj_t * tile_obj = (lv_obj_t *)JS_GetOpaque(argv[0], js_lv_obj_class_id) ;
    if( !tile_obj ){
        THROW_EXCEPTION("arg tile_obj of method lvgl.Obj.setTile() must be a beapi.lvgl.Obj")
    }
    bool anim_en = JS_ToBool(ctx, argv[1]) ;
    lv_obj_set_tile(thisobj, tile_obj, anim_en) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_set_tile_id(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("lvgl.Obj.setTileId() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.setTileId() must be called as a lvgl.Obj method")
    }
    uint32_t col_id ;
    if(!JS_ToUint32(ctx, &col_id, argv[0])){
        THROW_EXCEPTION("arg col_id of method lvgl.Obj.setTileId() must be a number")
    }
    uint32_t row_id ;
    if(!JS_ToUint32(ctx, &row_id, argv[1])){
        THROW_EXCEPTION("arg row_id of method lvgl.Obj.setTileId() must be a number")
    }
    bool anim_en = JS_ToBool(ctx, argv[2]) ;
    lv_obj_set_tile_id(thisobj, col_id, row_id, anim_en) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_move_foreground(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.moveForeground() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.moveForeground() must be called as a lvgl.Obj method")
    }
    lv_obj_move_foreground(thisobj) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_move_background(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.moveBackground() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.moveBackground() must be called as a lvgl.Obj method")
    }
    lv_obj_move_background(thisobj) ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_obj_get_child_id(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<0) {
        THROW_EXCEPTION("lvgl.Obj.getChildId() missing arg")
    }
    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_lv_obj_class_id) ;
    if(!thisobj) {
        THROW_EXCEPTION("lvgl.Obj.getChildId() must be called as a lvgl.Obj method")
    }
    return JS_NewUint32(ctx,lv_obj_get_child_id(thisobj)) ;
}


static const JSCFunctionListEntry js_lv_obj_proto_funcs[] = {
    JS_CFUNC_DEF("addFlag", 0, js_lv_obj_add_flag),
    JS_CFUNC_DEF("clearFlag", 0, js_lv_obj_clear_flag),
    JS_CFUNC_DEF("addState", 0, js_lv_obj_add_state),
    JS_CFUNC_DEF("clearState", 0, js_lv_obj_clear_state),
    JS_CFUNC_DEF("hasFlag", 0, js_lv_obj_has_flag),
    JS_CFUNC_DEF("hasFlagAny", 0, js_lv_obj_has_flag_any),
    JS_CFUNC_DEF("getState", 0, js_lv_obj_get_state),
    JS_CFUNC_DEF("hasState", 0, js_lv_obj_has_state),
    JS_CFUNC_DEF("allocateSpecAttr", 0, js_lv_obj_allocate_spec_attr),
    JS_CFUNC_DEF("isValid", 0, js_lv_obj_is_valid),
    JS_CFUNC_DEF("dpx", 0, js_lv_obj_dpx),
    JS_CFUNC_DEF("isEditable", 0, js_lv_obj_is_editable),
    JS_CFUNC_DEF("setPos", 0, js_lv_obj_set_pos),
    JS_CFUNC_DEF("setX", 0, js_lv_obj_set_x),
    JS_CFUNC_DEF("setY", 0, js_lv_obj_set_y),
    JS_CFUNC_DEF("setSize", 0, js_lv_obj_set_size),
    JS_CFUNC_DEF("refrSize", 0, js_lv_obj_refr_size),
    JS_CFUNC_DEF("setWidth", 0, js_lv_obj_set_width),
    JS_CFUNC_DEF("setHeight", 0, js_lv_obj_set_height),
    JS_CFUNC_DEF("setContentWidth", 0, js_lv_obj_set_content_width),
    JS_CFUNC_DEF("setContentHeight", 0, js_lv_obj_set_content_height),
    JS_CFUNC_DEF("setLayout", 0, js_lv_obj_set_layout),
    JS_CFUNC_DEF("isLayoutPositioned", 0, js_lv_obj_is_layout_positioned),
    JS_CFUNC_DEF("markLayoutAsDirty", 0, js_lv_obj_mark_layout_as_dirty),
    JS_CFUNC_DEF("updateLayout", 0, js_lv_obj_update_layout),
    JS_CFUNC_DEF("setAlign", 0, js_lv_obj_set_align),
    JS_CFUNC_DEF("align", 0, js_lv_obj_align),
    JS_CFUNC_DEF("alignTo", 0, js_lv_obj_align_to),
    JS_CFUNC_DEF("center", 0, js_lv_obj_center),
    JS_CFUNC_DEF("getCoords", 0, js_lv_obj_get_coords),
    JS_CFUNC_DEF("getX", 0, js_lv_obj_get_x),
    JS_CFUNC_DEF("getY", 0, js_lv_obj_get_y),
    JS_CFUNC_DEF("getXAligned", 0, js_lv_obj_get_x_aligned),
    JS_CFUNC_DEF("getYAligned", 0, js_lv_obj_get_y_aligned),
    JS_CFUNC_DEF("getWidth", 0, js_lv_obj_get_width),
    JS_CFUNC_DEF("getHeight", 0, js_lv_obj_get_height),
    JS_CFUNC_DEF("getContentWidth", 0, js_lv_obj_get_content_width),
    JS_CFUNC_DEF("getContentHeight", 0, js_lv_obj_get_content_height),
    JS_CFUNC_DEF("getContentCoords", 0, js_lv_obj_get_content_coords),
    JS_CFUNC_DEF("getSelfWidth", 0, js_lv_obj_get_self_width),
    JS_CFUNC_DEF("getSelfHeight", 0, js_lv_obj_get_self_height),
    JS_CFUNC_DEF("refreshSelfSize", 0, js_lv_obj_refresh_self_size),
    JS_CFUNC_DEF("refrPos", 0, js_lv_obj_refr_pos),
    JS_CFUNC_DEF("moveTo", 0, js_lv_obj_move_to),
    JS_CFUNC_DEF("moveChildrenBy", 0, js_lv_obj_move_children_by),
    JS_CFUNC_DEF("invalidateArea", 0, js_lv_obj_invalidate_area),
    JS_CFUNC_DEF("invalidate", 0, js_lv_obj_invalidate),
    JS_CFUNC_DEF("areaIsVisible", 0, js_lv_obj_area_is_visible),
    JS_CFUNC_DEF("isVisible", 0, js_lv_obj_is_visible),
    JS_CFUNC_DEF("setExtClickArea", 0, js_lv_obj_set_ext_click_area),
    JS_CFUNC_DEF("getClickArea", 0, js_lv_obj_get_click_area),
    JS_CFUNC_DEF("hitTest", 0, js_lv_obj_hit_test),
    JS_CFUNC_DEF("setScrollbarMode", 0, js_lv_obj_set_scrollbar_mode),
    JS_CFUNC_DEF("setScrollDir", 0, js_lv_obj_set_scroll_dir),
    JS_CFUNC_DEF("setScrollSnapX", 0, js_lv_obj_set_scroll_snap_x),
    JS_CFUNC_DEF("setScrollSnapY", 0, js_lv_obj_set_scroll_snap_y),
    JS_CFUNC_DEF("getScrollbarMode", 0, js_lv_obj_get_scrollbar_mode),
    JS_CFUNC_DEF("getScrollDir", 0, js_lv_obj_get_scroll_dir),
    JS_CFUNC_DEF("getScrollSnapX", 0, js_lv_obj_get_scroll_snap_x),
    JS_CFUNC_DEF("getScrollSnapY", 0, js_lv_obj_get_scroll_snap_y),
    JS_CFUNC_DEF("getScrollX", 0, js_lv_obj_get_scroll_x),
    JS_CFUNC_DEF("getScrollY", 0, js_lv_obj_get_scroll_y),
    JS_CFUNC_DEF("getScrollTop", 0, js_lv_obj_get_scroll_top),
    JS_CFUNC_DEF("getScrollBottom", 0, js_lv_obj_get_scroll_bottom),
    JS_CFUNC_DEF("getScrollLeft", 0, js_lv_obj_get_scroll_left),
    JS_CFUNC_DEF("getScrollRight", 0, js_lv_obj_get_scroll_right),
    JS_CFUNC_DEF("getScrollEnd", 0, js_lv_obj_get_scroll_end),
    JS_CFUNC_DEF("scrollBy", 0, js_lv_obj_scroll_by),
    JS_CFUNC_DEF("scrollTo", 0, js_lv_obj_scroll_to),
    JS_CFUNC_DEF("scrollToX", 0, js_lv_obj_scroll_to_x),
    JS_CFUNC_DEF("scrollToY", 0, js_lv_obj_scroll_to_y),
    JS_CFUNC_DEF("scrollToView", 0, js_lv_obj_scroll_to_view),
    JS_CFUNC_DEF("scrollToViewRecursive", 0, js_lv_obj_scroll_to_view_recursive),
    JS_CFUNC_DEF("isScrolling", 0, js_lv_obj_is_scrolling),
    JS_CFUNC_DEF("updateSnap", 0, js_lv_obj_update_snap),
    JS_CFUNC_DEF("getScrollbarArea", 0, js_lv_obj_get_scrollbar_area),
    JS_CFUNC_DEF("scrollbarInvalidate", 0, js_lv_obj_scrollbar_invalidate),
    JS_CFUNC_DEF("readjustScroll", 0, js_lv_obj_readjust_scroll),
    JS_CFUNC_DEF("fadeIn", 0, js_lv_obj_fade_in),
    JS_CFUNC_DEF("fadeOut", 0, js_lv_obj_fade_out),
    JS_CFUNC_DEF("moveToIndex", 0, js_lv_obj_move_to_index),
    JS_CFUNC_DEF("getScreen", 0, js_lv_obj_get_screen),
    JS_CFUNC_DEF("getParent", 0, js_lv_obj_get_parent),
    JS_CFUNC_DEF("getChild", 0, js_lv_obj_get_child),
    JS_CFUNC_DEF("getChildCnt", 0, js_lv_obj_get_child_cnt),
    JS_CFUNC_DEF("getIndex", 0, js_lv_obj_get_index),
    JS_CFUNC_DEF("setTile", 0, js_lv_obj_set_tile),
    JS_CFUNC_DEF("setTileId", 0, js_lv_obj_set_tile_id),
    JS_CFUNC_DEF("moveForeground", 0, js_lv_obj_move_foreground),
    JS_CFUNC_DEF("moveBackground", 0, js_lv_obj_move_background),
    JS_CFUNC_DEF("getChildId", 0, js_lv_obj_get_child_id),
} ;
#define __def_js_lv_obj_proto_funcs__

// AUTO GENERATE CODE END [METHOD LIST] --------



static const JSCFunctionListEntry js_lv_arc_proto_funcs[] = {} ;
static const JSCFunctionListEntry js_lv_bar_proto_funcs[] = {} ;
static const JSCFunctionListEntry js_lv_btn_proto_funcs[] = {} ;
static const JSCFunctionListEntry js_lv_btnmatrix_proto_funcs[] = {} ;
static const JSCFunctionListEntry js_lv_canvas_proto_funcs[] = {} ;
static const JSCFunctionListEntry js_lv_checkbox_proto_funcs[] = {} ;
static const JSCFunctionListEntry js_lv_dropdown_proto_funcs[] = {} ;
static const JSCFunctionListEntry js_lv_img_proto_funcs[] = {} ;
static const JSCFunctionListEntry js_lv_label_proto_funcs[] = {} ;
static const JSCFunctionListEntry js_lv_line_proto_funcs[] = {} ;
static const JSCFunctionListEntry js_lv_roller_proto_funcs[] = {} ;
static const JSCFunctionListEntry js_lv_slider_proto_funcs[] = {} ;
static const JSCFunctionListEntry js_lv_switch_proto_funcs[] = {} ;
static const JSCFunctionListEntry js_lv_table_proto_funcs[] = {} ;
static const JSCFunctionListEntry js_lv_textarea_proto_funcs[] = {} ;





void require_vlgl_js_widgets(JSContext *ctx, JSValue lvgl) {

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beapi = JS_GetPropertyStr(ctx, global, "beapi") ;
    JSValue EventEmitter = JS_GetPropertyStr(ctx, beapi, "EventEmitter") ;
    JSValue EventEmitterProto = JS_GetPropertyStr(ctx, EventEmitter, "prototype") ;



// AUTO GENERATE CODE START [REGISTER CLASS] --------
    // define js class lvgl.lv_obj
    JS_NewClass(JS_GetRuntime(ctx), js_lv_obj_class_id, &js_lv_obj_class);
    JSValue proto_lv_obj = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto_lv_obj, js_lv_obj_proto_funcs, countof(js_lv_obj_proto_funcs));
    JSValue jscotr_lv_obj = JS_NewCFunction2(ctx, js_lv_obj_constructor, "lvgl.Obj", 1, JS_CFUNC_constructor, 0) ;
    JS_SetConstructor(ctx, jscotr_lv_obj, proto_lv_obj) ;
    JS_SetPropertyStr(ctx, lvgl, "Obj", jscotr_lv_obj);
    JS_SetPropertyStr(ctx, proto_lv_obj, "__proto__", EventEmitterProto);
    JS_SetClassProto(ctx, js_lv_obj_class_id, proto_lv_obj);

    // define js class lvgl.lv_arc
    JS_NewClass(JS_GetRuntime(ctx), js_lv_arc_class_id, &js_lv_arc_class);
    JSValue proto_lv_arc = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto_lv_arc, js_lv_arc_proto_funcs, countof(js_lv_arc_proto_funcs));
    JSValue jscotr_lv_arc = JS_NewCFunction2(ctx, js_lv_arc_constructor, "lvgl.Arc", 1, JS_CFUNC_constructor, 0) ;
    JS_SetConstructor(ctx, jscotr_lv_arc, proto_lv_arc) ;
    JS_SetPropertyStr(ctx, lvgl, "Arc", jscotr_lv_arc);
    JS_SetPropertyStr(ctx, proto_lv_arc, "__proto__", proto_lv_obj);
    JS_SetClassProto(ctx, js_lv_arc_class_id, proto_lv_arc);

    // define js class lvgl.lv_bar
    JS_NewClass(JS_GetRuntime(ctx), js_lv_bar_class_id, &js_lv_bar_class);
    JSValue proto_lv_bar = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto_lv_bar, js_lv_bar_proto_funcs, countof(js_lv_bar_proto_funcs));
    JSValue jscotr_lv_bar = JS_NewCFunction2(ctx, js_lv_bar_constructor, "lvgl.Bar", 1, JS_CFUNC_constructor, 0) ;
    JS_SetConstructor(ctx, jscotr_lv_bar, proto_lv_bar) ;
    JS_SetPropertyStr(ctx, lvgl, "Bar", jscotr_lv_bar);
    JS_SetPropertyStr(ctx, proto_lv_bar, "__proto__", proto_lv_obj);
    JS_SetClassProto(ctx, js_lv_bar_class_id, proto_lv_bar);

    // define js class lvgl.lv_btn
    JS_NewClass(JS_GetRuntime(ctx), js_lv_btn_class_id, &js_lv_btn_class);
    JSValue proto_lv_btn = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto_lv_btn, js_lv_btn_proto_funcs, countof(js_lv_btn_proto_funcs));
    JSValue jscotr_lv_btn = JS_NewCFunction2(ctx, js_lv_btn_constructor, "lvgl.Btn", 1, JS_CFUNC_constructor, 0) ;
    JS_SetConstructor(ctx, jscotr_lv_btn, proto_lv_btn) ;
    JS_SetPropertyStr(ctx, lvgl, "Btn", jscotr_lv_btn);
    JS_SetPropertyStr(ctx, proto_lv_btn, "__proto__", proto_lv_obj);
    JS_SetClassProto(ctx, js_lv_btn_class_id, proto_lv_btn);

    // define js class lvgl.lv_btnmatrix
    JS_NewClass(JS_GetRuntime(ctx), js_lv_btnmatrix_class_id, &js_lv_btnmatrix_class);
    JSValue proto_lv_btnmatrix = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto_lv_btnmatrix, js_lv_btnmatrix_proto_funcs, countof(js_lv_btnmatrix_proto_funcs));
    JSValue jscotr_lv_btnmatrix = JS_NewCFunction2(ctx, js_lv_btnmatrix_constructor, "lvgl.BtnMatrix", 1, JS_CFUNC_constructor, 0) ;
    JS_SetConstructor(ctx, jscotr_lv_btnmatrix, proto_lv_btnmatrix) ;
    JS_SetPropertyStr(ctx, lvgl, "BtnMatrix", jscotr_lv_btnmatrix);
    JS_SetPropertyStr(ctx, proto_lv_btnmatrix, "__proto__", proto_lv_obj);
    JS_SetClassProto(ctx, js_lv_btnmatrix_class_id, proto_lv_btnmatrix);

    // define js class lvgl.lv_canvas
    JS_NewClass(JS_GetRuntime(ctx), js_lv_canvas_class_id, &js_lv_canvas_class);
    JSValue proto_lv_canvas = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto_lv_canvas, js_lv_canvas_proto_funcs, countof(js_lv_canvas_proto_funcs));
    JSValue jscotr_lv_canvas = JS_NewCFunction2(ctx, js_lv_canvas_constructor, "lvgl.Canvas", 1, JS_CFUNC_constructor, 0) ;
    JS_SetConstructor(ctx, jscotr_lv_canvas, proto_lv_canvas) ;
    JS_SetPropertyStr(ctx, lvgl, "Canvas", jscotr_lv_canvas);
    JS_SetPropertyStr(ctx, proto_lv_canvas, "__proto__", proto_lv_obj);
    JS_SetClassProto(ctx, js_lv_canvas_class_id, proto_lv_canvas);

    // define js class lvgl.lv_checkbox
    JS_NewClass(JS_GetRuntime(ctx), js_lv_checkbox_class_id, &js_lv_checkbox_class);
    JSValue proto_lv_checkbox = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto_lv_checkbox, js_lv_checkbox_proto_funcs, countof(js_lv_checkbox_proto_funcs));
    JSValue jscotr_lv_checkbox = JS_NewCFunction2(ctx, js_lv_checkbox_constructor, "lvgl.Checkbox", 1, JS_CFUNC_constructor, 0) ;
    JS_SetConstructor(ctx, jscotr_lv_checkbox, proto_lv_checkbox) ;
    JS_SetPropertyStr(ctx, lvgl, "Checkbox", jscotr_lv_checkbox);
    JS_SetPropertyStr(ctx, proto_lv_checkbox, "__proto__", proto_lv_obj);
    JS_SetClassProto(ctx, js_lv_checkbox_class_id, proto_lv_checkbox);

    // define js class lvgl.lv_dropdown
    JS_NewClass(JS_GetRuntime(ctx), js_lv_dropdown_class_id, &js_lv_dropdown_class);
    JSValue proto_lv_dropdown = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto_lv_dropdown, js_lv_dropdown_proto_funcs, countof(js_lv_dropdown_proto_funcs));
    JSValue jscotr_lv_dropdown = JS_NewCFunction2(ctx, js_lv_dropdown_constructor, "lvgl.Dropdown", 1, JS_CFUNC_constructor, 0) ;
    JS_SetConstructor(ctx, jscotr_lv_dropdown, proto_lv_dropdown) ;
    JS_SetPropertyStr(ctx, lvgl, "Dropdown", jscotr_lv_dropdown);
    JS_SetPropertyStr(ctx, proto_lv_dropdown, "__proto__", proto_lv_obj);
    JS_SetClassProto(ctx, js_lv_dropdown_class_id, proto_lv_dropdown);

    // define js class lvgl.lv_img
    JS_NewClass(JS_GetRuntime(ctx), js_lv_img_class_id, &js_lv_img_class);
    JSValue proto_lv_img = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto_lv_img, js_lv_img_proto_funcs, countof(js_lv_img_proto_funcs));
    JSValue jscotr_lv_img = JS_NewCFunction2(ctx, js_lv_img_constructor, "lvgl.Img", 1, JS_CFUNC_constructor, 0) ;
    JS_SetConstructor(ctx, jscotr_lv_img, proto_lv_img) ;
    JS_SetPropertyStr(ctx, lvgl, "Img", jscotr_lv_img);
    JS_SetPropertyStr(ctx, proto_lv_img, "__proto__", proto_lv_obj);
    JS_SetClassProto(ctx, js_lv_img_class_id, proto_lv_img);

    // define js class lvgl.lv_label
    JS_NewClass(JS_GetRuntime(ctx), js_lv_label_class_id, &js_lv_label_class);
    JSValue proto_lv_label = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto_lv_label, js_lv_label_proto_funcs, countof(js_lv_label_proto_funcs));
    JSValue jscotr_lv_label = JS_NewCFunction2(ctx, js_lv_label_constructor, "lvgl.Label", 1, JS_CFUNC_constructor, 0) ;
    JS_SetConstructor(ctx, jscotr_lv_label, proto_lv_label) ;
    JS_SetPropertyStr(ctx, lvgl, "Label", jscotr_lv_label);
    JS_SetPropertyStr(ctx, proto_lv_label, "__proto__", proto_lv_obj);
    JS_SetClassProto(ctx, js_lv_label_class_id, proto_lv_label);

    // define js class lvgl.lv_line
    JS_NewClass(JS_GetRuntime(ctx), js_lv_line_class_id, &js_lv_line_class);
    JSValue proto_lv_line = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto_lv_line, js_lv_line_proto_funcs, countof(js_lv_line_proto_funcs));
    JSValue jscotr_lv_line = JS_NewCFunction2(ctx, js_lv_line_constructor, "lvgl.Line", 1, JS_CFUNC_constructor, 0) ;
    JS_SetConstructor(ctx, jscotr_lv_line, proto_lv_line) ;
    JS_SetPropertyStr(ctx, lvgl, "Line", jscotr_lv_line);
    JS_SetPropertyStr(ctx, proto_lv_line, "__proto__", proto_lv_obj);
    JS_SetClassProto(ctx, js_lv_line_class_id, proto_lv_line);

    // define js class lvgl.lv_roller
    JS_NewClass(JS_GetRuntime(ctx), js_lv_roller_class_id, &js_lv_roller_class);
    JSValue proto_lv_roller = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto_lv_roller, js_lv_roller_proto_funcs, countof(js_lv_roller_proto_funcs));
    JSValue jscotr_lv_roller = JS_NewCFunction2(ctx, js_lv_roller_constructor, "lvgl.Roller", 1, JS_CFUNC_constructor, 0) ;
    JS_SetConstructor(ctx, jscotr_lv_roller, proto_lv_roller) ;
    JS_SetPropertyStr(ctx, lvgl, "Roller", jscotr_lv_roller);
    JS_SetPropertyStr(ctx, proto_lv_roller, "__proto__", proto_lv_obj);
    JS_SetClassProto(ctx, js_lv_roller_class_id, proto_lv_roller);

    // define js class lvgl.lv_slider
    JS_NewClass(JS_GetRuntime(ctx), js_lv_slider_class_id, &js_lv_slider_class);
    JSValue proto_lv_slider = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto_lv_slider, js_lv_slider_proto_funcs, countof(js_lv_slider_proto_funcs));
    JSValue jscotr_lv_slider = JS_NewCFunction2(ctx, js_lv_slider_constructor, "lvgl.Slider", 1, JS_CFUNC_constructor, 0) ;
    JS_SetConstructor(ctx, jscotr_lv_slider, proto_lv_slider) ;
    JS_SetPropertyStr(ctx, lvgl, "Slider", jscotr_lv_slider);
    JS_SetPropertyStr(ctx, proto_lv_slider, "__proto__", proto_lv_obj);
    JS_SetClassProto(ctx, js_lv_slider_class_id, proto_lv_slider);

    // define js class lvgl.lv_switch
    JS_NewClass(JS_GetRuntime(ctx), js_lv_switch_class_id, &js_lv_switch_class);
    JSValue proto_lv_switch = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto_lv_switch, js_lv_switch_proto_funcs, countof(js_lv_switch_proto_funcs));
    JSValue jscotr_lv_switch = JS_NewCFunction2(ctx, js_lv_switch_constructor, "lvgl.Switch", 1, JS_CFUNC_constructor, 0) ;
    JS_SetConstructor(ctx, jscotr_lv_switch, proto_lv_switch) ;
    JS_SetPropertyStr(ctx, lvgl, "Switch", jscotr_lv_switch);
    JS_SetPropertyStr(ctx, proto_lv_switch, "__proto__", proto_lv_obj);
    JS_SetClassProto(ctx, js_lv_switch_class_id, proto_lv_switch);

    // define js class lvgl.lv_table
    JS_NewClass(JS_GetRuntime(ctx), js_lv_table_class_id, &js_lv_table_class);
    JSValue proto_lv_table = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto_lv_table, js_lv_table_proto_funcs, countof(js_lv_table_proto_funcs));
    JSValue jscotr_lv_table = JS_NewCFunction2(ctx, js_lv_table_constructor, "lvgl.Table", 1, JS_CFUNC_constructor, 0) ;
    JS_SetConstructor(ctx, jscotr_lv_table, proto_lv_table) ;
    JS_SetPropertyStr(ctx, lvgl, "Table", jscotr_lv_table);
    JS_SetPropertyStr(ctx, proto_lv_table, "__proto__", proto_lv_obj);
    JS_SetClassProto(ctx, js_lv_table_class_id, proto_lv_table);

    // define js class lvgl.lv_textarea
    JS_NewClass(JS_GetRuntime(ctx), js_lv_textarea_class_id, &js_lv_textarea_class);
    JSValue proto_lv_textarea = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto_lv_textarea, js_lv_textarea_proto_funcs, countof(js_lv_textarea_proto_funcs));
    JSValue jscotr_lv_textarea = JS_NewCFunction2(ctx, js_lv_textarea_constructor, "lvgl.TextArea", 1, JS_CFUNC_constructor, 0) ;
    JS_SetConstructor(ctx, jscotr_lv_textarea, proto_lv_textarea) ;
    JS_SetPropertyStr(ctx, lvgl, "TextArea", jscotr_lv_textarea);
    JS_SetPropertyStr(ctx, proto_lv_textarea, "__proto__", proto_lv_obj);
    JS_SetClassProto(ctx, js_lv_textarea_class_id, proto_lv_textarea);

// AUTO GENERATE CODE END [REGISTER CLASS] --------


    JS_FreeValue(ctx, global);
    JS_FreeValue(ctx, beapi);
    JS_FreeValue(ctx, EventEmitter);
    JS_FreeValue(ctx, EventEmitterProto);
}



void vlgl_js_widgets_init() {
    // class id 全局, 分配一次
// AUTO GENERATE CODE START [REGISTER CLASS ID] --------
    JS_NewClassID(&js_lv_obj_class_id);
    JS_NewClassID(&js_lv_arc_class_id);
    JS_NewClassID(&js_lv_bar_class_id);
    JS_NewClassID(&js_lv_btn_class_id);
    JS_NewClassID(&js_lv_btnmatrix_class_id);
    JS_NewClassID(&js_lv_canvas_class_id);
    JS_NewClassID(&js_lv_checkbox_class_id);
    JS_NewClassID(&js_lv_dropdown_class_id);
    JS_NewClassID(&js_lv_img_class_id);
    JS_NewClassID(&js_lv_label_class_id);
    JS_NewClassID(&js_lv_line_class_id);
    JS_NewClassID(&js_lv_roller_class_id);
    JS_NewClassID(&js_lv_slider_class_id);
    JS_NewClassID(&js_lv_switch_class_id);
    JS_NewClassID(&js_lv_table_class_id);
    JS_NewClassID(&js_lv_textarea_class_id);
// AUTO GENERATE CODE END [REGISTER CLASS ID] --------
}