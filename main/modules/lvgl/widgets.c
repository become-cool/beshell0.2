#include "widgets.h"
#include "style.h"
#include "be_lv_struct_wrapper.h"
#include "utils.h"
#include "module_fs.h"
#include "images.h"

#include "font_msyh.h"
#include "font_msyh_3500.h"
#include "font_msyh_5000.h"
#include "font_wqy-zenhei.h"
#include "font_source-han-sans.h"



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

    // param
    JSValue param = JS_UNDEFINED ;
    if(event->code==LV_EVENT_DRAW_MAIN && event->param) {
        param = JS_NewObjectClass(ctx, js_lv_area_class_id) ;
        JS_SetOpaque(param, event->param) ;
    }
    else if(event->code==LV_EVENT_GESTURE && event->param) {
        // printf("--------- gesture\n") ;
        lv_dir_t dir = lv_indev_get_gesture_dir(event->param) ;
        param = lv_dir_const_to_jsstr(ctx, dir) ;
    }
    

    MAKE_ARGV4( cbargv, jsname, jsname, jstarget, param )

    JSValue func_emit = js_get_prop(ctx, jstarget, 1, "emit") ;
    
    JSValue ret = JS_Call(ctx, func_emit, jstarget, 4, cbargv) ;
    if(JS_IsException(ret)) {
        js_std_dump_error(ctx) ;
    }
    JS_FreeValue(ctx, ret) ;

    // 解除 opaque 后释放 param
    if( !JS_IsUndefined(param) ) {
        if( JS_IsObject(param) ) {
            JS_SetOpaque(param, NULL) ;
        }
        JS_FreeValue(ctx, param) ;
    }

    free(cbargv) ;
    JS_FreeValue(ctx, func_emit) ;
    JS_FreeValue(ctx, jsname) ;


    // 引用计数 -1
    if(event->code==LV_EVENT_DELETE){
        JS_SetOpaque(jstarget, NULL) ;
        // JS_FreeValue(ctx, jstarget) ;
    }
}

/**
 * 3种调用方法：
 *  1. js_lv_obj_wrapper(ctx, obj, Cotr, 0)
 *      js 中定义的 lv.Obj派生类, 需要提供 JS构造函数
 *  2. js_lv_obj_wrapper(ctx, obj, JS_UNDEFINED, clsid)
 *      lv 原生对象(c 实现的类)
 *  3. js_lv_obj_wrapper(ctx, obj, JS_UNDEFINED, 0)
 *      根据 obj 类型确定 clsid
 */
JSValue js_lv_obj_wrapper(JSContext *ctx, lv_obj_t * cobj, JSValue cotr, JSClassID clsid) {

    JSValue jsobj = JS_UNDEFINED ;

    void * _jsobj = lv_obj_get_user_data(cobj) ;
    if(_jsobj) {
        jsobj = JS_MKPTR(JS_TAG_OBJECT, _jsobj) ;
        JS_DupValue(ctx, jsobj) ;
    }
    else {
        if(clsid) {
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
        }
        else {
            clsid = be_lv_type_to_js_class(cobj);
            jsobj = JS_NewObjectClass(ctx, clsid) ;
        }

        lv_obj_set_user_data(cobj, JS_VALUE_GET_PTR(jsobj)) ;
        JS_SetOpaque(jsobj, cobj) ;
        
        JSValue jsFuncLvObjInit = js_get_glob_prop(ctx, 3, "beapi", "lvgl", "__lv_obj_init") ;
        JSValue ret = JS_Call(ctx, jsFuncLvObjInit, jsobj, 0, NULL) ;
        JS_FreeValue(ctx, ret) ;
        JS_FreeValue(ctx, jsFuncLvObjInit) ;
        
        lv_obj_add_event_cb(cobj, js_lv_event_cb, LV_EVENT_DELETE, ctx) ;

        // 引用计数 +1 , 触发 LV_EVENT_DELETE 时 -1
        // JS_DupValue(ctx, jsobj) ;
    }
    
    return jsobj ;
}


