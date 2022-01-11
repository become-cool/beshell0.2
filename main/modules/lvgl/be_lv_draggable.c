#include "be_lv_draggable.h"
#include "lvgl.h"
#include "utils.h"
#include "cutils.h"
#include "widgets.h"
#include "widgets_gen.h"
#include "module_lvgl.h"


typedef struct {
    lv_obj_t * follower ;
    lv_coord_t ox ;
    lv_coord_t oy ;
} be_lv_draggable_follower_t ;

typedef struct {

    JSContext * ctx ;

    JSValue jstarget ;
    lv_obj_t * target ;
    lv_coord_t ox ;
    lv_coord_t oy ;

    bool enabled ;
    bool dragging ;

    be_lv_draggable_follower_t * followers ;
    uint16_t follower_cnt ;

    JSValue callback_start ;
    JSValue callback_dragging ;
    JSValue callback_stop ;

    JSValue pos ;

} be_lv_draggable_t ;

static void clear_followers(be_lv_draggable_t * draggable) {
    // printf("clear_followers()\n") ;
    // for(int i=0;i<draggable->follower_cnt;i++) {
        // JS_FreeValue(draggable->ctx, draggable->followers[i].target) ;
    // }
    if(draggable->followers) {
        free(draggable->followers) ;
        draggable->followers = NULL ;
    }
    draggable->follower_cnt = 0 ;
}

static void js_lv_draggable_start(lv_event_t * event) {
    if(!event->user_data) {
        return ;
    }
    be_lv_draggable_t * draggable = event->user_data ;
    if(!draggable->enabled || draggable->dragging) {
        return ;
    }
    lv_area_t coords ;
    lv_obj_get_coords(draggable->target, &coords) ;

    draggable->ox = coords.x1 - indev_input_x ;
    draggable->oy = coords.y1 - indev_input_y ;

    if( JS_IsFunction(draggable->ctx, draggable->callback_start) ) {
        JSValue ret = JS_Call(draggable->ctx, draggable->callback_start, draggable->jstarget, 0, NULL) ;   
        if(JS_IsException(ret)) {
            js_std_dump_error(draggable->ctx) ;
        }
        else if( JS_IsFalse(draggable ->ctx, ret) ) {
            JS_FreeValue(draggable->ctx, ret) ;
            return ;
        }
        JS_FreeValue(draggable->ctx, ret) ;
    }

    draggable->dragging = true ;
}

static void js_lv_draggable_stop(lv_event_t * event) {
    if(!event->user_data) {
        return ;
    }
    be_lv_draggable_t * draggable = event->user_data ;
    if(!draggable->enabled || !draggable->dragging) {
        return ;
    }

    // call onstop
    if( JS_IsFunction(draggable->ctx, draggable->callback_stop) ) {
        JSValue jsfalse = JS_FALSE ;
        JSValue ret = JS_Call(draggable->ctx, draggable->callback_stop, draggable->jstarget, 1, &jsfalse) ;
        if(JS_IsException(ret)){
            js_std_dump_error(draggable->ctx) ;
        }
        JS_FreeValue(draggable->ctx, ret) ;
    }

    draggable->dragging = false ;
}

static void js_lv_draggable_dragging(lv_event_t * event) {
    if(!event->user_data) {
        return ;
    }
    be_lv_draggable_t * draggable = event->user_data ;
    if(!draggable->enabled || !draggable->dragging) {
        return ;
    }
    static bool use_x = true ;
    static bool use_y = true ;
    static int32_t x = 0 ;
    static int32_t y = 0 ;
    
    use_x = use_y = true ;
    x = indev_input_x + draggable->ox ;
    y = indev_input_y + draggable->oy ;

    // call ondragging
    if( JS_IsFunction(draggable->ctx, draggable->callback_dragging)  ) {
        JS_SetPropertyStr(draggable->ctx, draggable->pos, "x", JS_NewInt32(draggable->ctx, x)) ;
        JS_SetPropertyStr(draggable->ctx, draggable->pos, "y", JS_NewInt32(draggable->ctx, y)) ;

        JSValue ret = JS_Call(draggable->ctx, draggable->callback_dragging, draggable->jstarget, 1, &draggable->pos) ;
        if(JS_IsException(ret)) {
            js_std_dump_error(draggable->ctx) ;
        }
        else if( JS_IsFalse(draggable ->ctx, ret) ) {
            JS_FreeValue(draggable->ctx, ret) ;
            // call onstop
            if( JS_IsFunction(draggable->ctx, draggable->callback_stop) ) {
                JSValue jstrue = JS_TRUE ;
                ret = JS_Call(draggable->ctx, draggable->callback_stop, draggable->jstarget, 1, &jstrue) ;
                if(JS_IsException(ret)){
                    js_std_dump_error(draggable->ctx) ;
                }
                JS_FreeValue(draggable->ctx, ret) ;
            }
        
            return ;
        }
        JS_FreeValue(draggable->ctx, ret) ;

        JSValue jsx = JS_GetPropertyStr(draggable->ctx, draggable->pos, "x") ;
        if( JS_IsFalse(draggable->ctx,jsx) || JS_ToInt32(draggable->ctx, &x, jsx)!=0 ) {
            use_x = false ;
        }
        JS_FreeValue(draggable->ctx, jsx) ;
        
        JSValue jsy = JS_GetPropertyStr(draggable->ctx, draggable->pos, "y") ;
        if( JS_IsFalse(draggable->ctx,jsy) || JS_ToInt32(draggable->ctx, &y, jsy)!=0 ) {
            use_y = false ;
        }
        JS_FreeValue(draggable->ctx, jsy) ;
    }

    // self
    if(use_x) {
        lv_obj_set_coord_x(draggable->target, x) ;
    }
    if(use_y) {
        lv_obj_set_coord_y(draggable->target, y) ;
    }
    // if(use_x || use_y) {
    //     lv_obj_update_layout(draggable->target) ;
    // }

    // followers
    for(int i=0; i<draggable->follower_cnt; i++) {
        if(use_x) {
            lv_obj_set_coord_x(draggable->followers[i].follower, x + draggable->followers[i].ox) ;
        }
        if(use_y) {
            lv_obj_set_coord_y(draggable->followers[i].follower, y + draggable->followers[i].oy) ;
        }
    }
}

