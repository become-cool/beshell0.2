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

    JSValue jsname = lv_event_code_const_to_jsstr(ctx, event->code) ;

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
    if(!lv_event_code_jsstr_to_const(ctx, argv[0], &eventcode)) {
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
    if(!lv_event_code_jsstr_to_const(ctx, argv[0], &eventcode)) {
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
    CHECK_ARGC(2)
    ARGV_TO_INT16(0, x)
    ARGV_TO_INT16(1, y)
    
    THIS_LBOBJ("Obj", "setCoords", thisobj)

    int16_t dx = x - thisobj->coords.x1 ;
    int16_t dy = y - thisobj->coords.y1 ;
    
    lv_obj_set_x(thisobj, lv_obj_get_x_aligned(thisobj) + dx) ;
    lv_obj_set_y(thisobj, lv_obj_get_y_aligned(thisobj) + dy) ;
    
    return JS_UNDEFINED ;
}
JSValue js_lv_obj_get_coords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    THIS_LBOBJ("Obj", "getCoords", thisobj)
    
    JSValue arr = JS_NewArray(ctx) ;
    JS_SetPropertyUint32(ctx, arr, 0, JS_NewInt32(ctx, thisobj->coords.x1));
    JS_SetPropertyUint32(ctx, arr, 1, JS_NewInt32(ctx, thisobj->coords.y1));
    
    return arr ;
}
JSValue js_lv_obj_move(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_INT16(0, dx)
    ARGV_TO_INT16(1, dy)
    THIS_LBOBJ("Obj", "move", thisobj)
    lv_obj_set_x(thisobj, lv_obj_get_x_aligned(thisobj) + dx) ;
    lv_obj_set_y(thisobj, lv_obj_get_y_aligned(thisobj) + dy) ;
    return JS_UNDEFINED ;
}
JSValue js_lv_obj_move_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_INT16(0, delta)
    THIS_LBOBJ("Obj", "moveX", thisobj)
    lv_obj_set_x(thisobj, lv_obj_get_x_aligned(thisobj) + delta) ;
    return JS_UNDEFINED ;
}
JSValue js_lv_obj_move_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_INT16(0, delta)
    THIS_LBOBJ("Obj", "moveY", thisobj)
    lv_obj_set_y(thisobj, lv_obj_get_y_aligned(thisobj) + delta) ;
    return JS_UNDEFINED ;
}

