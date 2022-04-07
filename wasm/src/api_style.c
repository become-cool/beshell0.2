#include <stdlib.h>
#include <emscripten.h>
#define LV_USE_FLEX 1
#include "lv_conf.h"
#include "lvgl.h"
#include "../../main/debug.h"

// const_mapping.c
char * lv_flex_flow_to_name(uint32_t v) ;
char * lv_flex_align_to_name(uint32_t v) ;
char * lv_align_to_name(uint32_t v) ;
char * lv_base_dir_to_name(uint32_t v) ;
char * lv_border_side_to_name(uint32_t v) ;
char * lv_text_align_to_name(uint32_t v) ;

lv_style_prop_t lv_style_prop_to_value(char *) ;

#define STYLE_TYPE_NUMBER   1
#define STYLE_TYPE_STRING   2     // const name or other string
#define STYLE_TYPE_COLOR    3
#define STYLE_TYPE_UNKNOW   0
uint8_t _style_datatype(lv_style_prop_t prop) {
    if( prop==LV_STYLE_FLEX_FLOW
        || prop==LV_STYLE_FLEX_MAIN_PLACE
        || prop==LV_STYLE_FLEX_CROSS_PLACE
        || prop==LV_STYLE_FLEX_TRACK_PLACE
    ) {
        return STYLE_TYPE_STRING ;
    }
    if( prop==LV_STYLE_FLEX_GROW ) {
        return STYLE_TYPE_NUMBER ;
    }
    switch(prop) {
        case LV_STYLE_ALIGN:
        case LV_STYLE_BASE_DIR:
        case LV_STYLE_BORDER_SIDE:
        case LV_STYLE_TEXT_ALIGN:
            return STYLE_TYPE_STRING ;

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
            return STYLE_TYPE_NUMBER ;

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
            return STYLE_TYPE_COLOR ;

        default:
            return STYLE_TYPE_UNKNOW ;
    }
}
EMSCRIPTEN_KEEPALIVE int lv_style_datatype(char * stylename) {
    lv_style_prop_t prop = lv_style_prop_to_value(stylename) ;
    if(((int32_t)prop)<0) {
        return STYLE_TYPE_UNKNOW ;
    }
    return _style_datatype(prop) ;
}

EMSCRIPTEN_KEEPALIVE bool js_lv_obj_set_style_num(lv_obj_t * obj, char * stylename, int value, lv_style_selector_t selector) {
    lv_style_prop_t prop = lv_style_prop_to_value(stylename) ;
    if(((int32_t)prop)<0) {
        return false ;
    }
    if(_style_datatype(prop)!=STYLE_TYPE_NUMBER) {
        return false ;
    }
    lv_style_value_t styleValue ;
    styleValue.num = value ;    

    lv_obj_set_local_style_prop(obj, prop, styleValue, selector) ;
    return true ;
}
EMSCRIPTEN_KEEPALIVE bool js_lv_obj_set_style_string(lv_obj_t * obj, char * stylename, char * value, lv_style_selector_t selector) {
    lv_style_prop_t prop = lv_style_prop_to_value(stylename) ;
    if(((int32_t)prop)<0) {
        return false ;
    }
    if(_style_datatype(prop)!=STYLE_TYPE_STRING) {
        return false ;
    }
    lv_style_value_t styleValue ;
    styleValue.ptr = (void *)value ;    

    lv_obj_set_local_style_prop(obj, prop, styleValue, selector) ;
    return true ;
}
EMSCRIPTEN_KEEPALIVE bool js_lv_obj_set_style_color(lv_obj_t * obj, char * stylename, int value, lv_style_selector_t selector) {
    lv_style_prop_t prop = lv_style_prop_to_value(stylename) ;
    if(((int32_t)prop)<0) {
        return false ;
    }
    if(_style_datatype(prop)!=STYLE_TYPE_COLOR) {
        return false ;
    }
    lv_style_value_t styleValue ;
    styleValue.color.full = value ;    

    lv_obj_set_local_style_prop(obj, prop, styleValue, selector) ;
    return true ;
}


char * lv_style_value_to_string(lv_style_prop_t prop, lv_style_value_t value) {
    if( prop==LV_STYLE_FLEX_FLOW ){
        return lv_flex_flow_to_name((uint32_t)value.num) ;
    }
    else if( prop==LV_STYLE_FLEX_MAIN_PLACE
        || prop==LV_STYLE_FLEX_CROSS_PLACE
        || prop==LV_STYLE_FLEX_TRACK_PLACE
    ) {
        return lv_flex_align_to_name((uint32_t)value.num) ;
    }
    switch(prop) {
        case LV_STYLE_ALIGN:
            return lv_align_to_name((uint32_t)value.num) ;
        case LV_STYLE_BASE_DIR:
            return lv_base_dir_to_name((uint32_t)value.num) ;
        case LV_STYLE_BORDER_SIDE:
            return lv_border_side_to_name((uint32_t)value.num) ;
        case LV_STYLE_TEXT_ALIGN:
            return lv_text_align_to_name((uint32_t)value.num) ;
    }
    
    return "unknow type" ;
}

 EMSCRIPTEN_KEEPALIVE void lv_obj_get_style(lv_obj_t * obj, char * styleName, lv_style_selector_t selector) {

    lv_style_prop_t prop = lv_style_prop_to_value(styleName);
    if(((int32_t)prop)>=0) {
        lv_style_value_t value = lv_obj_get_style_prop(obj, selector, prop) ;

        uint8_t datatype = _style_datatype(prop) ;

        switch(datatype) {
            case STYLE_TYPE_NUMBER:
                EM_ASM_ARGS({
                    Module.__return = $0;
                    Module.__lastError = null
                }, value.num);
                return ;
            case STYLE_TYPE_COLOR:
                EM_ASM_ARGS({
                    Module.__return = $0;
                    Module.__lastError = null
                }, value.color.full);
                return ;
            case STYLE_TYPE_STRING: {
                char * sval = lv_style_value_to_string(prop, value) ;
                if(sval!=NULL) {
                    EM_ASM_ARGS({
                        Module.__return = Module.asString($0);
                        Module.__lastError = null
                    }, sval);
                    return ;
                }
            }
        }
    }

    EM_ASM_ARGS({
        Module.__lastError = new Error("unknow style name:"+Module.asString($0))
    }, styleName);
}

// JSValue js_lv_obj_get_local_style(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    
//     lv_style_selector_t selector = LV_PART_MAIN | LV_STATE_DEFAULT ;
//     if(argc>1) {
//         if(JS_ToUint32(ctx, &selector, argv[1])!=0) {
//             THROW_EXCEPTION("invalid arg part")
//         }
//     }
    
//     THIS_LVOBJ("Obj", "localStyle", thisobj)

//     for(int i = 0; i < thisobj->style_cnt; i++) {
//         if(thisobj->styles[i].is_local && thisobj->styles[i].selector == selector) {
//             JSValue jsstyle = lv_style_wrapper(ctx, thisobj->styles[i].style) ;
//             JS_DupValue(ctx,jsstyle) ;
//             return jsstyle ;
//         }
//     }
//     return JS_UNDEFINED ;
// }