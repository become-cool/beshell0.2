#include "style.h"
#include "utils.h"
#include "cutils.h"
#include "widgets_gen.h"


lv_style_prop_t LV_STYLE_JSVALUE ;
JSClassID js_lv_style_class_id ;

static JSValue create_js_style(JSContext *ctx, lv_style_t * style) {

    JSValue jsstyle = JS_NewObjectClass(ctx, js_lv_style_class_id) ;
    JS_SetOpaque(jsstyle, style) ;    

    lv_style_value_t v = {.ptr = JS_VALUE_GET_PTR(jsstyle)};
    lv_style_set_prop(style, LV_STYLE_JSVALUE, v);

    return jsstyle ;
}


JSValue lv_style_wrapper(JSContext * ctx, lv_style_t * style) {
    lv_style_value_t v = {.ptr = NULL};
    if(!lv_style_get_prop(style, LV_STYLE_JSVALUE, &v)) {
        return create_js_style(ctx, style) ;
    }

    else {
        return JS_MKPTR(JS_TAG_OBJECT, v.ptr) ;
    }

}


// AUTO GENERATE CODE START [STYLE VALUE SETTER/GETTER] --------

JSValue lv_style_value_to_js(JSContext * ctx, lv_style_prop_t prop, lv_style_value_t value) {
    if( prop==LV_STYLE_FLEX_FLOW ){
        return lv_flex_flow_const_to_jsstr(ctx, value.num) ;
    }
    else if( prop==LV_STYLE_FLEX_MAIN_PLACE
        || prop==LV_STYLE_FLEX_CROSS_PLACE
        || prop==LV_STYLE_FLEX_TRACK_PLACE
    ) {
        return lv_flex_align_const_to_jsstr(ctx, value.num) ;
    }
    else if( prop==LV_STYLE_FLEX_GROW ) {
        return JS_NewUint32(ctx, value.num) ;
    }
    switch(prop) {
        case LV_STYLE_ALIGN:
            return lv_align_const_to_jsstr(ctx, value.num) ;
        case LV_STYLE_BASE_DIR:
            return lv_base_dir_const_to_jsstr(ctx, value.num) ;
        case LV_STYLE_BORDER_SIDE:
            return lv_border_side_const_to_jsstr(ctx, value.num) ;
        case LV_STYLE_TEXT_ALIGN:
            return lv_text_align_const_to_jsstr(ctx, value.num) ;

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
        case LV_STYLE_CLIP_CORNER: 
        case LV_STYLE_OPA: 
        case LV_STYLE_COLOR_FILTER_OPA: 
        case LV_STYLE_ANIM_TIME: 
        case LV_STYLE_ANIM_SPEED: 
        case LV_STYLE_BLEND_MODE: 
        case LV_STYLE_LAYOUT: 
        case LV_STYLE_BG_OPA: 
        case LV_STYLE_BG_GRAD_DIR: 
        case LV_STYLE_BG_IMG_OPA: 
        case LV_STYLE_BG_IMG_RECOLOR_OPA: 
        case LV_STYLE_BG_IMG_TILED: 
        case LV_STYLE_BORDER_OPA: 
        case LV_STYLE_BORDER_POST: 
        case LV_STYLE_TEXT_OPA: 
        case LV_STYLE_TEXT_DECOR: 
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
        default:
            return JS_NewString(ctx, "unknow type") ;
    }
}