JSValue lv_style_value_to_js(JSContext * ctx, lv_style_prop_t prop, lv_style_value_t value) {
    switch(prop) {
        case LV_STYLE_WIDTH:
        case LV_STYLE_MIN_WIDTH:
        case LV_STYLE_MAX_WIDTH:
        case LV_STYLE_HEIGHT:
        case LV_STYLE_MIN_HEIGHT:
        case LV_STYLE_MAX_HEIGHT:
        case LV_STYLE_X:
        case LV_STYLE_Y:
        case LV_STYLE_TRANSFORM_WIDTH:
        case LV_STYLE_TRANSFORM_HEIGHT:
        case LV_STYLE_TRANSLATE_X:
        case LV_STYLE_TRANSLATE_Y:
        case LV_STYLE_TRANSFORM_ZOOM:
        case LV_STYLE_TRANSFORM_ANGLE:
        case LV_STYLE_PAD_TOP:
        case LV_STYLE_PAD_BOTTOM:
        case LV_STYLE_PAD_LEFT:
        case LV_STYLE_PAD_RIGHT:
        case LV_STYLE_PAD_ROW:
        case LV_STYLE_PAD_COLUMN:
        case LV_STYLE_RADIUS:
        case LV_STYLE_BG_MAIN_STOP:
        case LV_STYLE_BG_GRAD_STOP:
        case LV_STYLE_BORDER_WIDTH:
        case LV_STYLE_TEXT_LETTER_SPACE:
        case LV_STYLE_TEXT_LINE_SPACE:
        case LV_STYLE_OUTLINE_WIDTH:
        case LV_STYLE_OUTLINE_PAD:
        case LV_STYLE_SHADOW_WIDTH:
        case LV_STYLE_SHADOW_OFS_X:
        case LV_STYLE_SHADOW_OFS_Y:
        case LV_STYLE_SHADOW_SPREAD:
        case LV_STYLE_LINE_WIDTH:
        case LV_STYLE_LINE_DASH_WIDTH:
        case LV_STYLE_LINE_DASH_GAP:
        case LV_STYLE_ARC_WIDTH:
            return JS_NewInt32(ctx, (int32_t)value.num) ;

        case LV_STYLE_ALIGN:
        case LV_STYLE_CLIP_CORNER:
        case LV_STYLE_OPA:
        case LV_STYLE_COLOR_FILTER_OPA:
        case LV_STYLE_ANIM_TIME:
        case LV_STYLE_ANIM_SPEED:
        case LV_STYLE_BLEND_MODE:
        case LV_STYLE_LAYOUT:
        case LV_STYLE_BASE_DIR:
        case LV_STYLE_BG_OPA:
        case LV_STYLE_BG_GRAD_DIR:
        case LV_STYLE_BG_IMG_OPA:
        case LV_STYLE_BG_IMG_RECOLOR_OPA:
        case LV_STYLE_BG_IMG_TILED:
        case LV_STYLE_BORDER_OPA:
        case LV_STYLE_BORDER_SIDE:
        case LV_STYLE_BORDER_POST:
        case LV_STYLE_TEXT_OPA:
        case LV_STYLE_TEXT_DECOR:
        case LV_STYLE_TEXT_ALIGN:
        case LV_STYLE_IMG_OPA:
        case LV_STYLE_IMG_RECOLOR_OPA:
        case LV_STYLE_OUTLINE_OPA:
        case LV_STYLE_SHADOW_OPA:
        case LV_STYLE_LINE_ROUNDED:
        case LV_STYLE_LINE_OPA:
        case LV_STYLE_ARC_ROUNDED:
        case LV_STYLE_ARC_OPA:
            return JS_NewUint32(ctx, value.num) ;

        case LV_STYLE_BG_COLOR:
        case LV_STYLE_BG_COLOR_FILTERED:
        case LV_STYLE_BG_GRAD_COLOR:
        case LV_STYLE_BG_GRAD_COLOR_FILTERED:
        case LV_STYLE_BG_IMG_RECOLOR:
        case LV_STYLE_BG_IMG_RECOLOR_FILTERED:
        case LV_STYLE_BORDER_COLOR:
        case LV_STYLE_BORDER_COLOR_FILTERED:
        case LV_STYLE_TEXT_COLOR:
        case LV_STYLE_TEXT_COLOR_FILTERED:
        case LV_STYLE_IMG_RECOLOR:
        case LV_STYLE_IMG_RECOLOR_FILTERED:
        case LV_STYLE_OUTLINE_COLOR:
        case LV_STYLE_OUTLINE_COLOR_FILTERED:
        case LV_STYLE_SHADOW_COLOR:
        case LV_STYLE_SHADOW_COLOR_FILTERED:
        case LV_STYLE_LINE_COLOR:
        case LV_STYLE_LINE_COLOR_FILTERED:
        case LV_STYLE_ARC_COLOR:
        case LV_STYLE_ARC_COLOR_FILTERED:
            return JS_NewUint32(ctx, value.color.full) ;
    }
    return JS_NewString(ctx, "unknow type") ;
}