JSValue js_lv_obj_as(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    if( !JS_IsFunction(ctx, argv[0]) ) {
        THROW_EXCEPTION("arg class must be a function")
    }
    THIS_LVOBJ("Obj", "as", thisobj)
    
    JSValue prototype = JS_GetPropertyStr(ctx, argv[0], "prototype") ;
    if(! JS_IsObject(prototype) ){
        THROW_EXCEPTION("invalid class")
    }
    JSClassID classid ;
    if( !JS_GetClassIDFromProto(ctx, prototype, &classid) ){
        JS_FreeValue(ctx, prototype) ;
        THROW_EXCEPTION("invalid class")
    }

    void * _jsobj = lv_obj_get_user_data(thisobj) ;
    JSValue jsobj ;
    if(_jsobj) {
        jsobj = JS_MKPTR(JS_TAG_OBJECT, _jsobj) ;
        JS_FreeValue(ctx, jsobj) ;
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    
    return JS_DupValue( ctx, js_lv_obj_wrapper(ctx, thisobj, argv[0], classid) );
}




JSValue js_lv_group_wrapper(JSContext *ctx, lv_group_t * group) {
    if(!group) {
        return JS_NULL ;
    }
    void * ptr = (void*) group->user_data ;

    // 该对象由 js 创建
    if(ptr) {
        return JS_DupValue(ctx, JS_MKPTR(JS_TAG_OBJECT,ptr)) ;
    }

    // 对象 不是 js 创建
    else {
        // @todo 包装成 js 对象 (不在 js_lv_disp_finalizer 中 free)
        THROW_EXCEPTION("disp not created by js runtime") ;
    }
}


JSValue js_lv_group_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv) {
    JSValue jsobj = JS_NewObjectClass(ctx, lv_group_js_class_id()) ;
    lv_group_t * cgrp = lv_group_create() ;
    JS_SetOpaque(jsobj, cgrp) ;
    cgrp->user_data = JS_VALUE_GET_PTR(jsobj) ;
    JS_SetPropertyStr(ctx, jsobj, "_handlers", JS_NewObject(ctx));
    return jsobj ;
}
void js_lv_group_finalizer(JSRuntime *rt, JSValue val){
    lv_group_t * cgrp = (lv_group_t *) JS_GetOpaque(val, lv_group_js_class_id()) ;
    if(cgrp) {
        lv_group_del(cgrp) ;
        JS_SetOpaque(val, NULL) ;
    }
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
        return JS_UNDEFINED ;
    }
    if(find_event_dsc(thisobj, eventcode)!=NULL) {
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
        return JS_UNDEFINED ;
    }
    lv_event_dsc_t * event_dsc = find_event_dsc(thisobj, eventcode) ;
    if(event_dsc) {
        // printf("disable event: %d \n", eventcode) ;
        lv_obj_remove_event_dsc(thisobj, event_dsc) ;
    }
    return JS_UNDEFINED ;
}

JSValue js_lvgl_is_event_name(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    uint8_t eventcode ;
    return lv_event_code_jsstr_to_const(ctx, argv[0], &eventcode)? JS_TRUE : JS_FALSE ;
}

JSValue js_lv_obj_is_screen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    THIS_LVOBJ("Obj", "isScreen", thisobj)
    return lv_obj_get_parent(thisobj)==NULL? JS_TRUE: JS_FALSE ;
}

void lv_obj_set_coord_x(lv_obj_t * obj, lv_coord_t x) {
    lv_coord_t dx = x - obj->coords.x1 ;
    lv_obj_set_x(obj, lv_obj_get_x_aligned(obj) + dx) ;
}
void lv_obj_set_coord_y(lv_obj_t * obj, lv_coord_t y) {
    lv_coord_t dy = y - obj->coords.y1 ;
    lv_obj_set_y(obj, lv_obj_get_y_aligned(obj) + dy) ;
}

