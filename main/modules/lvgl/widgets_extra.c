#include "widgets_extra.h"
#include "utils.h"


void js_lv_obj_init(JSContext *ctx, JSValue jsobj) {
    
    JSValue jsFuncLvObjInit = js_get_glob_prop(ctx, 3, "beapi", "lvgl", "__lv_obj_init") ;

    JS_Call(ctx, jsFuncLvObjInit, jsobj, 0, NULL) ;

    JS_FreeValue(ctx, jsFuncLvObjInit) ;
}

JSValue js_lv_obj_wrapper(JSContext *ctx, lv_obj_t * cobj, JSClassID clsid) {

    JSValue jsobj = JS_UNDEFINED ;

    void * _jsobj = lv_obj_get_user_data(cobj) ;
    if(_jsobj) {
        jsobj = JS_MKPTR(JS_TAG_OBJECT, _jsobj) ;
    }
    else {
        jsobj = JS_NewObjectClass(ctx, clsid) ;

        lv_obj_set_user_data(cobj, JS_VALUE_GET_PTR(jsobj)) ;
        JS_SetOpaque(jsobj, cobj) ;        
        
        js_lv_obj_init(ctx, jsobj) ;

        JS_DupValue(ctx, jsobj) ;
    }
    
    return jsobj ;
}

static void js_lv_event_cb(lv_event_t * event) {

    if(!event->user_data) {
        printf("event->user_data==NULL, no ctx.") ;
        return ;
    }
    JSContext *ctx = (JSContext *)event->user_data ;
    if(!event->current_target->user_data) {
        return ;
    }
    JSValue jstarget = JS_MKPTR(JS_TAG_OBJECT, event->current_target->user_data) ;
    JSValue ObjCotr = js_get_glob_prop(ctx, 3, "beapi", "lvgl", "Obj") ;
    if(!JS_IsInstanceOf(ctx, jstarget, ObjCotr) ){
        printf("target is not a js lv object") ;
        return ;
    }

    JSValue jsname = lv_event_code_to_jsstr(ctx, event->code) ;

    // jstarget
    MAKE_ARGV1( cbargv, jsname )

    JSValue func_emit = js_get_prop(ctx, jstarget, 1, "emit") ;
    
    JS_Call(ctx, func_emit, jstarget, 1, cbargv) ;

    free(cbargv) ;
    JS_FreeValue(ctx, func_emit) ;
}

typedef struct _lv_event_dsc_t {
    lv_event_cb_t cb;
    void * user_data;
    lv_event_code_t filter : 8;
} lv_event_dsc_t;
static lv_event_dsc_t * find_event_dsc(lv_obj_t* obj, lv_event_code_t e) {
    if(!obj->spec_attr)
        return NULL ;

    if(!obj->spec_attr->event_dsc){
        return NULL ;
    }

    for(int i=0; i<obj->spec_attr->event_dsc_cnt; i++) {
        if(obj->spec_attr->event_dsc[i].cb!=js_lv_event_cb)
            continue ;
        if(obj->spec_attr->event_dsc[i].filter==LV_EVENT_ALL || obj->spec_attr->event_dsc[i].filter==e) {
            return & obj->spec_attr->event_dsc[i] ;
        }
    }
    
    return NULL ;
}

JSValue js_lv_obj_enable_event(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    THIS_LBOBJ("Obj", "enableEvent", thisobj)
    CHECK_ARGC(1)
    uint8_t eventcode ;
    if(!lv_event_jsstr_to_code(ctx, argv[0], &eventcode)) {
        return JS_EXCEPTION ;
    }

    if(find_event_dsc(thisobj, eventcode)!=NULL) {
        // printf("event aleady enabled \n") ;
        return JS_UNDEFINED ;
    }
    // printf("enable event: %d \n", eventcode) ;

    lv_obj_add_event_cb(thisobj, js_lv_event_cb, eventcode, ctx) ;
    return JS_UNDEFINED ;
}

JSValue js_lv_obj_disable_event(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    
    THIS_LBOBJ("Obj", "disableEvent", thisobj)
    CHECK_ARGC(1)
    uint8_t eventcode ;
    if(!lv_event_jsstr_to_code(ctx, argv[0], &eventcode)) {
        return JS_EXCEPTION ;
    }

    lv_event_dsc_t * event_dsc = find_event_dsc(thisobj, eventcode) ;
    if(event_dsc) {
        // printf("disable event: %d \n", eventcode) ;
        lv_obj_remove_event_dsc(thisobj, event_dsc) ;
    }
    return JS_UNDEFINED ;
}

JSValue js_lv_obj_is_screen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    THIS_LBOBJ("Obj", "isScreen", thisobj)
    return lv_obj_get_parent(thisobj)==NULL? JS_TRUE: JS_FALSE ;
}

JSValue js_lv_obj_ptr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_LBOBJ("Obj", "isScreen", thisobj)
    printf("%p\n", thisobj) ;
    return JS_NewUint32(ctx, (uint64_t)thisobj) ;
}

JSValue js_lv_obj_set_coords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    
    THIS_LBOBJ("Obj", "getCoords", thisobj)
    CHECK_ARGC(2)
dp(thisobj)

    ARGV_TO_INT16(0, x)
    ARGV_TO_INT16(1, y)
dp(thisobj)
    dn(thisobj->coords.x1)
    dn(thisobj->coords.y1)
    dn(thisobj->coords.x2)
    dn(thisobj->coords.y2)

    thisobj->coords.x2+= x - thisobj->coords.x1 ;
    thisobj->coords.y2+= y - thisobj->coords.y1 ;
    thisobj->coords.x1 = x ;
    thisobj->coords.y1 = y ;

    return JS_UNDEFINED ;
}
JSValue js_lv_obj_get_coords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    THIS_LBOBJ("Obj", "getCoords", thisobj)
    
    JSValue arr = JS_NewArray(ctx) ;
    JS_SetPropertyUint32(ctx, arr, 0, JS_NewInt32(ctx, thisobj->coords.x1));
    JS_SetPropertyUint32(ctx, arr, 1, JS_NewInt32(ctx, thisobj->coords.y1));
    
    return arr ;
}