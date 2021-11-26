#include "widgets.h"

#include "utils.h"
#include "cutils.h"




static JSClassID js_lvgl_object_class_id ;
static JSValue js_lvgl_object_constructor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    printf("js_lvgl_object_constructor()\n") ;

    

    // JS_SetOpaque(val, disp);
    
    return JS_UNDEFINED ;
}
static void js_lvgl_object_finalizer(JSRuntime *rt, JSValue val){
    printf("js_lvgl_object_finalizer()\n") ;
}
static JSClassDef js_lvgl_object_class = {
    "lvgl.Object",
    .call = js_lvgl_object_constructor,
    .finalizer = js_lvgl_object_finalizer,
};


static JSValue js_lvgl_object_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}
static JSValue js_lvgl_object_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}
static JSValue js_lvgl_object_set_size(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}


static const JSCFunctionListEntry js_lvgl_object_proto_funcs[] = {
    JS_CFUNC_DEF("height", 0, js_lvgl_object_height),
    JS_CFUNC_DEF("width", 0, js_lvgl_object_width),
    JS_CFUNC_DEF("setSize", 0, js_lvgl_object_set_size),
};



void vlgl_js_widgets_init() {
    // class id 全局, 分配一次
    JS_NewClassID(&js_lvgl_object_class_id);
}

void require_vlgl_js_widgets(JSContext *ctx, JSValue lvgl) {

    {
        JS_NewClass(JS_GetRuntime(ctx), js_lvgl_object_class_id, &js_lvgl_object_class);
        JSValue proto = JS_NewObject(ctx);
        JS_SetPropertyFunctionList(ctx, proto, js_lvgl_object_proto_funcs, countof(js_lvgl_object_proto_funcs));
        JS_SetClassProto(ctx, js_lvgl_object_class_id, proto);

        JSValue jsctro = JS_NewCFunction2(ctx, js_lvgl_object_constructor, "lvgl.Object", 1, JS_CFUNC_constructor, 0) ;
        JS_SetConstructor(ctx, jsctro, proto) ;
        JS_SetPropertyStr(ctx, lvgl, "Object", jsctro);
    }

}