JSValue js_lv_obj_set_coord_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        CHECK_ARGC(1)
    ARGV_TO_INT16(0, x)
    THIS_LVOBJ("Obj", "setCoordX", thisobj)
    lv_obj_set_coord_x(thisobj, x) ;
    return JS_UNDEFINED ;
}
JSValue js_lv_obj_set_coord_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_INT16(0, y)
    THIS_LVOBJ("Obj", "setCoordY", thisobj)
    lv_obj_set_coord_x(thisobj, y) ;
    return JS_UNDEFINED ;
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
JSValue js_lv_obj_get_coord_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_LVOBJ("Obj", "getCoordX", thisobj)    
    return JS_NewInt32(ctx, thisobj->coords.x1) ;
}
JSValue js_lv_obj_get_coord_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_LVOBJ("Obj", "getCoordY", thisobj)
    return JS_NewInt32(ctx, thisobj->coords.y1) ;
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
    THIS_LVOBJ("Obj", "allStyleValues", thisobj)
    JSValue jsstyles = JS_NewObject(ctx) ;
    for(int i = 0; i < thisobj->style_cnt; i++) {

        lv_style_t * style = thisobj->styles[i].style ;

        if(style->prop_cnt==1) {
            const char * propName = lv_style_prop_const_to_str(style->prop1) ;
            JSValue jsval = lv_style_value_to_js(ctx, style->prop1, style->v_p.value1) ;
            JS_SetPropertyStr(ctx, jsstyles, propName, jsval) ;
            // JS_FreeValue(ctx,jsval) ;
        }
        else {
            lv_style_value_t * values = (lv_style_value_t *)style->v_p.values_and_props;
            uint8_t * tmp = style->v_p.values_and_props + style->prop_cnt * sizeof(lv_style_value_t);
            uint16_t * props = (uint16_t *)tmp;

            for(int j=0;j<style->prop_cnt;j++) {
                const char * propName = lv_style_prop_const_to_str(props[j]) ;
                JSValue jsval = lv_style_value_to_js(ctx, props[j], values[j]) ;
                JS_SetPropertyStr(ctx, jsstyles, propName, jsval) ;
                // JS_FreeValue(ctx,jsval) ;
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
    char * jsStyleName = (char *)JS_ToCString(ctx, argv[0]) ;

    lv_style_selector_t selector = LV_PART_MAIN | LV_STATE_DEFAULT ;
    if(argc>2) {
        if(JS_ToUint32(ctx, &selector, argv[2])!=0) {
            THROW_EXCEPTION("invalid arg selector")
        }
    }

    THIS_LVOBJ("Obj", "getStyle", thisobj)

    lv_style_value_t value ;
    
    if(strcmp(jsStyleName,"pad")==0) {
        if(!lv_style_js_to_value(ctx, LV_STYLE_PAD_TOP, argv[1], &value)){
            THROW_EXCEPTION("style value invalid")
        }
        lv_obj_set_local_style_prop(thisobj, LV_STYLE_PAD_TOP, value, selector) ;
        lv_obj_set_local_style_prop(thisobj, LV_STYLE_PAD_BOTTOM, value, selector) ;
        lv_obj_set_local_style_prop(thisobj, LV_STYLE_PAD_LEFT, value, selector) ;
        lv_obj_set_local_style_prop(thisobj, LV_STYLE_PAD_RIGHT, value, selector) ;
    }
    else {
        lv_style_prop_t prop ;
        if(!lv_style_prop_str_to_const(jsStyleName, &prop)) {
            JS_ThrowReferenceError(ctx, "unknow style name: %s", jsStyleName) ;
            JS_FreeCString(ctx, jsStyleName) ;
            return JS_EXCEPTION ;
        }
        if(!lv_style_js_to_value(ctx, prop, argv[1], &value)){
            THROW_EXCEPTION("style value invalid")
        }

        lv_style_prop_jsstr_to_const(ctx, argv[0], &prop) ;
        lv_obj_set_local_style_prop(thisobj, prop, value, selector) ;
    }

    JS_FreeCString(ctx, jsStyleName) ;
    return JS_UNDEFINED ;
}

JSValue js_lv_obj_get_style(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    if(!JS_IsString(argv[0])) {
        THROW_EXCEPTION("arg style name must be a string")
    }
    lv_style_prop_t prop ;
    char * jsStyleName = (char *)JS_ToCString(ctx, argv[0]) ;
    if(!lv_style_prop_str_to_const(jsStyleName, &prop)) {
        JS_ThrowReferenceError(ctx, "unknow style name: %s", jsStyleName) ;
        JS_FreeCString(ctx, jsStyleName) ;
        return JS_EXCEPTION ;
    }
    JS_FreeCString(ctx, jsStyleName) ;
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
            JSValue jsstyle = lv_style_wrapper(ctx, thisobj->styles[i].style) ;
            JS_DupValue(ctx,jsstyle) ;
            return jsstyle ;
        }
    }
    return JS_UNDEFINED ;
}

