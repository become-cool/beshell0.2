#include "widgets_extra.h"
#include "style.h"
#include "utils.h"
#include "font_msyh.h"
#include "module_fs.h"


void js_lv_obj_init(JSContext *ctx, JSValue jsobj) {
    
    JSValue jsFuncLvObjInit = js_get_glob_prop(ctx, 3, "beapi", "lvgl", "__lv_obj_init") ;

    JS_Call(ctx, jsFuncLvObjInit, jsobj, 0, NULL) ;

    JS_FreeValue(ctx, jsFuncLvObjInit) ;
}

JSValue js_lv_obj_wrapper(JSContext *ctx, lv_obj_t * cobj, JSValue cotr, JSClassID clsid) {

    JSValue jsobj = JS_UNDEFINED ;

    void * _jsobj = lv_obj_get_user_data(cobj) ;
    if(_jsobj) {
        jsobj = JS_MKPTR(JS_TAG_OBJECT, _jsobj) ;
    }
    else {

        if(JS_IsUndefined(cotr)) {
            jsobj = JS_NewObjectClass(ctx, clsid) ;
        }
        else {
            JSValue proto = JS_GetPropertyStr(ctx, cotr, "prototype");
            if (JS_IsException(proto)) {
                JS_FreeValue(ctx, proto) ;
                return JS_EXCEPTION ;
            }
            jsobj = JS_NewObjectProtoClass(ctx, proto, clsid) ;
            JS_FreeValue(ctx, proto) ;
        }

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
    JS_FreeValue(ctx, ObjCotr) ;

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
    THIS_LVOBJ("Obj", "enableEvent", thisobj)
    CHECK_ARGC(1)
    uint8_t eventcode ;
    if(!lv_event_code_jsstr_to_const(ctx, argv[0], &eventcode)) {
        return JS_EXCEPTION ;
    }

    if(find_event_dsc(thisobj, eventcode)!=NULL) {
        // printf("event aleady enabled \n") ;
        return JS_UNDEFINED ;
    }

    lv_obj_add_event_cb(thisobj, js_lv_event_cb, eventcode, ctx) ;
    return JS_UNDEFINED ;
}

JSValue js_lv_obj_disable_event(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    
    THIS_LVOBJ("Obj", "disableEvent", thisobj)
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
    THIS_LVOBJ("Obj", "isScreen", thisobj)
    return lv_obj_get_parent(thisobj)==NULL? JS_TRUE: JS_FALSE ;
}

JSValue js_lv_obj_ptr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_LVOBJ("Obj", "isScreen", thisobj)
    printf("%p\n", thisobj) ;
    return JS_NewUint32(ctx, (uint64_t)thisobj) ;
}

JSValue js_lv_obj_set_coords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_INT16(0, x)
    ARGV_TO_INT16(1, y)
    
    THIS_LVOBJ("Obj", "setCoords", thisobj)

    int16_t dx = x - thisobj->coords.x1 ;
    int16_t dy = y - thisobj->coords.y1 ;
    
    lv_obj_set_x(thisobj, lv_obj_get_x_aligned(thisobj) + dx) ;
    lv_obj_set_y(thisobj, lv_obj_get_y_aligned(thisobj) + dy) ;
    
    return JS_UNDEFINED ;
}
JSValue js_lv_obj_get_coords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    THIS_LVOBJ("Obj", "getCoords", thisobj)
    
    JSValue arr = JS_NewArray(ctx) ;
    JS_SetPropertyUint32(ctx, arr, 0, JS_NewInt32(ctx, thisobj->coords.x1));
    JS_SetPropertyUint32(ctx, arr, 1, JS_NewInt32(ctx, thisobj->coords.y1));
    
    return arr ;
}
JSValue js_lv_obj_move(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_INT16(0, dx)
    ARGV_TO_INT16(1, dy)
    THIS_LVOBJ("Obj", "move", thisobj)
    lv_obj_set_x(thisobj, lv_obj_get_x_aligned(thisobj) + dx) ;
    lv_obj_set_y(thisobj, lv_obj_get_y_aligned(thisobj) + dy) ;
    return JS_UNDEFINED ;
}
JSValue js_lv_obj_move_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_INT16(0, delta)
    THIS_LVOBJ("Obj", "moveX", thisobj)
    lv_obj_set_x(thisobj, lv_obj_get_x_aligned(thisobj) + delta) ;
    return JS_UNDEFINED ;
}
JSValue js_lv_obj_move_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_INT16(0, delta)
    THIS_LVOBJ("Obj", "moveY", thisobj)
    lv_obj_set_y(thisobj, lv_obj_get_y_aligned(thisobj) + delta) ;
    return JS_UNDEFINED ;
}


