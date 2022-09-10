#include "widgets_gen.h"
#include "widgets.h"
#include "display.h"
#include "style.h"
#include "be_lv_struct_wrapper.h"

#include "utils.h"
#include "cutils.h"
#include "lvgl.h"
#include "lv_conf.h"
#include "lv_obj_init.js.h"


// AUTO GENERATE CODE START [CONST MAPPING] --------

bool lv_flex_flow_str_to_const(const char * name, lv_flex_flow_t* out) {
    if(strcmp(name,"row")==0) {
        (*out) = LV_FLEX_FLOW_ROW ;
    }
    else if(strcmp(name,"column")==0) {
        (*out) = LV_FLEX_FLOW_COLUMN ;
    }
    else if(strcmp(name,"row-wrap")==0) {
        (*out) = LV_FLEX_FLOW_ROW_WRAP ;
    }
    else if(strcmp(name,"row-reverse")==0) {
        (*out) = LV_FLEX_FLOW_ROW_REVERSE ;
    }
    else if(strcmp(name,"row-wrap-reverse")==0) {
        (*out) = LV_FLEX_FLOW_ROW_WRAP_REVERSE ;
    }
    else if(strcmp(name,"column-wrap")==0) {
        (*out) = LV_FLEX_FLOW_COLUMN_WRAP ;
    }
    else if(strcmp(name,"column-reverse")==0) {
        (*out) = LV_FLEX_FLOW_COLUMN_REVERSE ;
    }
    else if(strcmp(name,"column-wrap-reverse")==0) {
        (*out) = LV_FLEX_FLOW_COLUMN_WRAP_REVERSE ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_flex_flow_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_flex_flow_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_flex_flow_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_flex_flow_const_to_str(lv_flex_flow_t code) {

    switch(code) {
        case LV_FLEX_FLOW_ROW: return "row";
        case LV_FLEX_FLOW_COLUMN: return "column";
        case LV_FLEX_FLOW_ROW_WRAP: return "row-wrap";
        case LV_FLEX_FLOW_ROW_REVERSE: return "row-reverse";
        case LV_FLEX_FLOW_ROW_WRAP_REVERSE: return "row-wrap-reverse";
        case LV_FLEX_FLOW_COLUMN_WRAP: return "column-wrap";
        case LV_FLEX_FLOW_COLUMN_REVERSE: return "column-reverse";
        case LV_FLEX_FLOW_COLUMN_WRAP_REVERSE: return "column-wrap-reverse";

        default:
            return "unknow";
    }
}
JSValue lv_flex_flow_const_to_jsstr(JSContext *ctx, lv_flex_flow_t code) {
    return JS_NewString(ctx, lv_flex_flow_const_to_str(code));
}

bool lv_flex_align_str_to_const(const char * name, lv_flex_align_t* out) {
    if(strcmp(name,"start")==0) {
        (*out) = LV_FLEX_ALIGN_START ;
    }
    else if(strcmp(name,"end")==0) {
        (*out) = LV_FLEX_ALIGN_END ;
    }
    else if(strcmp(name,"center")==0) {
        (*out) = LV_FLEX_ALIGN_CENTER ;
    }
    else if(strcmp(name,"space-evenly")==0) {
        (*out) = LV_FLEX_ALIGN_SPACE_EVENLY ;
    }
    else if(strcmp(name,"space-around")==0) {
        (*out) = LV_FLEX_ALIGN_SPACE_AROUND ;
    }
    else if(strcmp(name,"space-between")==0) {
        (*out) = LV_FLEX_ALIGN_SPACE_BETWEEN ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_flex_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_flex_align_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_flex_align_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_flex_align_const_to_str(lv_flex_align_t code) {

    switch(code) {
        case LV_FLEX_ALIGN_START: return "start";
        case LV_FLEX_ALIGN_END: return "end";
        case LV_FLEX_ALIGN_CENTER: return "center";
        case LV_FLEX_ALIGN_SPACE_EVENLY: return "space-evenly";
        case LV_FLEX_ALIGN_SPACE_AROUND: return "space-around";
        case LV_FLEX_ALIGN_SPACE_BETWEEN: return "space-between";

        default:
            return "unknow";
    }
}
JSValue lv_flex_align_const_to_jsstr(JSContext *ctx, lv_flex_align_t code) {
    return JS_NewString(ctx, lv_flex_align_const_to_str(code));
}

bool lv_event_code_str_to_const(const char * name, lv_event_code_t* out) {
    if(strcmp(name,"all")==0) {
        (*out) = LV_EVENT_ALL ;
    }
    else if(strcmp(name,"pressed")==0) {
        (*out) = LV_EVENT_PRESSED ;
    }
    else if(strcmp(name,"pressing")==0) {
        (*out) = LV_EVENT_PRESSING ;
    }
    else if(strcmp(name,"press-lost")==0) {
        (*out) = LV_EVENT_PRESS_LOST ;
    }
    else if(strcmp(name,"short-clicked")==0) {
        (*out) = LV_EVENT_SHORT_CLICKED ;
    }
    else if(strcmp(name,"long-pressed")==0) {
        (*out) = LV_EVENT_LONG_PRESSED ;
    }
    else if(strcmp(name,"long-pressed-repeat")==0) {
        (*out) = LV_EVENT_LONG_PRESSED_REPEAT ;
    }
    else if(strcmp(name,"clicked")==0) {
        (*out) = LV_EVENT_CLICKED ;
    }
    else if(strcmp(name,"released")==0) {
        (*out) = LV_EVENT_RELEASED ;
    }
    else if(strcmp(name,"scroll-begin")==0) {
        (*out) = LV_EVENT_SCROLL_BEGIN ;
    }
    else if(strcmp(name,"scroll-end")==0) {
        (*out) = LV_EVENT_SCROLL_END ;
    }
    else if(strcmp(name,"scroll")==0) {
        (*out) = LV_EVENT_SCROLL ;
    }
    else if(strcmp(name,"gesture")==0) {
        (*out) = LV_EVENT_GESTURE ;
    }
    else if(strcmp(name,"key")==0) {
        (*out) = LV_EVENT_KEY ;
    }
    else if(strcmp(name,"focused")==0) {
        (*out) = LV_EVENT_FOCUSED ;
    }
    else if(strcmp(name,"defocused")==0) {
        (*out) = LV_EVENT_DEFOCUSED ;
    }
    else if(strcmp(name,"leave")==0) {
        (*out) = LV_EVENT_LEAVE ;
    }
    else if(strcmp(name,"hit-test")==0) {
        (*out) = LV_EVENT_HIT_TEST ;
    }
    else if(strcmp(name,"cover-check")==0) {
        (*out) = LV_EVENT_COVER_CHECK ;
    }
    else if(strcmp(name,"refr-ext-draw-size")==0) {
        (*out) = LV_EVENT_REFR_EXT_DRAW_SIZE ;
    }
    else if(strcmp(name,"draw-main-begin")==0) {
        (*out) = LV_EVENT_DRAW_MAIN_BEGIN ;
    }
    else if(strcmp(name,"draw-main")==0) {
        (*out) = LV_EVENT_DRAW_MAIN ;
    }
    else if(strcmp(name,"draw-main-end")==0) {
        (*out) = LV_EVENT_DRAW_MAIN_END ;
    }
    else if(strcmp(name,"draw-post-begin")==0) {
        (*out) = LV_EVENT_DRAW_POST_BEGIN ;
    }
    else if(strcmp(name,"draw-post")==0) {
        (*out) = LV_EVENT_DRAW_POST ;
    }
    else if(strcmp(name,"draw-post-end")==0) {
        (*out) = LV_EVENT_DRAW_POST_END ;
    }
    else if(strcmp(name,"draw-part-begin")==0) {
        (*out) = LV_EVENT_DRAW_PART_BEGIN ;
    }
    else if(strcmp(name,"draw-part-end")==0) {
        (*out) = LV_EVENT_DRAW_PART_END ;
    }
    else if(strcmp(name,"value-changed")==0) {
        (*out) = LV_EVENT_VALUE_CHANGED ;
    }
    else if(strcmp(name,"insert")==0) {
        (*out) = LV_EVENT_INSERT ;
    }
    else if(strcmp(name,"refresh")==0) {
        (*out) = LV_EVENT_REFRESH ;
    }
    else if(strcmp(name,"ready")==0) {
        (*out) = LV_EVENT_READY ;
    }
    else if(strcmp(name,"cancel")==0) {
        (*out) = LV_EVENT_CANCEL ;
    }
    else if(strcmp(name,"delete")==0) {
        (*out) = LV_EVENT_DELETE ;
    }
    else if(strcmp(name,"child-changed")==0) {
        (*out) = LV_EVENT_CHILD_CHANGED ;
    }
    else if(strcmp(name,"child-created")==0) {
        (*out) = LV_EVENT_CHILD_CREATED ;
    }
    else if(strcmp(name,"child-deleted")==0) {
        (*out) = LV_EVENT_CHILD_DELETED ;
    }
    else if(strcmp(name,"screen-unload-start")==0) {
        (*out) = LV_EVENT_SCREEN_UNLOAD_START ;
    }
    else if(strcmp(name,"screen-load-start")==0) {
        (*out) = LV_EVENT_SCREEN_LOAD_START ;
    }
    else if(strcmp(name,"screen-loaded")==0) {
        (*out) = LV_EVENT_SCREEN_LOADED ;
    }
    else if(strcmp(name,"screen-unloaded")==0) {
        (*out) = LV_EVENT_SCREEN_UNLOADED ;
    }
    else if(strcmp(name,"size-changed")==0) {
        (*out) = LV_EVENT_SIZE_CHANGED ;
    }
    else if(strcmp(name,"style-changed")==0) {
        (*out) = LV_EVENT_STYLE_CHANGED ;
    }
    else if(strcmp(name,"layout-changed")==0) {
        (*out) = LV_EVENT_LAYOUT_CHANGED ;
    }
    else if(strcmp(name,"get-self-size")==0) {
        (*out) = LV_EVENT_GET_SELF_SIZE ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_event_code_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_event_code_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_event_code_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_event_code_const_to_str(lv_event_code_t code) {

    switch(code) {
        case LV_EVENT_ALL: return "all";
        case LV_EVENT_PRESSED: return "pressed";
        case LV_EVENT_PRESSING: return "pressing";
        case LV_EVENT_PRESS_LOST: return "press-lost";
        case LV_EVENT_SHORT_CLICKED: return "short-clicked";
        case LV_EVENT_LONG_PRESSED: return "long-pressed";
        case LV_EVENT_LONG_PRESSED_REPEAT: return "long-pressed-repeat";
        case LV_EVENT_CLICKED: return "clicked";
        case LV_EVENT_RELEASED: return "released";
        case LV_EVENT_SCROLL_BEGIN: return "scroll-begin";
        case LV_EVENT_SCROLL_END: return "scroll-end";
        case LV_EVENT_SCROLL: return "scroll";
        case LV_EVENT_GESTURE: return "gesture";
        case LV_EVENT_KEY: return "key";
        case LV_EVENT_FOCUSED: return "focused";
        case LV_EVENT_DEFOCUSED: return "defocused";
        case LV_EVENT_LEAVE: return "leave";
        case LV_EVENT_HIT_TEST: return "hit-test";
        case LV_EVENT_COVER_CHECK: return "cover-check";
        case LV_EVENT_REFR_EXT_DRAW_SIZE: return "refr-ext-draw-size";
        case LV_EVENT_DRAW_MAIN_BEGIN: return "draw-main-begin";
        case LV_EVENT_DRAW_MAIN: return "draw-main";
        case LV_EVENT_DRAW_MAIN_END: return "draw-main-end";
        case LV_EVENT_DRAW_POST_BEGIN: return "draw-post-begin";
        case LV_EVENT_DRAW_POST: return "draw-post";
        case LV_EVENT_DRAW_POST_END: return "draw-post-end";
        case LV_EVENT_DRAW_PART_BEGIN: return "draw-part-begin";
        case LV_EVENT_DRAW_PART_END: return "draw-part-end";
        case LV_EVENT_VALUE_CHANGED: return "value-changed";
        case LV_EVENT_INSERT: return "insert";
        case LV_EVENT_REFRESH: return "refresh";
        case LV_EVENT_READY: return "ready";
        case LV_EVENT_CANCEL: return "cancel";
        case LV_EVENT_DELETE: return "delete";
        case LV_EVENT_CHILD_CHANGED: return "child-changed";
        case LV_EVENT_CHILD_CREATED: return "child-created";
        case LV_EVENT_CHILD_DELETED: return "child-deleted";
        case LV_EVENT_SCREEN_UNLOAD_START: return "screen-unload-start";
        case LV_EVENT_SCREEN_LOAD_START: return "screen-load-start";
        case LV_EVENT_SCREEN_LOADED: return "screen-loaded";
        case LV_EVENT_SCREEN_UNLOADED: return "screen-unloaded";
        case LV_EVENT_SIZE_CHANGED: return "size-changed";
        case LV_EVENT_STYLE_CHANGED: return "style-changed";
        case LV_EVENT_LAYOUT_CHANGED: return "layout-changed";
        case LV_EVENT_GET_SELF_SIZE: return "get-self-size";

        default:
            return "unknow";
    }
}
JSValue lv_event_code_const_to_jsstr(JSContext *ctx, lv_event_code_t code) {
    return JS_NewString(ctx, lv_event_code_const_to_str(code));
}

bool lv_dir_str_to_const(const char * name, lv_dir_t* out) {
    if(strcmp(name,"none")==0) {
        (*out) = LV_DIR_NONE ;
    }
    else if(strcmp(name,"left")==0) {
        (*out) = LV_DIR_LEFT ;
    }
    else if(strcmp(name,"right")==0) {
        (*out) = LV_DIR_RIGHT ;
    }
    else if(strcmp(name,"top")==0) {
        (*out) = LV_DIR_TOP ;
    }
    else if(strcmp(name,"bottom")==0) {
        (*out) = LV_DIR_BOTTOM ;
    }
    else if(strcmp(name,"hor")==0) {
        (*out) = LV_DIR_HOR ;
    }
    else if(strcmp(name,"ver")==0) {
        (*out) = LV_DIR_VER ;
    }
    else if(strcmp(name,"all")==0) {
        (*out) = LV_DIR_ALL ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_dir_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_dir_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_dir_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_dir_const_to_str(lv_dir_t code) {

    switch(code) {
        case LV_DIR_NONE: return "none";
        case LV_DIR_LEFT: return "left";
        case LV_DIR_RIGHT: return "right";
        case LV_DIR_TOP: return "top";
        case LV_DIR_BOTTOM: return "bottom";
        case LV_DIR_HOR: return "hor";
        case LV_DIR_VER: return "ver";
        case LV_DIR_ALL: return "all";

        default:
            return "unknow";
    }
}
JSValue lv_dir_const_to_jsstr(JSContext *ctx, lv_dir_t code) {
    return JS_NewString(ctx, lv_dir_const_to_str(code));
}

bool lv_align_str_to_const(const char * name, lv_align_t* out) {
    if(strcmp(name,"default")==0) {
        (*out) = LV_ALIGN_DEFAULT ;
    }
    else if(strcmp(name,"top-left")==0) {
        (*out) = LV_ALIGN_TOP_LEFT ;
    }
    else if(strcmp(name,"top-mid")==0) {
        (*out) = LV_ALIGN_TOP_MID ;
    }
    else if(strcmp(name,"top-right")==0) {
        (*out) = LV_ALIGN_TOP_RIGHT ;
    }
    else if(strcmp(name,"bottom-left")==0) {
        (*out) = LV_ALIGN_BOTTOM_LEFT ;
    }
    else if(strcmp(name,"bottom-mid")==0) {
        (*out) = LV_ALIGN_BOTTOM_MID ;
    }
    else if(strcmp(name,"bottom-right")==0) {
        (*out) = LV_ALIGN_BOTTOM_RIGHT ;
    }
    else if(strcmp(name,"left-mid")==0) {
        (*out) = LV_ALIGN_LEFT_MID ;
    }
    else if(strcmp(name,"right-mid")==0) {
        (*out) = LV_ALIGN_RIGHT_MID ;
    }
    else if(strcmp(name,"center")==0) {
        (*out) = LV_ALIGN_CENTER ;
    }
    else if(strcmp(name,"out-top-left")==0) {
        (*out) = LV_ALIGN_OUT_TOP_LEFT ;
    }
    else if(strcmp(name,"out-top-mid")==0) {
        (*out) = LV_ALIGN_OUT_TOP_MID ;
    }
    else if(strcmp(name,"out-top-right")==0) {
        (*out) = LV_ALIGN_OUT_TOP_RIGHT ;
    }
    else if(strcmp(name,"out-bottom-left")==0) {
        (*out) = LV_ALIGN_OUT_BOTTOM_LEFT ;
    }
    else if(strcmp(name,"out-bottom-mid")==0) {
        (*out) = LV_ALIGN_OUT_BOTTOM_MID ;
    }
    else if(strcmp(name,"out-bottom-right")==0) {
        (*out) = LV_ALIGN_OUT_BOTTOM_RIGHT ;
    }
    else if(strcmp(name,"out-left-top")==0) {
        (*out) = LV_ALIGN_OUT_LEFT_TOP ;
    }
    else if(strcmp(name,"out-left-mid")==0) {
        (*out) = LV_ALIGN_OUT_LEFT_MID ;
    }
    else if(strcmp(name,"out-left-bottom")==0) {
        (*out) = LV_ALIGN_OUT_LEFT_BOTTOM ;
    }
    else if(strcmp(name,"out-right-top")==0) {
        (*out) = LV_ALIGN_OUT_RIGHT_TOP ;
    }
    else if(strcmp(name,"out-right-mid")==0) {
        (*out) = LV_ALIGN_OUT_RIGHT_MID ;
    }
    else if(strcmp(name,"out-right-bottom")==0) {
        (*out) = LV_ALIGN_OUT_RIGHT_BOTTOM ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_align_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_align_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_align_const_to_str(lv_align_t code) {

    switch(code) {
        case LV_ALIGN_DEFAULT: return "default";
        case LV_ALIGN_TOP_LEFT: return "top-left";
        case LV_ALIGN_TOP_MID: return "top-mid";
        case LV_ALIGN_TOP_RIGHT: return "top-right";
        case LV_ALIGN_BOTTOM_LEFT: return "bottom-left";
        case LV_ALIGN_BOTTOM_MID: return "bottom-mid";
        case LV_ALIGN_BOTTOM_RIGHT: return "bottom-right";
        case LV_ALIGN_LEFT_MID: return "left-mid";
        case LV_ALIGN_RIGHT_MID: return "right-mid";
        case LV_ALIGN_CENTER: return "center";
        case LV_ALIGN_OUT_TOP_LEFT: return "out-top-left";
        case LV_ALIGN_OUT_TOP_MID: return "out-top-mid";
        case LV_ALIGN_OUT_TOP_RIGHT: return "out-top-right";
        case LV_ALIGN_OUT_BOTTOM_LEFT: return "out-bottom-left";
        case LV_ALIGN_OUT_BOTTOM_MID: return "out-bottom-mid";
        case LV_ALIGN_OUT_BOTTOM_RIGHT: return "out-bottom-right";
        case LV_ALIGN_OUT_LEFT_TOP: return "out-left-top";
        case LV_ALIGN_OUT_LEFT_MID: return "out-left-mid";
        case LV_ALIGN_OUT_LEFT_BOTTOM: return "out-left-bottom";
        case LV_ALIGN_OUT_RIGHT_TOP: return "out-right-top";
        case LV_ALIGN_OUT_RIGHT_MID: return "out-right-mid";
        case LV_ALIGN_OUT_RIGHT_BOTTOM: return "out-right-bottom";

        default:
            return "unknow";
    }
}
JSValue lv_align_const_to_jsstr(JSContext *ctx, lv_align_t code) {
    return JS_NewString(ctx, lv_align_const_to_str(code));
}

bool lv_border_side_str_to_const(const char * name, lv_border_side_t* out) {
    if(strcmp(name,"none")==0) {
        (*out) = LV_BORDER_SIDE_NONE ;
    }
    else if(strcmp(name,"bottom")==0) {
        (*out) = LV_BORDER_SIDE_BOTTOM ;
    }
    else if(strcmp(name,"top")==0) {
        (*out) = LV_BORDER_SIDE_TOP ;
    }
    else if(strcmp(name,"left")==0) {
        (*out) = LV_BORDER_SIDE_LEFT ;
    }
    else if(strcmp(name,"right")==0) {
        (*out) = LV_BORDER_SIDE_RIGHT ;
    }
    else if(strcmp(name,"full")==0) {
        (*out) = LV_BORDER_SIDE_FULL ;
    }
    else if(strcmp(name,"internal")==0) {
        (*out) = LV_BORDER_SIDE_INTERNAL ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_border_side_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_border_side_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_border_side_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_border_side_const_to_str(lv_border_side_t code) {

    switch(code) {
        case LV_BORDER_SIDE_NONE: return "none";
        case LV_BORDER_SIDE_BOTTOM: return "bottom";
        case LV_BORDER_SIDE_TOP: return "top";
        case LV_BORDER_SIDE_LEFT: return "left";
        case LV_BORDER_SIDE_RIGHT: return "right";
        case LV_BORDER_SIDE_FULL: return "full";
        case LV_BORDER_SIDE_INTERNAL: return "internal";

        default:
            return "unknow";
    }
}
JSValue lv_border_side_const_to_jsstr(JSContext *ctx, lv_border_side_t code) {
    return JS_NewString(ctx, lv_border_side_const_to_str(code));
}

bool lv_base_dir_str_to_const(const char * name, lv_base_dir_t* out) {
    if(strcmp(name,"ltr")==0) {
        (*out) = LV_BASE_DIR_LTR ;
    }
    else if(strcmp(name,"rtl")==0) {
        (*out) = LV_BASE_DIR_RTL ;
    }
    else if(strcmp(name,"auto")==0) {
        (*out) = LV_BASE_DIR_AUTO ;
    }
    else if(strcmp(name,"neutral")==0) {
        (*out) = LV_BASE_DIR_NEUTRAL ;
    }
    else if(strcmp(name,"weak")==0) {
        (*out) = LV_BASE_DIR_WEAK ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_base_dir_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_base_dir_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_base_dir_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_base_dir_const_to_str(lv_base_dir_t code) {

    switch(code) {
        case LV_BASE_DIR_LTR: return "ltr";
        case LV_BASE_DIR_RTL: return "rtl";
        case LV_BASE_DIR_AUTO: return "auto";
        case LV_BASE_DIR_NEUTRAL: return "neutral";
        case LV_BASE_DIR_WEAK: return "weak";

        default:
            return "unknow";
    }
}
JSValue lv_base_dir_const_to_jsstr(JSContext *ctx, lv_base_dir_t code) {
    return JS_NewString(ctx, lv_base_dir_const_to_str(code));
}

bool lv_style_prop_str_to_const(const char * name, lv_style_prop_t* out) {
    if(strcmp(name,"prop-inv")==0) {
        (*out) = LV_STYLE_PROP_INV ;
    }
    else if(strcmp(name,"width")==0) {
        (*out) = LV_STYLE_WIDTH ;
    }
    else if(strcmp(name,"min-width")==0) {
        (*out) = LV_STYLE_MIN_WIDTH ;
    }
    else if(strcmp(name,"max-width")==0) {
        (*out) = LV_STYLE_MAX_WIDTH ;
    }
    else if(strcmp(name,"height")==0) {
        (*out) = LV_STYLE_HEIGHT ;
    }
    else if(strcmp(name,"min-height")==0) {
        (*out) = LV_STYLE_MIN_HEIGHT ;
    }
    else if(strcmp(name,"max-height")==0) {
        (*out) = LV_STYLE_MAX_HEIGHT ;
    }
    else if(strcmp(name,"x")==0) {
        (*out) = LV_STYLE_X ;
    }
    else if(strcmp(name,"y")==0) {
        (*out) = LV_STYLE_Y ;
    }
    else if(strcmp(name,"align")==0) {
        (*out) = LV_STYLE_ALIGN ;
    }
    else if(strcmp(name,"transform-width")==0) {
        (*out) = LV_STYLE_TRANSFORM_WIDTH ;
    }
    else if(strcmp(name,"transform-height")==0) {
        (*out) = LV_STYLE_TRANSFORM_HEIGHT ;
    }
    else if(strcmp(name,"translate-x")==0) {
        (*out) = LV_STYLE_TRANSLATE_X ;
    }
    else if(strcmp(name,"translate-y")==0) {
        (*out) = LV_STYLE_TRANSLATE_Y ;
    }
    else if(strcmp(name,"transform-zoom")==0) {
        (*out) = LV_STYLE_TRANSFORM_ZOOM ;
    }
    else if(strcmp(name,"transform-angle")==0) {
        (*out) = LV_STYLE_TRANSFORM_ANGLE ;
    }
    else if(strcmp(name,"pad-top")==0) {
        (*out) = LV_STYLE_PAD_TOP ;
    }
    else if(strcmp(name,"pad-bottom")==0) {
        (*out) = LV_STYLE_PAD_BOTTOM ;
    }
    else if(strcmp(name,"pad-left")==0) {
        (*out) = LV_STYLE_PAD_LEFT ;
    }
    else if(strcmp(name,"pad-right")==0) {
        (*out) = LV_STYLE_PAD_RIGHT ;
    }
    else if(strcmp(name,"pad-row")==0) {
        (*out) = LV_STYLE_PAD_ROW ;
    }
    else if(strcmp(name,"pad-column")==0) {
        (*out) = LV_STYLE_PAD_COLUMN ;
    }
    else if(strcmp(name,"bg-color")==0) {
        (*out) = LV_STYLE_BG_COLOR ;
    }
    else if(strcmp(name,"bg-color-filtered")==0) {
        (*out) = LV_STYLE_BG_COLOR_FILTERED ;
    }
    else if(strcmp(name,"bg-opa")==0) {
        (*out) = LV_STYLE_BG_OPA ;
    }
    else if(strcmp(name,"bg-grad-color")==0) {
        (*out) = LV_STYLE_BG_GRAD_COLOR ;
    }
    else if(strcmp(name,"bg-grad-color-filtered")==0) {
        (*out) = LV_STYLE_BG_GRAD_COLOR_FILTERED ;
    }
    else if(strcmp(name,"bg-grad-dir")==0) {
        (*out) = LV_STYLE_BG_GRAD_DIR ;
    }
    else if(strcmp(name,"bg-main-stop")==0) {
        (*out) = LV_STYLE_BG_MAIN_STOP ;
    }
    else if(strcmp(name,"bg-grad-stop")==0) {
        (*out) = LV_STYLE_BG_GRAD_STOP ;
    }
    else if(strcmp(name,"bg-img-src")==0) {
        (*out) = LV_STYLE_BG_IMG_SRC ;
    }
    else if(strcmp(name,"bg-img-opa")==0) {
        (*out) = LV_STYLE_BG_IMG_OPA ;
    }
    else if(strcmp(name,"bg-img-recolor")==0) {
        (*out) = LV_STYLE_BG_IMG_RECOLOR ;
    }
    else if(strcmp(name,"bg-img-recolor-filtered")==0) {
        (*out) = LV_STYLE_BG_IMG_RECOLOR_FILTERED ;
    }
    else if(strcmp(name,"bg-img-recolor-opa")==0) {
        (*out) = LV_STYLE_BG_IMG_RECOLOR_OPA ;
    }
    else if(strcmp(name,"bg-img-tiled")==0) {
        (*out) = LV_STYLE_BG_IMG_TILED ;
    }
    else if(strcmp(name,"border-color")==0) {
        (*out) = LV_STYLE_BORDER_COLOR ;
    }
    else if(strcmp(name,"border-color-filtered")==0) {
        (*out) = LV_STYLE_BORDER_COLOR_FILTERED ;
    }
    else if(strcmp(name,"border-opa")==0) {
        (*out) = LV_STYLE_BORDER_OPA ;
    }
    else if(strcmp(name,"border-width")==0) {
        (*out) = LV_STYLE_BORDER_WIDTH ;
    }
    else if(strcmp(name,"border-side")==0) {
        (*out) = LV_STYLE_BORDER_SIDE ;
    }
    else if(strcmp(name,"border-post")==0) {
        (*out) = LV_STYLE_BORDER_POST ;
    }
    else if(strcmp(name,"outline-width")==0) {
        (*out) = LV_STYLE_OUTLINE_WIDTH ;
    }
    else if(strcmp(name,"outline-color")==0) {
        (*out) = LV_STYLE_OUTLINE_COLOR ;
    }
    else if(strcmp(name,"outline-color-filtered")==0) {
        (*out) = LV_STYLE_OUTLINE_COLOR_FILTERED ;
    }
    else if(strcmp(name,"outline-opa")==0) {
        (*out) = LV_STYLE_OUTLINE_OPA ;
    }
    else if(strcmp(name,"outline-pad")==0) {
        (*out) = LV_STYLE_OUTLINE_PAD ;
    }
    else if(strcmp(name,"shadow-width")==0) {
        (*out) = LV_STYLE_SHADOW_WIDTH ;
    }
    else if(strcmp(name,"shadow-ofs-x")==0) {
        (*out) = LV_STYLE_SHADOW_OFS_X ;
    }
    else if(strcmp(name,"shadow-ofs-y")==0) {
        (*out) = LV_STYLE_SHADOW_OFS_Y ;
    }
    else if(strcmp(name,"shadow-spread")==0) {
        (*out) = LV_STYLE_SHADOW_SPREAD ;
    }
    else if(strcmp(name,"shadow-color")==0) {
        (*out) = LV_STYLE_SHADOW_COLOR ;
    }
    else if(strcmp(name,"shadow-color-filtered")==0) {
        (*out) = LV_STYLE_SHADOW_COLOR_FILTERED ;
    }
    else if(strcmp(name,"shadow-opa")==0) {
        (*out) = LV_STYLE_SHADOW_OPA ;
    }
    else if(strcmp(name,"img-opa")==0) {
        (*out) = LV_STYLE_IMG_OPA ;
    }
    else if(strcmp(name,"img-recolor")==0) {
        (*out) = LV_STYLE_IMG_RECOLOR ;
    }
    else if(strcmp(name,"img-recolor-filtered")==0) {
        (*out) = LV_STYLE_IMG_RECOLOR_FILTERED ;
    }
    else if(strcmp(name,"img-recolor-opa")==0) {
        (*out) = LV_STYLE_IMG_RECOLOR_OPA ;
    }
    else if(strcmp(name,"line-width")==0) {
        (*out) = LV_STYLE_LINE_WIDTH ;
    }
    else if(strcmp(name,"line-dash-width")==0) {
        (*out) = LV_STYLE_LINE_DASH_WIDTH ;
    }
    else if(strcmp(name,"line-dash-gap")==0) {
        (*out) = LV_STYLE_LINE_DASH_GAP ;
    }
    else if(strcmp(name,"line-rounded")==0) {
        (*out) = LV_STYLE_LINE_ROUNDED ;
    }
    else if(strcmp(name,"line-color")==0) {
        (*out) = LV_STYLE_LINE_COLOR ;
    }
    else if(strcmp(name,"line-color-filtered")==0) {
        (*out) = LV_STYLE_LINE_COLOR_FILTERED ;
    }
    else if(strcmp(name,"line-opa")==0) {
        (*out) = LV_STYLE_LINE_OPA ;
    }
    else if(strcmp(name,"arc-width")==0) {
        (*out) = LV_STYLE_ARC_WIDTH ;
    }
    else if(strcmp(name,"arc-rounded")==0) {
        (*out) = LV_STYLE_ARC_ROUNDED ;
    }
    else if(strcmp(name,"arc-color")==0) {
        (*out) = LV_STYLE_ARC_COLOR ;
    }
    else if(strcmp(name,"arc-color-filtered")==0) {
        (*out) = LV_STYLE_ARC_COLOR_FILTERED ;
    }
    else if(strcmp(name,"arc-opa")==0) {
        (*out) = LV_STYLE_ARC_OPA ;
    }
    else if(strcmp(name,"arc-img-src")==0) {
        (*out) = LV_STYLE_ARC_IMG_SRC ;
    }
    else if(strcmp(name,"text-color")==0) {
        (*out) = LV_STYLE_TEXT_COLOR ;
    }
    else if(strcmp(name,"text-color-filtered")==0) {
        (*out) = LV_STYLE_TEXT_COLOR_FILTERED ;
    }
    else if(strcmp(name,"text-opa")==0) {
        (*out) = LV_STYLE_TEXT_OPA ;
    }
    else if(strcmp(name,"text-font")==0) {
        (*out) = LV_STYLE_TEXT_FONT ;
    }
    else if(strcmp(name,"text-letter-space")==0) {
        (*out) = LV_STYLE_TEXT_LETTER_SPACE ;
    }
    else if(strcmp(name,"text-line-space")==0) {
        (*out) = LV_STYLE_TEXT_LINE_SPACE ;
    }
    else if(strcmp(name,"text-decor")==0) {
        (*out) = LV_STYLE_TEXT_DECOR ;
    }
    else if(strcmp(name,"text-align")==0) {
        (*out) = LV_STYLE_TEXT_ALIGN ;
    }
    else if(strcmp(name,"radius")==0) {
        (*out) = LV_STYLE_RADIUS ;
    }
    else if(strcmp(name,"clip-corner")==0) {
        (*out) = LV_STYLE_CLIP_CORNER ;
    }
    else if(strcmp(name,"opa")==0) {
        (*out) = LV_STYLE_OPA ;
    }
    else if(strcmp(name,"color-filter-dsc")==0) {
        (*out) = LV_STYLE_COLOR_FILTER_DSC ;
    }
    else if(strcmp(name,"color-filter-opa")==0) {
        (*out) = LV_STYLE_COLOR_FILTER_OPA ;
    }
    else if(strcmp(name,"anim-time")==0) {
        (*out) = LV_STYLE_ANIM_TIME ;
    }
    else if(strcmp(name,"anim-speed")==0) {
        (*out) = LV_STYLE_ANIM_SPEED ;
    }
    else if(strcmp(name,"transition")==0) {
        (*out) = LV_STYLE_TRANSITION ;
    }
    else if(strcmp(name,"blend-mode")==0) {
        (*out) = LV_STYLE_BLEND_MODE ;
    }
    else if(strcmp(name,"layout")==0) {
        (*out) = LV_STYLE_LAYOUT ;
    }
    else if(strcmp(name,"base-dir")==0) {
        (*out) = LV_STYLE_BASE_DIR ;
    }
    else if(strcmp(name,"-last-built-in-prop")==0) {
        (*out) = _LV_STYLE_LAST_BUILT_IN_PROP ;
    }
    else if(strcmp(name,"flex-flow")==0) {
        (*out) = LV_STYLE_FLEX_FLOW ;
    }
    else if(strcmp(name,"flex-main-place")==0) {
        (*out) = LV_STYLE_FLEX_MAIN_PLACE ;
    }
    else if(strcmp(name,"flex-cross-place")==0) {
        (*out) = LV_STYLE_FLEX_CROSS_PLACE ;
    }
    else if(strcmp(name,"flex-track-place")==0) {
        (*out) = LV_STYLE_FLEX_TRACK_PLACE ;
    }
    else if(strcmp(name,"flex-grow")==0) {
        (*out) = LV_STYLE_FLEX_GROW ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_style_prop_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_style_prop_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_style_prop_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_style_prop_const_to_str(lv_style_prop_t code) {
    if(code==LV_STYLE_FLEX_GROW) {
        return "flex-grow" ;
    }

    switch(code) {
        case LV_STYLE_PROP_INV: return "prop-inv";
        case LV_STYLE_WIDTH: return "width";
        case LV_STYLE_MIN_WIDTH: return "min-width";
        case LV_STYLE_MAX_WIDTH: return "max-width";
        case LV_STYLE_HEIGHT: return "height";
        case LV_STYLE_MIN_HEIGHT: return "min-height";
        case LV_STYLE_MAX_HEIGHT: return "max-height";
        case LV_STYLE_X: return "x";
        case LV_STYLE_Y: return "y";
        case LV_STYLE_ALIGN: return "align";
        case LV_STYLE_TRANSFORM_WIDTH: return "transform-width";
        case LV_STYLE_TRANSFORM_HEIGHT: return "transform-height";
        case LV_STYLE_TRANSLATE_X: return "translate-x";
        case LV_STYLE_TRANSLATE_Y: return "translate-y";
        case LV_STYLE_TRANSFORM_ZOOM: return "transform-zoom";
        case LV_STYLE_TRANSFORM_ANGLE: return "transform-angle";
        case LV_STYLE_PAD_TOP: return "pad-top";
        case LV_STYLE_PAD_BOTTOM: return "pad-bottom";
        case LV_STYLE_PAD_LEFT: return "pad-left";
        case LV_STYLE_PAD_RIGHT: return "pad-right";
        case LV_STYLE_PAD_ROW: return "pad-row";
        case LV_STYLE_PAD_COLUMN: return "pad-column";
        case LV_STYLE_BG_COLOR: return "bg-color";
        case LV_STYLE_BG_COLOR_FILTERED: return "bg-color-filtered";
        case LV_STYLE_BG_OPA: return "bg-opa";
        case LV_STYLE_BG_GRAD_COLOR: return "bg-grad-color";
        case LV_STYLE_BG_GRAD_COLOR_FILTERED: return "bg-grad-color-filtered";
        case LV_STYLE_BG_GRAD_DIR: return "bg-grad-dir";
        case LV_STYLE_BG_MAIN_STOP: return "bg-main-stop";
        case LV_STYLE_BG_GRAD_STOP: return "bg-grad-stop";
        case LV_STYLE_BG_IMG_SRC: return "bg-img-src";
        case LV_STYLE_BG_IMG_OPA: return "bg-img-opa";
        case LV_STYLE_BG_IMG_RECOLOR: return "bg-img-recolor";
        case LV_STYLE_BG_IMG_RECOLOR_FILTERED: return "bg-img-recolor-filtered";
        case LV_STYLE_BG_IMG_RECOLOR_OPA: return "bg-img-recolor-opa";
        case LV_STYLE_BG_IMG_TILED: return "bg-img-tiled";
        case LV_STYLE_BORDER_COLOR: return "border-color";
        case LV_STYLE_BORDER_COLOR_FILTERED: return "border-color-filtered";
        case LV_STYLE_BORDER_OPA: return "border-opa";
        case LV_STYLE_BORDER_WIDTH: return "border-width";
        case LV_STYLE_BORDER_SIDE: return "border-side";
        case LV_STYLE_BORDER_POST: return "border-post";
        case LV_STYLE_OUTLINE_WIDTH: return "outline-width";
        case LV_STYLE_OUTLINE_COLOR: return "outline-color";
        case LV_STYLE_OUTLINE_COLOR_FILTERED: return "outline-color-filtered";
        case LV_STYLE_OUTLINE_OPA: return "outline-opa";
        case LV_STYLE_OUTLINE_PAD: return "outline-pad";
        case LV_STYLE_SHADOW_WIDTH: return "shadow-width";
        case LV_STYLE_SHADOW_OFS_X: return "shadow-ofs-x";
        case LV_STYLE_SHADOW_OFS_Y: return "shadow-ofs-y";
        case LV_STYLE_SHADOW_SPREAD: return "shadow-spread";
        case LV_STYLE_SHADOW_COLOR: return "shadow-color";
        case LV_STYLE_SHADOW_COLOR_FILTERED: return "shadow-color-filtered";
        case LV_STYLE_SHADOW_OPA: return "shadow-opa";
        case LV_STYLE_IMG_OPA: return "img-opa";
        case LV_STYLE_IMG_RECOLOR: return "img-recolor";
        case LV_STYLE_IMG_RECOLOR_FILTERED: return "img-recolor-filtered";
        case LV_STYLE_IMG_RECOLOR_OPA: return "img-recolor-opa";
        case LV_STYLE_LINE_WIDTH: return "line-width";
        case LV_STYLE_LINE_DASH_WIDTH: return "line-dash-width";
        case LV_STYLE_LINE_DASH_GAP: return "line-dash-gap";
        case LV_STYLE_LINE_ROUNDED: return "line-rounded";
        case LV_STYLE_LINE_COLOR: return "line-color";
        case LV_STYLE_LINE_COLOR_FILTERED: return "line-color-filtered";
        case LV_STYLE_LINE_OPA: return "line-opa";
        case LV_STYLE_ARC_WIDTH: return "arc-width";
        case LV_STYLE_ARC_ROUNDED: return "arc-rounded";
        case LV_STYLE_ARC_COLOR: return "arc-color";
        case LV_STYLE_ARC_COLOR_FILTERED: return "arc-color-filtered";
        case LV_STYLE_ARC_OPA: return "arc-opa";
        case LV_STYLE_ARC_IMG_SRC: return "arc-img-src";
        case LV_STYLE_TEXT_COLOR: return "text-color";
        case LV_STYLE_TEXT_COLOR_FILTERED: return "text-color-filtered";
        case LV_STYLE_TEXT_OPA: return "text-opa";
        case LV_STYLE_TEXT_FONT: return "text-font";
        case LV_STYLE_TEXT_LETTER_SPACE: return "text-letter-space";
        case LV_STYLE_TEXT_LINE_SPACE: return "text-line-space";
        case LV_STYLE_TEXT_DECOR: return "text-decor";
        case LV_STYLE_TEXT_ALIGN: return "text-align";
        case LV_STYLE_RADIUS: return "radius";
        case LV_STYLE_CLIP_CORNER: return "clip-corner";
        case LV_STYLE_OPA: return "opa";
        case LV_STYLE_COLOR_FILTER_DSC: return "color-filter-dsc";
        case LV_STYLE_COLOR_FILTER_OPA: return "color-filter-opa";
        case LV_STYLE_ANIM_TIME: return "anim-time";
        case LV_STYLE_ANIM_SPEED: return "anim-speed";
        case LV_STYLE_TRANSITION: return "transition";
        case LV_STYLE_BLEND_MODE: return "blend-mode";
        case LV_STYLE_LAYOUT: return "layout";
        case LV_STYLE_BASE_DIR: return "base-dir";
        case _LV_STYLE_LAST_BUILT_IN_PROP: return "-last-built-in-prop";

        default:
            return "unknow";
    }
}
JSValue lv_style_prop_const_to_jsstr(JSContext *ctx, lv_style_prop_t code) {
    return JS_NewString(ctx, lv_style_prop_const_to_str(code));
}

bool lv_scr_load_anim_str_to_const(const char * name, lv_scr_load_anim_t* out) {
    if(strcmp(name,"none")==0) {
        (*out) = LV_SCR_LOAD_ANIM_NONE ;
    }
    else if(strcmp(name,"over-left")==0) {
        (*out) = LV_SCR_LOAD_ANIM_OVER_LEFT ;
    }
    else if(strcmp(name,"over-right")==0) {
        (*out) = LV_SCR_LOAD_ANIM_OVER_RIGHT ;
    }
    else if(strcmp(name,"over-top")==0) {
        (*out) = LV_SCR_LOAD_ANIM_OVER_TOP ;
    }
    else if(strcmp(name,"over-bottom")==0) {
        (*out) = LV_SCR_LOAD_ANIM_OVER_BOTTOM ;
    }
    else if(strcmp(name,"move-left")==0) {
        (*out) = LV_SCR_LOAD_ANIM_MOVE_LEFT ;
    }
    else if(strcmp(name,"move-right")==0) {
        (*out) = LV_SCR_LOAD_ANIM_MOVE_RIGHT ;
    }
    else if(strcmp(name,"move-top")==0) {
        (*out) = LV_SCR_LOAD_ANIM_MOVE_TOP ;
    }
    else if(strcmp(name,"move-bottom")==0) {
        (*out) = LV_SCR_LOAD_ANIM_MOVE_BOTTOM ;
    }
    else if(strcmp(name,"fade-on")==0) {
        (*out) = LV_SCR_LOAD_ANIM_FADE_ON ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_scr_load_anim_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_scr_load_anim_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_scr_load_anim_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_scr_load_anim_const_to_str(lv_scr_load_anim_t code) {

    switch(code) {
        case LV_SCR_LOAD_ANIM_NONE: return "none";
        case LV_SCR_LOAD_ANIM_OVER_LEFT: return "over-left";
        case LV_SCR_LOAD_ANIM_OVER_RIGHT: return "over-right";
        case LV_SCR_LOAD_ANIM_OVER_TOP: return "over-top";
        case LV_SCR_LOAD_ANIM_OVER_BOTTOM: return "over-bottom";
        case LV_SCR_LOAD_ANIM_MOVE_LEFT: return "move-left";
        case LV_SCR_LOAD_ANIM_MOVE_RIGHT: return "move-right";
        case LV_SCR_LOAD_ANIM_MOVE_TOP: return "move-top";
        case LV_SCR_LOAD_ANIM_MOVE_BOTTOM: return "move-bottom";
        case LV_SCR_LOAD_ANIM_FADE_ON: return "fade-on";

        default:
            return "unknow";
    }
}
JSValue lv_scr_load_anim_const_to_jsstr(JSContext *ctx, lv_scr_load_anim_t code) {
    return JS_NewString(ctx, lv_scr_load_anim_const_to_str(code));
}

bool lv_state_str_to_const(const char * name, lv_state_t* out) {
    if(strcmp(name,"default")==0) {
        (*out) = LV_STATE_DEFAULT ;
    }
    else if(strcmp(name,"checked")==0) {
        (*out) = LV_STATE_CHECKED ;
    }
    else if(strcmp(name,"focused")==0) {
        (*out) = LV_STATE_FOCUSED ;
    }
    else if(strcmp(name,"focus-key")==0) {
        (*out) = LV_STATE_FOCUS_KEY ;
    }
    else if(strcmp(name,"edited")==0) {
        (*out) = LV_STATE_EDITED ;
    }
    else if(strcmp(name,"hovered")==0) {
        (*out) = LV_STATE_HOVERED ;
    }
    else if(strcmp(name,"pressed")==0) {
        (*out) = LV_STATE_PRESSED ;
    }
    else if(strcmp(name,"scrolled")==0) {
        (*out) = LV_STATE_SCROLLED ;
    }
    else if(strcmp(name,"disabled")==0) {
        (*out) = LV_STATE_DISABLED ;
    }
    else if(strcmp(name,"user-1")==0) {
        (*out) = LV_STATE_USER_1 ;
    }
    else if(strcmp(name,"user-2")==0) {
        (*out) = LV_STATE_USER_2 ;
    }
    else if(strcmp(name,"user-3")==0) {
        (*out) = LV_STATE_USER_3 ;
    }
    else if(strcmp(name,"user-4")==0) {
        (*out) = LV_STATE_USER_4 ;
    }
    else if(strcmp(name,"any")==0) {
        (*out) = LV_STATE_ANY ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_state_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_state_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_state_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_state_const_to_str(lv_state_t code) {

    switch(code) {
        case LV_STATE_DEFAULT: return "default";
        case LV_STATE_CHECKED: return "checked";
        case LV_STATE_FOCUSED: return "focused";
        case LV_STATE_FOCUS_KEY: return "focus-key";
        case LV_STATE_EDITED: return "edited";
        case LV_STATE_HOVERED: return "hovered";
        case LV_STATE_PRESSED: return "pressed";
        case LV_STATE_SCROLLED: return "scrolled";
        case LV_STATE_DISABLED: return "disabled";
        case LV_STATE_USER_1: return "user-1";
        case LV_STATE_USER_2: return "user-2";
        case LV_STATE_USER_3: return "user-3";
        case LV_STATE_USER_4: return "user-4";
        case LV_STATE_ANY: return "any";

        default:
            return "unknow";
    }
}
JSValue lv_state_const_to_jsstr(JSContext *ctx, lv_state_t code) {
    return JS_NewString(ctx, lv_state_const_to_str(code));
}

bool lv_obj_flag_str_to_const(const char * name, lv_obj_flag_t* out) {
    if(strcmp(name,"hidden")==0) {
        (*out) = LV_OBJ_FLAG_HIDDEN ;
    }
    else if(strcmp(name,"clickable")==0) {
        (*out) = LV_OBJ_FLAG_CLICKABLE ;
    }
    else if(strcmp(name,"click-focusable")==0) {
        (*out) = LV_OBJ_FLAG_CLICK_FOCUSABLE ;
    }
    else if(strcmp(name,"checkable")==0) {
        (*out) = LV_OBJ_FLAG_CHECKABLE ;
    }
    else if(strcmp(name,"scrollable")==0) {
        (*out) = LV_OBJ_FLAG_SCROLLABLE ;
    }
    else if(strcmp(name,"scroll-elastic")==0) {
        (*out) = LV_OBJ_FLAG_SCROLL_ELASTIC ;
    }
    else if(strcmp(name,"scroll-momentum")==0) {
        (*out) = LV_OBJ_FLAG_SCROLL_MOMENTUM ;
    }
    else if(strcmp(name,"scroll-one")==0) {
        (*out) = LV_OBJ_FLAG_SCROLL_ONE ;
    }
    else if(strcmp(name,"scroll-chain")==0) {
        (*out) = LV_OBJ_FLAG_SCROLL_CHAIN ;
    }
    else if(strcmp(name,"scroll-on-focus")==0) {
        (*out) = LV_OBJ_FLAG_SCROLL_ON_FOCUS ;
    }
    else if(strcmp(name,"snappable")==0) {
        (*out) = LV_OBJ_FLAG_SNAPPABLE ;
    }
    else if(strcmp(name,"press-lock")==0) {
        (*out) = LV_OBJ_FLAG_PRESS_LOCK ;
    }
    else if(strcmp(name,"event-bubble")==0) {
        (*out) = LV_OBJ_FLAG_EVENT_BUBBLE ;
    }
    else if(strcmp(name,"gesture-bubble")==0) {
        (*out) = LV_OBJ_FLAG_GESTURE_BUBBLE ;
    }
    else if(strcmp(name,"adv-hittest")==0) {
        (*out) = LV_OBJ_FLAG_ADV_HITTEST ;
    }
    else if(strcmp(name,"ignore-layout")==0) {
        (*out) = LV_OBJ_FLAG_IGNORE_LAYOUT ;
    }
    else if(strcmp(name,"floating")==0) {
        (*out) = LV_OBJ_FLAG_FLOATING ;
    }
    else if(strcmp(name,"layout-1")==0) {
        (*out) = LV_OBJ_FLAG_LAYOUT_1 ;
    }
    else if(strcmp(name,"layout-2")==0) {
        (*out) = LV_OBJ_FLAG_LAYOUT_2 ;
    }
    else if(strcmp(name,"widget-1")==0) {
        (*out) = LV_OBJ_FLAG_WIDGET_1 ;
    }
    else if(strcmp(name,"widget-2")==0) {
        (*out) = LV_OBJ_FLAG_WIDGET_2 ;
    }
    else if(strcmp(name,"user-1")==0) {
        (*out) = LV_OBJ_FLAG_USER_1 ;
    }
    else if(strcmp(name,"user-2")==0) {
        (*out) = LV_OBJ_FLAG_USER_2 ;
    }
    else if(strcmp(name,"user-3")==0) {
        (*out) = LV_OBJ_FLAG_USER_3 ;
    }
    else if(strcmp(name,"user-4")==0) {
        (*out) = LV_OBJ_FLAG_USER_4 ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_obj_flag_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_obj_flag_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_obj_flag_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_obj_flag_const_to_str(lv_obj_flag_t code) {

    switch(code) {
        case LV_OBJ_FLAG_HIDDEN: return "hidden";
        case LV_OBJ_FLAG_CLICKABLE: return "clickable";
        case LV_OBJ_FLAG_CLICK_FOCUSABLE: return "click-focusable";
        case LV_OBJ_FLAG_CHECKABLE: return "checkable";
        case LV_OBJ_FLAG_SCROLLABLE: return "scrollable";
        case LV_OBJ_FLAG_SCROLL_ELASTIC: return "scroll-elastic";
        case LV_OBJ_FLAG_SCROLL_MOMENTUM: return "scroll-momentum";
        case LV_OBJ_FLAG_SCROLL_ONE: return "scroll-one";
        case LV_OBJ_FLAG_SCROLL_CHAIN: return "scroll-chain";
        case LV_OBJ_FLAG_SCROLL_ON_FOCUS: return "scroll-on-focus";
        case LV_OBJ_FLAG_SNAPPABLE: return "snappable";
        case LV_OBJ_FLAG_PRESS_LOCK: return "press-lock";
        case LV_OBJ_FLAG_EVENT_BUBBLE: return "event-bubble";
        case LV_OBJ_FLAG_GESTURE_BUBBLE: return "gesture-bubble";
        case LV_OBJ_FLAG_ADV_HITTEST: return "adv-hittest";
        case LV_OBJ_FLAG_IGNORE_LAYOUT: return "ignore-layout";
        case LV_OBJ_FLAG_FLOATING: return "floating";
        case LV_OBJ_FLAG_LAYOUT_1: return "layout-1";
        case LV_OBJ_FLAG_LAYOUT_2: return "layout-2";
        case LV_OBJ_FLAG_WIDGET_1: return "widget-1";
        case LV_OBJ_FLAG_WIDGET_2: return "widget-2";
        case LV_OBJ_FLAG_USER_1: return "user-1";
        case LV_OBJ_FLAG_USER_2: return "user-2";
        case LV_OBJ_FLAG_USER_3: return "user-3";
        case LV_OBJ_FLAG_USER_4: return "user-4";

        default:
            return "unknow";
    }
}
JSValue lv_obj_flag_const_to_jsstr(JSContext *ctx, lv_obj_flag_t code) {
    return JS_NewString(ctx, lv_obj_flag_const_to_str(code));
}

bool lv_text_align_str_to_const(const char * name, lv_text_align_t* out) {
    if(strcmp(name,"auto")==0) {
        (*out) = LV_TEXT_ALIGN_AUTO ;
    }
    else if(strcmp(name,"left")==0) {
        (*out) = LV_TEXT_ALIGN_LEFT ;
    }
    else if(strcmp(name,"center")==0) {
        (*out) = LV_TEXT_ALIGN_CENTER ;
    }
    else if(strcmp(name,"right")==0) {
        (*out) = LV_TEXT_ALIGN_RIGHT ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_text_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_text_align_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_text_align_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_text_align_const_to_str(lv_text_align_t code) {

    switch(code) {
        case LV_TEXT_ALIGN_AUTO: return "auto";
        case LV_TEXT_ALIGN_LEFT: return "left";
        case LV_TEXT_ALIGN_CENTER: return "center";
        case LV_TEXT_ALIGN_RIGHT: return "right";

        default:
            return "unknow";
    }
}
JSValue lv_text_align_const_to_jsstr(JSContext *ctx, lv_text_align_t code) {
    return JS_NewString(ctx, lv_text_align_const_to_str(code));
}

bool lv_palette_str_to_const(const char * name, lv_palette_t* out) {
    if(strcmp(name,"red")==0) {
        (*out) = LV_PALETTE_RED ;
    }
    else if(strcmp(name,"pink")==0) {
        (*out) = LV_PALETTE_PINK ;
    }
    else if(strcmp(name,"purple")==0) {
        (*out) = LV_PALETTE_PURPLE ;
    }
    else if(strcmp(name,"deep-purple")==0) {
        (*out) = LV_PALETTE_DEEP_PURPLE ;
    }
    else if(strcmp(name,"indigo")==0) {
        (*out) = LV_PALETTE_INDIGO ;
    }
    else if(strcmp(name,"blue")==0) {
        (*out) = LV_PALETTE_BLUE ;
    }
    else if(strcmp(name,"light-blue")==0) {
        (*out) = LV_PALETTE_LIGHT_BLUE ;
    }
    else if(strcmp(name,"cyan")==0) {
        (*out) = LV_PALETTE_CYAN ;
    }
    else if(strcmp(name,"teal")==0) {
        (*out) = LV_PALETTE_TEAL ;
    }
    else if(strcmp(name,"green")==0) {
        (*out) = LV_PALETTE_GREEN ;
    }
    else if(strcmp(name,"light-green")==0) {
        (*out) = LV_PALETTE_LIGHT_GREEN ;
    }
    else if(strcmp(name,"lime")==0) {
        (*out) = LV_PALETTE_LIME ;
    }
    else if(strcmp(name,"yellow")==0) {
        (*out) = LV_PALETTE_YELLOW ;
    }
    else if(strcmp(name,"amber")==0) {
        (*out) = LV_PALETTE_AMBER ;
    }
    else if(strcmp(name,"orange")==0) {
        (*out) = LV_PALETTE_ORANGE ;
    }
    else if(strcmp(name,"deep-orange")==0) {
        (*out) = LV_PALETTE_DEEP_ORANGE ;
    }
    else if(strcmp(name,"brown")==0) {
        (*out) = LV_PALETTE_BROWN ;
    }
    else if(strcmp(name,"blue-grey")==0) {
        (*out) = LV_PALETTE_BLUE_GREY ;
    }
    else if(strcmp(name,"grey")==0) {
        (*out) = LV_PALETTE_GREY ;
    }
    else if(strcmp(name,"-last")==0) {
        (*out) = _LV_PALETTE_LAST ;
    }
    else if(strcmp(name,"none")==0) {
        (*out) = LV_PALETTE_NONE ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_palette_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_palette_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_palette_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_palette_const_to_str(lv_palette_t code) {

    switch(code) {
        case LV_PALETTE_RED: return "red";
        case LV_PALETTE_PINK: return "pink";
        case LV_PALETTE_PURPLE: return "purple";
        case LV_PALETTE_DEEP_PURPLE: return "deep-purple";
        case LV_PALETTE_INDIGO: return "indigo";
        case LV_PALETTE_BLUE: return "blue";
        case LV_PALETTE_LIGHT_BLUE: return "light-blue";
        case LV_PALETTE_CYAN: return "cyan";
        case LV_PALETTE_TEAL: return "teal";
        case LV_PALETTE_GREEN: return "green";
        case LV_PALETTE_LIGHT_GREEN: return "light-green";
        case LV_PALETTE_LIME: return "lime";
        case LV_PALETTE_YELLOW: return "yellow";
        case LV_PALETTE_AMBER: return "amber";
        case LV_PALETTE_ORANGE: return "orange";
        case LV_PALETTE_DEEP_ORANGE: return "deep-orange";
        case LV_PALETTE_BROWN: return "brown";
        case LV_PALETTE_BLUE_GREY: return "blue-grey";
        case LV_PALETTE_GREY: return "grey";
        case _LV_PALETTE_LAST: return "-last";
        case LV_PALETTE_NONE: return "none";

        default:
            return "unknow";
    }
}
JSValue lv_palette_const_to_jsstr(JSContext *ctx, lv_palette_t code) {
    return JS_NewString(ctx, lv_palette_const_to_str(code));
}

bool lv_label_long_mode_str_to_const(const char * name, lv_label_long_mode_t* out) {
    if(strcmp(name,"wrap")==0) {
        (*out) = LV_LABEL_LONG_WRAP ;
    }
    else if(strcmp(name,"dot")==0) {
        (*out) = LV_LABEL_LONG_DOT ;
    }
    else if(strcmp(name,"scroll")==0) {
        (*out) = LV_LABEL_LONG_SCROLL ;
    }
    else if(strcmp(name,"scroll-circular")==0) {
        (*out) = LV_LABEL_LONG_SCROLL_CIRCULAR ;
    }
    else if(strcmp(name,"clip")==0) {
        (*out) = LV_LABEL_LONG_CLIP ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_label_long_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_label_long_mode_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_label_long_mode_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_label_long_mode_const_to_str(lv_label_long_mode_t code) {

    switch(code) {
        case LV_LABEL_LONG_WRAP: return "wrap";
        case LV_LABEL_LONG_DOT: return "dot";
        case LV_LABEL_LONG_SCROLL: return "scroll";
        case LV_LABEL_LONG_SCROLL_CIRCULAR: return "scroll-circular";
        case LV_LABEL_LONG_CLIP: return "clip";

        default:
            return "unknow";
    }
}
JSValue lv_label_long_mode_const_to_jsstr(JSContext *ctx, lv_label_long_mode_t code) {
    return JS_NewString(ctx, lv_label_long_mode_const_to_str(code));
}

bool lv_keyboard_mode_str_to_const(const char * name, lv_keyboard_mode_t* out) {
    if(strcmp(name,"text-lower")==0) {
        (*out) = LV_KEYBOARD_MODE_TEXT_LOWER ;
    }
    else if(strcmp(name,"text-upper")==0) {
        (*out) = LV_KEYBOARD_MODE_TEXT_UPPER ;
    }
    else if(strcmp(name,"special")==0) {
        (*out) = LV_KEYBOARD_MODE_SPECIAL ;
    }
    else if(strcmp(name,"number")==0) {
        (*out) = LV_KEYBOARD_MODE_NUMBER ;
    }
    else if(strcmp(name,"user-1")==0) {
        (*out) = LV_KEYBOARD_MODE_USER_1 ;
    }
    else if(strcmp(name,"user-2")==0) {
        (*out) = LV_KEYBOARD_MODE_USER_2 ;
    }
    else if(strcmp(name,"user-3")==0) {
        (*out) = LV_KEYBOARD_MODE_USER_3 ;
    }
    else if(strcmp(name,"user-4")==0) {
        (*out) = LV_KEYBOARD_MODE_USER_4 ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_keyboard_mode_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_keyboard_mode_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_keyboard_mode_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_keyboard_mode_const_to_str(lv_keyboard_mode_t code) {

    switch(code) {
        case LV_KEYBOARD_MODE_TEXT_LOWER: return "text-lower";
        case LV_KEYBOARD_MODE_TEXT_UPPER: return "text-upper";
        case LV_KEYBOARD_MODE_SPECIAL: return "special";
        case LV_KEYBOARD_MODE_NUMBER: return "number";
        case LV_KEYBOARD_MODE_USER_1: return "user-1";
        case LV_KEYBOARD_MODE_USER_2: return "user-2";
        case LV_KEYBOARD_MODE_USER_3: return "user-3";
        case LV_KEYBOARD_MODE_USER_4: return "user-4";

        default:
            return "unknow";
    }
}
JSValue lv_keyboard_mode_const_to_jsstr(JSContext *ctx, lv_keyboard_mode_t code) {
    return JS_NewString(ctx, lv_keyboard_mode_const_to_str(code));
}

bool lv_img_cf_str_to_const(const char * name, lv_img_cf_t* out) {
    if(strcmp(name,"unknown")==0) {
        (*out) = LV_IMG_CF_UNKNOWN ;
    }
    else if(strcmp(name,"raw")==0) {
        (*out) = LV_IMG_CF_RAW ;
    }
    else if(strcmp(name,"raw-alpha")==0) {
        (*out) = LV_IMG_CF_RAW_ALPHA ;
    }
    else if(strcmp(name,"raw-chroma-keyed")==0) {
        (*out) = LV_IMG_CF_RAW_CHROMA_KEYED ;
    }
    else if(strcmp(name,"true-color")==0) {
        (*out) = LV_IMG_CF_TRUE_COLOR ;
    }
    else if(strcmp(name,"true-color-alpha")==0) {
        (*out) = LV_IMG_CF_TRUE_COLOR_ALPHA ;
    }
    else if(strcmp(name,"true-color-chroma-keyed")==0) {
        (*out) = LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED ;
    }
    else if(strcmp(name,"indexed-1bit")==0) {
        (*out) = LV_IMG_CF_INDEXED_1BIT ;
    }
    else if(strcmp(name,"indexed-2bit")==0) {
        (*out) = LV_IMG_CF_INDEXED_2BIT ;
    }
    else if(strcmp(name,"indexed-4bit")==0) {
        (*out) = LV_IMG_CF_INDEXED_4BIT ;
    }
    else if(strcmp(name,"indexed-8bit")==0) {
        (*out) = LV_IMG_CF_INDEXED_8BIT ;
    }
    else if(strcmp(name,"alpha-1bit")==0) {
        (*out) = LV_IMG_CF_ALPHA_1BIT ;
    }
    else if(strcmp(name,"alpha-2bit")==0) {
        (*out) = LV_IMG_CF_ALPHA_2BIT ;
    }
    else if(strcmp(name,"alpha-4bit")==0) {
        (*out) = LV_IMG_CF_ALPHA_4BIT ;
    }
    else if(strcmp(name,"alpha-8bit")==0) {
        (*out) = LV_IMG_CF_ALPHA_8BIT ;
    }
    else if(strcmp(name,"reserved-15")==0) {
        (*out) = LV_IMG_CF_RESERVED_15 ;
    }
    else if(strcmp(name,"reserved-16")==0) {
        (*out) = LV_IMG_CF_RESERVED_16 ;
    }
    else if(strcmp(name,"reserved-17")==0) {
        (*out) = LV_IMG_CF_RESERVED_17 ;
    }
    else if(strcmp(name,"reserved-18")==0) {
        (*out) = LV_IMG_CF_RESERVED_18 ;
    }
    else if(strcmp(name,"reserved-19")==0) {
        (*out) = LV_IMG_CF_RESERVED_19 ;
    }
    else if(strcmp(name,"reserved-20")==0) {
        (*out) = LV_IMG_CF_RESERVED_20 ;
    }
    else if(strcmp(name,"reserved-21")==0) {
        (*out) = LV_IMG_CF_RESERVED_21 ;
    }
    else if(strcmp(name,"reserved-22")==0) {
        (*out) = LV_IMG_CF_RESERVED_22 ;
    }
    else if(strcmp(name,"reserved-23")==0) {
        (*out) = LV_IMG_CF_RESERVED_23 ;
    }
    else if(strcmp(name,"user-encoded-0")==0) {
        (*out) = LV_IMG_CF_USER_ENCODED_0 ;
    }
    else if(strcmp(name,"user-encoded-1")==0) {
        (*out) = LV_IMG_CF_USER_ENCODED_1 ;
    }
    else if(strcmp(name,"user-encoded-2")==0) {
        (*out) = LV_IMG_CF_USER_ENCODED_2 ;
    }
    else if(strcmp(name,"user-encoded-3")==0) {
        (*out) = LV_IMG_CF_USER_ENCODED_3 ;
    }
    else if(strcmp(name,"user-encoded-4")==0) {
        (*out) = LV_IMG_CF_USER_ENCODED_4 ;
    }
    else if(strcmp(name,"user-encoded-5")==0) {
        (*out) = LV_IMG_CF_USER_ENCODED_5 ;
    }
    else if(strcmp(name,"user-encoded-6")==0) {
        (*out) = LV_IMG_CF_USER_ENCODED_6 ;
    }
    else if(strcmp(name,"user-encoded-7")==0) {
        (*out) = LV_IMG_CF_USER_ENCODED_7 ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_img_cf_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_img_cf_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_img_cf_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_img_cf_const_to_str(lv_img_cf_t code) {

    switch(code) {
        case LV_IMG_CF_UNKNOWN: return "unknown";
        case LV_IMG_CF_RAW: return "raw";
        case LV_IMG_CF_RAW_ALPHA: return "raw-alpha";
        case LV_IMG_CF_RAW_CHROMA_KEYED: return "raw-chroma-keyed";
        case LV_IMG_CF_TRUE_COLOR: return "true-color";
        case LV_IMG_CF_TRUE_COLOR_ALPHA: return "true-color-alpha";
        case LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED: return "true-color-chroma-keyed";
        case LV_IMG_CF_INDEXED_1BIT: return "indexed-1bit";
        case LV_IMG_CF_INDEXED_2BIT: return "indexed-2bit";
        case LV_IMG_CF_INDEXED_4BIT: return "indexed-4bit";
        case LV_IMG_CF_INDEXED_8BIT: return "indexed-8bit";
        case LV_IMG_CF_ALPHA_1BIT: return "alpha-1bit";
        case LV_IMG_CF_ALPHA_2BIT: return "alpha-2bit";
        case LV_IMG_CF_ALPHA_4BIT: return "alpha-4bit";
        case LV_IMG_CF_ALPHA_8BIT: return "alpha-8bit";
        case LV_IMG_CF_RESERVED_15: return "reserved-15";
        case LV_IMG_CF_RESERVED_16: return "reserved-16";
        case LV_IMG_CF_RESERVED_17: return "reserved-17";
        case LV_IMG_CF_RESERVED_18: return "reserved-18";
        case LV_IMG_CF_RESERVED_19: return "reserved-19";
        case LV_IMG_CF_RESERVED_20: return "reserved-20";
        case LV_IMG_CF_RESERVED_21: return "reserved-21";
        case LV_IMG_CF_RESERVED_22: return "reserved-22";
        case LV_IMG_CF_RESERVED_23: return "reserved-23";
        case LV_IMG_CF_USER_ENCODED_0: return "user-encoded-0";
        case LV_IMG_CF_USER_ENCODED_1: return "user-encoded-1";
        case LV_IMG_CF_USER_ENCODED_2: return "user-encoded-2";
        case LV_IMG_CF_USER_ENCODED_3: return "user-encoded-3";
        case LV_IMG_CF_USER_ENCODED_4: return "user-encoded-4";
        case LV_IMG_CF_USER_ENCODED_5: return "user-encoded-5";
        case LV_IMG_CF_USER_ENCODED_6: return "user-encoded-6";
        case LV_IMG_CF_USER_ENCODED_7: return "user-encoded-7";

        default:
            return "unknow";
    }
}
JSValue lv_img_cf_const_to_jsstr(JSContext *ctx, lv_img_cf_t code) {
    return JS_NewString(ctx, lv_img_cf_const_to_str(code));
}

bool lv_scroll_snap_str_to_const(const char * name, lv_scroll_snap_t* out) {
    if(strcmp(name,"none")==0) {
        (*out) = LV_SCROLL_SNAP_NONE ;
    }
    else if(strcmp(name,"start")==0) {
        (*out) = LV_SCROLL_SNAP_START ;
    }
    else if(strcmp(name,"end")==0) {
        (*out) = LV_SCROLL_SNAP_END ;
    }
    else if(strcmp(name,"center")==0) {
        (*out) = LV_SCROLL_SNAP_CENTER ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_scroll_snap_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_scroll_snap_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_scroll_snap_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_scroll_snap_const_to_str(lv_scroll_snap_t code) {

    switch(code) {
        case LV_SCROLL_SNAP_NONE: return "none";
        case LV_SCROLL_SNAP_START: return "start";
        case LV_SCROLL_SNAP_END: return "end";
        case LV_SCROLL_SNAP_CENTER: return "center";

        default:
            return "unknow";
    }
}
JSValue lv_scroll_snap_const_to_jsstr(JSContext *ctx, lv_scroll_snap_t code) {
    return JS_NewString(ctx, lv_scroll_snap_const_to_str(code));
}

bool lv_indev_type_str_to_const(const char * name, lv_indev_type_t* out) {
    if(strcmp(name,"none")==0) {
        (*out) = LV_INDEV_TYPE_NONE ;
    }
    else if(strcmp(name,"pointer")==0) {
        (*out) = LV_INDEV_TYPE_POINTER ;
    }
    else if(strcmp(name,"keypad")==0) {
        (*out) = LV_INDEV_TYPE_KEYPAD ;
    }
    else if(strcmp(name,"button")==0) {
        (*out) = LV_INDEV_TYPE_BUTTON ;
    }
    else if(strcmp(name,"encoder")==0) {
        (*out) = LV_INDEV_TYPE_ENCODER ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_indev_type_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_indev_type_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_indev_type_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_indev_type_const_to_str(lv_indev_type_t code) {

    switch(code) {
        case LV_INDEV_TYPE_NONE: return "none";
        case LV_INDEV_TYPE_POINTER: return "pointer";
        case LV_INDEV_TYPE_KEYPAD: return "keypad";
        case LV_INDEV_TYPE_BUTTON: return "button";
        case LV_INDEV_TYPE_ENCODER: return "encoder";

        default:
            return "unknow";
    }
}
JSValue lv_indev_type_const_to_jsstr(JSContext *ctx, lv_indev_type_t code) {
    return JS_NewString(ctx, lv_indev_type_const_to_str(code));
}

bool lv_group_refocus_policy_str_to_const(const char * name, lv_group_refocus_policy_t* out) {
    if(strcmp(name,"next")==0) {
        (*out) = LV_GROUP_REFOCUS_POLICY_NEXT ;
    }
    else if(strcmp(name,"prev")==0) {
        (*out) = LV_GROUP_REFOCUS_POLICY_PREV ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_group_refocus_policy_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_group_refocus_policy_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_group_refocus_policy_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_group_refocus_policy_const_to_str(lv_group_refocus_policy_t code) {

    switch(code) {
        case LV_GROUP_REFOCUS_POLICY_NEXT: return "next";
        case LV_GROUP_REFOCUS_POLICY_PREV: return "prev";

        default:
            return "unknow";
    }
}
JSValue lv_group_refocus_policy_const_to_jsstr(JSContext *ctx, lv_group_refocus_policy_t code) {
    return JS_NewString(ctx, lv_group_refocus_policy_const_to_str(code));
}

bool lv_part_str_to_const(const char * name, lv_part_t* out) {
    if(strcmp(name,"main")==0) {
        (*out) = LV_PART_MAIN ;
    }
    else if(strcmp(name,"scrollbar")==0) {
        (*out) = LV_PART_SCROLLBAR ;
    }
    else if(strcmp(name,"indicator")==0) {
        (*out) = LV_PART_INDICATOR ;
    }
    else if(strcmp(name,"knob")==0) {
        (*out) = LV_PART_KNOB ;
    }
    else if(strcmp(name,"selected")==0) {
        (*out) = LV_PART_SELECTED ;
    }
    else if(strcmp(name,"items")==0) {
        (*out) = LV_PART_ITEMS ;
    }
    else if(strcmp(name,"ticks")==0) {
        (*out) = LV_PART_TICKS ;
    }
    else if(strcmp(name,"cursor")==0) {
        (*out) = LV_PART_CURSOR ;
    }
    else if(strcmp(name,"custom-first")==0) {
        (*out) = LV_PART_CUSTOM_FIRST ;
    }
    else if(strcmp(name,"any")==0) {
        (*out) = LV_PART_ANY ;
    }

    else {
        return false ;
    }
    return true ;
}
bool lv_part_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_part_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = lv_part_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * lv_part_const_to_str(lv_part_t code) {

    switch(code) {
        case LV_PART_MAIN: return "main";
        case LV_PART_SCROLLBAR: return "scrollbar";
        case LV_PART_INDICATOR: return "indicator";
        case LV_PART_KNOB: return "knob";
        case LV_PART_SELECTED: return "selected";
        case LV_PART_ITEMS: return "items";
        case LV_PART_TICKS: return "ticks";
        case LV_PART_CURSOR: return "cursor";
        case LV_PART_CUSTOM_FIRST: return "custom-first";
        case LV_PART_ANY: return "any";

        default:
            return "unknow";
    }
}
JSValue lv_part_const_to_jsstr(JSContext *ctx, lv_part_t code) {
    return JS_NewString(ctx, lv_part_const_to_str(code));
}
// AUTO GENERATE CODE END [CONST MAPPING] --------

// AUTO GENERATE CODE START [DEFINE CLASS] --------
 // beapi.lvgl.Obj --
static JSClassID js_lv_obj_class_id ;
static JSValue js_lv_obj_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_obj_t * cobj = lv_obj_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_obj_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_obj_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_obj_finalizer()\n") ;
    lv_obj_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_obj_class = {
    "Obj",
    .finalizer = js_lv_obj_finalizer,
};

 // beapi.lvgl.Label --
static JSClassID js_lv_label_class_id ;
static JSValue js_lv_label_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_label_t * cobj = lv_label_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_label_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_label_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_label_finalizer()\n") ;
    lv_label_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_label_class = {
    "Label",
    .finalizer = js_lv_label_finalizer,
};

 // beapi.lvgl.Arc --
static JSClassID js_lv_arc_class_id ;
static JSValue js_lv_arc_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_arc_t * cobj = lv_arc_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_arc_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_arc_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_arc_finalizer()\n") ;
    lv_arc_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_arc_class = {
    "Arc",
    .finalizer = js_lv_arc_finalizer,
};

 // beapi.lvgl.Bar --
static JSClassID js_lv_bar_class_id ;
static JSValue js_lv_bar_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_bar_t * cobj = lv_bar_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_bar_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_bar_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_bar_finalizer()\n") ;
    lv_bar_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_bar_class = {
    "Bar",
    .finalizer = js_lv_bar_finalizer,
};

 // beapi.lvgl.Btn --
static JSClassID js_lv_btn_class_id ;
static JSValue js_lv_btn_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_btn_t * cobj = lv_btn_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_btn_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_btn_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_btn_finalizer()\n") ;
    lv_btn_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_btn_class = {
    "Btn",
    .finalizer = js_lv_btn_finalizer,
};

 // beapi.lvgl.BtnMatrix --
static JSClassID js_lv_btnmatrix_class_id ;
static JSValue js_lv_btnmatrix_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_btnmatrix_t * cobj = lv_btnmatrix_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_btnmatrix_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_btnmatrix_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_btnmatrix_finalizer()\n") ;
    lv_btnmatrix_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_btnmatrix_class = {
    "BtnMatrix",
    .finalizer = js_lv_btnmatrix_finalizer,
};

 // beapi.lvgl.Canvas --
static JSClassID js_lv_canvas_class_id ;
static JSValue js_lv_canvas_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_canvas_t * cobj = lv_canvas_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_canvas_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_canvas_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_canvas_finalizer()\n") ;
    lv_canvas_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_canvas_class = {
    "Canvas",
    .finalizer = js_lv_canvas_finalizer,
};

 // beapi.lvgl.Checkbox --
static JSClassID js_lv_checkbox_class_id ;
static JSValue js_lv_checkbox_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_checkbox_t * cobj = lv_checkbox_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_checkbox_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_checkbox_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_checkbox_finalizer()\n") ;
    lv_checkbox_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_checkbox_class = {
    "Checkbox",
    .finalizer = js_lv_checkbox_finalizer,
};

 // beapi.lvgl.Dropdown --
static JSClassID js_lv_dropdown_class_id ;
static JSValue js_lv_dropdown_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_dropdown_t * cobj = lv_dropdown_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_dropdown_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_dropdown_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_dropdown_finalizer()\n") ;
    lv_dropdown_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_dropdown_class = {
    "Dropdown",
    .finalizer = js_lv_dropdown_finalizer,
};

 // beapi.lvgl.Img --
static JSClassID js_lv_img_class_id ;
static JSValue js_lv_img_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_img_t * cobj = lv_img_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_img_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_img_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_img_finalizer()\n") ;
    lv_img_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_img_class = {
    "Img",
    .finalizer = js_lv_img_finalizer,
};

 // beapi.lvgl.Line --
static JSClassID js_lv_line_class_id ;
static JSValue js_lv_line_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_line_t * cobj = lv_line_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_line_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_line_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_line_finalizer()\n") ;
    lv_line_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_line_class = {
    "Line",
    .finalizer = js_lv_line_finalizer,
};

 // beapi.lvgl.Roller --
static JSClassID js_lv_roller_class_id ;
static JSValue js_lv_roller_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_roller_t * cobj = lv_roller_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_roller_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_roller_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_roller_finalizer()\n") ;
    lv_roller_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_roller_class = {
    "Roller",
    .finalizer = js_lv_roller_finalizer,
};

 // beapi.lvgl.Slider --
static JSClassID js_lv_slider_class_id ;
static JSValue js_lv_slider_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_slider_t * cobj = lv_slider_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_slider_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_slider_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_slider_finalizer()\n") ;
    lv_slider_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_slider_class = {
    "Slider",
    .finalizer = js_lv_slider_finalizer,
};

 // beapi.lvgl.Switch --
static JSClassID js_lv_switch_class_id ;
static JSValue js_lv_switch_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_switch_t * cobj = lv_switch_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_switch_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_switch_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_switch_finalizer()\n") ;
    lv_switch_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_switch_class = {
    "Switch",
    .finalizer = js_lv_switch_finalizer,
};

 // beapi.lvgl.Table --
static JSClassID js_lv_table_class_id ;
static JSValue js_lv_table_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_table_t * cobj = lv_table_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_table_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_table_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_table_finalizer()\n") ;
    lv_table_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_table_class = {
    "Table",
    .finalizer = js_lv_table_finalizer,
};

 // beapi.lvgl.TextArea --
static JSClassID js_lv_textarea_class_id ;
static JSValue js_lv_textarea_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_textarea_t * cobj = lv_textarea_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_textarea_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_textarea_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_textarea_finalizer()\n") ;
    lv_textarea_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_textarea_class = {
    "TextArea",
    .finalizer = js_lv_textarea_finalizer,
};

 // beapi.lvgl.MsgBox --
static JSClassID js_lv_msgbox_class_id ;

static void js_lv_msgbox_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_msgbox_finalizer()\n") ;
    lv_msgbox_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_msgbox_class = {
    "MsgBox",
    .finalizer = js_lv_msgbox_finalizer,
};

 // beapi.lvgl.Keyboard --
static JSClassID js_lv_keyboard_class_id ;
static JSValue js_lv_keyboard_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_keyboard_t * cobj = lv_keyboard_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_keyboard_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_keyboard_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_keyboard_finalizer()\n") ;
    lv_keyboard_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_keyboard_class = {
    "Keyboard",
    .finalizer = js_lv_keyboard_finalizer,
};

 // beapi.lvgl.TileView --
static JSClassID js_lv_tileview_class_id ;
static JSValue js_lv_tileview_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_tileview_t * cobj = lv_tileview_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_tileview_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_tileview_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_tileview_finalizer()\n") ;
    lv_tileview_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_tileview_class = {
    "TileView",
    .finalizer = js_lv_tileview_finalizer,
};

 // beapi.lvgl.TabView --
static JSClassID js_lv_tabview_class_id ;

static void js_lv_tabview_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_tabview_finalizer()\n") ;
    lv_tabview_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_tabview_class = {
    "TabView",
    .finalizer = js_lv_tabview_finalizer,
};

 // beapi.lvgl.Spinner --
static JSClassID js_lv_spinner_class_id ;

static void js_lv_spinner_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_spinner_finalizer()\n") ;
    lv_arc_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_spinner_class = {
    "Spinner",
    .finalizer = js_lv_spinner_finalizer,
};

 // beapi.lvgl.List --
static JSClassID js_lv_list_class_id ;
static JSValue js_lv_list_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    lv_obj_t * cobj = lv_list_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_lv_list_class_id) ;
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            JS_FreeValue(ctx, jsobj) ;
            THROW_EXCEPTION("arg json must be a object")
        }
        JSValue fromJson = JS_GetPropertyStr(ctx, jsobj, "fromJson") ;
        JSValue ret = JS_Call(ctx,fromJson,jsobj,1,&argv[1]) ;
        if(JS_IsException(ret)) {
            return JS_EXCEPTION ; 
        }
        JS_FreeValue(ctx,ret) ;
        JS_FreeValue(ctx,fromJson) ;
    }
    // JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}

static void js_lv_list_finalizer(JSRuntime *rt, JSValue val){
    // printf("js_lv_list_finalizer()\n") ;
    lv_obj_t * thisobj = JS_GetOpaqueInternal(val) ;
    if( thisobj && lv_obj_get_user_data(thisobj) == JS_VALUE_GET_PTR(val) ){
        lv_obj_set_user_data(thisobj, NULL) ;
    }
    // lv_obj_del(thisobj) ;
}

static JSClassDef js_lv_list_class = {
    "List",
    .finalizer = js_lv_list_finalizer,
};

 // beapi.lvgl.Group --
static JSClassID js_lv_group_class_id ;

static JSClassDef js_lv_group_class = {
    "Group",
    .finalizer = js_lv_group_finalizer,
};

// AUTO GENERATE CODE END [DEFINE CLASS] --------




// AUTO GENERATE CODE START [METHOD LIST] --------
static JSValue js_lv_obj_add_flag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.addFlag() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.addFlag() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_flag_t f ;
    if(!lv_obj_flag_jsstr_to_const(ctx, argv[0], &f)) {
        return JS_EXCEPTION ;
    }
    lv_obj_add_flag(thisobj, f) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_clear_flag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.clearFlag() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.clearFlag() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_flag_t f ;
    if(!lv_obj_flag_jsstr_to_const(ctx, argv[0], &f)) {
        return JS_EXCEPTION ;
    }
    lv_obj_clear_flag(thisobj, f) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_add_state(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.addState() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.addState() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_state_t state ;
    if(!lv_state_jsstr_to_const(ctx, argv[0], &state)) {
        return JS_EXCEPTION ;
    }
    lv_obj_add_state(thisobj, state) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_clear_state(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.clearState() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.clearState() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_state_t state ;
    if(!lv_state_jsstr_to_const(ctx, argv[0], &state)) {
        return JS_EXCEPTION ;
    }
    lv_obj_clear_state(thisobj, state) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_has_flag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.hasFlag() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.hasFlag() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_flag_t f ;
    if(!lv_obj_flag_jsstr_to_const(ctx, argv[0], &f)) {
        return JS_EXCEPTION ;
    }
    JSValue retval = JS_NewBool(ctx,lv_obj_has_flag(thisobj, f)) ;
    return retval ;
}

static JSValue js_lv_obj_has_flag_any(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.hasFlagAny() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.hasFlagAny() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_flag_t f ;
    if(!lv_obj_flag_jsstr_to_const(ctx, argv[0], &f)) {
        return JS_EXCEPTION ;
    }
    JSValue retval = JS_NewBool(ctx,lv_obj_has_flag_any(thisobj, f)) ;
    return retval ;
}

static JSValue js_lv_obj_get_state(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.state() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_obj_get_state(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_has_state(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.hasState() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.hasState() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_state_t state ;
    if(!lv_state_jsstr_to_const(ctx, argv[0], &state)) {
        return JS_EXCEPTION ;
    }
    JSValue retval = JS_NewBool(ctx,lv_obj_has_state(thisobj, state)) ;
    return retval ;
}

static JSValue js_lv_obj_get_group(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.group() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = js_lv_group_wrapper(ctx,lv_obj_get_group(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_is_group_def(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.isGroupDef() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_obj_is_group_def(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_allocate_spec_attr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.allocateSpecAttr() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_allocate_spec_attr(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_is_valid(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.isValid() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_obj_is_valid(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_dpx(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.dpx() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.dpx() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t n ;
    if(JS_ToInt32(ctx, (int32_t *) &n, argv[0])!=0){
        THROW_EXCEPTION("arg n of method Obj.dpx() must be a number")
    }
    JSValue retval = JS_NewInt32(ctx,lv_obj_dpx(thisobj, n)) ;
    return retval ;
}

static JSValue js_lv_obj_is_editable(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.isEditable() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_obj_is_editable(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_set_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Obj.setPos() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setPos() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method Obj.setPos() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method Obj.setPos() must be a number")
    }
    lv_obj_set_pos(thisobj, x, y) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.setX() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setX() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method Obj.setX() must be a number")
    }
    lv_obj_set_x(thisobj, x) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.setY() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setY() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[0])!=0){
        THROW_EXCEPTION("arg y of method Obj.setY() must be a number")
    }
    lv_obj_set_y(thisobj, y) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_size(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Obj.setSize() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setSize() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t w ;
    if(JS_ToInt32(ctx, (int32_t *) &w, argv[0])!=0){
        THROW_EXCEPTION("arg w of method Obj.setSize() must be a number")
    }
    int16_t h ;
    if(JS_ToInt32(ctx, (int32_t *) &h, argv[1])!=0){
        THROW_EXCEPTION("arg h of method Obj.setSize() must be a number")
    }
    lv_obj_set_size(thisobj, w, h) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_refr_size(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.refrSize() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_obj_refr_size(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_set_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.setWidth() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setWidth() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t w ;
    if(JS_ToInt32(ctx, (int32_t *) &w, argv[0])!=0){
        THROW_EXCEPTION("arg w of method Obj.setWidth() must be a number")
    }
    lv_obj_set_width(thisobj, w) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.setHeight() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setHeight() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t h ;
    if(JS_ToInt32(ctx, (int32_t *) &h, argv[0])!=0){
        THROW_EXCEPTION("arg h of method Obj.setHeight() must be a number")
    }
    lv_obj_set_height(thisobj, h) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_content_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.setContentWidth() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setContentWidth() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t w ;
    if(JS_ToInt32(ctx, (int32_t *) &w, argv[0])!=0){
        THROW_EXCEPTION("arg w of method Obj.setContentWidth() must be a number")
    }
    lv_obj_set_content_width(thisobj, w) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_content_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.setContentHeight() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setContentHeight() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t h ;
    if(JS_ToInt32(ctx, (int32_t *) &h, argv[0])!=0){
        THROW_EXCEPTION("arg h of method Obj.setContentHeight() must be a number")
    }
    lv_obj_set_content_height(thisobj, h) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_layout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.setLayout() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setLayout() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t layout ;
    if(JS_ToUint32(ctx, (uint32_t *) &layout, argv[0])!=0){
        THROW_EXCEPTION("arg layout of method Obj.setLayout() must be a number")
    }
    lv_obj_set_layout(thisobj, layout) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_is_layout_positioned(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.isLayoutPositioned() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_obj_is_layout_positioned(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_mark_layout_as_dirty(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.markLayoutAsDirty() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_mark_layout_as_dirty(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_update_layout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.updateLayout() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_update_layout(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_align(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.setAlign() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setAlign() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_align_t align ;
    if(!lv_align_jsstr_to_const(ctx, argv[0], &align)) {
        return JS_EXCEPTION ;
    }
    lv_obj_set_align(thisobj, align) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_align(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("Obj.align() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.align() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_align_t align ;
    if(!lv_align_jsstr_to_const(ctx, argv[0], &align)) {
        return JS_EXCEPTION ;
    }
    int16_t x_ofs ;
    if(JS_ToInt32(ctx, (int32_t *) &x_ofs, argv[1])!=0){
        THROW_EXCEPTION("arg x_ofs of method Obj.align() must be a number")
    }
    int16_t y_ofs ;
    if(JS_ToInt32(ctx, (int32_t *) &y_ofs, argv[2])!=0){
        THROW_EXCEPTION("arg y_ofs of method Obj.align() must be a number")
    }
    lv_obj_align(thisobj, align, x_ofs, y_ofs) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_align_to(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<4) {
        THROW_EXCEPTION("Obj.alignTo() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.alignTo() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_t * base = (lv_obj_t *)JS_GetOpaqueInternal(argv[0]) ;
    if( !base ){
        THROW_EXCEPTION("arg base of method Obj.alignTo() must be a beapi.lvgl.Obj")
    }
    lv_align_t align ;
    if(!lv_align_jsstr_to_const(ctx, argv[1], &align)) {
        return JS_EXCEPTION ;
    }
    int16_t x_ofs ;
    if(JS_ToInt32(ctx, (int32_t *) &x_ofs, argv[2])!=0){
        THROW_EXCEPTION("arg x_ofs of method Obj.alignTo() must be a number")
    }
    int16_t y_ofs ;
    if(JS_ToInt32(ctx, (int32_t *) &y_ofs, argv[3])!=0){
        THROW_EXCEPTION("arg y_ofs of method Obj.alignTo() must be a number")
    }
    lv_obj_align_to(thisobj, base, align, x_ofs, y_ofs) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_center(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.center() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_center(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_get_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.x() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_x(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.y() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_y(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_x_aligned(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.xAligned() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_x_aligned(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_y_aligned(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.yAligned() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_y_aligned(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.width() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_width(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.height() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_height(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_content_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.contentWidth() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_content_width(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_content_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.contentHeight() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_content_height(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_content_coords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.contentCoords() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.contentCoords() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg area of method Obj.contentCoords() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t area ;
    JSValue jsarea_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jsarea_x1)){
        THROW_EXCEPTION("arg area of method Obj.contentCoords() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x1, jsarea_x1)!=0) {
        THROW_EXCEPTION("property x1 of arg area is not a number")
    }
    JSValue jsarea_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jsarea_y1)){
        THROW_EXCEPTION("arg area of method Obj.contentCoords() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y1, jsarea_y1)!=0) {
        THROW_EXCEPTION("property y1 of arg area is not a number")
    }
    JSValue jsarea_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jsarea_x2)){
        THROW_EXCEPTION("arg area of method Obj.contentCoords() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x2, jsarea_x2)!=0) {
        THROW_EXCEPTION("property x2 of arg area is not a number")
    }
    JSValue jsarea_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jsarea_y2)){
        THROW_EXCEPTION("arg area of method Obj.contentCoords() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y2, jsarea_y2)!=0) {
        THROW_EXCEPTION("property y2 of arg area is not a number")
    }
    lv_obj_get_content_coords(thisobj, &area) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_get_self_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.selfWidth() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_self_width(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_self_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.selfHeight() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_self_height(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_refresh_self_size(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.refreshSelfSize() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_obj_refresh_self_size(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_refr_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.refrPos() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_refr_pos(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_move_to(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Obj.moveTo() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.moveTo() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method Obj.moveTo() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method Obj.moveTo() must be a number")
    }
    lv_obj_move_to(thisobj, x, y) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_move_children_by(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("Obj.moveChildrenBy() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.moveChildrenBy() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x_diff ;
    if(JS_ToInt32(ctx, (int32_t *) &x_diff, argv[0])!=0){
        THROW_EXCEPTION("arg x_diff of method Obj.moveChildrenBy() must be a number")
    }
    int16_t y_diff ;
    if(JS_ToInt32(ctx, (int32_t *) &y_diff, argv[1])!=0){
        THROW_EXCEPTION("arg y_diff of method Obj.moveChildrenBy() must be a number")
    }
    bool ignore_floating = JS_ToBool(ctx, argv[2]) ;
    lv_obj_move_children_by(thisobj, x_diff, y_diff, ignore_floating) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_invalidate_area(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.invalidateArea() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.invalidateArea() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg area of method Obj.invalidateArea() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t area ;
    JSValue jsarea_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jsarea_x1)){
        THROW_EXCEPTION("arg area of method Obj.invalidateArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x1, jsarea_x1)!=0) {
        THROW_EXCEPTION("property x1 of arg area is not a number")
    }
    JSValue jsarea_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jsarea_y1)){
        THROW_EXCEPTION("arg area of method Obj.invalidateArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y1, jsarea_y1)!=0) {
        THROW_EXCEPTION("property y1 of arg area is not a number")
    }
    JSValue jsarea_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jsarea_x2)){
        THROW_EXCEPTION("arg area of method Obj.invalidateArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x2, jsarea_x2)!=0) {
        THROW_EXCEPTION("property x2 of arg area is not a number")
    }
    JSValue jsarea_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jsarea_y2)){
        THROW_EXCEPTION("arg area of method Obj.invalidateArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y2, jsarea_y2)!=0) {
        THROW_EXCEPTION("property y2 of arg area is not a number")
    }
    lv_obj_invalidate_area(thisobj, &area) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_invalidate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.invalidate() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_invalidate(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_area_is_visible(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.areaIsVisible() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.areaIsVisible() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg area of method Obj.areaIsVisible() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t area ;
    JSValue jsarea_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jsarea_x1)){
        THROW_EXCEPTION("arg area of method Obj.areaIsVisible() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x1, jsarea_x1)!=0) {
        THROW_EXCEPTION("property x1 of arg area is not a number")
    }
    JSValue jsarea_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jsarea_y1)){
        THROW_EXCEPTION("arg area of method Obj.areaIsVisible() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y1, jsarea_y1)!=0) {
        THROW_EXCEPTION("property y1 of arg area is not a number")
    }
    JSValue jsarea_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jsarea_x2)){
        THROW_EXCEPTION("arg area of method Obj.areaIsVisible() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x2, jsarea_x2)!=0) {
        THROW_EXCEPTION("property x2 of arg area is not a number")
    }
    JSValue jsarea_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jsarea_y2)){
        THROW_EXCEPTION("arg area of method Obj.areaIsVisible() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y2, jsarea_y2)!=0) {
        THROW_EXCEPTION("property y2 of arg area is not a number")
    }
    JSValue retval = JS_NewBool(ctx,lv_obj_area_is_visible(thisobj, &area)) ;
    return retval ;
}

static JSValue js_lv_obj_is_visible(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.isVisible() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_obj_is_visible(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_set_ext_click_area(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.setExtClickArea() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setExtClickArea() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t size ;
    if(JS_ToInt32(ctx, (int32_t *) &size, argv[0])!=0){
        THROW_EXCEPTION("arg size of method Obj.setExtClickArea() must be a number")
    }
    lv_obj_set_ext_click_area(thisobj, size) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_get_click_area(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.clickArea() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.clickArea() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg area of method Obj.clickArea() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t area ;
    JSValue jsarea_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jsarea_x1)){
        THROW_EXCEPTION("arg area of method Obj.clickArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x1, jsarea_x1)!=0) {
        THROW_EXCEPTION("property x1 of arg area is not a number")
    }
    JSValue jsarea_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jsarea_y1)){
        THROW_EXCEPTION("arg area of method Obj.clickArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y1, jsarea_y1)!=0) {
        THROW_EXCEPTION("property y1 of arg area is not a number")
    }
    JSValue jsarea_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jsarea_x2)){
        THROW_EXCEPTION("arg area of method Obj.clickArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x2, jsarea_x2)!=0) {
        THROW_EXCEPTION("property x2 of arg area is not a number")
    }
    JSValue jsarea_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jsarea_y2)){
        THROW_EXCEPTION("arg area of method Obj.clickArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y2, jsarea_y2)!=0) {
        THROW_EXCEPTION("property y2 of arg area is not a number")
    }
    lv_obj_get_click_area(thisobj, &area) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_hit_test(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.hitTest() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.hitTest() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg point of method Obj.hitTest() must be a object{x,y}")
    }
    lv_point_t point ;
    JSValue jspoint_x = JS_GetPropertyStr(ctx, argv[0], "x") ;
    if(!JS_IsNumber(jspoint_x)){
        THROW_EXCEPTION("arg point of method Obj.hitTest() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&point.x, jspoint_x)!=0) {
        THROW_EXCEPTION("property x of arg point is not a number")
    }
    JSValue jspoint_y = JS_GetPropertyStr(ctx, argv[0], "y") ;
    if(!JS_IsNumber(jspoint_y)){
        THROW_EXCEPTION("arg point of method Obj.hitTest() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&point.y, jspoint_y)!=0) {
        THROW_EXCEPTION("property y of arg point is not a number")
    }
    JSValue retval = JS_NewBool(ctx,lv_obj_hit_test(thisobj, &point)) ;
    return retval ;
}

static JSValue js_lv_obj_set_scrollbar_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.setScrollbarMode() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setScrollbarMode() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t mode ;
    if(JS_ToUint32(ctx, (uint32_t *) &mode, argv[0])!=0){
        THROW_EXCEPTION("arg mode of method Obj.setScrollbarMode() must be a number")
    }
    lv_obj_set_scrollbar_mode(thisobj, mode) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_scroll_dir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.setScrollDir() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setScrollDir() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_dir_t dir ;
    if(!lv_dir_jsstr_to_const(ctx, argv[0], &dir)) {
        return JS_EXCEPTION ;
    }
    lv_obj_set_scroll_dir(thisobj, dir) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_scroll_snap_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.setScrollSnapX() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setScrollSnapX() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_scroll_snap_t align ;
    if(!lv_scroll_snap_jsstr_to_const(ctx, argv[0], &align)) {
        return JS_EXCEPTION ;
    }
    lv_obj_set_scroll_snap_x(thisobj, align) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_scroll_snap_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.setScrollSnapY() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setScrollSnapY() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_scroll_snap_t align ;
    if(!lv_scroll_snap_jsstr_to_const(ctx, argv[0], &align)) {
        return JS_EXCEPTION ;
    }
    lv_obj_set_scroll_snap_y(thisobj, align) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_get_scrollbar_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollbarMode() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_obj_get_scrollbar_mode(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_dir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollDir() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = lv_dir_const_to_jsstr(ctx,lv_obj_get_scroll_dir(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_snap_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollSnapX() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_obj_get_scroll_snap_x(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_snap_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollSnapY() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_obj_get_scroll_snap_y(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollX() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_scroll_x(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollY() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_scroll_y(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_top(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollTop() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_scroll_top(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_bottom(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollBottom() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_scroll_bottom(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_left(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollLeft() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_scroll_left(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_right(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollRight() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_scroll_right(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_end(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.scrollEnd() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollEnd() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg end of method Obj.scrollEnd() must be a object{x,y}")
    }
    lv_point_t end ;
    JSValue jsend_x = JS_GetPropertyStr(ctx, argv[0], "x") ;
    if(!JS_IsNumber(jsend_x)){
        THROW_EXCEPTION("arg end of method Obj.scrollEnd() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&end.x, jsend_x)!=0) {
        THROW_EXCEPTION("property x of arg end is not a number")
    }
    JSValue jsend_y = JS_GetPropertyStr(ctx, argv[0], "y") ;
    if(!JS_IsNumber(jsend_y)){
        THROW_EXCEPTION("arg end of method Obj.scrollEnd() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&end.y, jsend_y)!=0) {
        THROW_EXCEPTION("property y of arg end is not a number")
    }
    lv_obj_get_scroll_end(thisobj, &end) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_scroll_by(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("Obj.scrollBy() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollBy() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method Obj.scrollBy() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method Obj.scrollBy() must be a number")
    }
    bool anim_en = JS_ToBool(ctx, argv[2]) ;
    lv_obj_scroll_by(thisobj, x, y, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_scroll_to(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("Obj.scrollTo() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollTo() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method Obj.scrollTo() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method Obj.scrollTo() must be a number")
    }
    bool anim_en = JS_ToBool(ctx, argv[2]) ;
    lv_obj_scroll_to(thisobj, x, y, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_scroll_to_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Obj.scrollToX() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollToX() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method Obj.scrollToX() must be a number")
    }
    bool anim_en = JS_ToBool(ctx, argv[1]) ;
    lv_obj_scroll_to_x(thisobj, x, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_scroll_to_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Obj.scrollToY() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollToY() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[0])!=0){
        THROW_EXCEPTION("arg y of method Obj.scrollToY() must be a number")
    }
    bool anim_en = JS_ToBool(ctx, argv[1]) ;
    lv_obj_scroll_to_y(thisobj, y, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_scroll_to_view(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.scrollToView() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollToView() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool anim_en = JS_ToBool(ctx, argv[0]) ;
    lv_obj_scroll_to_view(thisobj, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_scroll_to_view_recursive(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.scrollToViewRecursive() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollToViewRecursive() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool anim_en = JS_ToBool(ctx, argv[0]) ;
    lv_obj_scroll_to_view_recursive(thisobj, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_is_scrolling(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.isScrolling() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_obj_is_scrolling(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_update_snap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.updateSnap() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.updateSnap() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool anim_en = JS_ToBool(ctx, argv[0]) ;
    lv_obj_update_snap(thisobj, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_get_scrollbar_area(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Obj.scrollbarArea() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollbarArea() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg hor of method Obj.scrollbarArea() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t hor ;
    JSValue jshor_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jshor_x1)){
        THROW_EXCEPTION("arg hor of method Obj.scrollbarArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&hor.x1, jshor_x1)!=0) {
        THROW_EXCEPTION("property x1 of arg hor is not a number")
    }
    JSValue jshor_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jshor_y1)){
        THROW_EXCEPTION("arg hor of method Obj.scrollbarArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&hor.y1, jshor_y1)!=0) {
        THROW_EXCEPTION("property y1 of arg hor is not a number")
    }
    JSValue jshor_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jshor_x2)){
        THROW_EXCEPTION("arg hor of method Obj.scrollbarArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&hor.x2, jshor_x2)!=0) {
        THROW_EXCEPTION("property x2 of arg hor is not a number")
    }
    JSValue jshor_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jshor_y2)){
        THROW_EXCEPTION("arg hor of method Obj.scrollbarArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&hor.y2, jshor_y2)!=0) {
        THROW_EXCEPTION("property y2 of arg hor is not a number")
    }
    if(!JS_IsObject(argv[1])){
        THROW_EXCEPTION("arg ver of method Obj.scrollbarArea() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t ver ;
    JSValue jsver_x1 = JS_GetPropertyStr(ctx, argv[1], "x1") ;
    if(!JS_IsNumber(jsver_x1)){
        THROW_EXCEPTION("arg ver of method Obj.scrollbarArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&ver.x1, jsver_x1)!=0) {
        THROW_EXCEPTION("property x1 of arg ver is not a number")
    }
    JSValue jsver_y1 = JS_GetPropertyStr(ctx, argv[1], "y1") ;
    if(!JS_IsNumber(jsver_y1)){
        THROW_EXCEPTION("arg ver of method Obj.scrollbarArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&ver.y1, jsver_y1)!=0) {
        THROW_EXCEPTION("property y1 of arg ver is not a number")
    }
    JSValue jsver_x2 = JS_GetPropertyStr(ctx, argv[1], "x2") ;
    if(!JS_IsNumber(jsver_x2)){
        THROW_EXCEPTION("arg ver of method Obj.scrollbarArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&ver.x2, jsver_x2)!=0) {
        THROW_EXCEPTION("property x2 of arg ver is not a number")
    }
    JSValue jsver_y2 = JS_GetPropertyStr(ctx, argv[1], "y2") ;
    if(!JS_IsNumber(jsver_y2)){
        THROW_EXCEPTION("arg ver of method Obj.scrollbarArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&ver.y2, jsver_y2)!=0) {
        THROW_EXCEPTION("property y2 of arg ver is not a number")
    }
    lv_obj_get_scrollbar_area(thisobj, &hor, &ver) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_scrollbar_invalidate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.scrollbarInvalidate() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_scrollbar_invalidate(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_readjust_scroll(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.readjustScroll() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.readjustScroll() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool anim_en = JS_ToBool(ctx, argv[0]) ;
    lv_obj_readjust_scroll(thisobj, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_add_style(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.addStyle() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.addStyle() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if( !qjs_instanceof(ctx, argv[0], js_lv_style_class_id) ) {
        THROW_EXCEPTION("arg style of method Obj.addStyle() must be a lvgl.Style")
    }
    lv_style_t * style = JS_GetOpaque(argv[0], js_lv_style_class_id) ;
    if(!style) {
        THROW_EXCEPTION("arg style of method Obj.addStyle() is invalid")
    }
    uint32_t selector = LV_PART_MAIN | LV_STATE_DEFAULT ;
    if(argc>1){
        if(JS_ToUint32(ctx, (uint32_t *) &selector, argv[1])!=0){
            THROW_EXCEPTION("arg selector of method Obj.addStyle() must be a number")
        }
    }
    lv_obj_add_style(thisobj, style, selector) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_remove_style(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.removeStyle() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.removeStyle() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if( !qjs_instanceof(ctx, argv[0], js_lv_style_class_id) ) {
        THROW_EXCEPTION("arg style of method Obj.removeStyle() must be a lvgl.Style")
    }
    lv_style_t * style = JS_GetOpaque(argv[0], js_lv_style_class_id) ;
    if(!style) {
        THROW_EXCEPTION("arg style of method Obj.removeStyle() is invalid")
    }
    uint32_t selector = LV_PART_MAIN | LV_STATE_DEFAULT ;
    if(argc>1){
        if(JS_ToUint32(ctx, (uint32_t *) &selector, argv[1])!=0){
            THROW_EXCEPTION("arg selector of method Obj.removeStyle() must be a number")
        }
    }
    lv_obj_remove_style(thisobj, style, selector) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_remove_style_all(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.removeStyleAll() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_remove_style_all(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_fade_in(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Obj.fadeIn() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.fadeIn() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t time ;
    if(JS_ToUint32(ctx, (uint32_t *) &time, argv[0])!=0){
        THROW_EXCEPTION("arg time of method Obj.fadeIn() must be a number")
    }
    uint32_t delay ;
    if(JS_ToUint32(ctx, (uint32_t *) &delay, argv[1])!=0){
        THROW_EXCEPTION("arg delay of method Obj.fadeIn() must be a number")
    }
    lv_obj_fade_in(thisobj, time, delay) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_fade_out(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Obj.fadeOut() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.fadeOut() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t time ;
    if(JS_ToUint32(ctx, (uint32_t *) &time, argv[0])!=0){
        THROW_EXCEPTION("arg time of method Obj.fadeOut() must be a number")
    }
    uint32_t delay ;
    if(JS_ToUint32(ctx, (uint32_t *) &delay, argv[1])!=0){
        THROW_EXCEPTION("arg delay of method Obj.fadeOut() must be a number")
    }
    lv_obj_fade_out(thisobj, time, delay) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_del(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.del() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_del(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_clean(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.clean() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_clean(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_del_async(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.delAsync() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_del_async(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_parent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.setParent() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setParent() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_t * parent = (lv_obj_t *)JS_GetOpaqueInternal(argv[0]) ;
    if( !parent ){
        THROW_EXCEPTION("arg parent of method Obj.setParent() must be a beapi.lvgl.Obj")
    }
    lv_obj_set_parent(thisobj, parent) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_move_to_index(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.moveToIndex() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.moveToIndex() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int32_t index ;
    if(JS_ToInt32(ctx, (int32_t *) &index, argv[0])!=0){
        THROW_EXCEPTION("arg index of method Obj.moveToIndex() must be a number")
    }
    lv_obj_move_to_index(thisobj, index) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_get_screen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.screen() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_obj_get_screen(thisobj);
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, 0) ;
        JS_DupValue(ctx, retval) ;
    }
    return retval ;
}

static JSValue js_lv_obj_get_disp(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.disp() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = js_lv_disp_wrapper(ctx,lv_obj_get_disp(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_parent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.parent() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_obj_get_parent(thisobj);
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, 0) ;
        JS_DupValue(ctx, retval) ;
    }
    return retval ;
}

static JSValue js_lv_obj_get_child(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.child() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.child() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int32_t id ;
    if(JS_ToInt32(ctx, (int32_t *) &id, argv[0])!=0){
        THROW_EXCEPTION("arg id of method Obj.child() must be a number")
    }
    JSValue retval = JS_NULL ;
    void * lvobj = lv_obj_get_child(thisobj, id);
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, 0) ;
        JS_DupValue(ctx, retval) ;
    }
    return retval ;
}

static JSValue js_lv_obj_get_child_cnt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.childCnt() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_obj_get_child_cnt(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_index(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.index() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_obj_get_index(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_set_flex_flow(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.setFlexFlow() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setFlexFlow() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_flex_flow_t flow ;
    if(!lv_flex_flow_jsstr_to_const(ctx, argv[0], &flow)) {
        return JS_EXCEPTION ;
    }
    lv_obj_set_flex_flow(thisobj, flow) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_flex_align(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("Obj.setFlexAlign() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setFlexAlign() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_flex_align_t main_place ;
    if(!lv_flex_align_jsstr_to_const(ctx, argv[0], &main_place)) {
        return JS_EXCEPTION ;
    }
    lv_flex_align_t cross_place ;
    if(!lv_flex_align_jsstr_to_const(ctx, argv[1], &cross_place)) {
        return JS_EXCEPTION ;
    }
    lv_flex_align_t track_cross_place ;
    if(!lv_flex_align_jsstr_to_const(ctx, argv[2], &track_cross_place)) {
        return JS_EXCEPTION ;
    }
    lv_obj_set_flex_align(thisobj, main_place, cross_place, track_cross_place) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_flex_grow(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Obj.setFlexGrow() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setFlexGrow() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t grow ;
    if(JS_ToUint32(ctx, (uint32_t *) &grow, argv[0])!=0){
        THROW_EXCEPTION("arg grow of method Obj.setFlexGrow() must be a number")
    }
    lv_obj_set_flex_grow(thisobj, grow) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_tile(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Obj.setTile() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setTile() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_t * tile_obj = (lv_obj_t *)JS_GetOpaqueInternal(argv[0]) ;
    if( !tile_obj ){
        THROW_EXCEPTION("arg tile_obj of method Obj.setTile() must be a beapi.lvgl.Obj")
    }
    bool anim_en = JS_ToBool(ctx, argv[1]) ;
    lv_obj_set_tile(thisobj, tile_obj, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_tile_id(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("Obj.setTileId() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.setTileId() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t col_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &col_id, argv[0])!=0){
        THROW_EXCEPTION("arg col_id of method Obj.setTileId() must be a number")
    }
    uint32_t row_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &row_id, argv[1])!=0){
        THROW_EXCEPTION("arg row_id of method Obj.setTileId() must be a number")
    }
    bool anim_en = JS_ToBool(ctx, argv[2]) ;
    lv_obj_set_tile_id(thisobj, col_id, row_id, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_move_foreground(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.moveForeground() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_move_foreground(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_move_background(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.moveBackground() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_move_background(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_get_child_id(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Obj.childId() must be called as a Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_obj_get_child_id(thisobj)) ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_obj_proto_funcs[] = {
    JS_CFUNC_DEF("enableEvent", 0, js_lv_obj_enable_event),
    JS_CFUNC_DEF("disableEvent", 0, js_lv_obj_disable_event),
    JS_CFUNC_DEF("isScreen", 0, js_lv_obj_is_screen),
    JS_CFUNC_DEF("coordX", 0, js_lv_obj_get_coord_x),
    JS_CFUNC_DEF("coordY", 0, js_lv_obj_get_coord_y),
    JS_CFUNC_DEF("coords", 0, js_lv_obj_get_coords),
    JS_CFUNC_DEF("setCoordX", 0, js_lv_obj_set_coord_x),
    JS_CFUNC_DEF("setCoordY", 0, js_lv_obj_set_coord_y),
    JS_CFUNC_DEF("setCoords", 0, js_lv_obj_set_coords),
    JS_CFUNC_DEF("move", 0, js_lv_obj_move),
    JS_CFUNC_DEF("moveX", 0, js_lv_obj_move_x),
    JS_CFUNC_DEF("moveY", 0, js_lv_obj_move_y),
    JS_CFUNC_DEF("allStyleValues", 0, js_lv_obj_get_all_style_values),
    JS_CFUNC_DEF("setStyle", 0, js_lv_obj_set_style),
    JS_CFUNC_DEF("style", 0, js_lv_obj_get_style),
    JS_CFUNC_DEF("refreshStyle", 0, js_lv_obj_refresh_style),
    JS_CFUNC_DEF("localStyle", 0, js_lv_obj_get_local_style),
    JS_CFUNC_DEF("fontHeight", 0, js_lv_obj_get_font_height),
    JS_CFUNC_DEF("as", 0, js_lv_obj_as),
    JS_CFUNC_DEF("ptr", 0, js_lv_obj_ptr),
    JS_CFUNC_DEF("abortScroll", 0, js_lv_obj_abort_scroll),
    JS_CFUNC_DEF("addFlag", 0, js_lv_obj_add_flag),
    JS_CFUNC_DEF("clearFlag", 0, js_lv_obj_clear_flag),
    JS_CFUNC_DEF("addState", 0, js_lv_obj_add_state),
    JS_CFUNC_DEF("clearState", 0, js_lv_obj_clear_state),
    JS_CFUNC_DEF("hasFlag", 0, js_lv_obj_has_flag),
    JS_CFUNC_DEF("hasFlagAny", 0, js_lv_obj_has_flag_any),
    JS_CFUNC_DEF("state", 0, js_lv_obj_get_state),
    JS_CFUNC_DEF("hasState", 0, js_lv_obj_has_state),
    JS_CFUNC_DEF("group", 0, js_lv_obj_get_group),
    JS_CFUNC_DEF("isGroupDef", 0, js_lv_obj_is_group_def),
    JS_CFUNC_DEF("allocateSpecAttr", 0, js_lv_obj_allocate_spec_attr),
    JS_CFUNC_DEF("isValid", 0, js_lv_obj_is_valid),
    JS_CFUNC_DEF("dpx", 0, js_lv_obj_dpx),
    JS_CFUNC_DEF("isEditable", 0, js_lv_obj_is_editable),
    JS_CFUNC_DEF("setPos", 0, js_lv_obj_set_pos),
    JS_CFUNC_DEF("setX", 0, js_lv_obj_set_x),
    JS_CFUNC_DEF("setY", 0, js_lv_obj_set_y),
    JS_CFUNC_DEF("setSize", 0, js_lv_obj_set_size),
    JS_CFUNC_DEF("refrSize", 0, js_lv_obj_refr_size),
    JS_CFUNC_DEF("setWidth", 0, js_lv_obj_set_width),
    JS_CFUNC_DEF("setHeight", 0, js_lv_obj_set_height),
    JS_CFUNC_DEF("setContentWidth", 0, js_lv_obj_set_content_width),
    JS_CFUNC_DEF("setContentHeight", 0, js_lv_obj_set_content_height),
    JS_CFUNC_DEF("setLayout", 0, js_lv_obj_set_layout),
    JS_CFUNC_DEF("isLayoutPositioned", 0, js_lv_obj_is_layout_positioned),
    JS_CFUNC_DEF("markLayoutAsDirty", 0, js_lv_obj_mark_layout_as_dirty),
    JS_CFUNC_DEF("updateLayout", 0, js_lv_obj_update_layout),
    JS_CFUNC_DEF("setAlign", 0, js_lv_obj_set_align),
    JS_CFUNC_DEF("align", 0, js_lv_obj_align),
    JS_CFUNC_DEF("alignTo", 0, js_lv_obj_align_to),
    JS_CFUNC_DEF("center", 0, js_lv_obj_center),
    JS_CFUNC_DEF("x", 0, js_lv_obj_get_x),
    JS_CFUNC_DEF("y", 0, js_lv_obj_get_y),
    JS_CFUNC_DEF("xAligned", 0, js_lv_obj_get_x_aligned),
    JS_CFUNC_DEF("yAligned", 0, js_lv_obj_get_y_aligned),
    JS_CFUNC_DEF("width", 0, js_lv_obj_get_width),
    JS_CFUNC_DEF("height", 0, js_lv_obj_get_height),
    JS_CFUNC_DEF("contentWidth", 0, js_lv_obj_get_content_width),
    JS_CFUNC_DEF("contentHeight", 0, js_lv_obj_get_content_height),
    JS_CFUNC_DEF("contentCoords", 0, js_lv_obj_get_content_coords),
    JS_CFUNC_DEF("selfWidth", 0, js_lv_obj_get_self_width),
    JS_CFUNC_DEF("selfHeight", 0, js_lv_obj_get_self_height),
    JS_CFUNC_DEF("refreshSelfSize", 0, js_lv_obj_refresh_self_size),
    JS_CFUNC_DEF("refrPos", 0, js_lv_obj_refr_pos),
    JS_CFUNC_DEF("moveTo", 0, js_lv_obj_move_to),
    JS_CFUNC_DEF("moveChildrenBy", 0, js_lv_obj_move_children_by),
    JS_CFUNC_DEF("invalidateArea", 0, js_lv_obj_invalidate_area),
    JS_CFUNC_DEF("invalidate", 0, js_lv_obj_invalidate),
    JS_CFUNC_DEF("areaIsVisible", 0, js_lv_obj_area_is_visible),
    JS_CFUNC_DEF("isVisible", 0, js_lv_obj_is_visible),
    JS_CFUNC_DEF("setExtClickArea", 0, js_lv_obj_set_ext_click_area),
    JS_CFUNC_DEF("clickArea", 0, js_lv_obj_get_click_area),
    JS_CFUNC_DEF("hitTest", 0, js_lv_obj_hit_test),
    JS_CFUNC_DEF("setScrollbarMode", 0, js_lv_obj_set_scrollbar_mode),
    JS_CFUNC_DEF("setScrollDir", 0, js_lv_obj_set_scroll_dir),
    JS_CFUNC_DEF("setScrollSnapX", 0, js_lv_obj_set_scroll_snap_x),
    JS_CFUNC_DEF("setScrollSnapY", 0, js_lv_obj_set_scroll_snap_y),
    JS_CFUNC_DEF("scrollbarMode", 0, js_lv_obj_get_scrollbar_mode),
    JS_CFUNC_DEF("scrollDir", 0, js_lv_obj_get_scroll_dir),
    JS_CFUNC_DEF("scrollSnapX", 0, js_lv_obj_get_scroll_snap_x),
    JS_CFUNC_DEF("scrollSnapY", 0, js_lv_obj_get_scroll_snap_y),
    JS_CFUNC_DEF("scrollX", 0, js_lv_obj_get_scroll_x),
    JS_CFUNC_DEF("scrollY", 0, js_lv_obj_get_scroll_y),
    JS_CFUNC_DEF("scrollTop", 0, js_lv_obj_get_scroll_top),
    JS_CFUNC_DEF("scrollBottom", 0, js_lv_obj_get_scroll_bottom),
    JS_CFUNC_DEF("scrollLeft", 0, js_lv_obj_get_scroll_left),
    JS_CFUNC_DEF("scrollRight", 0, js_lv_obj_get_scroll_right),
    JS_CFUNC_DEF("scrollEnd", 0, js_lv_obj_get_scroll_end),
    JS_CFUNC_DEF("scrollBy", 0, js_lv_obj_scroll_by),
    JS_CFUNC_DEF("scrollTo", 0, js_lv_obj_scroll_to),
    JS_CFUNC_DEF("scrollToX", 0, js_lv_obj_scroll_to_x),
    JS_CFUNC_DEF("scrollToY", 0, js_lv_obj_scroll_to_y),
    JS_CFUNC_DEF("scrollToView", 0, js_lv_obj_scroll_to_view),
    JS_CFUNC_DEF("scrollToViewRecursive", 0, js_lv_obj_scroll_to_view_recursive),
    JS_CFUNC_DEF("isScrolling", 0, js_lv_obj_is_scrolling),
    JS_CFUNC_DEF("updateSnap", 0, js_lv_obj_update_snap),
    JS_CFUNC_DEF("scrollbarArea", 0, js_lv_obj_get_scrollbar_area),
    JS_CFUNC_DEF("scrollbarInvalidate", 0, js_lv_obj_scrollbar_invalidate),
    JS_CFUNC_DEF("readjustScroll", 0, js_lv_obj_readjust_scroll),
    JS_CFUNC_DEF("addStyle", 0, js_lv_obj_add_style),
    JS_CFUNC_DEF("removeStyle", 0, js_lv_obj_remove_style),
    JS_CFUNC_DEF("removeStyleAll", 0, js_lv_obj_remove_style_all),
    JS_CFUNC_DEF("fadeIn", 0, js_lv_obj_fade_in),
    JS_CFUNC_DEF("fadeOut", 0, js_lv_obj_fade_out),
    JS_CFUNC_DEF("del", 0, js_lv_obj_del),
    JS_CFUNC_DEF("clean", 0, js_lv_obj_clean),
    JS_CFUNC_DEF("delAsync", 0, js_lv_obj_del_async),
    JS_CFUNC_DEF("setParent", 0, js_lv_obj_set_parent),
    JS_CFUNC_DEF("moveToIndex", 0, js_lv_obj_move_to_index),
    JS_CFUNC_DEF("screen", 0, js_lv_obj_get_screen),
    JS_CFUNC_DEF("disp", 0, js_lv_obj_get_disp),
    JS_CFUNC_DEF("parent", 0, js_lv_obj_get_parent),
    JS_CFUNC_DEF("child", 0, js_lv_obj_get_child),
    JS_CFUNC_DEF("childCnt", 0, js_lv_obj_get_child_cnt),
    JS_CFUNC_DEF("index", 0, js_lv_obj_get_index),
    JS_CFUNC_DEF("setFlexFlow", 0, js_lv_obj_set_flex_flow),
    JS_CFUNC_DEF("setFlexAlign", 0, js_lv_obj_set_flex_align),
    JS_CFUNC_DEF("setFlexGrow", 0, js_lv_obj_set_flex_grow),
    JS_CFUNC_DEF("setTile", 0, js_lv_obj_set_tile),
    JS_CFUNC_DEF("setTileId", 0, js_lv_obj_set_tile_id),
    JS_CFUNC_DEF("moveForeground", 0, js_lv_obj_move_foreground),
    JS_CFUNC_DEF("moveBackground", 0, js_lv_obj_move_background),
    JS_CFUNC_DEF("childId", 0, js_lv_obj_get_child_id),
} ;
#define __def_js_lv_obj_proto_funcs__


static const JSCFunctionListEntry js_lv_obj_static_funcs[] = {
} ;
static JSValue js_lv_label_set_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Label.setText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Label.setText() must be called as a Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * text = (char *)JS_ToCString(ctx, argv[0]) ;
    lv_label_set_text(thisobj, text) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, text) ;
    return retval ;
}

static JSValue js_lv_label_set_text_static(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Label.setTextStatic() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Label.setTextStatic() must be called as a Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * text = (char *)JS_ToCString(ctx, argv[0]) ;
    lv_label_set_text_static(thisobj, text) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, text) ;
    return retval ;
}

static JSValue js_lv_label_set_long_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Label.setLongMode() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Label.setLongMode() must be called as a Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_label_long_mode_t long_mode ;
    if(!lv_label_long_mode_jsstr_to_const(ctx, argv[0], &long_mode)) {
        return JS_EXCEPTION ;
    }
    lv_label_set_long_mode(thisobj, long_mode) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_label_set_recolor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Label.setRecolor() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Label.setRecolor() must be called as a Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool en = JS_ToBool(ctx, argv[0]) ;
    lv_label_set_recolor(thisobj, en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_label_set_text_sel_start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Label.setTextSelStart() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Label.setTextSelStart() must be called as a Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t index ;
    if(JS_ToUint32(ctx, (uint32_t *) &index, argv[0])!=0){
        THROW_EXCEPTION("arg index of method Label.setTextSelStart() must be a number")
    }
    lv_label_set_text_sel_start(thisobj, index) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_label_set_text_sel_end(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Label.setTextSelEnd() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Label.setTextSelEnd() must be called as a Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t index ;
    if(JS_ToUint32(ctx, (uint32_t *) &index, argv[0])!=0){
        THROW_EXCEPTION("arg index of method Label.setTextSelEnd() must be a number")
    }
    lv_label_set_text_sel_end(thisobj, index) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_label_get_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Label.text() must be called as a Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_label_get_text(thisobj)) ;
    return retval ;
}

static JSValue js_lv_label_get_long_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Label.longMode() must be called as a Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_label_get_long_mode(thisobj)) ;
    return retval ;
}

static JSValue js_lv_label_get_recolor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Label.recolor() must be called as a Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_label_get_recolor(thisobj)) ;
    return retval ;
}

static JSValue js_lv_label_get_letter_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Label.letterPos() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Label.letterPos() must be called as a Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t char_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &char_id, argv[0])!=0){
        THROW_EXCEPTION("arg char_id of method Label.letterPos() must be a number")
    }
    if(!JS_IsObject(argv[1])){
        THROW_EXCEPTION("arg pos of method Label.letterPos() must be a object{x,y}")
    }
    lv_point_t pos ;
    JSValue jspos_x = JS_GetPropertyStr(ctx, argv[1], "x") ;
    if(!JS_IsNumber(jspos_x)){
        THROW_EXCEPTION("arg pos of method Label.letterPos() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&pos.x, jspos_x)!=0) {
        THROW_EXCEPTION("property x of arg pos is not a number")
    }
    JSValue jspos_y = JS_GetPropertyStr(ctx, argv[1], "y") ;
    if(!JS_IsNumber(jspos_y)){
        THROW_EXCEPTION("arg pos of method Label.letterPos() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&pos.y, jspos_y)!=0) {
        THROW_EXCEPTION("property y of arg pos is not a number")
    }
    lv_label_get_letter_pos(thisobj, char_id, &pos) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_label_get_letter_on(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Label.letterOn() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Label.letterOn() must be called as a Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg pos_in of method Label.letterOn() must be a object{x,y}")
    }
    lv_point_t pos_in ;
    JSValue jspos_in_x = JS_GetPropertyStr(ctx, argv[0], "x") ;
    if(!JS_IsNumber(jspos_in_x)){
        THROW_EXCEPTION("arg pos_in of method Label.letterOn() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&pos_in.x, jspos_in_x)!=0) {
        THROW_EXCEPTION("property x of arg pos_in is not a number")
    }
    JSValue jspos_in_y = JS_GetPropertyStr(ctx, argv[0], "y") ;
    if(!JS_IsNumber(jspos_in_y)){
        THROW_EXCEPTION("arg pos_in of method Label.letterOn() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&pos_in.y, jspos_in_y)!=0) {
        THROW_EXCEPTION("property y of arg pos_in is not a number")
    }
    JSValue retval = JS_NewUint32(ctx,lv_label_get_letter_on(thisobj, &pos_in)) ;
    return retval ;
}

static JSValue js_lv_label_is_char_under_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Label.isCharUnderPos() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Label.isCharUnderPos() must be called as a Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg pos of method Label.isCharUnderPos() must be a object{x,y}")
    }
    lv_point_t pos ;
    JSValue jspos_x = JS_GetPropertyStr(ctx, argv[0], "x") ;
    if(!JS_IsNumber(jspos_x)){
        THROW_EXCEPTION("arg pos of method Label.isCharUnderPos() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&pos.x, jspos_x)!=0) {
        THROW_EXCEPTION("property x of arg pos is not a number")
    }
    JSValue jspos_y = JS_GetPropertyStr(ctx, argv[0], "y") ;
    if(!JS_IsNumber(jspos_y)){
        THROW_EXCEPTION("arg pos of method Label.isCharUnderPos() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&pos.y, jspos_y)!=0) {
        THROW_EXCEPTION("property y of arg pos is not a number")
    }
    JSValue retval = JS_NewBool(ctx,lv_label_is_char_under_pos(thisobj, &pos)) ;
    return retval ;
}

static JSValue js_lv_label_get_text_selection_start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Label.textSelectionStart() must be called as a Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_label_get_text_selection_start(thisobj)) ;
    return retval ;
}

static JSValue js_lv_label_get_text_selection_end(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Label.textSelectionEnd() must be called as a Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_label_get_text_selection_end(thisobj)) ;
    return retval ;
}

static JSValue js_lv_label_ins_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Label.insText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Label.insText() must be called as a Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t pos ;
    if(JS_ToUint32(ctx, (uint32_t *) &pos, argv[0])!=0){
        THROW_EXCEPTION("arg pos of method Label.insText() must be a number")
    }
    char * txt = (char *)JS_ToCString(ctx, argv[1]) ;
    lv_label_ins_text(thisobj, pos, txt) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, txt) ;
    return retval ;
}

static JSValue js_lv_label_cut_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Label.cutText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Label.cutText() must be called as a Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t pos ;
    if(JS_ToUint32(ctx, (uint32_t *) &pos, argv[0])!=0){
        THROW_EXCEPTION("arg pos of method Label.cutText() must be a number")
    }
    uint32_t cnt ;
    if(JS_ToUint32(ctx, (uint32_t *) &cnt, argv[1])!=0){
        THROW_EXCEPTION("arg cnt of method Label.cutText() must be a number")
    }
    lv_label_cut_text(thisobj, pos, cnt) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_label_proto_funcs[] = {
    JS_CFUNC_DEF("setFont", 0, js_lv_label_set_font),
    JS_CFUNC_DEF("setText", 0, js_lv_label_set_text),
    JS_CFUNC_DEF("setTextStatic", 0, js_lv_label_set_text_static),
    JS_CFUNC_DEF("setLongMode", 0, js_lv_label_set_long_mode),
    JS_CFUNC_DEF("setRecolor", 0, js_lv_label_set_recolor),
    JS_CFUNC_DEF("setTextSelStart", 0, js_lv_label_set_text_sel_start),
    JS_CFUNC_DEF("setTextSelEnd", 0, js_lv_label_set_text_sel_end),
    JS_CFUNC_DEF("text", 0, js_lv_label_get_text),
    JS_CFUNC_DEF("longMode", 0, js_lv_label_get_long_mode),
    JS_CFUNC_DEF("recolor", 0, js_lv_label_get_recolor),
    JS_CFUNC_DEF("letterPos", 0, js_lv_label_get_letter_pos),
    JS_CFUNC_DEF("letterOn", 0, js_lv_label_get_letter_on),
    JS_CFUNC_DEF("isCharUnderPos", 0, js_lv_label_is_char_under_pos),
    JS_CFUNC_DEF("textSelectionStart", 0, js_lv_label_get_text_selection_start),
    JS_CFUNC_DEF("textSelectionEnd", 0, js_lv_label_get_text_selection_end),
    JS_CFUNC_DEF("insText", 0, js_lv_label_ins_text),
    JS_CFUNC_DEF("cutText", 0, js_lv_label_cut_text),
} ;
#define __def_js_lv_label_proto_funcs__


static const JSCFunctionListEntry js_lv_label_static_funcs[] = {
} ;
static JSValue js_lv_arc_set_start_angle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Arc.setStartAngle() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.setStartAngle() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t start ;
    if(JS_ToUint32(ctx, (uint32_t *) &start, argv[0])!=0){
        THROW_EXCEPTION("arg start of method Arc.setStartAngle() must be a number")
    }
    lv_arc_set_start_angle(thisobj, start) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_end_angle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Arc.setEndAngle() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.setEndAngle() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t end ;
    if(JS_ToUint32(ctx, (uint32_t *) &end, argv[0])!=0){
        THROW_EXCEPTION("arg end of method Arc.setEndAngle() must be a number")
    }
    lv_arc_set_end_angle(thisobj, end) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_angles(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Arc.setAngles() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.setAngles() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t start ;
    if(JS_ToUint32(ctx, (uint32_t *) &start, argv[0])!=0){
        THROW_EXCEPTION("arg start of method Arc.setAngles() must be a number")
    }
    uint16_t end ;
    if(JS_ToUint32(ctx, (uint32_t *) &end, argv[1])!=0){
        THROW_EXCEPTION("arg end of method Arc.setAngles() must be a number")
    }
    lv_arc_set_angles(thisobj, start, end) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_bg_start_angle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Arc.setBgStartAngle() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.setBgStartAngle() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t start ;
    if(JS_ToUint32(ctx, (uint32_t *) &start, argv[0])!=0){
        THROW_EXCEPTION("arg start of method Arc.setBgStartAngle() must be a number")
    }
    lv_arc_set_bg_start_angle(thisobj, start) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_bg_end_angle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Arc.setBgEndAngle() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.setBgEndAngle() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t end ;
    if(JS_ToUint32(ctx, (uint32_t *) &end, argv[0])!=0){
        THROW_EXCEPTION("arg end of method Arc.setBgEndAngle() must be a number")
    }
    lv_arc_set_bg_end_angle(thisobj, end) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_bg_angles(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Arc.setBgAngles() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.setBgAngles() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t start ;
    if(JS_ToUint32(ctx, (uint32_t *) &start, argv[0])!=0){
        THROW_EXCEPTION("arg start of method Arc.setBgAngles() must be a number")
    }
    uint16_t end ;
    if(JS_ToUint32(ctx, (uint32_t *) &end, argv[1])!=0){
        THROW_EXCEPTION("arg end of method Arc.setBgAngles() must be a number")
    }
    lv_arc_set_bg_angles(thisobj, start, end) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_rotation(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Arc.setRotation() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.setRotation() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t rotation ;
    if(JS_ToUint32(ctx, (uint32_t *) &rotation, argv[0])!=0){
        THROW_EXCEPTION("arg rotation of method Arc.setRotation() must be a number")
    }
    lv_arc_set_rotation(thisobj, rotation) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Arc.setMode() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.setMode() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t type ;
    if(JS_ToUint32(ctx, (uint32_t *) &type, argv[0])!=0){
        THROW_EXCEPTION("arg type of method Arc.setMode() must be a number")
    }
    lv_arc_set_mode(thisobj, type) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Arc.setValue() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.setValue() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t value ;
    if(JS_ToInt32(ctx, (int32_t *) &value, argv[0])!=0){
        THROW_EXCEPTION("arg value of method Arc.setValue() must be a number")
    }
    lv_arc_set_value(thisobj, value) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_range(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Arc.setRange() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.setRange() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t min ;
    if(JS_ToInt32(ctx, (int32_t *) &min, argv[0])!=0){
        THROW_EXCEPTION("arg min of method Arc.setRange() must be a number")
    }
    int16_t max ;
    if(JS_ToInt32(ctx, (int32_t *) &max, argv[1])!=0){
        THROW_EXCEPTION("arg max of method Arc.setRange() must be a number")
    }
    lv_arc_set_range(thisobj, min, max) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_change_rate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Arc.setChangeRate() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.setChangeRate() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t rate ;
    if(JS_ToUint32(ctx, (uint32_t *) &rate, argv[0])!=0){
        THROW_EXCEPTION("arg rate of method Arc.setChangeRate() must be a number")
    }
    lv_arc_set_change_rate(thisobj, rate) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_get_angle_start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.angleStart() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_arc_get_angle_start(thisobj)) ;
    return retval ;
}

static JSValue js_lv_arc_get_angle_end(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.angleEnd() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_arc_get_angle_end(thisobj)) ;
    return retval ;
}

static JSValue js_lv_arc_get_bg_angle_start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.bgAngleStart() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_arc_get_bg_angle_start(thisobj)) ;
    return retval ;
}

static JSValue js_lv_arc_get_bg_angle_end(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.bgAngleEnd() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_arc_get_bg_angle_end(thisobj)) ;
    return retval ;
}

static JSValue js_lv_arc_get_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.value() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_arc_get_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_arc_get_min_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.minValue() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_arc_get_min_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_arc_get_max_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.maxValue() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_arc_get_max_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_arc_get_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Arc.mode() must be called as a Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_arc_get_mode(thisobj)) ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_arc_proto_funcs[] = {
    JS_CFUNC_DEF("setStartAngle", 0, js_lv_arc_set_start_angle),
    JS_CFUNC_DEF("setEndAngle", 0, js_lv_arc_set_end_angle),
    JS_CFUNC_DEF("setAngles", 0, js_lv_arc_set_angles),
    JS_CFUNC_DEF("setBgStartAngle", 0, js_lv_arc_set_bg_start_angle),
    JS_CFUNC_DEF("setBgEndAngle", 0, js_lv_arc_set_bg_end_angle),
    JS_CFUNC_DEF("setBgAngles", 0, js_lv_arc_set_bg_angles),
    JS_CFUNC_DEF("setRotation", 0, js_lv_arc_set_rotation),
    JS_CFUNC_DEF("setMode", 0, js_lv_arc_set_mode),
    JS_CFUNC_DEF("setValue", 0, js_lv_arc_set_value),
    JS_CFUNC_DEF("setRange", 0, js_lv_arc_set_range),
    JS_CFUNC_DEF("setChangeRate", 0, js_lv_arc_set_change_rate),
    JS_CFUNC_DEF("angleStart", 0, js_lv_arc_get_angle_start),
    JS_CFUNC_DEF("angleEnd", 0, js_lv_arc_get_angle_end),
    JS_CFUNC_DEF("bgAngleStart", 0, js_lv_arc_get_bg_angle_start),
    JS_CFUNC_DEF("bgAngleEnd", 0, js_lv_arc_get_bg_angle_end),
    JS_CFUNC_DEF("value", 0, js_lv_arc_get_value),
    JS_CFUNC_DEF("minValue", 0, js_lv_arc_get_min_value),
    JS_CFUNC_DEF("maxValue", 0, js_lv_arc_get_max_value),
    JS_CFUNC_DEF("mode", 0, js_lv_arc_get_mode),
} ;
#define __def_js_lv_arc_proto_funcs__


static const JSCFunctionListEntry js_lv_arc_static_funcs[] = {
} ;
static JSValue js_lv_bar_set_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Bar.setValue() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Bar.setValue() must be called as a Bar method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int32_t value ;
    if(JS_ToInt32(ctx, (int32_t *) &value, argv[0])!=0){
        THROW_EXCEPTION("arg value of method Bar.setValue() must be a number")
    }
    bool anim = LV_ANIM_ON ;
    if(argc>1) {
        anim = JS_ToBool(ctx, argv[1]) ;
    }
    lv_bar_set_value(thisobj, value, anim) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_bar_set_start_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Bar.setStartValue() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Bar.setStartValue() must be called as a Bar method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int32_t start_value ;
    if(JS_ToInt32(ctx, (int32_t *) &start_value, argv[0])!=0){
        THROW_EXCEPTION("arg start_value of method Bar.setStartValue() must be a number")
    }
    bool anim = JS_ToBool(ctx, argv[1]) ;
    lv_bar_set_start_value(thisobj, start_value, anim) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_bar_set_range(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Bar.setRange() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Bar.setRange() must be called as a Bar method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int32_t min ;
    if(JS_ToInt32(ctx, (int32_t *) &min, argv[0])!=0){
        THROW_EXCEPTION("arg min of method Bar.setRange() must be a number")
    }
    int32_t max ;
    if(JS_ToInt32(ctx, (int32_t *) &max, argv[1])!=0){
        THROW_EXCEPTION("arg max of method Bar.setRange() must be a number")
    }
    lv_bar_set_range(thisobj, min, max) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_bar_set_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Bar.setMode() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Bar.setMode() must be called as a Bar method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t mode ;
    if(JS_ToUint32(ctx, (uint32_t *) &mode, argv[0])!=0){
        THROW_EXCEPTION("arg mode of method Bar.setMode() must be a number")
    }
    lv_bar_set_mode(thisobj, mode) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_bar_get_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Bar.value() must be called as a Bar method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_bar_get_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_bar_get_start_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Bar.startValue() must be called as a Bar method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_bar_get_start_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_bar_get_min_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Bar.minValue() must be called as a Bar method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_bar_get_min_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_bar_get_max_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Bar.maxValue() must be called as a Bar method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_bar_get_max_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_bar_get_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Bar.mode() must be called as a Bar method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_bar_get_mode(thisobj)) ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_bar_proto_funcs[] = {
    JS_CFUNC_DEF("setValue", 0, js_lv_bar_set_value),
    JS_CFUNC_DEF("setStartValue", 0, js_lv_bar_set_start_value),
    JS_CFUNC_DEF("setRange", 0, js_lv_bar_set_range),
    JS_CFUNC_DEF("setMode", 0, js_lv_bar_set_mode),
    JS_CFUNC_DEF("value", 0, js_lv_bar_get_value),
    JS_CFUNC_DEF("startValue", 0, js_lv_bar_get_start_value),
    JS_CFUNC_DEF("minValue", 0, js_lv_bar_get_min_value),
    JS_CFUNC_DEF("maxValue", 0, js_lv_bar_get_max_value),
    JS_CFUNC_DEF("mode", 0, js_lv_bar_get_mode),
} ;
#define __def_js_lv_bar_proto_funcs__


static const JSCFunctionListEntry js_lv_bar_static_funcs[] = {
} ;

static const JSCFunctionListEntry js_lv_btn_proto_funcs[] = {
} ;
#define __def_js_lv_btn_proto_funcs__


static const JSCFunctionListEntry js_lv_btn_static_funcs[] = {
} ;
static JSValue js_lv_btnmatrix_get_popovers(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("BtnMatrix.popovers() must be called as a BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_btnmatrix_get_popovers(thisobj)) ;
    return retval ;
}

static JSValue js_lv_btnmatrix_set_selected_btn(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("BtnMatrix.setSelectedBtn() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("BtnMatrix.setSelectedBtn() must be called as a BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t btn_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &btn_id, argv[0])!=0){
        THROW_EXCEPTION("arg btn_id of method BtnMatrix.setSelectedBtn() must be a number")
    }
    lv_btnmatrix_set_selected_btn(thisobj, btn_id) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_btnmatrix_set_btn_ctrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("BtnMatrix.setBtnCtrl() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("BtnMatrix.setBtnCtrl() must be called as a BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t btn_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &btn_id, argv[0])!=0){
        THROW_EXCEPTION("arg btn_id of method BtnMatrix.setBtnCtrl() must be a number")
    }
    uint8_t ctrl ;
    if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[1])!=0){
        THROW_EXCEPTION("arg ctrl of method BtnMatrix.setBtnCtrl() must be a number")
    }
    lv_btnmatrix_set_btn_ctrl(thisobj, btn_id, ctrl) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_btnmatrix_clear_btn_ctrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("BtnMatrix.clearBtnCtrl() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("BtnMatrix.clearBtnCtrl() must be called as a BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t btn_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &btn_id, argv[0])!=0){
        THROW_EXCEPTION("arg btn_id of method BtnMatrix.clearBtnCtrl() must be a number")
    }
    uint8_t ctrl ;
    if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[1])!=0){
        THROW_EXCEPTION("arg ctrl of method BtnMatrix.clearBtnCtrl() must be a number")
    }
    lv_btnmatrix_clear_btn_ctrl(thisobj, btn_id, ctrl) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_btnmatrix_set_btn_ctrl_all(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("BtnMatrix.setBtnCtrlAll() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("BtnMatrix.setBtnCtrlAll() must be called as a BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t ctrl ;
    if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[0])!=0){
        THROW_EXCEPTION("arg ctrl of method BtnMatrix.setBtnCtrlAll() must be a number")
    }
    lv_btnmatrix_set_btn_ctrl_all(thisobj, ctrl) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_btnmatrix_clear_btn_ctrl_all(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("BtnMatrix.clearBtnCtrlAll() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("BtnMatrix.clearBtnCtrlAll() must be called as a BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t ctrl ;
    if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[0])!=0){
        THROW_EXCEPTION("arg ctrl of method BtnMatrix.clearBtnCtrlAll() must be a number")
    }
    lv_btnmatrix_clear_btn_ctrl_all(thisobj, ctrl) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_btnmatrix_set_btn_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("BtnMatrix.setBtnWidth() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("BtnMatrix.setBtnWidth() must be called as a BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t btn_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &btn_id, argv[0])!=0){
        THROW_EXCEPTION("arg btn_id of method BtnMatrix.setBtnWidth() must be a number")
    }
    uint8_t width ;
    if(JS_ToUint32(ctx, (uint32_t *) &width, argv[1])!=0){
        THROW_EXCEPTION("arg width of method BtnMatrix.setBtnWidth() must be a number")
    }
    lv_btnmatrix_set_btn_width(thisobj, btn_id, width) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_btnmatrix_set_one_checked(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("BtnMatrix.setOneChecked() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("BtnMatrix.setOneChecked() must be called as a BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool en = JS_ToBool(ctx, argv[0]) ;
    lv_btnmatrix_set_one_checked(thisobj, en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_btnmatrix_get_selected_btn(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("BtnMatrix.selectedBtn() must be called as a BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_btnmatrix_get_selected_btn(thisobj)) ;
    return retval ;
}

static JSValue js_lv_btnmatrix_get_btn_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("BtnMatrix.btnText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("BtnMatrix.btnText() must be called as a BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t btn_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &btn_id, argv[0])!=0){
        THROW_EXCEPTION("arg btn_id of method BtnMatrix.btnText() must be a number")
    }
    JSValue retval = JS_NewString(ctx,lv_btnmatrix_get_btn_text(thisobj, btn_id)) ;
    return retval ;
}

static JSValue js_lv_btnmatrix_has_btn_ctrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("BtnMatrix.hasBtnCtrl() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("BtnMatrix.hasBtnCtrl() must be called as a BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t btn_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &btn_id, argv[0])!=0){
        THROW_EXCEPTION("arg btn_id of method BtnMatrix.hasBtnCtrl() must be a number")
    }
    uint8_t ctrl ;
    if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[1])!=0){
        THROW_EXCEPTION("arg ctrl of method BtnMatrix.hasBtnCtrl() must be a number")
    }
    JSValue retval = JS_NewBool(ctx,lv_btnmatrix_has_btn_ctrl(thisobj, btn_id, ctrl)) ;
    return retval ;
}

static JSValue js_lv_btnmatrix_get_one_checked(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("BtnMatrix.oneChecked() must be called as a BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_btnmatrix_get_one_checked(thisobj)) ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_btnmatrix_proto_funcs[] = {
    JS_CFUNC_DEF("popovers", 0, js_lv_btnmatrix_get_popovers),
    JS_CFUNC_DEF("setSelectedBtn", 0, js_lv_btnmatrix_set_selected_btn),
    JS_CFUNC_DEF("setBtnCtrl", 0, js_lv_btnmatrix_set_btn_ctrl),
    JS_CFUNC_DEF("clearBtnCtrl", 0, js_lv_btnmatrix_clear_btn_ctrl),
    JS_CFUNC_DEF("setBtnCtrlAll", 0, js_lv_btnmatrix_set_btn_ctrl_all),
    JS_CFUNC_DEF("clearBtnCtrlAll", 0, js_lv_btnmatrix_clear_btn_ctrl_all),
    JS_CFUNC_DEF("setBtnWidth", 0, js_lv_btnmatrix_set_btn_width),
    JS_CFUNC_DEF("setOneChecked", 0, js_lv_btnmatrix_set_one_checked),
    JS_CFUNC_DEF("selectedBtn", 0, js_lv_btnmatrix_get_selected_btn),
    JS_CFUNC_DEF("btnText", 0, js_lv_btnmatrix_get_btn_text),
    JS_CFUNC_DEF("hasBtnCtrl", 0, js_lv_btnmatrix_has_btn_ctrl),
    JS_CFUNC_DEF("oneChecked", 0, js_lv_btnmatrix_get_one_checked),
} ;
#define __def_js_lv_btnmatrix_proto_funcs__


static const JSCFunctionListEntry js_lv_btnmatrix_static_funcs[] = {
} ;
static JSValue js_lv_canvas_set_px_color(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("Canvas.setPxColor() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Canvas.setPxColor() must be called as a Canvas method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method Canvas.setPxColor() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method Canvas.setPxColor() must be a number")
    }
    uint16_t c ;
    if(JS_ToUint32(ctx, (uint32_t *) &c, argv[2])!=0){
        THROW_EXCEPTION("arg c of method Canvas.setPxColor() must be a number")
    }
    lv_canvas_set_px_color(thisobj, x, y, (lv_color_t)c) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_canvas_set_px(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("Canvas.setPx() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Canvas.setPx() must be called as a Canvas method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method Canvas.setPx() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method Canvas.setPx() must be a number")
    }
    uint16_t c ;
    if(JS_ToUint32(ctx, (uint32_t *) &c, argv[2])!=0){
        THROW_EXCEPTION("arg c of method Canvas.setPx() must be a number")
    }
    lv_canvas_set_px(thisobj, x, y, (lv_color_t)c) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_canvas_set_px_opa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("Canvas.setPxOpa() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Canvas.setPxOpa() must be called as a Canvas method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method Canvas.setPxOpa() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method Canvas.setPxOpa() must be a number")
    }
    uint8_t opa ;
    if(JS_ToUint32(ctx, (uint32_t *) &opa, argv[2])!=0){
        THROW_EXCEPTION("arg opa of method Canvas.setPxOpa() must be a number")
    }
    lv_canvas_set_px_opa(thisobj, x, y, opa) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_canvas_set_palette(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Canvas.setPalette() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Canvas.setPalette() must be called as a Canvas method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t id ;
    if(JS_ToUint32(ctx, (uint32_t *) &id, argv[0])!=0){
        THROW_EXCEPTION("arg id of method Canvas.setPalette() must be a number")
    }
    uint16_t c ;
    if(JS_ToUint32(ctx, (uint32_t *) &c, argv[1])!=0){
        THROW_EXCEPTION("arg c of method Canvas.setPalette() must be a number")
    }
    lv_canvas_set_palette(thisobj, id, (lv_color_t)c) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_canvas_get_px(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Canvas.px() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Canvas.px() must be called as a Canvas method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method Canvas.px() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method Canvas.px() must be a number")
    }
    JSValue retval = JS_NewUint32(ctx,lv_canvas_get_px(thisobj, x, y).full) ;
    return retval ;
}

static JSValue js_lv_canvas_blur_hor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Canvas.blurHor() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Canvas.blurHor() must be called as a Canvas method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg area of method Canvas.blurHor() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t area ;
    JSValue jsarea_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jsarea_x1)){
        THROW_EXCEPTION("arg area of method Canvas.blurHor() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x1, jsarea_x1)!=0) {
        THROW_EXCEPTION("property x1 of arg area is not a number")
    }
    JSValue jsarea_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jsarea_y1)){
        THROW_EXCEPTION("arg area of method Canvas.blurHor() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y1, jsarea_y1)!=0) {
        THROW_EXCEPTION("property y1 of arg area is not a number")
    }
    JSValue jsarea_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jsarea_x2)){
        THROW_EXCEPTION("arg area of method Canvas.blurHor() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x2, jsarea_x2)!=0) {
        THROW_EXCEPTION("property x2 of arg area is not a number")
    }
    JSValue jsarea_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jsarea_y2)){
        THROW_EXCEPTION("arg area of method Canvas.blurHor() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y2, jsarea_y2)!=0) {
        THROW_EXCEPTION("property y2 of arg area is not a number")
    }
    uint16_t r ;
    if(JS_ToUint32(ctx, (uint32_t *) &r, argv[1])!=0){
        THROW_EXCEPTION("arg r of method Canvas.blurHor() must be a number")
    }
    lv_canvas_blur_hor(thisobj, &area, r) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_canvas_blur_ver(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Canvas.blurVer() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Canvas.blurVer() must be called as a Canvas method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg area of method Canvas.blurVer() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t area ;
    JSValue jsarea_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jsarea_x1)){
        THROW_EXCEPTION("arg area of method Canvas.blurVer() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x1, jsarea_x1)!=0) {
        THROW_EXCEPTION("property x1 of arg area is not a number")
    }
    JSValue jsarea_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jsarea_y1)){
        THROW_EXCEPTION("arg area of method Canvas.blurVer() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y1, jsarea_y1)!=0) {
        THROW_EXCEPTION("property y1 of arg area is not a number")
    }
    JSValue jsarea_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jsarea_x2)){
        THROW_EXCEPTION("arg area of method Canvas.blurVer() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x2, jsarea_x2)!=0) {
        THROW_EXCEPTION("property x2 of arg area is not a number")
    }
    JSValue jsarea_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jsarea_y2)){
        THROW_EXCEPTION("arg area of method Canvas.blurVer() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y2, jsarea_y2)!=0) {
        THROW_EXCEPTION("property y2 of arg area is not a number")
    }
    uint16_t r ;
    if(JS_ToUint32(ctx, (uint32_t *) &r, argv[1])!=0){
        THROW_EXCEPTION("arg r of method Canvas.blurVer() must be a number")
    }
    lv_canvas_blur_ver(thisobj, &area, r) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_canvas_fill_bg(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Canvas.fillBg() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Canvas.fillBg() must be called as a Canvas method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t color ;
    if(JS_ToUint32(ctx, (uint32_t *) &color, argv[0])!=0){
        THROW_EXCEPTION("arg color of method Canvas.fillBg() must be a number")
    }
    uint8_t opa ;
    if(JS_ToUint32(ctx, (uint32_t *) &opa, argv[1])!=0){
        THROW_EXCEPTION("arg opa of method Canvas.fillBg() must be a number")
    }
    lv_canvas_fill_bg(thisobj, (lv_color_t)color, opa) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_canvas_draw_rect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<5) {
        THROW_EXCEPTION("Canvas.drawRect() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Canvas.drawRect() must be called as a Canvas method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method Canvas.drawRect() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method Canvas.drawRect() must be a number")
    }
    int16_t w ;
    if(JS_ToInt32(ctx, (int32_t *) &w, argv[2])!=0){
        THROW_EXCEPTION("arg w of method Canvas.drawRect() must be a number")
    }
    int16_t h ;
    if(JS_ToInt32(ctx, (int32_t *) &h, argv[3])!=0){
        THROW_EXCEPTION("arg h of method Canvas.drawRect() must be a number")
    }
    lv_draw_rect_dsc_t * draw_dsc = JS_GetOpaque(argv[4], js_lv_draw_rect_dsc_class_id);
    if(!draw_dsc) {
        THROW_EXCEPTION("arg draw_dsc of method Canvas.drawRect() must be a lv.DrawRectDsc object")
    }
    lv_canvas_draw_rect(thisobj, x, y, w, h, draw_dsc) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_canvas_draw_arc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<6) {
        THROW_EXCEPTION("Canvas.drawArc() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Canvas.drawArc() must be called as a Canvas method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method Canvas.drawArc() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method Canvas.drawArc() must be a number")
    }
    int16_t r ;
    if(JS_ToInt32(ctx, (int32_t *) &r, argv[2])!=0){
        THROW_EXCEPTION("arg r of method Canvas.drawArc() must be a number")
    }
    int32_t start_angle ;
    if(JS_ToInt32(ctx, (int32_t *) &start_angle, argv[3])!=0){
        THROW_EXCEPTION("arg start_angle of method Canvas.drawArc() must be a number")
    }
    int32_t end_angle ;
    if(JS_ToInt32(ctx, (int32_t *) &end_angle, argv[4])!=0){
        THROW_EXCEPTION("arg end_angle of method Canvas.drawArc() must be a number")
    }
    lv_draw_arc_dsc_t * draw_dsc = JS_GetOpaque(argv[5], js_lv_draw_arc_dsc_class_id);
    if(!draw_dsc) {
        THROW_EXCEPTION("arg draw_dsc of method Canvas.drawArc() must be a lv.DrawArcDsc object")
    }
    lv_canvas_draw_arc(thisobj, x, y, r, start_angle, end_angle, draw_dsc) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_canvas_proto_funcs[] = {
    JS_CFUNC_DEF("malloc", 0, js_lv_canvas_malloc),
    JS_CFUNC_DEF("setPxColor", 0, js_lv_canvas_set_px_color),
    JS_CFUNC_DEF("setPx", 0, js_lv_canvas_set_px),
    JS_CFUNC_DEF("setPxOpa", 0, js_lv_canvas_set_px_opa),
    JS_CFUNC_DEF("setPalette", 0, js_lv_canvas_set_palette),
    JS_CFUNC_DEF("px", 0, js_lv_canvas_get_px),
    JS_CFUNC_DEF("blurHor", 0, js_lv_canvas_blur_hor),
    JS_CFUNC_DEF("blurVer", 0, js_lv_canvas_blur_ver),
    JS_CFUNC_DEF("fillBg", 0, js_lv_canvas_fill_bg),
    JS_CFUNC_DEF("drawRect", 0, js_lv_canvas_draw_rect),
    JS_CFUNC_DEF("drawArc", 0, js_lv_canvas_draw_arc),
} ;
#define __def_js_lv_canvas_proto_funcs__


static const JSCFunctionListEntry js_lv_canvas_static_funcs[] = {
} ;
static JSValue js_lv_checkbox_set_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Checkbox.setText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Checkbox.setText() must be called as a Checkbox method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * txt = (char *)JS_ToCString(ctx, argv[0]) ;
    lv_checkbox_set_text(thisobj, txt) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, txt) ;
    return retval ;
}

static JSValue js_lv_checkbox_set_text_static(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Checkbox.setTextStatic() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Checkbox.setTextStatic() must be called as a Checkbox method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * txt = (char *)JS_ToCString(ctx, argv[0]) ;
    lv_checkbox_set_text_static(thisobj, txt) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, txt) ;
    return retval ;
}

static JSValue js_lv_checkbox_get_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Checkbox.text() must be called as a Checkbox method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_checkbox_get_text(thisobj)) ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_checkbox_proto_funcs[] = {
    JS_CFUNC_DEF("setFont", 0, js_lv_label_set_font),
    JS_CFUNC_DEF("setText", 0, js_lv_checkbox_set_text),
    JS_CFUNC_DEF("setTextStatic", 0, js_lv_checkbox_set_text_static),
    JS_CFUNC_DEF("text", 0, js_lv_checkbox_get_text),
} ;
#define __def_js_lv_checkbox_proto_funcs__


static const JSCFunctionListEntry js_lv_checkbox_static_funcs[] = {
} ;
static JSValue js_lv_dropdown_set_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Dropdown.setText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Dropdown.setText() must be called as a Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * txt = (char *)JS_ToCString(ctx, argv[0]) ;
    lv_dropdown_set_text(thisobj, txt) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, txt) ;
    return retval ;
}

static JSValue js_lv_dropdown_set_options(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Dropdown.setOptions() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Dropdown.setOptions() must be called as a Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * options = (char *)JS_ToCString(ctx, argv[0]) ;
    lv_dropdown_set_options(thisobj, options) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, options) ;
    return retval ;
}

static JSValue js_lv_dropdown_set_options_static(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Dropdown.setOptionsStatic() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Dropdown.setOptionsStatic() must be called as a Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * options = (char *)JS_ToCString(ctx, argv[0]) ;
    lv_dropdown_set_options_static(thisobj, options) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, options) ;
    return retval ;
}

static JSValue js_lv_dropdown_add_option(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Dropdown.addOption() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Dropdown.addOption() must be called as a Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * option = (char *)JS_ToCString(ctx, argv[0]) ;
    uint32_t pos ;
    if(JS_ToUint32(ctx, (uint32_t *) &pos, argv[1])!=0){
        THROW_EXCEPTION("arg pos of method Dropdown.addOption() must be a number")
    }
    lv_dropdown_add_option(thisobj, option, pos) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, option) ;
    return retval ;
}

static JSValue js_lv_dropdown_clear_options(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Dropdown.clearOptions() must be called as a Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_dropdown_clear_options(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_dropdown_set_selected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Dropdown.setSelected() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Dropdown.setSelected() must be called as a Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t sel_opt ;
    if(JS_ToUint32(ctx, (uint32_t *) &sel_opt, argv[0])!=0){
        THROW_EXCEPTION("arg sel_opt of method Dropdown.setSelected() must be a number")
    }
    lv_dropdown_set_selected(thisobj, sel_opt) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_dropdown_set_dir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Dropdown.setDir() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Dropdown.setDir() must be called as a Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_dir_t dir ;
    if(!lv_dir_jsstr_to_const(ctx, argv[0], &dir)) {
        return JS_EXCEPTION ;
    }
    lv_dropdown_set_dir(thisobj, dir) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_dropdown_set_selected_highlight(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Dropdown.setSelectedHighlight() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Dropdown.setSelectedHighlight() must be called as a Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool en = JS_ToBool(ctx, argv[0]) ;
    lv_dropdown_set_selected_highlight(thisobj, en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_dropdown_get_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Dropdown.text() must be called as a Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_dropdown_get_text(thisobj)) ;
    return retval ;
}

static JSValue js_lv_dropdown_get_options(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Dropdown.options() must be called as a Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_dropdown_get_options(thisobj)) ;
    return retval ;
}

static JSValue js_lv_dropdown_get_selected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Dropdown.selected() must be called as a Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_dropdown_get_selected(thisobj)) ;
    return retval ;
}

static JSValue js_lv_dropdown_get_option_cnt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Dropdown.optionCnt() must be called as a Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_dropdown_get_option_cnt(thisobj)) ;
    return retval ;
}

static JSValue js_lv_dropdown_get_selected_str(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Dropdown.selectedStr() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Dropdown.selectedStr() must be called as a Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * buf = (char *)JS_ToCString(ctx, argv[0]) ;
    uint32_t buf_size ;
    if(JS_ToUint32(ctx, (uint32_t *) &buf_size, argv[1])!=0){
        THROW_EXCEPTION("arg buf_size of method Dropdown.selectedStr() must be a number")
    }
    lv_dropdown_get_selected_str(thisobj, buf, buf_size) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, buf) ;
    return retval ;
}

static JSValue js_lv_dropdown_get_symbol(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Dropdown.symbol() must be called as a Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_dropdown_get_symbol(thisobj)) ;
    return retval ;
}

static JSValue js_lv_dropdown_get_selected_highlight(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Dropdown.selectedHighlight() must be called as a Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_dropdown_get_selected_highlight(thisobj)) ;
    return retval ;
}

static JSValue js_lv_dropdown_get_dir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Dropdown.dir() must be called as a Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = lv_dir_const_to_jsstr(ctx,lv_dropdown_get_dir(thisobj)) ;
    return retval ;
}

static JSValue js_lv_dropdown_open(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Dropdown.open() must be called as a Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_dropdown_open(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_dropdown_close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Dropdown.close() must be called as a Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_dropdown_close(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_dropdown_proto_funcs[] = {
    JS_CFUNC_DEF("setText", 0, js_lv_dropdown_set_text),
    JS_CFUNC_DEF("setOptions", 0, js_lv_dropdown_set_options),
    JS_CFUNC_DEF("setOptionsStatic", 0, js_lv_dropdown_set_options_static),
    JS_CFUNC_DEF("addOption", 0, js_lv_dropdown_add_option),
    JS_CFUNC_DEF("clearOptions", 0, js_lv_dropdown_clear_options),
    JS_CFUNC_DEF("setSelected", 0, js_lv_dropdown_set_selected),
    JS_CFUNC_DEF("setDir", 0, js_lv_dropdown_set_dir),
    JS_CFUNC_DEF("setSelectedHighlight", 0, js_lv_dropdown_set_selected_highlight),
    JS_CFUNC_DEF("text", 0, js_lv_dropdown_get_text),
    JS_CFUNC_DEF("options", 0, js_lv_dropdown_get_options),
    JS_CFUNC_DEF("selected", 0, js_lv_dropdown_get_selected),
    JS_CFUNC_DEF("optionCnt", 0, js_lv_dropdown_get_option_cnt),
    JS_CFUNC_DEF("selectedStr", 0, js_lv_dropdown_get_selected_str),
    JS_CFUNC_DEF("symbol", 0, js_lv_dropdown_get_symbol),
    JS_CFUNC_DEF("selectedHighlight", 0, js_lv_dropdown_get_selected_highlight),
    JS_CFUNC_DEF("dir", 0, js_lv_dropdown_get_dir),
    JS_CFUNC_DEF("open", 0, js_lv_dropdown_open),
    JS_CFUNC_DEF("close", 0, js_lv_dropdown_close),
} ;
#define __def_js_lv_dropdown_proto_funcs__


static const JSCFunctionListEntry js_lv_dropdown_static_funcs[] = {
} ;
static JSValue js_lv_img_set_offset_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Img.setOffsetX() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Img.setOffsetX() must be called as a Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method Img.setOffsetX() must be a number")
    }
    lv_img_set_offset_x(thisobj, x) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_img_set_offset_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Img.setOffsetY() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Img.setOffsetY() must be called as a Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[0])!=0){
        THROW_EXCEPTION("arg y of method Img.setOffsetY() must be a number")
    }
    lv_img_set_offset_y(thisobj, y) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_img_set_angle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Img.setAngle() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Img.setAngle() must be called as a Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t angle ;
    if(JS_ToInt32(ctx, (int32_t *) &angle, argv[0])!=0){
        THROW_EXCEPTION("arg angle of method Img.setAngle() must be a number")
    }
    lv_img_set_angle(thisobj, angle) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_img_set_pivot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Img.setPivot() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Img.setPivot() must be called as a Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method Img.setPivot() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method Img.setPivot() must be a number")
    }
    lv_img_set_pivot(thisobj, x, y) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_img_set_zoom(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Img.setZoom() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Img.setZoom() must be called as a Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t zoom ;
    if(JS_ToUint32(ctx, (uint32_t *) &zoom, argv[0])!=0){
        THROW_EXCEPTION("arg zoom of method Img.setZoom() must be a number")
    }
    lv_img_set_zoom(thisobj, zoom) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_img_set_antialias(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Img.setAntialias() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Img.setAntialias() must be called as a Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool antialias = JS_ToBool(ctx, argv[0]) ;
    lv_img_set_antialias(thisobj, antialias) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_img_set_size_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Img.setSizeMode() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Img.setSizeMode() must be called as a Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t mode ;
    if(JS_ToUint32(ctx, (uint32_t *) &mode, argv[0])!=0){
        THROW_EXCEPTION("arg mode of method Img.setSizeMode() must be a number")
    }
    lv_img_set_size_mode(thisobj, mode) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_img_get_offset_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Img.offsetX() must be called as a Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_img_get_offset_x(thisobj)) ;
    return retval ;
}

static JSValue js_lv_img_get_offset_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Img.offsetY() must be called as a Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_img_get_offset_y(thisobj)) ;
    return retval ;
}

static JSValue js_lv_img_get_angle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Img.angle() must be called as a Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_img_get_angle(thisobj)) ;
    return retval ;
}

static JSValue js_lv_img_get_pivot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Img.pivot() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Img.pivot() must be called as a Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg pivot of method Img.pivot() must be a object{x,y}")
    }
    lv_point_t pivot ;
    JSValue jspivot_x = JS_GetPropertyStr(ctx, argv[0], "x") ;
    if(!JS_IsNumber(jspivot_x)){
        THROW_EXCEPTION("arg pivot of method Img.pivot() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&pivot.x, jspivot_x)!=0) {
        THROW_EXCEPTION("property x of arg pivot is not a number")
    }
    JSValue jspivot_y = JS_GetPropertyStr(ctx, argv[0], "y") ;
    if(!JS_IsNumber(jspivot_y)){
        THROW_EXCEPTION("arg pivot of method Img.pivot() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&pivot.y, jspivot_y)!=0) {
        THROW_EXCEPTION("property y of arg pivot is not a number")
    }
    lv_img_get_pivot(thisobj, &pivot) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_img_get_zoom(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Img.zoom() must be called as a Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_img_get_zoom(thisobj)) ;
    return retval ;
}

static JSValue js_lv_img_get_antialias(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Img.antialias() must be called as a Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_img_get_antialias(thisobj)) ;
    return retval ;
}

static JSValue js_lv_img_get_size_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Img.sizeMode() must be called as a Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_img_get_size_mode(thisobj)) ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_img_proto_funcs[] = {
    JS_CFUNC_DEF("setSrc", 0, js_lv_img_set_src),
    JS_CFUNC_DEF("setSymbol", 0, js_lv_img_set_symbol),
    JS_CFUNC_DEF("setOffsetX", 0, js_lv_img_set_offset_x),
    JS_CFUNC_DEF("setOffsetY", 0, js_lv_img_set_offset_y),
    JS_CFUNC_DEF("setAngle", 0, js_lv_img_set_angle),
    JS_CFUNC_DEF("setPivot", 0, js_lv_img_set_pivot),
    JS_CFUNC_DEF("setZoom", 0, js_lv_img_set_zoom),
    JS_CFUNC_DEF("setAntialias", 0, js_lv_img_set_antialias),
    JS_CFUNC_DEF("setSizeMode", 0, js_lv_img_set_size_mode),
    JS_CFUNC_DEF("offsetX", 0, js_lv_img_get_offset_x),
    JS_CFUNC_DEF("offsetY", 0, js_lv_img_get_offset_y),
    JS_CFUNC_DEF("angle", 0, js_lv_img_get_angle),
    JS_CFUNC_DEF("pivot", 0, js_lv_img_get_pivot),
    JS_CFUNC_DEF("zoom", 0, js_lv_img_get_zoom),
    JS_CFUNC_DEF("antialias", 0, js_lv_img_get_antialias),
    JS_CFUNC_DEF("sizeMode", 0, js_lv_img_get_size_mode),
} ;
#define __def_js_lv_img_proto_funcs__


static const JSCFunctionListEntry js_lv_img_static_funcs[] = {
} ;
static JSValue js_lv_line_set_y_invert(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Line.setYInvert() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Line.setYInvert() must be called as a Line method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool en = JS_ToBool(ctx, argv[0]) ;
    lv_line_set_y_invert(thisobj, en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_line_get_y_invert(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Line.yInvert() must be called as a Line method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_line_get_y_invert(thisobj)) ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_line_proto_funcs[] = {
    JS_CFUNC_DEF("setPoints", 0, lv_line_set_points),
    JS_CFUNC_DEF("setYInvert", 0, js_lv_line_set_y_invert),
    JS_CFUNC_DEF("yInvert", 0, js_lv_line_get_y_invert),
} ;
#define __def_js_lv_line_proto_funcs__


static const JSCFunctionListEntry js_lv_line_static_funcs[] = {
} ;
static JSValue js_lv_roller_set_options(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Roller.setOptions() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Roller.setOptions() must be called as a Roller method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * options = (char *)JS_ToCString(ctx, argv[0]) ;
    uint8_t mode ;
    if(JS_ToUint32(ctx, (uint32_t *) &mode, argv[1])!=0){
        THROW_EXCEPTION("arg mode of method Roller.setOptions() must be a number")
    }
    lv_roller_set_options(thisobj, options, mode) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, options) ;
    return retval ;
}

static JSValue js_lv_roller_set_selected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Roller.setSelected() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Roller.setSelected() must be called as a Roller method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t sel_opt ;
    if(JS_ToUint32(ctx, (uint32_t *) &sel_opt, argv[0])!=0){
        THROW_EXCEPTION("arg sel_opt of method Roller.setSelected() must be a number")
    }
    bool anim = JS_ToBool(ctx, argv[1]) ;
    lv_roller_set_selected(thisobj, sel_opt, anim) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_roller_set_visible_row_count(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Roller.setVisibleRowCount() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Roller.setVisibleRowCount() must be called as a Roller method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t row_cnt ;
    if(JS_ToUint32(ctx, (uint32_t *) &row_cnt, argv[0])!=0){
        THROW_EXCEPTION("arg row_cnt of method Roller.setVisibleRowCount() must be a number")
    }
    lv_roller_set_visible_row_count(thisobj, row_cnt) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_roller_get_selected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Roller.selected() must be called as a Roller method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_roller_get_selected(thisobj)) ;
    return retval ;
}

static JSValue js_lv_roller_get_selected_str(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Roller.selectedStr() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Roller.selectedStr() must be called as a Roller method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * buf = (char *)JS_ToCString(ctx, argv[0]) ;
    uint32_t buf_size ;
    if(JS_ToUint32(ctx, (uint32_t *) &buf_size, argv[1])!=0){
        THROW_EXCEPTION("arg buf_size of method Roller.selectedStr() must be a number")
    }
    lv_roller_get_selected_str(thisobj, buf, buf_size) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, buf) ;
    return retval ;
}

static JSValue js_lv_roller_get_options(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Roller.options() must be called as a Roller method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_roller_get_options(thisobj)) ;
    return retval ;
}

static JSValue js_lv_roller_get_option_cnt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Roller.optionCnt() must be called as a Roller method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_roller_get_option_cnt(thisobj)) ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_roller_proto_funcs[] = {
    JS_CFUNC_DEF("setOptions", 0, js_lv_roller_set_options),
    JS_CFUNC_DEF("setSelected", 0, js_lv_roller_set_selected),
    JS_CFUNC_DEF("setVisibleRowCount", 0, js_lv_roller_set_visible_row_count),
    JS_CFUNC_DEF("selected", 0, js_lv_roller_get_selected),
    JS_CFUNC_DEF("selectedStr", 0, js_lv_roller_get_selected_str),
    JS_CFUNC_DEF("options", 0, js_lv_roller_get_options),
    JS_CFUNC_DEF("optionCnt", 0, js_lv_roller_get_option_cnt),
} ;
#define __def_js_lv_roller_proto_funcs__


static const JSCFunctionListEntry js_lv_roller_static_funcs[] = {
} ;
static JSValue js_lv_slider_set_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Slider.setValue() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Slider.setValue() must be called as a Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int32_t value ;
    if(JS_ToInt32(ctx, (int32_t *) &value, argv[0])!=0){
        THROW_EXCEPTION("arg value of method Slider.setValue() must be a number")
    }
    bool anim = true ;
    if(argc>1) {
        anim = JS_ToBool(ctx, argv[1]) ;
    }
    lv_slider_set_value(thisobj, value, anim) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_slider_set_left_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Slider.setLeftValue() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Slider.setLeftValue() must be called as a Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int32_t value ;
    if(JS_ToInt32(ctx, (int32_t *) &value, argv[0])!=0){
        THROW_EXCEPTION("arg value of method Slider.setLeftValue() must be a number")
    }
    bool anim = true ;
    if(argc>1) {
        anim = JS_ToBool(ctx, argv[1]) ;
    }
    lv_slider_set_left_value(thisobj, value, anim) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_slider_set_range(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Slider.setRange() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Slider.setRange() must be called as a Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int32_t min ;
    if(JS_ToInt32(ctx, (int32_t *) &min, argv[0])!=0){
        THROW_EXCEPTION("arg min of method Slider.setRange() must be a number")
    }
    int32_t max ;
    if(JS_ToInt32(ctx, (int32_t *) &max, argv[1])!=0){
        THROW_EXCEPTION("arg max of method Slider.setRange() must be a number")
    }
    lv_slider_set_range(thisobj, min, max) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_slider_set_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Slider.setMode() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Slider.setMode() must be called as a Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t mode ;
    if(JS_ToUint32(ctx, (uint32_t *) &mode, argv[0])!=0){
        THROW_EXCEPTION("arg mode of method Slider.setMode() must be a number")
    }
    lv_slider_set_mode(thisobj, mode) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_slider_get_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Slider.value() must be called as a Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_slider_get_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_slider_get_left_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Slider.leftValue() must be called as a Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_slider_get_left_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_slider_get_min_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Slider.minValue() must be called as a Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_slider_get_min_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_slider_get_max_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Slider.maxValue() must be called as a Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_slider_get_max_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_slider_is_dragged(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Slider.isDragged() must be called as a Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_slider_is_dragged(thisobj)) ;
    return retval ;
}

static JSValue js_lv_slider_get_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Slider.mode() must be called as a Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_slider_get_mode(thisobj)) ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_slider_proto_funcs[] = {
    JS_CFUNC_DEF("setValue", 0, js_lv_slider_set_value),
    JS_CFUNC_DEF("setLeftValue", 0, js_lv_slider_set_left_value),
    JS_CFUNC_DEF("setRange", 0, js_lv_slider_set_range),
    JS_CFUNC_DEF("setMode", 0, js_lv_slider_set_mode),
    JS_CFUNC_DEF("value", 0, js_lv_slider_get_value),
    JS_CFUNC_DEF("leftValue", 0, js_lv_slider_get_left_value),
    JS_CFUNC_DEF("minValue", 0, js_lv_slider_get_min_value),
    JS_CFUNC_DEF("maxValue", 0, js_lv_slider_get_max_value),
    JS_CFUNC_DEF("isDragged", 0, js_lv_slider_is_dragged),
    JS_CFUNC_DEF("mode", 0, js_lv_slider_get_mode),
} ;
#define __def_js_lv_slider_proto_funcs__


static const JSCFunctionListEntry js_lv_slider_static_funcs[] = {
} ;

static const JSCFunctionListEntry js_lv_switch_proto_funcs[] = {
} ;
#define __def_js_lv_switch_proto_funcs__


static const JSCFunctionListEntry js_lv_switch_static_funcs[] = {
} ;
static JSValue js_lv_table_set_cell_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("Table.setCellValue() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Table.setCellValue() must be called as a Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t row ;
    if(JS_ToUint32(ctx, (uint32_t *) &row, argv[0])!=0){
        THROW_EXCEPTION("arg row of method Table.setCellValue() must be a number")
    }
    uint16_t col ;
    if(JS_ToUint32(ctx, (uint32_t *) &col, argv[1])!=0){
        THROW_EXCEPTION("arg col of method Table.setCellValue() must be a number")
    }
    char * txt = (char *)JS_ToCString(ctx, argv[2]) ;
    lv_table_set_cell_value(thisobj, row, col, txt) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, txt) ;
    return retval ;
}

static JSValue js_lv_table_set_row_cnt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Table.setRowCnt() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Table.setRowCnt() must be called as a Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t row_cnt ;
    if(JS_ToUint32(ctx, (uint32_t *) &row_cnt, argv[0])!=0){
        THROW_EXCEPTION("arg row_cnt of method Table.setRowCnt() must be a number")
    }
    lv_table_set_row_cnt(thisobj, row_cnt) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_table_set_col_cnt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Table.setColCnt() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Table.setColCnt() must be called as a Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t col_cnt ;
    if(JS_ToUint32(ctx, (uint32_t *) &col_cnt, argv[0])!=0){
        THROW_EXCEPTION("arg col_cnt of method Table.setColCnt() must be a number")
    }
    lv_table_set_col_cnt(thisobj, col_cnt) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_table_set_col_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Table.setColWidth() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Table.setColWidth() must be called as a Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t col_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &col_id, argv[0])!=0){
        THROW_EXCEPTION("arg col_id of method Table.setColWidth() must be a number")
    }
    int16_t w ;
    if(JS_ToInt32(ctx, (int32_t *) &w, argv[1])!=0){
        THROW_EXCEPTION("arg w of method Table.setColWidth() must be a number")
    }
    lv_table_set_col_width(thisobj, col_id, w) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_table_add_cell_ctrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("Table.addCellCtrl() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Table.addCellCtrl() must be called as a Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t row ;
    if(JS_ToUint32(ctx, (uint32_t *) &row, argv[0])!=0){
        THROW_EXCEPTION("arg row of method Table.addCellCtrl() must be a number")
    }
    uint16_t col ;
    if(JS_ToUint32(ctx, (uint32_t *) &col, argv[1])!=0){
        THROW_EXCEPTION("arg col of method Table.addCellCtrl() must be a number")
    }
    uint8_t ctrl ;
    if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[2])!=0){
        THROW_EXCEPTION("arg ctrl of method Table.addCellCtrl() must be a number")
    }
    lv_table_add_cell_ctrl(thisobj, row, col, ctrl) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_table_clear_cell_ctrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("Table.clearCellCtrl() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Table.clearCellCtrl() must be called as a Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t row ;
    if(JS_ToUint32(ctx, (uint32_t *) &row, argv[0])!=0){
        THROW_EXCEPTION("arg row of method Table.clearCellCtrl() must be a number")
    }
    uint16_t col ;
    if(JS_ToUint32(ctx, (uint32_t *) &col, argv[1])!=0){
        THROW_EXCEPTION("arg col of method Table.clearCellCtrl() must be a number")
    }
    uint8_t ctrl ;
    if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[2])!=0){
        THROW_EXCEPTION("arg ctrl of method Table.clearCellCtrl() must be a number")
    }
    lv_table_clear_cell_ctrl(thisobj, row, col, ctrl) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_table_get_cell_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Table.cellValue() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Table.cellValue() must be called as a Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t row ;
    if(JS_ToUint32(ctx, (uint32_t *) &row, argv[0])!=0){
        THROW_EXCEPTION("arg row of method Table.cellValue() must be a number")
    }
    uint16_t col ;
    if(JS_ToUint32(ctx, (uint32_t *) &col, argv[1])!=0){
        THROW_EXCEPTION("arg col of method Table.cellValue() must be a number")
    }
    JSValue retval = JS_NewString(ctx,lv_table_get_cell_value(thisobj, row, col)) ;
    return retval ;
}

static JSValue js_lv_table_get_row_cnt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Table.rowCnt() must be called as a Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_table_get_row_cnt(thisobj)) ;
    return retval ;
}

static JSValue js_lv_table_get_col_cnt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Table.colCnt() must be called as a Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_table_get_col_cnt(thisobj)) ;
    return retval ;
}

static JSValue js_lv_table_get_col_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Table.colWidth() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Table.colWidth() must be called as a Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t col ;
    if(JS_ToUint32(ctx, (uint32_t *) &col, argv[0])!=0){
        THROW_EXCEPTION("arg col of method Table.colWidth() must be a number")
    }
    JSValue retval = JS_NewInt32(ctx,lv_table_get_col_width(thisobj, col)) ;
    return retval ;
}

static JSValue js_lv_table_has_cell_ctrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("Table.hasCellCtrl() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Table.hasCellCtrl() must be called as a Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t row ;
    if(JS_ToUint32(ctx, (uint32_t *) &row, argv[0])!=0){
        THROW_EXCEPTION("arg row of method Table.hasCellCtrl() must be a number")
    }
    uint16_t col ;
    if(JS_ToUint32(ctx, (uint32_t *) &col, argv[1])!=0){
        THROW_EXCEPTION("arg col of method Table.hasCellCtrl() must be a number")
    }
    uint8_t ctrl ;
    if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[2])!=0){
        THROW_EXCEPTION("arg ctrl of method Table.hasCellCtrl() must be a number")
    }
    JSValue retval = JS_NewBool(ctx,lv_table_has_cell_ctrl(thisobj, row, col, ctrl)) ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_table_proto_funcs[] = {
    JS_CFUNC_DEF("setCellValue", 0, js_lv_table_set_cell_value),
    JS_CFUNC_DEF("setRowCnt", 0, js_lv_table_set_row_cnt),
    JS_CFUNC_DEF("setColCnt", 0, js_lv_table_set_col_cnt),
    JS_CFUNC_DEF("setColWidth", 0, js_lv_table_set_col_width),
    JS_CFUNC_DEF("addCellCtrl", 0, js_lv_table_add_cell_ctrl),
    JS_CFUNC_DEF("clearCellCtrl", 0, js_lv_table_clear_cell_ctrl),
    JS_CFUNC_DEF("cellValue", 0, js_lv_table_get_cell_value),
    JS_CFUNC_DEF("rowCnt", 0, js_lv_table_get_row_cnt),
    JS_CFUNC_DEF("colCnt", 0, js_lv_table_get_col_cnt),
    JS_CFUNC_DEF("colWidth", 0, js_lv_table_get_col_width),
    JS_CFUNC_DEF("hasCellCtrl", 0, js_lv_table_has_cell_ctrl),
} ;
#define __def_js_lv_table_proto_funcs__


static const JSCFunctionListEntry js_lv_table_static_funcs[] = {
} ;
static JSValue js_lv_textarea_add_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("TextArea.addText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.addText() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * txt = (char *)JS_ToCString(ctx, argv[0]) ;
    lv_textarea_add_text(thisobj, txt) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, txt) ;
    return retval ;
}

static JSValue js_lv_textarea_del_char(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.delChar() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_textarea_del_char(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_del_char_forward(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.delCharForward() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_textarea_del_char_forward(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_set_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("TextArea.setText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.setText() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * txt = (char *)JS_ToCString(ctx, argv[0]) ;
    lv_textarea_set_text(thisobj, txt) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, txt) ;
    return retval ;
}

static JSValue js_lv_textarea_set_placeholder_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("TextArea.setPlaceholderText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.setPlaceholderText() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * txt = (char *)JS_ToCString(ctx, argv[0]) ;
    lv_textarea_set_placeholder_text(thisobj, txt) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, txt) ;
    return retval ;
}

static JSValue js_lv_textarea_set_cursor_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("TextArea.setCursorPos() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.setCursorPos() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int32_t pos ;
    if(JS_ToInt32(ctx, (int32_t *) &pos, argv[0])!=0){
        THROW_EXCEPTION("arg pos of method TextArea.setCursorPos() must be a number")
    }
    lv_textarea_set_cursor_pos(thisobj, pos) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_set_cursor_click_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("TextArea.setCursorClickPos() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.setCursorClickPos() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool en = JS_ToBool(ctx, argv[0]) ;
    lv_textarea_set_cursor_click_pos(thisobj, en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_set_password_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("TextArea.setPasswordMode() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.setPasswordMode() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool en = JS_ToBool(ctx, argv[0]) ;
    lv_textarea_set_password_mode(thisobj, en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_set_one_line(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("TextArea.setOneLine() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.setOneLine() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool en = JS_ToBool(ctx, argv[0]) ;
    lv_textarea_set_one_line(thisobj, en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_set_accepted_chars(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("TextArea.setAcceptedChars() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.setAcceptedChars() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * list = (char *)JS_ToCString(ctx, argv[0]) ;
    lv_textarea_set_accepted_chars(thisobj, list) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, list) ;
    return retval ;
}

static JSValue js_lv_textarea_set_max_length(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("TextArea.setMaxLength() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.setMaxLength() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t num ;
    if(JS_ToUint32(ctx, (uint32_t *) &num, argv[0])!=0){
        THROW_EXCEPTION("arg num of method TextArea.setMaxLength() must be a number")
    }
    lv_textarea_set_max_length(thisobj, num) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_set_insert_replace(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("TextArea.setInsertReplace() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.setInsertReplace() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * txt = (char *)JS_ToCString(ctx, argv[0]) ;
    lv_textarea_set_insert_replace(thisobj, txt) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, txt) ;
    return retval ;
}

static JSValue js_lv_textarea_set_text_selection(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("TextArea.setTextSelection() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.setTextSelection() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool en = JS_ToBool(ctx, argv[0]) ;
    lv_textarea_set_text_selection(thisobj, en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_set_password_show_time(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("TextArea.setPasswordShowTime() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.setPasswordShowTime() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t time ;
    if(JS_ToUint32(ctx, (uint32_t *) &time, argv[0])!=0){
        THROW_EXCEPTION("arg time of method TextArea.setPasswordShowTime() must be a number")
    }
    lv_textarea_set_password_show_time(thisobj, time) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_set_align(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("TextArea.setAlign() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.setAlign() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t align ;
    if(JS_ToUint32(ctx, (uint32_t *) &align, argv[0])!=0){
        THROW_EXCEPTION("arg align of method TextArea.setAlign() must be a number")
    }
    lv_textarea_set_align(thisobj, align) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_get_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.text() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_textarea_get_text(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_get_placeholder_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.placeholderText() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_textarea_get_placeholder_text(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_get_label(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.label() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_textarea_get_label(thisobj);
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, 0) ;
        JS_DupValue(ctx, retval) ;
    }
    return retval ;
}

static JSValue js_lv_textarea_get_cursor_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.cursorPos() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_textarea_get_cursor_pos(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_get_cursor_click_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.cursorClickPos() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_textarea_get_cursor_click_pos(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_get_password_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.passwordMode() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_textarea_get_password_mode(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_get_one_line(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.oneLine() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_textarea_get_one_line(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_get_accepted_chars(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.acceptedChars() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_textarea_get_accepted_chars(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_get_max_length(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.maxLength() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_textarea_get_max_length(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_text_is_selected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.textIsSelected() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_textarea_text_is_selected(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_get_text_selection(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.textSelection() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_textarea_get_text_selection(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_get_password_show_time(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.passwordShowTime() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_textarea_get_password_show_time(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_clear_selection(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.clearSelection() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_textarea_clear_selection(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_cursor_right(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.cursorRight() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_textarea_cursor_right(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_cursor_left(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.cursorLeft() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_textarea_cursor_left(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_cursor_down(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.cursorDown() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_textarea_cursor_down(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_cursor_up(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TextArea.cursorUp() must be called as a TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_textarea_cursor_up(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_textarea_proto_funcs[] = {
    JS_CFUNC_DEF("addText", 0, js_lv_textarea_add_text),
    JS_CFUNC_DEF("delChar", 0, js_lv_textarea_del_char),
    JS_CFUNC_DEF("delCharForward", 0, js_lv_textarea_del_char_forward),
    JS_CFUNC_DEF("setText", 0, js_lv_textarea_set_text),
    JS_CFUNC_DEF("setPlaceholderText", 0, js_lv_textarea_set_placeholder_text),
    JS_CFUNC_DEF("setCursorPos", 0, js_lv_textarea_set_cursor_pos),
    JS_CFUNC_DEF("setCursorClickPos", 0, js_lv_textarea_set_cursor_click_pos),
    JS_CFUNC_DEF("setPasswordMode", 0, js_lv_textarea_set_password_mode),
    JS_CFUNC_DEF("setOneLine", 0, js_lv_textarea_set_one_line),
    JS_CFUNC_DEF("setAcceptedChars", 0, js_lv_textarea_set_accepted_chars),
    JS_CFUNC_DEF("setMaxLength", 0, js_lv_textarea_set_max_length),
    JS_CFUNC_DEF("setInsertReplace", 0, js_lv_textarea_set_insert_replace),
    JS_CFUNC_DEF("setTextSelection", 0, js_lv_textarea_set_text_selection),
    JS_CFUNC_DEF("setPasswordShowTime", 0, js_lv_textarea_set_password_show_time),
    JS_CFUNC_DEF("setAlign", 0, js_lv_textarea_set_align),
    JS_CFUNC_DEF("text", 0, js_lv_textarea_get_text),
    JS_CFUNC_DEF("placeholderText", 0, js_lv_textarea_get_placeholder_text),
    JS_CFUNC_DEF("label", 0, js_lv_textarea_get_label),
    JS_CFUNC_DEF("cursorPos", 0, js_lv_textarea_get_cursor_pos),
    JS_CFUNC_DEF("cursorClickPos", 0, js_lv_textarea_get_cursor_click_pos),
    JS_CFUNC_DEF("passwordMode", 0, js_lv_textarea_get_password_mode),
    JS_CFUNC_DEF("oneLine", 0, js_lv_textarea_get_one_line),
    JS_CFUNC_DEF("acceptedChars", 0, js_lv_textarea_get_accepted_chars),
    JS_CFUNC_DEF("maxLength", 0, js_lv_textarea_get_max_length),
    JS_CFUNC_DEF("textIsSelected", 0, js_lv_textarea_text_is_selected),
    JS_CFUNC_DEF("textSelection", 0, js_lv_textarea_get_text_selection),
    JS_CFUNC_DEF("passwordShowTime", 0, js_lv_textarea_get_password_show_time),
    JS_CFUNC_DEF("clearSelection", 0, js_lv_textarea_clear_selection),
    JS_CFUNC_DEF("cursorRight", 0, js_lv_textarea_cursor_right),
    JS_CFUNC_DEF("cursorLeft", 0, js_lv_textarea_cursor_left),
    JS_CFUNC_DEF("cursorDown", 0, js_lv_textarea_cursor_down),
    JS_CFUNC_DEF("cursorUp", 0, js_lv_textarea_cursor_up),
} ;
#define __def_js_lv_textarea_proto_funcs__


static const JSCFunctionListEntry js_lv_textarea_static_funcs[] = {
} ;
static JSValue js_lv_msgbox_get_title(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("MsgBox.title() must be called as a MsgBox method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_msgbox_get_title(thisobj);
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, 0) ;
        JS_DupValue(ctx, retval) ;
    }
    return retval ;
}

static JSValue js_lv_msgbox_get_close_btn(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("MsgBox.closeBtn() must be called as a MsgBox method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_msgbox_get_close_btn(thisobj);
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, 0) ;
        JS_DupValue(ctx, retval) ;
    }
    return retval ;
}

static JSValue js_lv_msgbox_get_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("MsgBox.text() must be called as a MsgBox method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_msgbox_get_text(thisobj);
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, 0) ;
        JS_DupValue(ctx, retval) ;
    }
    return retval ;
}

static JSValue js_lv_msgbox_get_content(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("MsgBox.content() must be called as a MsgBox method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_msgbox_get_content(thisobj);
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, 0) ;
        JS_DupValue(ctx, retval) ;
    }
    return retval ;
}

static JSValue js_lv_msgbox_get_btns(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("MsgBox.btns() must be called as a MsgBox method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_msgbox_get_btns(thisobj);
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, 0) ;
        JS_DupValue(ctx, retval) ;
    }
    return retval ;
}

static JSValue js_lv_msgbox_get_active_btn(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("MsgBox.activeBtn() must be called as a MsgBox method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_msgbox_get_active_btn(thisobj)) ;
    return retval ;
}

static JSValue js_lv_msgbox_get_active_btn_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("MsgBox.activeBtnText() must be called as a MsgBox method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_msgbox_get_active_btn_text(thisobj)) ;
    return retval ;
}

static JSValue js_lv_msgbox_close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("MsgBox.close() must be called as a MsgBox method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_msgbox_close(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_msgbox_close_async(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("MsgBox.closeAsync() must be called as a MsgBox method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_msgbox_close_async(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_msgbox_proto_funcs[] = {
    JS_CFUNC_DEF("title", 0, js_lv_msgbox_get_title),
    JS_CFUNC_DEF("closeBtn", 0, js_lv_msgbox_get_close_btn),
    JS_CFUNC_DEF("text", 0, js_lv_msgbox_get_text),
    JS_CFUNC_DEF("content", 0, js_lv_msgbox_get_content),
    JS_CFUNC_DEF("btns", 0, js_lv_msgbox_get_btns),
    JS_CFUNC_DEF("activeBtn", 0, js_lv_msgbox_get_active_btn),
    JS_CFUNC_DEF("activeBtnText", 0, js_lv_msgbox_get_active_btn_text),
    JS_CFUNC_DEF("close", 0, js_lv_msgbox_close),
    JS_CFUNC_DEF("closeAsync", 0, js_lv_msgbox_close_async),
} ;
#define __def_js_lv_msgbox_proto_funcs__


static const JSCFunctionListEntry js_lv_msgbox_static_funcs[] = {
} ;
static JSValue js_lv_keyboard_set_textarea(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Keyboard.setTextarea() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Keyboard.setTextarea() must be called as a Keyboard method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_t * textarea = (lv_obj_t *)JS_GetOpaqueInternal(argv[0]) ;
    if( !textarea ){
        THROW_EXCEPTION("arg textarea of method Keyboard.setTextarea() must be a beapi.lvgl.Obj")
    }
    lv_keyboard_set_textarea(thisobj, textarea) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_keyboard_set_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Keyboard.setMode() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Keyboard.setMode() must be called as a Keyboard method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_keyboard_mode_t mode ;
    if(!lv_keyboard_mode_jsstr_to_const(ctx, argv[0], &mode)) {
        return JS_EXCEPTION ;
    }
    lv_keyboard_set_mode(thisobj, mode) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_keyboard_set_popovers(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Keyboard.setPopovers() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Keyboard.setPopovers() must be called as a Keyboard method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool en = JS_ToBool(ctx, argv[0]) ;
    lv_keyboard_set_popovers(thisobj, en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_keyboard_get_textarea(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Keyboard.textarea() must be called as a Keyboard method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_keyboard_get_textarea(thisobj);
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, 0) ;
        JS_DupValue(ctx, retval) ;
    }
    return retval ;
}

static JSValue js_lv_keyboard_get_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Keyboard.mode() must be called as a Keyboard method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_keyboard_get_mode(thisobj)) ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_keyboard_proto_funcs[] = {
    JS_CFUNC_DEF("setTextarea", 0, js_lv_keyboard_set_textarea),
    JS_CFUNC_DEF("setMode", 0, js_lv_keyboard_set_mode),
    JS_CFUNC_DEF("setPopovers", 0, js_lv_keyboard_set_popovers),
    JS_CFUNC_DEF("textarea", 0, js_lv_keyboard_get_textarea),
    JS_CFUNC_DEF("mode", 0, js_lv_keyboard_get_mode),
} ;
#define __def_js_lv_keyboard_proto_funcs__


static const JSCFunctionListEntry js_lv_keyboard_static_funcs[] = {
} ;
static JSValue js_lv_tileview_add_tile(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("TileView.addTile() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TileView.addTile() must be called as a TileView method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t col_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &col_id, argv[0])!=0){
        THROW_EXCEPTION("arg col_id of method TileView.addTile() must be a number")
    }
    uint8_t row_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &row_id, argv[1])!=0){
        THROW_EXCEPTION("arg row_id of method TileView.addTile() must be a number")
    }
    uint8_t dir ;
    if(JS_ToUint32(ctx, (uint32_t *) &dir, argv[2])!=0){
        THROW_EXCEPTION("arg dir of method TileView.addTile() must be a number")
    }
    JSValue retval = JS_NULL ;
    void * lvobj = lv_tileview_add_tile(thisobj, col_id, row_id, dir);
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, 0) ;
        JS_DupValue(ctx, retval) ;
    }
    return retval ;
}

static JSValue js_lv_tileview_get_tile_act(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TileView.tileAct() must be called as a TileView method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_tileview_get_tile_act(thisobj);
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, 0) ;
        JS_DupValue(ctx, retval) ;
    }
    return retval ;
}


static const JSCFunctionListEntry js_lv_tileview_proto_funcs[] = {
    JS_CFUNC_DEF("addTile", 0, js_lv_tileview_add_tile),
    JS_CFUNC_DEF("tileAct", 0, js_lv_tileview_get_tile_act),
} ;
#define __def_js_lv_tileview_proto_funcs__


static const JSCFunctionListEntry js_lv_tileview_static_funcs[] = {
} ;
static JSValue js_lv_tabview_add_tab(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("TabView.addTab() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TabView.addTab() must be called as a TabView method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * name = (char *)JS_ToCString(ctx, argv[0]) ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_tabview_add_tab(thisobj, name);
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, 0) ;
        JS_DupValue(ctx, retval) ;
    }
    JS_FreeCString(ctx, name) ;
    return retval ;
}

static JSValue js_lv_tabview_get_content(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TabView.content() must be called as a TabView method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_tabview_get_content(thisobj);
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, 0) ;
        JS_DupValue(ctx, retval) ;
    }
    return retval ;
}

static JSValue js_lv_tabview_get_tab_btns(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TabView.tabBtns() must be called as a TabView method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_tabview_get_tab_btns(thisobj);
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, 0) ;
        JS_DupValue(ctx, retval) ;
    }
    return retval ;
}

static JSValue js_lv_tabview_set_act(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("TabView.setAct() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TabView.setAct() must be called as a TabView method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t id ;
    if(JS_ToUint32(ctx, (uint32_t *) &id, argv[0])!=0){
        THROW_EXCEPTION("arg id of method TabView.setAct() must be a number")
    }
    bool anim_en = JS_ToBool(ctx, argv[1]) ;
    lv_tabview_set_act(thisobj, id, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_tabview_get_tab_act(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TabView.tabAct() must be called as a TabView method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_tabview_get_tab_act(thisobj)) ;
    return retval ;
}

static JSValue js_lv_tabview_set_anim(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("TabView.setAnim() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TabView.setAnim() must be called as a TabView method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool anim_en = JS_ToBool(ctx, argv[0]) ;
    lv_tabview_set_anim(thisobj, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_tabview_get_anim(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("TabView.anim() must be called as a TabView method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_tabview_get_anim(thisobj)) ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_tabview_proto_funcs[] = {
    JS_CFUNC_DEF("addTab", 0, js_lv_tabview_add_tab),
    JS_CFUNC_DEF("content", 0, js_lv_tabview_get_content),
    JS_CFUNC_DEF("tabBtns", 0, js_lv_tabview_get_tab_btns),
    JS_CFUNC_DEF("setAct", 0, js_lv_tabview_set_act),
    JS_CFUNC_DEF("tabAct", 0, js_lv_tabview_get_tab_act),
    JS_CFUNC_DEF("setAnim", 0, js_lv_tabview_set_anim),
    JS_CFUNC_DEF("anim", 0, js_lv_tabview_get_anim),
} ;
#define __def_js_lv_tabview_proto_funcs__


static const JSCFunctionListEntry js_lv_tabview_static_funcs[] = {
} ;

static const JSCFunctionListEntry js_lv_spinner_proto_funcs[] = {
} ;
#define __def_js_lv_spinner_proto_funcs__


static const JSCFunctionListEntry js_lv_spinner_static_funcs[] = {
} ;
static JSValue js_lv_list_add_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("List.addText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("List.addText() must be called as a List method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * txt = (char *)JS_ToCString(ctx, argv[0]) ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_list_add_text(thisobj, txt);
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, 0) ;
        JS_DupValue(ctx, retval) ;
    }
    JS_FreeCString(ctx, txt) ;
    return retval ;
}

static JSValue js_lv_list_get_btn_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("List.getBtnText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("List.getBtnText() must be called as a List method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_t * btn = (lv_obj_t *)JS_GetOpaqueInternal(argv[0]) ;
    if( !btn ){
        THROW_EXCEPTION("arg btn of method List.getBtnText() must be a beapi.lvgl.Obj")
    }
    JSValue retval = JS_NewString(ctx,lv_list_get_btn_text(thisobj, btn)) ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_list_proto_funcs[] = {
    JS_CFUNC_DEF("addBtn", 0, js_lv_list_add_btn),
    JS_CFUNC_DEF("addText", 0, js_lv_list_add_text),
    JS_CFUNC_DEF("getBtnText", 0, js_lv_list_get_btn_text),
} ;
#define __def_js_lv_list_proto_funcs__


static const JSCFunctionListEntry js_lv_list_static_funcs[] = {
} ;
static JSValue js_lv_group_del(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Group.del() must be called as a Group method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_group_del(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_group_set_default(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Group.setDefault() must be called as a Group method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_group_set_default(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_group_add_obj(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Group.addObj() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Group.addObj() must be called as a Group method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_t * obj = (lv_obj_t *)JS_GetOpaqueInternal(argv[0]) ;
    if( !obj ){
        THROW_EXCEPTION("arg obj of method Group.addObj() must be a beapi.lvgl.Obj")
    }
    lv_group_add_obj(thisobj, obj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_group_swap_obj(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("Group.swapObj() missing arg")
    }
    lv_obj_t * obj1 = (lv_obj_t *)JS_GetOpaqueInternal(argv[0]) ;
    if( !obj1 ){
        THROW_EXCEPTION("arg obj1 of method Group.swapObj() must be a beapi.lvgl.Obj")
    }
    lv_obj_t * obj2 = (lv_obj_t *)JS_GetOpaqueInternal(argv[1]) ;
    if( !obj2 ){
        THROW_EXCEPTION("arg obj2 of method Group.swapObj() must be a beapi.lvgl.Obj")
    }
    lv_group_swap_obj(obj1, obj2) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_group_remove_obj(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Group.removeObj() missing arg")
    }
    lv_obj_t * obj = (lv_obj_t *)JS_GetOpaqueInternal(argv[0]) ;
    if( !obj ){
        THROW_EXCEPTION("arg obj of method Group.removeObj() must be a beapi.lvgl.Obj")
    }
    lv_group_remove_obj(obj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_group_remove_all_objs(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Group.removeAllObjs() must be called as a Group method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_group_remove_all_objs(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_group_focus_obj(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Group.focusObj() missing arg")
    }
    lv_obj_t * obj = (lv_obj_t *)JS_GetOpaqueInternal(argv[0]) ;
    if( !obj ){
        THROW_EXCEPTION("arg obj of method Group.focusObj() must be a beapi.lvgl.Obj")
    }
    lv_group_focus_obj(obj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_group_focus_next(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Group.focusNext() must be called as a Group method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_group_focus_next(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_group_focus_prev(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Group.focusPrev() must be called as a Group method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_group_focus_prev(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_group_focus_freeze(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Group.focusFreeze() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Group.focusFreeze() must be called as a Group method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool en = JS_ToBool(ctx, argv[0]) ;
    lv_group_focus_freeze(thisobj, en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_group_send_data(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Group.sendData() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Group.sendData() must be called as a Group method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t c ;
    if(JS_ToUint32(ctx, (uint32_t *) &c, argv[0])!=0){
        THROW_EXCEPTION("arg c of method Group.sendData() must be a number")
    }
    JSValue retval = JS_NewBool(ctx,lv_group_send_data(thisobj, c)) ;
    return retval ;
}

static JSValue js_lv_group_set_refocus_policy(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Group.setRefocusPolicy() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Group.setRefocusPolicy() must be called as a Group method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_group_refocus_policy_t policy ;
    if(!lv_group_refocus_policy_jsstr_to_const(ctx, argv[0], &policy)) {
        return JS_EXCEPTION ;
    }
    lv_group_set_refocus_policy(thisobj, policy) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_group_set_editing(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Group.setEditing() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Group.setEditing() must be called as a Group method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool edit = JS_ToBool(ctx, argv[0]) ;
    lv_group_set_editing(thisobj, edit) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_group_set_wrap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("Group.setWrap() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Group.setWrap() must be called as a Group method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool en = JS_ToBool(ctx, argv[0]) ;
    lv_group_set_wrap(thisobj, en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_group_get_focused(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Group.focused() must be called as a Group method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_group_get_focused(thisobj);
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, 0) ;
        JS_DupValue(ctx, retval) ;
    }
    return retval ;
}

static JSValue js_lv_group_get_editing(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Group.editing() must be called as a Group method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_group_get_editing(thisobj)) ;
    return retval ;
}

static JSValue js_lv_group_get_wrap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Group.wrap() must be called as a Group method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_group_get_wrap(thisobj)) ;
    return retval ;
}

static JSValue js_lv_group_get_obj_count(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("Group.objCount() must be called as a Group method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_group_get_obj_count(thisobj)) ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_group_proto_funcs[] = {
    JS_CFUNC_DEF("allObjs", 0, js_lv_group_all_objs),
    JS_CFUNC_DEF("del", 0, js_lv_group_del),
    JS_CFUNC_DEF("setDefault", 0, js_lv_group_set_default),
    JS_CFUNC_DEF("addObj", 0, js_lv_group_add_obj),
    JS_CFUNC_DEF("removeAllObjs", 0, js_lv_group_remove_all_objs),
    JS_CFUNC_DEF("focusNext", 0, js_lv_group_focus_next),
    JS_CFUNC_DEF("focusPrev", 0, js_lv_group_focus_prev),
    JS_CFUNC_DEF("focusFreeze", 0, js_lv_group_focus_freeze),
    JS_CFUNC_DEF("sendData", 0, js_lv_group_send_data),
    JS_CFUNC_DEF("setRefocusPolicy", 0, js_lv_group_set_refocus_policy),
    JS_CFUNC_DEF("setEditing", 0, js_lv_group_set_editing),
    JS_CFUNC_DEF("setWrap", 0, js_lv_group_set_wrap),
    JS_CFUNC_DEF("focused", 0, js_lv_group_get_focused),
    JS_CFUNC_DEF("editing", 0, js_lv_group_get_editing),
    JS_CFUNC_DEF("wrap", 0, js_lv_group_get_wrap),
    JS_CFUNC_DEF("objCount", 0, js_lv_group_get_obj_count),
} ;
#define __def_js_lv_group_proto_funcs__


static const JSCFunctionListEntry js_lv_group_static_funcs[] = {
    JS_CFUNC_DEF("swapObj", 0, js_lv_group_swap_obj),
    JS_CFUNC_DEF("removeObj", 0, js_lv_group_remove_obj),
    JS_CFUNC_DEF("focusObj", 0, js_lv_group_focus_obj),
} ;
// AUTO GENERATE CODE END [METHOD LIST] --------




// AUTO GENERATE CODE START [DEF PROTOS] --------
// JSValue proto_lv_obj ;
// JSValue proto_lv_label ;
// JSValue proto_lv_arc ;
// JSValue proto_lv_bar ;
// JSValue proto_lv_btn ;
// JSValue proto_lv_btnmatrix ;
// JSValue proto_lv_canvas ;
// JSValue proto_lv_checkbox ;
// JSValue proto_lv_dropdown ;
// JSValue proto_lv_img ;
// JSValue proto_lv_line ;
// JSValue proto_lv_roller ;
// JSValue proto_lv_slider ;
// JSValue proto_lv_switch ;
// JSValue proto_lv_table ;
// JSValue proto_lv_textarea ;
// JSValue proto_lv_msgbox ;
// JSValue proto_lv_keyboard ;
// JSValue proto_lv_tileview ;
// JSValue proto_lv_list ;
// AUTO GENERATE CODE END [DEF PROTOS] --------

JSClassID be_lv_type_to_js_class(lv_obj_t * obj) {
    
// AUTO GENERATE CODE START [TYPE TO PROTO] --------
    if(lv_obj_check_type(obj, & lv_obj_class)) {
        return js_lv_obj_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_label_class)) {
        return js_lv_label_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_arc_class)) {
        return js_lv_arc_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_bar_class)) {
        return js_lv_bar_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_btn_class)) {
        return js_lv_btn_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_btnmatrix_class)) {
        return js_lv_btnmatrix_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_canvas_class)) {
        return js_lv_canvas_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_checkbox_class)) {
        return js_lv_checkbox_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_dropdown_class)) {
        return js_lv_dropdown_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_img_class)) {
        return js_lv_img_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_line_class)) {
        return js_lv_line_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_roller_class)) {
        return js_lv_roller_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_slider_class)) {
        return js_lv_slider_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_switch_class)) {
        return js_lv_switch_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_table_class)) {
        return js_lv_table_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_textarea_class)) {
        return js_lv_textarea_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_msgbox_class)) {
        return js_lv_msgbox_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_keyboard_class)) {
        return js_lv_keyboard_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_tileview_class)) {
        return js_lv_tileview_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_tabview_class)) {
        return js_lv_tabview_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_spinner_class)) {
        return js_lv_spinner_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_list_class)) {
        return js_lv_list_class_id ;
    }
// AUTO GENERATE CODE END [TYPE TO PROTO] --------


    else if(lv_obj_check_type(obj, & lv_list_text_class)) {
        return js_lv_label_class_id ;
    }
    else if(lv_obj_check_type(obj, & lv_list_btn_class)) {
        return js_lv_btn_class_id ;
    }

    printf("unknow lv_obj_t class\n") ;
    return js_lv_obj_class_id ;
}



void be_lv_widgets_gen_require(JSContext *ctx, JSValue lvgl) {

    JSValue EventEmitterProto = js_get_glob_prop(ctx, 3, "beapi", "EventEmitter", "prototype") ;

// AUTO GENERATE CODE START [REGISTER CLASS] --------
    // define js class lvgl.lv_obj    
    JSValue proto_lv_obj = qjs_def_class2(
                ctx, "Obj", js_lv_obj_class_id, &js_lv_obj_class, "Obj"
                , js_lv_obj_constructor
                , js_lv_obj_proto_funcs, countof(js_lv_obj_proto_funcs)
                , js_lv_obj_static_funcs, countof(js_lv_obj_static_funcs)
                , EventEmitterProto, lvgl) ;

    // define js class lvgl.lv_label    
    JSValue proto_lv_label = qjs_def_class2(
                ctx, "Label", js_lv_label_class_id, &js_lv_label_class, "Label"
                , js_lv_label_constructor
                , js_lv_label_proto_funcs, countof(js_lv_label_proto_funcs)
                , js_lv_label_static_funcs, countof(js_lv_label_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_arc    
    JSValue proto_lv_arc = qjs_def_class2(
                ctx, "Arc", js_lv_arc_class_id, &js_lv_arc_class, "Arc"
                , js_lv_arc_constructor
                , js_lv_arc_proto_funcs, countof(js_lv_arc_proto_funcs)
                , js_lv_arc_static_funcs, countof(js_lv_arc_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_bar    
    JSValue proto_lv_bar = qjs_def_class2(
                ctx, "Bar", js_lv_bar_class_id, &js_lv_bar_class, "Bar"
                , js_lv_bar_constructor
                , js_lv_bar_proto_funcs, countof(js_lv_bar_proto_funcs)
                , js_lv_bar_static_funcs, countof(js_lv_bar_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_btn    
    JSValue proto_lv_btn = qjs_def_class2(
                ctx, "Btn", js_lv_btn_class_id, &js_lv_btn_class, "Btn"
                , js_lv_btn_constructor
                , js_lv_btn_proto_funcs, countof(js_lv_btn_proto_funcs)
                , js_lv_btn_static_funcs, countof(js_lv_btn_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_btnmatrix    
    JSValue proto_lv_btnmatrix = qjs_def_class2(
                ctx, "BtnMatrix", js_lv_btnmatrix_class_id, &js_lv_btnmatrix_class, "BtnMatrix"
                , js_lv_btnmatrix_constructor
                , js_lv_btnmatrix_proto_funcs, countof(js_lv_btnmatrix_proto_funcs)
                , js_lv_btnmatrix_static_funcs, countof(js_lv_btnmatrix_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_canvas    
    JSValue proto_lv_canvas = qjs_def_class2(
                ctx, "Canvas", js_lv_canvas_class_id, &js_lv_canvas_class, "Canvas"
                , js_lv_canvas_constructor
                , js_lv_canvas_proto_funcs, countof(js_lv_canvas_proto_funcs)
                , js_lv_canvas_static_funcs, countof(js_lv_canvas_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_checkbox    
    JSValue proto_lv_checkbox = qjs_def_class2(
                ctx, "Checkbox", js_lv_checkbox_class_id, &js_lv_checkbox_class, "Checkbox"
                , js_lv_checkbox_constructor
                , js_lv_checkbox_proto_funcs, countof(js_lv_checkbox_proto_funcs)
                , js_lv_checkbox_static_funcs, countof(js_lv_checkbox_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_dropdown    
    JSValue proto_lv_dropdown = qjs_def_class2(
                ctx, "Dropdown", js_lv_dropdown_class_id, &js_lv_dropdown_class, "Dropdown"
                , js_lv_dropdown_constructor
                , js_lv_dropdown_proto_funcs, countof(js_lv_dropdown_proto_funcs)
                , js_lv_dropdown_static_funcs, countof(js_lv_dropdown_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_img    
    JSValue proto_lv_img = qjs_def_class2(
                ctx, "Img", js_lv_img_class_id, &js_lv_img_class, "Img"
                , js_lv_img_constructor
                , js_lv_img_proto_funcs, countof(js_lv_img_proto_funcs)
                , js_lv_img_static_funcs, countof(js_lv_img_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_line    
    JSValue proto_lv_line = qjs_def_class2(
                ctx, "Line", js_lv_line_class_id, &js_lv_line_class, "Line"
                , js_lv_line_constructor
                , js_lv_line_proto_funcs, countof(js_lv_line_proto_funcs)
                , js_lv_line_static_funcs, countof(js_lv_line_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_roller    
    JSValue proto_lv_roller = qjs_def_class2(
                ctx, "Roller", js_lv_roller_class_id, &js_lv_roller_class, "Roller"
                , js_lv_roller_constructor
                , js_lv_roller_proto_funcs, countof(js_lv_roller_proto_funcs)
                , js_lv_roller_static_funcs, countof(js_lv_roller_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_slider    
    JSValue proto_lv_slider = qjs_def_class2(
                ctx, "Slider", js_lv_slider_class_id, &js_lv_slider_class, "Slider"
                , js_lv_slider_constructor
                , js_lv_slider_proto_funcs, countof(js_lv_slider_proto_funcs)
                , js_lv_slider_static_funcs, countof(js_lv_slider_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_switch    
    JSValue proto_lv_switch = qjs_def_class2(
                ctx, "Switch", js_lv_switch_class_id, &js_lv_switch_class, "Switch"
                , js_lv_switch_constructor
                , js_lv_switch_proto_funcs, countof(js_lv_switch_proto_funcs)
                , js_lv_switch_static_funcs, countof(js_lv_switch_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_table    
    JSValue proto_lv_table = qjs_def_class2(
                ctx, "Table", js_lv_table_class_id, &js_lv_table_class, "Table"
                , js_lv_table_constructor
                , js_lv_table_proto_funcs, countof(js_lv_table_proto_funcs)
                , js_lv_table_static_funcs, countof(js_lv_table_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_textarea    
    JSValue proto_lv_textarea = qjs_def_class2(
                ctx, "TextArea", js_lv_textarea_class_id, &js_lv_textarea_class, "TextArea"
                , js_lv_textarea_constructor
                , js_lv_textarea_proto_funcs, countof(js_lv_textarea_proto_funcs)
                , js_lv_textarea_static_funcs, countof(js_lv_textarea_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_msgbox    
    JSValue proto_lv_msgbox = qjs_def_class2(
                ctx, "MsgBox", js_lv_msgbox_class_id, &js_lv_msgbox_class, "MsgBox"
                , js_lv_msgbox_constructor
                , js_lv_msgbox_proto_funcs, countof(js_lv_msgbox_proto_funcs)
                , js_lv_msgbox_static_funcs, countof(js_lv_msgbox_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_keyboard    
    JSValue proto_lv_keyboard = qjs_def_class2(
                ctx, "Keyboard", js_lv_keyboard_class_id, &js_lv_keyboard_class, "Keyboard"
                , js_lv_keyboard_constructor
                , js_lv_keyboard_proto_funcs, countof(js_lv_keyboard_proto_funcs)
                , js_lv_keyboard_static_funcs, countof(js_lv_keyboard_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_tileview    
    JSValue proto_lv_tileview = qjs_def_class2(
                ctx, "TileView", js_lv_tileview_class_id, &js_lv_tileview_class, "TileView"
                , js_lv_tileview_constructor
                , js_lv_tileview_proto_funcs, countof(js_lv_tileview_proto_funcs)
                , js_lv_tileview_static_funcs, countof(js_lv_tileview_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_tabview    
    JSValue proto_lv_tabview = qjs_def_class2(
                ctx, "TabView", js_lv_tabview_class_id, &js_lv_tabview_class, "TabView"
                , js_lv_tabview_constructor
                , js_lv_tabview_proto_funcs, countof(js_lv_tabview_proto_funcs)
                , js_lv_tabview_static_funcs, countof(js_lv_tabview_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_spinner    
    JSValue proto_lv_spinner = qjs_def_class2(
                ctx, "Spinner", js_lv_spinner_class_id, &js_lv_spinner_class, "Spinner"
                , js_lv_spinner_constructor
                , js_lv_spinner_proto_funcs, countof(js_lv_spinner_proto_funcs)
                , js_lv_spinner_static_funcs, countof(js_lv_spinner_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_list    
    JSValue proto_lv_list = qjs_def_class2(
                ctx, "List", js_lv_list_class_id, &js_lv_list_class, "List"
                , js_lv_list_constructor
                , js_lv_list_proto_funcs, countof(js_lv_list_proto_funcs)
                , js_lv_list_static_funcs, countof(js_lv_list_static_funcs)
                , proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_group    
    JSValue proto_lv_group = qjs_def_class2(
                ctx, "Group", js_lv_group_class_id, &js_lv_group_class, "Group"
                , js_lv_group_constructor
                , js_lv_group_proto_funcs, countof(js_lv_group_proto_funcs)
                , js_lv_group_static_funcs, countof(js_lv_group_static_funcs)
                , EventEmitterProto, lvgl) ;

// AUTO GENERATE CODE END [REGISTER CLASS] --------

    JSValue ret = JS_Eval(ctx, (const char *)lv_obj_init_js, lv_obj_init_js_len, "[native]lv_obj_init", JS_EVAL_TYPE_GLOBAL|JS_EVAL_FLAG_STRIP) ;
    JS_FreeValue(ctx, ret);



    JS_FreeValue(ctx, EventEmitterProto);
}



void be_lv_widgets_gen_init() {
    // class id 全局, 分配一次
// AUTO GENERATE CODE START [REGISTER CLASS ID] --------
    JS_NewClassID(&js_lv_obj_class_id);
    JS_NewClassID(&js_lv_label_class_id);
    JS_NewClassID(&js_lv_arc_class_id);
    JS_NewClassID(&js_lv_bar_class_id);
    JS_NewClassID(&js_lv_btn_class_id);
    JS_NewClassID(&js_lv_btnmatrix_class_id);
    JS_NewClassID(&js_lv_canvas_class_id);
    JS_NewClassID(&js_lv_checkbox_class_id);
    JS_NewClassID(&js_lv_dropdown_class_id);
    JS_NewClassID(&js_lv_img_class_id);
    JS_NewClassID(&js_lv_line_class_id);
    JS_NewClassID(&js_lv_roller_class_id);
    JS_NewClassID(&js_lv_slider_class_id);
    JS_NewClassID(&js_lv_switch_class_id);
    JS_NewClassID(&js_lv_table_class_id);
    JS_NewClassID(&js_lv_textarea_class_id);
    JS_NewClassID(&js_lv_msgbox_class_id);
    JS_NewClassID(&js_lv_keyboard_class_id);
    JS_NewClassID(&js_lv_tileview_class_id);
    JS_NewClassID(&js_lv_tabview_class_id);
    JS_NewClassID(&js_lv_spinner_class_id);
    JS_NewClassID(&js_lv_list_class_id);
    JS_NewClassID(&js_lv_group_class_id);
// AUTO GENERATE CODE END [REGISTER CLASS ID] --------
}

JSClassID lv_obj_js_class_id() {
    return js_lv_obj_class_id ;
}
JSClassID lv_label_js_class_id() {
    return js_lv_label_class_id ;
}
JSClassID lv_btn_js_class_id() {
    return js_lv_btn_class_id ;
}
JSClassID lv_tabview_js_class_id() {
    return js_lv_tabview_class_id ;
}
JSClassID lv_spinner_js_class_id() {
    return js_lv_spinner_class_id ;
}
JSClassID lv_group_js_class_id() {
    return js_lv_group_class_id ;
}