bool lv_style_js_to_value(JSContext * ctx, lv_style_prop_t prop, JSValue jsval, lv_style_value_t * value) {
    if( prop==LV_STYLE_FLEX_FLOW ){
        return lv_flex_flow_jsstr_to_const(ctx, jsval, &(value->num)) ;
    }
    else if( prop==LV_STYLE_FLEX_MAIN_PLACE
        || prop==LV_STYLE_FLEX_CROSS_PLACE
        || prop==LV_STYLE_FLEX_TRACK_PLACE
    ) {
        return lv_flex_align_jsstr_to_const(ctx, jsval, &(value->num)) ;
    }
    else if( prop==LV_STYLE_FLEX_GROW ) {
        return JS_ToUint32(ctx, (uint32_t*)&(value->num), jsval)==0 ;
    }
    switch(prop) {
        case LV_STYLE_ALIGN:
            return lv_align_jsstr_to_const(ctx, jsval, &(value->num)) ;
        case LV_STYLE_BASE_DIR:
            return lv_base_dir_jsstr_to_const(ctx, jsval, &(value->num)) ;
        case LV_STYLE_BORDER_SIDE:
            return lv_border_side_jsstr_to_const(ctx, jsval, &(value->num)) ;
        case LV_STYLE_TEXT_ALIGN:
            return lv_text_align_jsstr_to_const(ctx, jsval, &(value->num)) ;

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

            return JS_ToInt32(ctx, &(value->num), jsval)==0 ;
        case LV_STYLE_CLIP_CORNER: 
        case LV_STYLE_OPA: 
        case LV_STYLE_COLOR_FILTER_OPA: 
        case LV_STYLE_ANIM_TIME: 
        case LV_STYLE_ANIM_SPEED: 
        case LV_STYLE_BLEND_MODE: 
        case LV_STYLE_LAYOUT: 
        case LV_STYLE_BG_OPA: 
        case LV_STYLE_BG_GRAD_DIR: 
        case LV_STYLE_BG_IMG_OPA: 
        case LV_STYLE_BG_IMG_RECOLOR_OPA: 
        case LV_STYLE_BG_IMG_TILED: 
        case LV_STYLE_BORDER_OPA: 
        case LV_STYLE_BORDER_POST: 
        case LV_STYLE_TEXT_OPA: 
        case LV_STYLE_TEXT_DECOR: 
        case LV_STYLE_IMG_OPA: 
        case LV_STYLE_IMG_RECOLOR_OPA: 
        case LV_STYLE_OUTLINE_OPA: 
        case LV_STYLE_SHADOW_OPA: 
        case LV_STYLE_LINE_ROUNDED: 
        case LV_STYLE_LINE_OPA: 
        case LV_STYLE_ARC_ROUNDED: 
        case LV_STYLE_ARC_OPA: 

            return JS_ToUint32(ctx, (uint32_t*)&(value->num), jsval)==0 ;
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
        default:
            return false ;
    }
}
// AUTO GENERATE CODE END [STYLE VALUE SETTER/GETTER] --------

JSValue _js_lv_style_set_prop(JSContext *ctx, lv_style_t * style, JSValue jsProp, JSValue jsValue) {

    lv_style_value_t value ;
    const char * propName = JS_ToCString(ctx, jsProp) ;

    if( strcmp(propName,"pad")==0 ) {
        if(JS_ToInt32(ctx, &(value.num), jsValue)!=0) {
            THROW_EXCEPTION("invalid pad value")
        }
        lv_style_set_prop(style, LV_STYLE_PAD_TOP, value) ;
        lv_style_set_prop(style, LV_STYLE_PAD_BOTTOM, value) ;
        lv_style_set_prop(style, LV_STYLE_PAD_LEFT, value) ;
        lv_style_set_prop(style, LV_STYLE_PAD_RIGHT, value) ;
        
        JS_FreeCString(ctx, propName) ;
        return JS_UNDEFINED ;
    }

    lv_style_prop_t prop ;
    if(!lv_style_prop_jsstr_to_const(ctx, jsProp, &prop)){
        JS_ThrowReferenceError(ctx, "unknow style name: %s", propName) ;
        JS_FreeCString(ctx, propName) ;
        return JS_EXCEPTION ;
    }
    JS_FreeCString(ctx, propName) ;

    if(!lv_style_js_to_value(ctx, prop, jsValue, &value)){
        THROW_EXCEPTION("style value invalid")
    }

    lv_style_set_prop(style, prop, value) ;

    return JS_UNDEFINED ;
}


static JSValue js_lv_style_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv) {
    lv_style_t * style = malloc(sizeof(lv_style_t));
    lv_style_init(style) ;

    if(argc>0 && JS_IsObject(argv[0])) {
        uint32_t len;
        JSPropertyEnum *tab;
        if (JS_GetOwnPropertyNames(ctx, &tab, &len, argv[0], JS_GPN_STRING_MASK | JS_GPN_ENUM_ONLY) < 0) {
            THROW_EXCEPTION("invalid styles object") ;
        }
        for(uint32_t i = 0; i < len; i++) {
            JSValue key = JS_AtomToValue(ctx, tab[i].atom);
            JSValue val = JS_GetProperty(ctx, argv[0], tab[i].atom);

            if( JS_IsException(_js_lv_style_set_prop(ctx, style, key, val)) ) {
                free(style) ;
                return JS_EXCEPTION ;
            }
        }
    }

    return create_js_style(ctx, style) ;
}

static void js_lvgl_style_finalizer(JSRuntime *rt, JSValue val) {

    // printf("js_lvgl_style_finalizer()\n") ;

    // lv_style_t * style = (lv_style_t *)JS_GetOpaque(val, js_lv_style_class_id) ;
    // if(style) { 
    //     lv_style_reset(style) ;
    //     free(style) ;
    // }
}
static JSClassDef js_lv_style_class = {
    "lvgl.Style",
    .finalizer = js_lvgl_style_finalizer,
};