JSValue js_lv_obj_get_all_style_values(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_LVOBJ("Obj", "getAllStyles", thisobj)
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

    THIS_LVOBJ("Obj", "getStyle", thisobj)

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

    THIS_LVOBJ("Obj", "style", thisobj)

    lv_style_value_t value = lv_obj_get_style_prop(thisobj, selector, prop) ;

    return lv_style_value_to_js(ctx, prop, value) ;
}

JSValue js_lv_obj_get_local_style(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    
    lv_style_selector_t selector = LV_PART_MAIN | LV_STATE_DEFAULT ;
    if(argc>1) {
        if(JS_ToUint32(ctx, &selector, argv[1])!=0) {
            THROW_EXCEPTION("invalid arg part")
        }
    }
    
    THIS_LVOBJ("Obj", "localStyle", thisobj)

    for(int i = 0; i < thisobj->style_cnt; i++) {
        if(thisobj->styles[i].is_local && thisobj->styles[i].selector == selector) {
            dn(i)
            JSValue jsstyle = lv_style_wrapper(ctx, thisobj->styles[i].style) ;
            JS_DupValue(ctx,jsstyle) ;
            return jsstyle ;
        }
    }
    return JS_UNDEFINED ;
}


JSValue js_lv_obj_refresh_style(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
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

    THIS_LVOBJ("Obj", "style", thisobj)

    lv_obj_refresh_style(thisobj, selector, prop);

    return JS_UNDEFINED ;
}

JSValue js_lv_obj_get_font_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    THIS_LVOBJ("Obj", "style", thisobj)
    lv_style_selector_t selector = LV_PART_MAIN | LV_STATE_DEFAULT ;
    if(argc>0) {
        if(JS_ToUint32(ctx, &selector, argv[0])!=0) {
            THROW_EXCEPTION("invalid arg part")
        }
    }
    lv_font_t * font = lv_obj_get_style_text_font(thisobj, selector) ;
    return JS_NewInt32(ctx, font? font->line_height: 0)  ;
}
JSValue js_lv_label_set_font(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_LVOBJ("Obj", "style", thisobj)
    ARGV_TO_STRING(0, fontname)

    lv_font_t * font = NULL ;
    if( strcmp("m8", fontname)==0 ) {
        font = &lv_font_montserrat_8 ;
    }
    if( strcmp("m10", fontname)==0 ) {
        font = &lv_font_montserrat_10 ;
    }
    else if( strcmp("m12", fontname)==0 ) {
        font = &lv_font_montserrat_12 ;
    }
    else if( strcmp("m16", fontname)==0 ) {
        font = &lv_font_montserrat_16 ;
    }
    else if( strcmp("m24", fontname)==0 ) {
        font = &lv_font_montserrat_24 ;
    }
    else if( strcmp("m32", fontname)==0 ) {
        font = &lv_font_montserrat_32 ;
    }
    else if( strcmp("m40", fontname)==0 ) {
        font = &lv_font_montserrat_40 ;
    }
    else if( strcmp("m48", fontname)==0 ) {
        font = &lv_font_montserrat_48 ;
    }
    else if( strcmp("msyh", fontname)==0 ) {
        font = font_msyh() ;
    }

    JSValue ret = JS_UNDEFINED ;

    if(!font) {
        JS_ThrowReferenceError(ctx, "unknow font name: %s", fontname) ;
        ret = JS_EXCEPTION ;
    }
    else {
        lv_obj_set_style_text_font(thisobj, font, LV_PART_MAIN | LV_STATE_DEFAULT ) ;
    }

    JS_FreeCString(ctx, fontname) ;

    return ret ;
}


JSValue js_lv_img_set_src(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_LVOBJ("Img", "setSrc", thisimg)
    ARGV_TO_STRING_E(0, jssrc, "arg src must be a string")

    char * _csrc = vfspath_to_fs(jssrc) ;
    JS_FreeCString(ctx, jssrc) ;

    char * csrc = mallocf("%c:%s", LV_USE_FS_STDIO, _csrc) ;
    free(_csrc) ;

    lv_img_set_src(thisimg, csrc) ;
    free(csrc) ;

    return JS_UNDEFINED ;
}
JSValue js_lv_img_set_symbol(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_LVOBJ("Img", "setSymbol", thisimg)
    ARGV_TO_STRING_E(0, symbol, "arg symbol must be a string")
    lv_img_set_src(thisimg, symbol) ;
    JS_FreeCString(ctx, symbol) ;
    return JS_UNDEFINED ;
}