JSValue js_lv_obj_is_style_name(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_style_prop_t prop ;
    return JS_NewBool(ctx, lv_style_prop_jsstr_to_const(ctx, argv[0], &prop)) ;
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

// extern lv_font_t be_font_symbol_16 ;

JSValue js_lv_label_set_font(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_LVOBJ("Obj", "setFont", thisobj)
    ARGV_TO_STRING(0, fontname)

    lv_font_t * font = NULL ;
    if( strcmp("m8", fontname)==0 ) {
        font = &lv_font_montserrat_8 ;
    }
#ifdef LV_FONT_MONTSERRAT_10
    if( strcmp("m10", fontname)==0 ) {
        font = &lv_font_montserrat_10 ;
    }
#endif
    else if( strcmp("m12", fontname)==0 ) {
        font = &lv_font_montserrat_12 ;
    }
#ifdef LV_FONT_MONTSERRAT_14
    else if( strcmp("m14", fontname)==0 ) {
        font = &lv_font_montserrat_14 ;
    }
#endif
    else if( strcmp("m16", fontname)==0 ) {
        font = &lv_font_montserrat_16 ;
    }
#ifdef LV_FONT_MONTSERRAT_18
    else if( strcmp("m18", fontname)==0 ) {
        font = &lv_font_montserrat_18 ;
    }
#endif
#ifdef LV_FONT_MONTSERRAT_20
    else if( strcmp("m20", fontname)==0 ) {
        font = &lv_font_montserrat_20 ;
    }
#endif
#ifdef LV_FONT_MONTSERRAT_22
    else if( strcmp("m22", fontname)==0 ) {
        font = &lv_font_montserrat_22 ;
    }
#endif
    else if( strcmp("m24", fontname)==0 ) {
        font = &lv_font_montserrat_24 ;
    }
    else if( strcmp("m32", fontname)==0 ) {
        font = &lv_font_montserrat_32 ;
    }
    else if( strcmp("m36", fontname)==0 ) {
        font = &lv_font_montserrat_36 ;
    }
    else if( strcmp("m40", fontname)==0 ) {
        font = &lv_font_montserrat_40 ;
    }
    else if( strcmp("m48", fontname)==0 ) {
        font = &lv_font_montserrat_48 ;
    }
    else if( strcmp("msyh", fontname)==0 ) {
        font = font_msyh_3500() ;
        
    }
    // else if( strcmp("source-han-sans", fontname)==0 ) {
    //     font = font_source_han_sans() ;
    // }
    // else if( strcmp("s16", fontname)==0 ) {
    //     font = & be_font_symbol_16 ;
    // }

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

    if(!csrc) {
        THROW_EXCEPTION("out of memory?") ;
    }

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

// lv_obj_t * lv_msgbox_create(lv_obj_t * parent, const char * title, const char * txt, const char * btn_txts[],
//                             bool add_close_btn)
JSValue js_lv_msgbox_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv) {
    CHECK_ARGC(5)
    lv_obj_t * cparent = NULL ;
    if(!JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }

    ARGV_TO_STRING(1, title)
    ARGV_TO_STRING(2, msg)

    if(JS_IsArray(ctx, argv[3])!=1) {
        JS_FreeCString(ctx, title) ;
        JS_FreeCString(ctx, msg) ;
        THROW_EXCEPTION("argv btns must be a String[]")
    }
    JSValue jslen = JS_GetPropertyStr(ctx, argv[3], "length") ;
    uint32_t btnsLen ;
    if(JS_ToUint32(ctx, &btnsLen, jslen)!=0){
        JS_FreeCString(ctx, title) ;
        JS_FreeCString(ctx, msg) ;
        THROW_EXCEPTION("argv btns must be a String[]")        
    }
    JS_FreeValue(ctx, jslen) ;

    const char ** btns = malloc( sizeof(char*)*btnsLen ) ;
    for(uint32_t i=0; i<btnsLen; i++) {
        JSValue btnTxt = JS_GetPropertyUint32(ctx, argv[3], i) ;
        btns[i] = JS_ToCString(ctx, btnTxt) ;
        JS_FreeValue(ctx, btnTxt) ;
    }

    bool useCloseBtn = JS_ToBool(ctx, argv[4]) ;

    lv_obj_t * cobj = lv_msgbox_create(cparent, title, msg, btns ,useCloseBtn) ;

    JS_FreeCString(ctx, title) ;
    JS_FreeCString(ctx, msg) ;
    for(uint32_t i=0; i<btnsLen; i++) {
        JS_FreeCString(ctx, btns[i]) ;
    }
    free(btns) ;

    return JS_DupValue( ctx, js_lv_obj_wrapper(ctx, cobj, new_target, lv_obj_js_class_id()) ) ;
}

JSValue js_lv_list_add_btn(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("List.addBtn() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("List.addBtn() must be called as a List method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * icon = NULL ;
    if( JS_IsString(argv[0]) ) {
        icon = (char *)JS_ToCString(ctx, argv[0]) ;
    }
    char * txt = (char *)JS_ToCString(ctx, argv[1]) ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_list_add_btn(thisobj, icon, txt);
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, lv_btn_js_class_id()) ;
        JS_DupValue(ctx, retval) ;
    }
    if(icon) {
        JS_FreeCString(ctx, icon) ;
    }
    JS_FreeCString(ctx, txt) ;
    return retval ;
}