static JSClassID js_lv_draggable_class_id ;
static JSValue js_lv_draggable_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){

    CHECK_ARGC(1)
    CHECK_INSOF_LVOBJ("Obj", argv[0], "arg widget must a lvgl.Obj")
    lv_obj_t * target = JS_GetOpaqueInternal(argv[0]) ;
    if(!target) {
        THROW_EXCEPTION("arg widget must a lvgl.Obj")
    }

    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_draggable_class_id) ;

    be_lv_draggable_t * draggable = malloc(sizeof(be_lv_draggable_t)) ;
    draggable->jstarget = JS_DupValue(ctx, argv[0]);
    draggable->target = target ;
    draggable->ox = 0 ;
    draggable->oy = 0 ;
    draggable->ctx = ctx ;
    draggable->enabled = true ;
    draggable->dragging = false ;
    draggable->followers = NULL ;
    draggable->follower_cnt = 0 ;
    draggable->callback_start = JS_UNDEFINED ;
    draggable->callback_dragging = JS_UNDEFINED ;
    draggable->callback_stop = JS_UNDEFINED ;
    draggable->pos = JS_NewObject(ctx) ;

    JS_SetOpaque(jsobj, draggable) ;

    lv_obj_add_flag(target, LV_OBJ_FLAG_CLICKABLE) ;
    lv_obj_add_event_cb(target, js_lv_draggable_start, LV_EVENT_PRESSED, draggable) ;
    lv_obj_add_event_cb(target, js_lv_draggable_dragging, LV_EVENT_PRESSING, draggable) ;
    lv_obj_add_event_cb(target, js_lv_draggable_stop, LV_EVENT_RELEASED, draggable) ;

    return JS_DupValue(ctx, jsobj) ;
}
static void js_lv_draggable_finalizer(JSRuntime *rt, JSValue this_val){
    be_lv_draggable_t * draggable = JS_GetOpaque(this_val, js_lv_draggable_class_id) ;
    if(draggable) {
        JS_FreeValue(draggable->ctx, draggable->jstarget) ;
        JS_FreeValue(draggable->ctx, draggable->pos) ;
        JS_FreeValue(draggable->ctx, draggable->callback_start) ;
        JS_FreeValue(draggable->ctx, draggable->callback_dragging) ;
        JS_FreeValue(draggable->ctx, draggable->callback_stop) ;
        clear_followers(draggable) ;
        free(draggable) ;
    }
}
static JSClassDef js_lv_draggable_class = {
    "js_full_name",
    .finalizer = js_lv_draggable_finalizer,
} ;

