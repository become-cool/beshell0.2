#include "widgets.h"

#include "utils.h"
#include "cutils.h"
#include "lvgl.h"
#include "lv_conf.h"




static JSClassID js_lv_obj_class_id ;
static JSValue js_lv_obj_constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    printf("js_lv_obj_constructor()\n") ;

    // JS_SetOpaque(val, disp);
    
    return JS_UNDEFINED ;
}
static void js_lv_obj_finalizer(JSRuntime *rt, JSValue val){
    printf("js_lv_obj_finalizer()\n") ;
}
static JSClassDef js_lv_obj_class = {
    "lvgl.Object",
    .call = js_lv_obj_constructor,
    .finalizer = js_lv_obj_finalizer,
};





// AUTO GENERATE CODE START --------

// AUTO GENERATE CODE END --------





void vlgl_js_widgets_init() {
    // class id 全局, 分配一次
    JS_NewClassID(&js_lv_obj_class_id);
}

void require_vlgl_js_widgets(JSContext *ctx, JSValue lvgl) {

    {
        JS_NewClass(JS_GetRuntime(ctx), js_lv_obj_class_id, &js_lv_obj_class);
        JSValue proto = JS_NewObject(ctx);
        JS_SetPropertyFunctionList(ctx, proto, js_lv_obj_proto_funcs, countof(js_lv_obj_proto_funcs));
        JS_SetClassProto(ctx, js_lv_obj_class_id, proto);

        JSValue jsctro = JS_NewCFunction2(ctx, js_lv_obj_constructor, "lvgl.Object", 1, JS_CFUNC_constructor, 0) ;
        JS_SetConstructor(ctx, jsctro, proto) ;
        JS_SetPropertyStr(ctx, lvgl, "Object", jsctro);
    }

}