JSValue js_lv_obj_ptr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_LVOBJ("Obj","ptr", thisobj)
    return JS_NewInt64(ctx, (int64_t)thisobj) ;
}
JSValue js_lv_obj_from_ptr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    int64_t ptr ;
    if(JS_ToInt64(ctx, &ptr, argv[0])!=0){
        THROW_EXCEPTION("invalid ptr value")
    }
    JSValue jsobj =  js_lv_obj_wrapper(ctx, (lv_obj_t*)ptr, JS_UNDEFINED, lv_obj_js_class_id()) ;
    return JS_DupValue(ctx, jsobj) ;
}

#ifndef SIMULATION
// JSValue js_lv_set_debug_log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
//     lv_set_debug_log( JS_ToBool(ctx, argv[0])) ;
//     return JS_UNDEFINED ;
// }
#endif



JSValue js_lv_canvas_malloc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_UINT16(0, w)
    ARGV_TO_UINT16(0, h)
    THIS_LVOBJ("Canvas","malloc", thisobj)

    lv_img_cf_t color_format = LV_IMG_CF_TRUE_COLOR_ALPHA ;
    if(argc>2) {
        if(!lv_img_cf_jsstr_to_const(ctx, argv[2], &color_format)) {
            THROW_EXCEPTION("invalid color format")
        }
    }
    size_t buff_len ;
    if(color_format==LV_IMG_CF_TRUE_COLOR) {
        buff_len = LV_CANVAS_BUF_SIZE_TRUE_COLOR(w, h) ;
    }
    else if(color_format==LV_IMG_CF_TRUE_COLOR_ALPHA) {
        buff_len = LV_CANVAS_BUF_SIZE_TRUE_COLOR_ALPHA(w, h) ;
    }
    else if(color_format==LV_IMG_CF_INDEXED_1BIT) {
        buff_len = LV_CANVAS_BUF_SIZE_INDEXED_1BIT(w, h) ;
    }
    else if(color_format==LV_IMG_CF_INDEXED_2BIT) {
        buff_len = LV_CANVAS_BUF_SIZE_INDEXED_2BIT(w, h) ;
    }
    else if(color_format==LV_IMG_CF_INDEXED_4BIT) {
        buff_len = LV_CANVAS_BUF_SIZE_INDEXED_4BIT(w, h) ;
    }
    else if(color_format==LV_IMG_CF_INDEXED_8BIT) {
        buff_len = LV_CANVAS_BUF_SIZE_INDEXED_8BIT(w, h) ;
    }
    else {
        THROW_EXCEPTION("not supported color format")
    }

    const char * buff = malloc(buff_len) ;
    if(!buff) {
        THROW_EXCEPTION("out of memory?")
    }

    lv_canvas_set_buffer(thisobj, buff, w, h, color_format);

    return JS_NewUint32(ctx, buff_len) ;
}
// JSValue js_lv_canvas_free(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
//     THIS_OBJ("Canvas","malloc", thisobj, lv_canvas_t)
//     if(thisobj->dsc.data_size && thisobj->dsc.data) {
//         free(thisobj->dsc.data) ;
//     }
//     return JS_UNDEFINED ;
// }


