#include "module_mg.h"
#include "utils.h"
#include "cutils.h"
#include "mongoose.h"




static JSClassID js_mg_mgr_class_id ;
static void js_mg_mgr_finalizer(JSRuntime *rt, JSValue val){
    printf("js_mg_mgr_finalizer()") ;
}
static JSClassDef js_mg_mgr_class = {
    "mg.Mgr",
    .finalizer = js_mg_mgr_finalizer,
} ;
static const JSCFunctionListEntry js_mg_mgr_proto_funcs[] = {
    // JS_CFUNC_DEF("xxxx", 0, js_xxxx),
} ;

static JSValue js_mg_listen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    JSValue jsobj = JS_NewObjectClass(ctx, js_mg_mgr_class_id) ;
    return jsobj ;
}




void be_module_mg_init() {
    JS_NewClassID(&js_mg_mgr_class_id);
}

void be_module_mg_require(JSContext *ctx) {
    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue mg = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "mg", mg);


    qjs_def_class(ctx, "Roller", js_mg_mgr_class_id, &js_mg_mgr_class
                , "mg.Mgr", NULL, js_mg_mgr_proto_funcs, countof(js_mg_mgr_proto_funcs), JS_UNDEFINED, beapi) ;

    
    JS_SetPropertyStr(ctx, mg, "listen", JS_NewCFunction(ctx, js_mg_listen, "listen", 1));
    
    JS_FreeValue(ctx, beapi);
}

void be_module_mg_loop(JSContext *ctx) {
}

void be_module_mg_reset(JSContext *ctx) {
}