bool lv_style_js_to_value(JSContext * ctx, lv_style_prop_t prop, JSValue jsval, lv_style_value_t * value) {
    switch(prop) {        case LV_STYLE_WIDTH:
        case LV_STYLE_MIN_WIDTH:
        case LV_STYLE_MAX_WIDTH:
        case LV_STYLE_HEIGHT:
        case LV_STYLE_MIN_HEIGHT:
        case LV_STYLE_MAX_HEIGHT:
        case LV_STYLE_X:
        case LV_STYLE_Y:
        case LV_STYLE_TRANSFORM_WIDTH:
        case LV_STYLE_TRANSFORM_HEIGHT:
        case LV_STYLE_TRANSLATE_X:
        case LV_STYLE_TRANSLATE_Y:
        case LV_STYLE_TRANSFORM_ZOOM:
        case LV_STYLE_TRANSFORM_ANGLE:
        case LV_STYLE_PAD_TOP:
        case LV_STYLE_PAD_BOTTOM:
        case LV_STYLE_PAD_LEFT:
        case LV_STYLE_PAD_RIGHT:
        case LV_STYLE_PAD_ROW:
        case LV_STYLE_PAD_COLUMN:
        case LV_STYLE_RADIUS:
        case LV_STYLE_BG_MAIN_STOP:
        case LV_STYLE_BG_GRAD_STOP:
        case LV_STYLE_BORDER_WIDTH:
        case LV_STYLE_TEXT_LETTER_SPACE:
        case LV_STYLE_TEXT_LINE_SPACE:
        case LV_STYLE_OUTLINE_WIDTH:
        case LV_STYLE_OUTLINE_PAD:
        case LV_STYLE_SHADOW_WIDTH:
        case LV_STYLE_SHADOW_OFS_X:
        case LV_STYLE_SHADOW_OFS_Y:
        case LV_STYLE_SHADOW_SPREAD:
        case LV_STYLE_LINE_WIDTH:
        case LV_STYLE_LINE_DASH_WIDTH:
        case LV_STYLE_LINE_DASH_GAP:
        case LV_STYLE_ARC_WIDTH:
            return JS_ToInt32(ctx, &(value->num), jsval)==0 ;

        case LV_STYLE_ALIGN:
        case LV_STYLE_CLIP_CORNER:
        case LV_STYLE_OPA:
        case LV_STYLE_COLOR_FILTER_OPA:
        case LV_STYLE_ANIM_TIME:
        case LV_STYLE_ANIM_SPEED:
        case LV_STYLE_BLEND_MODE:
        case LV_STYLE_LAYOUT:
        case LV_STYLE_BASE_DIR:
        case LV_STYLE_BG_OPA:
        case LV_STYLE_BG_GRAD_DIR:
        case LV_STYLE_BG_IMG_OPA:
        case LV_STYLE_BG_IMG_RECOLOR_OPA:
        case LV_STYLE_BG_IMG_TILED:
        case LV_STYLE_BORDER_OPA:
        case LV_STYLE_BORDER_SIDE:
        case LV_STYLE_BORDER_POST:
        case LV_STYLE_TEXT_OPA:
        case LV_STYLE_TEXT_DECOR:
        case LV_STYLE_TEXT_ALIGN:
        case LV_STYLE_IMG_OPA:
        case LV_STYLE_IMG_RECOLOR_OPA:
        case LV_STYLE_OUTLINE_OPA:
        case LV_STYLE_SHADOW_OPA:
        case LV_STYLE_LINE_ROUNDED:
        case LV_STYLE_LINE_OPA:
        case LV_STYLE_ARC_ROUNDED:
        case LV_STYLE_ARC_OPA:
            return JS_ToUint32(ctx, &(value->num), jsval)==0 ;

        case LV_STYLE_BG_COLOR:
        case LV_STYLE_BG_COLOR_FILTERED:
        case LV_STYLE_BG_GRAD_COLOR:
        case LV_STYLE_BG_GRAD_COLOR_FILTERED:
        case LV_STYLE_BG_IMG_RECOLOR:
        case LV_STYLE_BG_IMG_RECOLOR_FILTERED:
        case LV_STYLE_BORDER_COLOR:
        case LV_STYLE_BORDER_COLOR_FILTERED:
        case LV_STYLE_TEXT_COLOR:
        case LV_STYLE_TEXT_COLOR_FILTERED:
        case LV_STYLE_IMG_RECOLOR:
        case LV_STYLE_IMG_RECOLOR_FILTERED:
        case LV_STYLE_OUTLINE_COLOR:
        case LV_STYLE_OUTLINE_COLOR_FILTERED:
        case LV_STYLE_SHADOW_COLOR:
        case LV_STYLE_SHADOW_COLOR_FILTERED:
        case LV_STYLE_LINE_COLOR:
        case LV_STYLE_LINE_COLOR_FILTERED:
        case LV_STYLE_ARC_COLOR:
        case LV_STYLE_ARC_COLOR_FILTERED:
            return JS_ToUint32(ctx, &(value->color.full), jsval)==0 ;
    }
    return false ;
}