JSValue js_lv_obj_abort_scroll(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_LVOBJ("Obj","abortScroll", thisobj)

    lv_indev_t * indev = lv_indev_get_next(NULL);
    while(indev) {
        if(indev->driver->type == LV_INDEV_TYPE_POINTER) {
            if(indev->proc.types.pointer.scroll_obj == thisobj) {
                indev->proc.types.pointer.scroll_obj = NULL ;
                return JS_TRUE ;
            }
        }
        indev = lv_indev_get_next(indev);
    }

    return JS_FALSE ;
}

static JSValue js_lv_weather_img(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)

    CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
    lv_obj_t * cparent = JS_GetOpaqueInternal(argv[0]) ;
    if(!cparent) {
        THROW_EXCEPTION("invalid arg parent")
    }

    ARGV_TO_UINT8(1, idx)
    if(idx>13) {
        THROW_EXCEPTION("weather image index must >=0 and <=8")
    }

    lv_obj_t * weatherImg = lv_img_create(cparent);
    if(idx==0) {
        lv_img_set_src(weatherImg, &ali);
    }
    else {
        lv_img_set_src(weatherImg, &cat);
    }

    return js_lv_obj_wrapper(ctx, weatherImg, JS_UNDEFINED, 0) ;
}

void be_lv_widgets_init() {
}

void be_lv_widgets_require(JSContext *ctx, JSValue lvgl) {
    JS_SetPropertyStr(ctx, lvgl, "isEventName", JS_NewCFunction(ctx, js_lvgl_is_event_name, "isEventName", 1));
    JS_SetPropertyStr(ctx, lvgl, "isStyleName", JS_NewCFunction(ctx, js_lv_obj_is_style_name, "isStyleName", 1));
    JS_SetPropertyStr(ctx, lvgl, "fromPtr", JS_NewCFunction(ctx, js_lv_obj_from_ptr, "fromPtr", 1));
    JS_SetPropertyStr(ctx, lvgl, "weatherImg", JS_NewCFunction(ctx, js_lv_weather_img, "weatherImg", 1));
#ifndef SIMULATION
    // JS_SetPropertyStr(ctx, lvgl, "setDebugLog", JS_NewCFunction(ctx, js_lv_set_debug_log, "setDebugLog", 1));
#endif
}


JSValue js_lv_tabview_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    
    int16_t tab_size = 30 ;
    if(argc>=3 && JS_ToInt32(ctx, (int32_t *) &tab_size, argv[2])!=0){
        THROW_EXCEPTION("arg tab_size of method TabView.create() must be a number")
    }

    lv_dir_t tab_pos = LV_DIR_TOP ;
    if(argc>=2 && !lv_dir_jsstr_to_const(ctx, argv[1], &tab_pos)) {
        return JS_EXCEPTION ;
    }

    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_tabview_t * cobj = lv_tabview_create(cparent, tab_pos, tab_size) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, lv_tabview_js_class_id()) ;
    return jsobj ;
}


JSValue js_lv_spinner_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    
    uint32_t spin_time = 1500 ;
    if(argc>=2 && JS_ToUint32(ctx, &spin_time, argv[1])!=0){
        THROW_EXCEPTION("arg tab_size of method TabView.create() must be a number")
    }
    uint32_t arc_length = 60 ;
    if(argc>=3 && JS_ToUint32(ctx, &arc_length, argv[2])!=0){
        THROW_EXCEPTION("arg tab_size of method TabView.create() must be a number")
    }

    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_arc_t * cobj = lv_spinner_create(cparent, spin_time, arc_length) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, lv_spinner_js_class_id()) ;
    return jsobj ;
}


JSValue js_lv_group_all_objs(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Group.del() must be called as a Group method")
    }
    lv_group_t * thisgrp = lv_userdata ;
    
    JSValue array = JS_NewArray(ctx) ;
    uint idx = 0 ;
    lv_obj_t ** ppobj;
    _LV_LL_READ(&thisgrp->obj_ll, ppobj) {
        JSValue jsobj = js_lv_obj_wrapper(ctx, *ppobj, JS_UNDEFINED, 0) ;
        JS_SetPropertyUint32(ctx, array, idx++, jsobj) ;
    }

    return array ;
}


JSValue js_lv_set_alloc_spiram(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    lv_set_alloc_spiram( JS_ToBool(ctx,argv[0])? 1: 0 ) ;
    return JS_UNDEFINED ;
}