#define FETCH_DRAGGABLE(methodName, var)                                                \
    {                                                                                   \
        JSValue ObjCotr = js_get_glob_prop(ctx, 3, "beapi", "lvgl", "Draggable");       \
        if(!JS_IsInstanceOf(ctx, this_val, ObjCotr)) {                                  \
            JS_FreeValue(ctx,ObjCotr) ;                                                 \
            THROW_EXCEPTION("%s() must be called with a Draggable object", #methodName) \
        }                                                                               \
        JS_FreeValue(ctx,ObjCotr) ;                                                     \
    }                                                                                   \
    be_lv_draggable_t * var = JS_GetOpaque(this_val, js_lv_draggable_class_id) ;        \
    if(!var) {                                                                          \
        THROW_EXCEPTION("%s() must be called with a Draggable object", #methodName)     \
    }

static JSValue js_lv_draggable_set_start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    FETCH_DRAGGABLE("setsStart", draggable)

    JS_FreeValue(ctx, draggable->callback_start) ;
    draggable->callback_start = JS_DupValue(ctx, argv[0]) ;

    return JS_UNDEFINED ;
}
static JSValue js_lv_draggable_set_dragging(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    FETCH_DRAGGABLE("setDragging", draggable)

    JS_FreeValue(ctx, draggable->callback_dragging) ;
    draggable->callback_dragging = JS_DupValue(ctx, argv[0]) ;

    return JS_UNDEFINED ;
}
static JSValue js_lv_draggable_set_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    FETCH_DRAGGABLE("setStop", draggable)

    JS_FreeValue(ctx, draggable->callback_stop) ;
    draggable->callback_stop = JS_DupValue(ctx, argv[0]) ;

    return JS_UNDEFINED ;
}
static JSValue js_lv_draggable_enable(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    FETCH_DRAGGABLE("enable", draggable)
    draggable->enabled = true ;
    return JS_UNDEFINED ;
}
static JSValue js_lv_draggable_disable(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    FETCH_DRAGGABLE("disable", draggable)
    draggable->enabled = false ;
    return JS_UNDEFINED ;
}
static JSValue js_lv_draggable_set_followers(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    FETCH_DRAGGABLE("setFollowers", draggable)

    clear_followers(draggable) ;

    if( JS_IsNull(argv[0]) ) {
        return ;
    }

    if(!JS_IsArray(ctx, argv[0])) {
        THROW_EXCEPTION("arg widgets must be an array")
    }
    JSValue jslen = JS_GetPropertyStr(ctx, argv[0], "length") ;
    uint32_t len = 0 ;
    JS_ToUint32(ctx, &len, jslen) ;

    draggable->follower_cnt = len ;
    
    if(len==0) {
        return JS_UNDEFINED ;
    }

    draggable->followers = malloc(sizeof(be_lv_draggable_follower_t) * len) ;
    if(!draggable->followers) {
        draggable->follower_cnt = 0 ;
        THROW_EXCEPTION("out of memory")
    }
    
    lv_area_t target_coords ;
    lv_obj_get_coords(draggable->target, &target_coords) ;

    JSValue ObjCotr = js_get_glob_prop(ctx, 3, "beapi", "lvgl", "Obj") ;
    for(int i=0;i<len;i++){
        JSValue jswidget = JS_GetPropertyUint32(ctx, argv[0], i) ;
        if(!JS_IsInstanceOf(ctx, jswidget, ObjCotr)) {
            JS_FreeValue(ctx,jswidget) ;
            JS_FreeValue(ctx,ObjCotr) ;
            clear_followers(draggable) ;
            THROW_EXCEPTION("follow widget must be a lv.Obj") ;
        }

        lv_obj_t * widget = JS_GetOpaqueInternal(jswidget) ;
        JS_FreeValue(ctx,jswidget) ;

        if(!widget) {
            JS_FreeValue(ctx,jswidget) ;
            JS_FreeValue(ctx,ObjCotr) ;
            clear_followers(draggable) ;
            THROW_EXCEPTION("follow widget must be a valid lv.Obj") ;
        }

        lv_area_t follower_coords ;
        lv_obj_get_coords(widget, &follower_coords) ;

        draggable->followers[i].follower = widget ;
        draggable->followers[i].ox = follower_coords.x1 - target_coords.x1 ;
        draggable->followers[i].oy = follower_coords.y1 - target_coords.y1 ;

        // dn2(i, draggable->followers[i].ox)
        // dn2(i, draggable->followers[i].oy)
    }

    JS_FreeValue(ctx,ObjCotr) ;
    
    return JS_UNDEFINED ;
}

static const JSCFunctionListEntry js_lv_draggable_proto_funcs[] = {
    JS_CFUNC_DEF("setStart", 0, js_lv_draggable_set_start),
    JS_CFUNC_DEF("setDragging", 0, js_lv_draggable_set_dragging),
    JS_CFUNC_DEF("setStop", 0, js_lv_draggable_set_stop),
    JS_CFUNC_DEF("enable", 0, js_lv_draggable_enable),
    JS_CFUNC_DEF("disable", 0, js_lv_draggable_disable),
    JS_CFUNC_DEF("setFollowers", 0, js_lv_draggable_set_followers),
} ;



void be_lv_draggable_init() {
    JS_NewClassID(&js_lv_draggable_class_id);
}

void be_lv_draggable_require(JSContext *ctx, JSValue lvgl) {
    QJS_DEF_CLASS(lv_draggable, "Draggable", "lv.Draggable", JS_UNDEFINED, lvgl) ;
}