static JSValue js_lv_style_to_string(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    printf("js_lv_style_to_string") ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_style_get_prop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    if(!JS_IsString(argv[0])) {
        THROW_EXCEPTION("arg style name must be a string")
    }
    lv_style_prop_t prop ;
    if(!lv_style_prop_jsstr_to_const(ctx, argv[0], &prop)){
        THROW_EXCEPTION("unknow style name pass in")
    }

    THIS_OBJ("Style", "get", cstyle, lv_style_t)

    lv_style_value_t value ;
    if( !lv_style_get_prop(cstyle, prop, &value) ){
        return JS_UNDEFINED ;
    }

    return lv_style_value_to_js(ctx, prop, value) ;
}


static JSValue js_lv_style_set_prop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    if( argc==1 && JS_IsObject(argv[0]) ) {
        
        return JS_UNDEFINED ;
    }

    CHECK_ARGC(2)
    if(!JS_IsString(argv[0])) {
        THROW_EXCEPTION("arg style name must be a string")
    }

    THIS_OBJ("Style", "set", cstyle, lv_style_t)

    return _js_lv_style_set_prop(ctx, cstyle, argv[0], argv[1]) ;
}
static JSValue js_lv_style_props(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    THIS_OBJ("Style", "props", style, lv_style_t)

    JSValue arr = JS_NewArray(ctx) ;

    if(style->prop_cnt==1) {
        const char * propName = lv_style_prop_const_to_str(style->prop1) ;
        JS_SetPropertyUint32(ctx, arr, 0, JS_NewString(ctx, propName));
    }
    else {
        lv_style_value_t * values = (lv_style_value_t *)style->v_p.values_and_props;
        uint8_t * tmp = style->v_p.values_and_props + style->prop_cnt * sizeof(lv_style_value_t);
        uint16_t * props = (uint16_t *)tmp;

        for(size_t j=0;j<style->prop_cnt;j++) {
            const char * propName = lv_style_prop_const_to_str(props[j]) ;
            JS_SetPropertyUint32(ctx, arr, j, JS_NewString(ctx, propName));
        }
    }

    return arr ;
}


// static JSValue js_lv_style_setFont(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
//     THIS_OBJ("Style", "props", style, lv_style_t)
//     lv_style_set_text_font(style, &lv_font_montserrat_48) ;
//     return JS_UNDEFINED ;
// }

static JSValue js_lv_style_set_pad_all(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_INT16(0, pad)
    THIS_OBJ("Style", "setPadAll", style, lv_style_t)

    lv_style_set_pad_all(style, pad) ;

    return JS_UNDEFINED ;
}

static const JSCFunctionListEntry js_lv_style_proto_funcs[] = {
    JS_CFUNC_DEF("get", 0, js_lv_style_get_prop),
    JS_CFUNC_DEF("set", 0, js_lv_style_set_prop),
    JS_CFUNC_DEF("props", 0, js_lv_style_props),
    JS_CFUNC_DEF("setPadAll", 0, js_lv_style_set_pad_all),
    // JS_CFUNC_DEF("setFont", 0, js_lv_style_setFont),
} ;


static JSValue js_lv_palette_color(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)

    ARGV_TO_STRING_E(0, colorName, "arg color name must be a string")

    lv_palette_t palette ;
    if(!lv_palette_str_to_const(colorName, &palette)) {
        JS_ThrowReferenceError(ctx, "unknow color name: %s", colorName) ;
        JS_FreeCString(ctx, colorName) ;
        return JS_EXCEPTION ;
    }
    JS_FreeCString(ctx, colorName) ;

    if(argc>1) {
        ARGV_TO_UINT8(1, level)
        if( level<1 || level>4 ) {
            THROW_EXCEPTION("arg level must between 1 to 4")
        }
        lv_color_t color = lv_palette_darken(palette,level) ;
        return JS_NewUint32(ctx, color.full) ;
    } 
    else {
        lv_color_t color = lv_palette_main(palette) ;
        return JS_NewUint32(ctx, color.full) ;
    }

}



void init_lvgl_style() {
    LV_STYLE_JSVALUE = lv_style_register_prop() ;
    JS_NewClassID(&js_lv_style_class_id);
}

void require_vlgl_js_style(JSContext *ctx, JSValue lvgl) {
    QJS_DEF_CLASS(lv_style, "Style", "lvgl.Style", JS_UNDEFINED, lvgl)

    // JSValue palette = JS_NewObject(ctx) ;
    // JS_SetPropertyStr(ctx, "palette", palette) ;
    JS_SetPropertyStr(ctx, lvgl, "palette", JS_NewCFunction(ctx, js_lv_palette_color, "palette", 1));
}