JSValue js_lv_obj_get_all_style_values(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_LBOBJ("Obj", "getAllStyles", thisobj)
    JSValue jsstyles = JS_NewObject(ctx) ;
    for(int i = 0; i < thisobj->style_cnt; i++) {

        lv_style_t * style = thisobj->styles[i].style ;

        if(style->prop_cnt==1) {
            const char * propName = lv_style_prop_const_to_str(style->prop1) ;
            JSValue jsval = lv_style_value_to_js(ctx, style->prop1, style->v_p.value1) ;
            JS_SetPropertyStr(ctx, jsstyles, propName, jsval) ;
        }
        else {
            lv_style_value_t * values = (lv_style_value_t *)style->v_p.values_and_props;
            uint8_t * tmp = style->v_p.values_and_props + style->prop_cnt * sizeof(lv_style_value_t);
            uint16_t * props = (uint16_t *)tmp;

            for(int j=0;j<style->prop_cnt;j++) {
                const char * propName = lv_style_prop_const_to_str(props[j]) ;
                JSValue jsval = lv_style_value_to_js(ctx, props[j], values[j]) ;
                JS_SetPropertyStr(ctx, jsstyles, propName, jsval) ;
            }
        }
    }

    return jsstyles ;
}


JSValue js_lv_obj_set_style(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    if(!JS_IsString(argv[0])) {
        THROW_EXCEPTION("arg style name must be a string")
    }
    lv_style_prop_t prop ;
    if(!lv_style_prop_jsstr_to_const(ctx, argv[0], &prop)){
        THROW_EXCEPTION("unknow style name pass in")
    }

    lv_style_value_t value ;
    if(!lv_style_js_to_value(ctx, prop, argv[1], &value)){
        THROW_EXCEPTION("style value invalid")
    }

    lv_style_selector_t selector = LV_PART_MAIN | LV_STATE_DEFAULT ;
    if(argc>2) {
        if(JS_ToUint32(ctx, &selector, argv[2])!=0) {
            THROW_EXCEPTION("invalid arg selector")
        }
    }

    THIS_LBOBJ("Obj", "getStyle", thisobj)
    
    lv_obj_set_local_style_prop(thisobj, prop, value, selector) ;

    return JS_UNDEFINED ;
}

JSValue js_lv_obj_get_style(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    if(!JS_IsString(argv[0])) {
        THROW_EXCEPTION("arg style name must be a string")
    }
    lv_style_prop_t prop ;
    if(!lv_style_prop_jsstr_to_const(ctx, argv[0], &prop)){
        THROW_EXCEPTION("unknow style name pass in")
    }
    lv_style_selector_t selector = LV_PART_MAIN | LV_STATE_DEFAULT ;
    if(argc>1) {
        if(JS_ToUint32(ctx, &selector, argv[1])!=0) {
            THROW_EXCEPTION("invalid arg part")
        }
    }

    THIS_LBOBJ("Obj", "getStyle", thisobj)

    lv_style_value_t value = lv_obj_get_style_prop(thisobj, selector, prop) ;

    return lv_style_value_to_js(ctx, prop, value) ;
}