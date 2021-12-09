#include "widgets.h"
#include "widgets_extra.h"

#include "utils.h"
#include "cutils.h"
#include "lvgl.h"
#include "lv_conf.h"


unsigned char lv_obj_init_js[] = {
  0x62, 0x65, 0x61, 0x70, 0x69, 0x2e, 0x6c, 0x76, 0x67, 0x6c, 0x2e, 0x5f,
  0x5f, 0x6c, 0x76, 0x5f, 0x6f, 0x62, 0x6a, 0x5f, 0x69, 0x6e, 0x69, 0x74,
  0x20, 0x3d, 0x20, 0x66, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f, 0x6e, 0x20,
  0x28, 0x29, 0x20, 0x7b, 0x0d, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x74, 0x68,
  0x69, 0x73, 0x2e, 0x5f, 0x68, 0x61, 0x6e, 0x64, 0x6c, 0x65, 0x73, 0x3d,
  0x7b, 0x7d, 0x0d, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x74, 0x68, 0x69, 0x73,
  0x2e, 0x6f, 0x6e, 0x28, 0x22, 0x23, 0x45, 0x56, 0x45, 0x4e, 0x54, 0x2e,
  0x41, 0x44, 0x44, 0x23, 0x22, 0x2c, 0x28, 0x65, 0x76, 0x65, 0x6e, 0x74,
  0x4e, 0x61, 0x6d, 0x65, 0x29, 0x3d, 0x3e, 0x7b, 0x0d, 0x0a, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x74, 0x68, 0x69, 0x73, 0x2e, 0x65,
  0x6e, 0x61, 0x62, 0x6c, 0x65, 0x45, 0x76, 0x65, 0x6e, 0x74, 0x28, 0x65,
  0x76, 0x65, 0x6e, 0x74, 0x4e, 0x61, 0x6d, 0x65, 0x29, 0x0d, 0x0a, 0x20,
  0x20, 0x20, 0x20, 0x7d, 0x29, 0x0d, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x74,
  0x68, 0x69, 0x73, 0x2e, 0x6f, 0x6e, 0x28, 0x22, 0x23, 0x45, 0x56, 0x45,
  0x4e, 0x54, 0x2e, 0x43, 0x4c, 0x45, 0x41, 0x52, 0x23, 0x22, 0x2c, 0x28,
  0x65, 0x76, 0x65, 0x6e, 0x74, 0x4e, 0x61, 0x6d, 0x65, 0x29, 0x3d, 0x3e,
  0x7b, 0x0d, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x74,
  0x68, 0x69, 0x73, 0x2e, 0x64, 0x69, 0x73, 0x61, 0x62, 0x6c, 0x65, 0x45,
  0x76, 0x65, 0x6e, 0x74, 0x28, 0x65, 0x76, 0x65, 0x6e, 0x74, 0x4e, 0x61,
  0x6d, 0x65, 0x29, 0x0d, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x7d, 0x29, 0x0d,
  0x0a, 0x7d
};
unsigned int lv_obj_init_js_len = 242;


// AUTO GENERATE CODE START [CONST MAPPING] --------

bool lv_flex_flow_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_flex_flow_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    if(strcmp(cstr,"row")==0) {
        (*out) = LV_FLEX_FLOW_ROW ;
    }
    else if(strcmp(cstr,"column")==0) {
        (*out) = LV_FLEX_FLOW_COLUMN ;
    }
    else if(strcmp(cstr,"row-wrap")==0) {
        (*out) = LV_FLEX_FLOW_ROW_WRAP ;
    }
    else if(strcmp(cstr,"row-reverse")==0) {
        (*out) = LV_FLEX_FLOW_ROW_REVERSE ;
    }
    else if(strcmp(cstr,"row-wrap-reverse")==0) {
        (*out) = LV_FLEX_FLOW_ROW_WRAP_REVERSE ;
    }
    else if(strcmp(cstr,"column-wrap")==0) {
        (*out) = LV_FLEX_FLOW_COLUMN_WRAP ;
    }
    else if(strcmp(cstr,"column-reverse")==0) {
        (*out) = LV_FLEX_FLOW_COLUMN_REVERSE ;
    }
    else if(strcmp(cstr,"column-wrap-reverse")==0) {
        (*out) = LV_FLEX_FLOW_COLUMN_WRAP_REVERSE ;
    }

    else {
        JS_ThrowReferenceError(ctx, "unkonw lv_flex_flow pass in: %s", cstr) ;
        JS_FreeCString(ctx, cstr) ;
        return false ;
    }
    JS_FreeCString(ctx, cstr) ;
    return true ;
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
    }
    return "unkonw";
}
JSValue lv_flex_flow_const_to_jsstr(JSContext *ctx, lv_flex_flow_t code) {
    return JS_NewString(ctx, lv_flex_flow_const_to_str(code));
}

bool lv_flex_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_flex_align_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    if(strcmp(cstr,"start")==0) {
        (*out) = LV_FLEX_ALIGN_START ;
    }
    else if(strcmp(cstr,"end")==0) {
        (*out) = LV_FLEX_ALIGN_END ;
    }
    else if(strcmp(cstr,"center")==0) {
        (*out) = LV_FLEX_ALIGN_CENTER ;
    }
    else if(strcmp(cstr,"space-evenly")==0) {
        (*out) = LV_FLEX_ALIGN_SPACE_EVENLY ;
    }
    else if(strcmp(cstr,"space-around")==0) {
        (*out) = LV_FLEX_ALIGN_SPACE_AROUND ;
    }
    else if(strcmp(cstr,"space-between")==0) {
        (*out) = LV_FLEX_ALIGN_SPACE_BETWEEN ;
    }

    else {
        JS_ThrowReferenceError(ctx, "unkonw lv_flex_align pass in: %s", cstr) ;
        JS_FreeCString(ctx, cstr) ;
        return false ;
    }
    JS_FreeCString(ctx, cstr) ;
    return true ;
}
const char * lv_flex_align_const_to_str(lv_flex_align_t code) {
    switch(code) {
        case LV_FLEX_ALIGN_START: return "start";
        case LV_FLEX_ALIGN_END: return "end";
        case LV_FLEX_ALIGN_CENTER: return "center";
        case LV_FLEX_ALIGN_SPACE_EVENLY: return "space-evenly";
        case LV_FLEX_ALIGN_SPACE_AROUND: return "space-around";
        case LV_FLEX_ALIGN_SPACE_BETWEEN: return "space-between";
    }
    return "unkonw";
}
JSValue lv_flex_align_const_to_jsstr(JSContext *ctx, lv_flex_align_t code) {
    return JS_NewString(ctx, lv_flex_align_const_to_str(code));
}

bool lv_event_code_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_event_code_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    if(strcmp(cstr,"all")==0) {
        (*out) = LV_EVENT_ALL ;
    }
    else if(strcmp(cstr,"pressed")==0) {
        (*out) = LV_EVENT_PRESSED ;
    }
    else if(strcmp(cstr,"pressing")==0) {
        (*out) = LV_EVENT_PRESSING ;
    }
    else if(strcmp(cstr,"press-lost")==0) {
        (*out) = LV_EVENT_PRESS_LOST ;
    }
    else if(strcmp(cstr,"short-clicked")==0) {
        (*out) = LV_EVENT_SHORT_CLICKED ;
    }
    else if(strcmp(cstr,"long-pressed")==0) {
        (*out) = LV_EVENT_LONG_PRESSED ;
    }
    else if(strcmp(cstr,"long-pressed-repeat")==0) {
        (*out) = LV_EVENT_LONG_PRESSED_REPEAT ;
    }
    else if(strcmp(cstr,"clicked")==0) {
        (*out) = LV_EVENT_CLICKED ;
    }
    else if(strcmp(cstr,"released")==0) {
        (*out) = LV_EVENT_RELEASED ;
    }
    else if(strcmp(cstr,"scroll-begin")==0) {
        (*out) = LV_EVENT_SCROLL_BEGIN ;
    }
    else if(strcmp(cstr,"scroll-end")==0) {
        (*out) = LV_EVENT_SCROLL_END ;
    }
    else if(strcmp(cstr,"scroll")==0) {
        (*out) = LV_EVENT_SCROLL ;
    }
    else if(strcmp(cstr,"gesture")==0) {
        (*out) = LV_EVENT_GESTURE ;
    }
    else if(strcmp(cstr,"key")==0) {
        (*out) = LV_EVENT_KEY ;
    }
    else if(strcmp(cstr,"focused")==0) {
        (*out) = LV_EVENT_FOCUSED ;
    }
    else if(strcmp(cstr,"defocused")==0) {
        (*out) = LV_EVENT_DEFOCUSED ;
    }
    else if(strcmp(cstr,"leave")==0) {
        (*out) = LV_EVENT_LEAVE ;
    }
    else if(strcmp(cstr,"hit-test")==0) {
        (*out) = LV_EVENT_HIT_TEST ;
    }
    else if(strcmp(cstr,"cover-check")==0) {
        (*out) = LV_EVENT_COVER_CHECK ;
    }
    else if(strcmp(cstr,"refr-ext-draw-size")==0) {
        (*out) = LV_EVENT_REFR_EXT_DRAW_SIZE ;
    }
    else if(strcmp(cstr,"draw-main-begin")==0) {
        (*out) = LV_EVENT_DRAW_MAIN_BEGIN ;
    }
    else if(strcmp(cstr,"draw-main")==0) {
        (*out) = LV_EVENT_DRAW_MAIN ;
    }
    else if(strcmp(cstr,"draw-main-end")==0) {
        (*out) = LV_EVENT_DRAW_MAIN_END ;
    }
    else if(strcmp(cstr,"draw-post-begin")==0) {
        (*out) = LV_EVENT_DRAW_POST_BEGIN ;
    }
    else if(strcmp(cstr,"draw-post")==0) {
        (*out) = LV_EVENT_DRAW_POST ;
    }
    else if(strcmp(cstr,"draw-post-end")==0) {
        (*out) = LV_EVENT_DRAW_POST_END ;
    }
    else if(strcmp(cstr,"draw-part-begin")==0) {
        (*out) = LV_EVENT_DRAW_PART_BEGIN ;
    }
    else if(strcmp(cstr,"draw-part-end")==0) {
        (*out) = LV_EVENT_DRAW_PART_END ;
    }
    else if(strcmp(cstr,"value-changed")==0) {
        (*out) = LV_EVENT_VALUE_CHANGED ;
    }
    else if(strcmp(cstr,"insert")==0) {
        (*out) = LV_EVENT_INSERT ;
    }
    else if(strcmp(cstr,"refresh")==0) {
        (*out) = LV_EVENT_REFRESH ;
    }
    else if(strcmp(cstr,"ready")==0) {
        (*out) = LV_EVENT_READY ;
    }
    else if(strcmp(cstr,"cancel")==0) {
        (*out) = LV_EVENT_CANCEL ;
    }
    else if(strcmp(cstr,"delete")==0) {
        (*out) = LV_EVENT_DELETE ;
    }
    else if(strcmp(cstr,"child-changed")==0) {
        (*out) = LV_EVENT_CHILD_CHANGED ;
    }
    else if(strcmp(cstr,"child-created")==0) {
        (*out) = LV_EVENT_CHILD_CREATED ;
    }
    else if(strcmp(cstr,"child-deleted")==0) {
        (*out) = LV_EVENT_CHILD_DELETED ;
    }
    else if(strcmp(cstr,"screen-unload-start")==0) {
        (*out) = LV_EVENT_SCREEN_UNLOAD_START ;
    }
    else if(strcmp(cstr,"screen-load-start")==0) {
        (*out) = LV_EVENT_SCREEN_LOAD_START ;
    }
    else if(strcmp(cstr,"screen-loaded")==0) {
        (*out) = LV_EVENT_SCREEN_LOADED ;
    }
    else if(strcmp(cstr,"screen-unloaded")==0) {
        (*out) = LV_EVENT_SCREEN_UNLOADED ;
    }
    else if(strcmp(cstr,"size-changed")==0) {
        (*out) = LV_EVENT_SIZE_CHANGED ;
    }
    else if(strcmp(cstr,"style-changed")==0) {
        (*out) = LV_EVENT_STYLE_CHANGED ;
    }
    else if(strcmp(cstr,"layout-changed")==0) {
        (*out) = LV_EVENT_LAYOUT_CHANGED ;
    }
    else if(strcmp(cstr,"get-self-size")==0) {
        (*out) = LV_EVENT_GET_SELF_SIZE ;
    }

    else {
        JS_ThrowReferenceError(ctx, "unkonw lv_event_code pass in: %s", cstr) ;
        JS_FreeCString(ctx, cstr) ;
        return false ;
    }
    JS_FreeCString(ctx, cstr) ;
    return true ;
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
    }
    return "unkonw";
}
JSValue lv_event_code_const_to_jsstr(JSContext *ctx, lv_event_code_t code) {
    return JS_NewString(ctx, lv_event_code_const_to_str(code));
}

bool lv_dir_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_dir_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    if(strcmp(cstr,"none")==0) {
        (*out) = LV_DIR_NONE ;
    }
    else if(strcmp(cstr,"left")==0) {
        (*out) = LV_DIR_LEFT ;
    }
    else if(strcmp(cstr,"right")==0) {
        (*out) = LV_DIR_RIGHT ;
    }
    else if(strcmp(cstr,"top")==0) {
        (*out) = LV_DIR_TOP ;
    }
    else if(strcmp(cstr,"bottom")==0) {
        (*out) = LV_DIR_BOTTOM ;
    }
    else if(strcmp(cstr,"hor")==0) {
        (*out) = LV_DIR_HOR ;
    }
    else if(strcmp(cstr,"ver")==0) {
        (*out) = LV_DIR_VER ;
    }
    else if(strcmp(cstr,"all")==0) {
        (*out) = LV_DIR_ALL ;
    }

    else {
        JS_ThrowReferenceError(ctx, "unkonw lv_dir pass in: %s", cstr) ;
        JS_FreeCString(ctx, cstr) ;
        return false ;
    }
    JS_FreeCString(ctx, cstr) ;
    return true ;
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
    }
    return "unkonw";
}
JSValue lv_dir_const_to_jsstr(JSContext *ctx, lv_dir_t code) {
    return JS_NewString(ctx, lv_dir_const_to_str(code));
}

bool lv_align_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_align_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    if(strcmp(cstr,"default")==0) {
        (*out) = LV_ALIGN_DEFAULT ;
    }
    else if(strcmp(cstr,"top-left")==0) {
        (*out) = LV_ALIGN_TOP_LEFT ;
    }
    else if(strcmp(cstr,"top-mid")==0) {
        (*out) = LV_ALIGN_TOP_MID ;
    }
    else if(strcmp(cstr,"top-right")==0) {
        (*out) = LV_ALIGN_TOP_RIGHT ;
    }
    else if(strcmp(cstr,"bottom-left")==0) {
        (*out) = LV_ALIGN_BOTTOM_LEFT ;
    }
    else if(strcmp(cstr,"bottom-mid")==0) {
        (*out) = LV_ALIGN_BOTTOM_MID ;
    }
    else if(strcmp(cstr,"bottom-right")==0) {
        (*out) = LV_ALIGN_BOTTOM_RIGHT ;
    }
    else if(strcmp(cstr,"left-mid")==0) {
        (*out) = LV_ALIGN_LEFT_MID ;
    }
    else if(strcmp(cstr,"right-mid")==0) {
        (*out) = LV_ALIGN_RIGHT_MID ;
    }
    else if(strcmp(cstr,"center")==0) {
        (*out) = LV_ALIGN_CENTER ;
    }
    else if(strcmp(cstr,"out-top-left")==0) {
        (*out) = LV_ALIGN_OUT_TOP_LEFT ;
    }
    else if(strcmp(cstr,"out-top-mid")==0) {
        (*out) = LV_ALIGN_OUT_TOP_MID ;
    }
    else if(strcmp(cstr,"out-top-right")==0) {
        (*out) = LV_ALIGN_OUT_TOP_RIGHT ;
    }
    else if(strcmp(cstr,"out-bottom-left")==0) {
        (*out) = LV_ALIGN_OUT_BOTTOM_LEFT ;
    }
    else if(strcmp(cstr,"out-bottom-mid")==0) {
        (*out) = LV_ALIGN_OUT_BOTTOM_MID ;
    }
    else if(strcmp(cstr,"out-bottom-right")==0) {
        (*out) = LV_ALIGN_OUT_BOTTOM_RIGHT ;
    }
    else if(strcmp(cstr,"out-left-top")==0) {
        (*out) = LV_ALIGN_OUT_LEFT_TOP ;
    }
    else if(strcmp(cstr,"out-left-mid")==0) {
        (*out) = LV_ALIGN_OUT_LEFT_MID ;
    }
    else if(strcmp(cstr,"out-left-bottom")==0) {
        (*out) = LV_ALIGN_OUT_LEFT_BOTTOM ;
    }
    else if(strcmp(cstr,"out-right-top")==0) {
        (*out) = LV_ALIGN_OUT_RIGHT_TOP ;
    }
    else if(strcmp(cstr,"out-right-mid")==0) {
        (*out) = LV_ALIGN_OUT_RIGHT_MID ;
    }
    else if(strcmp(cstr,"out-right-bottom")==0) {
        (*out) = LV_ALIGN_OUT_RIGHT_BOTTOM ;
    }

    else {
        JS_ThrowReferenceError(ctx, "unkonw lv_align pass in: %s", cstr) ;
        JS_FreeCString(ctx, cstr) ;
        return false ;
    }
    JS_FreeCString(ctx, cstr) ;
    return true ;
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
    }
    return "unkonw";
}
JSValue lv_align_const_to_jsstr(JSContext *ctx, lv_align_t code) {
    return JS_NewString(ctx, lv_align_const_to_str(code));
}

bool lv_border_side_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_border_side_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    if(strcmp(cstr,"none")==0) {
        (*out) = LV_BORDER_SIDE_NONE ;
    }
    else if(strcmp(cstr,"bottom")==0) {
        (*out) = LV_BORDER_SIDE_BOTTOM ;
    }
    else if(strcmp(cstr,"top")==0) {
        (*out) = LV_BORDER_SIDE_TOP ;
    }
    else if(strcmp(cstr,"left")==0) {
        (*out) = LV_BORDER_SIDE_LEFT ;
    }
    else if(strcmp(cstr,"right")==0) {
        (*out) = LV_BORDER_SIDE_RIGHT ;
    }
    else if(strcmp(cstr,"full")==0) {
        (*out) = LV_BORDER_SIDE_FULL ;
    }
    else if(strcmp(cstr,"internal")==0) {
        (*out) = LV_BORDER_SIDE_INTERNAL ;
    }

    else {
        JS_ThrowReferenceError(ctx, "unkonw lv_border_side pass in: %s", cstr) ;
        JS_FreeCString(ctx, cstr) ;
        return false ;
    }
    JS_FreeCString(ctx, cstr) ;
    return true ;
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
    }
    return "unkonw";
}
JSValue lv_border_side_const_to_jsstr(JSContext *ctx, lv_border_side_t code) {
    return JS_NewString(ctx, lv_border_side_const_to_str(code));
}

bool lv_base_dir_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_base_dir_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    if(strcmp(cstr,"ltr")==0) {
        (*out) = LV_BASE_DIR_LTR ;
    }
    else if(strcmp(cstr,"rtl")==0) {
        (*out) = LV_BASE_DIR_RTL ;
    }
    else if(strcmp(cstr,"auto")==0) {
        (*out) = LV_BASE_DIR_AUTO ;
    }
    else if(strcmp(cstr,"neutral")==0) {
        (*out) = LV_BASE_DIR_NEUTRAL ;
    }
    else if(strcmp(cstr,"weak")==0) {
        (*out) = LV_BASE_DIR_WEAK ;
    }

    else {
        JS_ThrowReferenceError(ctx, "unkonw lv_base_dir pass in: %s", cstr) ;
        JS_FreeCString(ctx, cstr) ;
        return false ;
    }
    JS_FreeCString(ctx, cstr) ;
    return true ;
}
const char * lv_base_dir_const_to_str(lv_base_dir_t code) {
    switch(code) {
        case LV_BASE_DIR_LTR: return "ltr";
        case LV_BASE_DIR_RTL: return "rtl";
        case LV_BASE_DIR_AUTO: return "auto";
        case LV_BASE_DIR_NEUTRAL: return "neutral";
        case LV_BASE_DIR_WEAK: return "weak";
    }
    return "unkonw";
}
JSValue lv_base_dir_const_to_jsstr(JSContext *ctx, lv_base_dir_t code) {
    return JS_NewString(ctx, lv_base_dir_const_to_str(code));
}

bool lv_style_prop_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_style_prop_t* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    if(strcmp(cstr,"prop-inv")==0) {
        (*out) = LV_STYLE_PROP_INV ;
    }
    else if(strcmp(cstr,"width")==0) {
        (*out) = LV_STYLE_WIDTH ;
    }
    else if(strcmp(cstr,"min-width")==0) {
        (*out) = LV_STYLE_MIN_WIDTH ;
    }
    else if(strcmp(cstr,"max-width")==0) {
        (*out) = LV_STYLE_MAX_WIDTH ;
    }
    else if(strcmp(cstr,"height")==0) {
        (*out) = LV_STYLE_HEIGHT ;
    }
    else if(strcmp(cstr,"min-height")==0) {
        (*out) = LV_STYLE_MIN_HEIGHT ;
    }
    else if(strcmp(cstr,"max-height")==0) {
        (*out) = LV_STYLE_MAX_HEIGHT ;
    }
    else if(strcmp(cstr,"x")==0) {
        (*out) = LV_STYLE_X ;
    }
    else if(strcmp(cstr,"y")==0) {
        (*out) = LV_STYLE_Y ;
    }
    else if(strcmp(cstr,"align")==0) {
        (*out) = LV_STYLE_ALIGN ;
    }
    else if(strcmp(cstr,"transform-width")==0) {
        (*out) = LV_STYLE_TRANSFORM_WIDTH ;
    }
    else if(strcmp(cstr,"transform-height")==0) {
        (*out) = LV_STYLE_TRANSFORM_HEIGHT ;
    }
    else if(strcmp(cstr,"translate-x")==0) {
        (*out) = LV_STYLE_TRANSLATE_X ;
    }
    else if(strcmp(cstr,"translate-y")==0) {
        (*out) = LV_STYLE_TRANSLATE_Y ;
    }
    else if(strcmp(cstr,"transform-zoom")==0) {
        (*out) = LV_STYLE_TRANSFORM_ZOOM ;
    }
    else if(strcmp(cstr,"transform-angle")==0) {
        (*out) = LV_STYLE_TRANSFORM_ANGLE ;
    }
    else if(strcmp(cstr,"pad-top")==0) {
        (*out) = LV_STYLE_PAD_TOP ;
    }
    else if(strcmp(cstr,"pad-bottom")==0) {
        (*out) = LV_STYLE_PAD_BOTTOM ;
    }
    else if(strcmp(cstr,"pad-left")==0) {
        (*out) = LV_STYLE_PAD_LEFT ;
    }
    else if(strcmp(cstr,"pad-right")==0) {
        (*out) = LV_STYLE_PAD_RIGHT ;
    }
    else if(strcmp(cstr,"pad-row")==0) {
        (*out) = LV_STYLE_PAD_ROW ;
    }
    else if(strcmp(cstr,"pad-column")==0) {
        (*out) = LV_STYLE_PAD_COLUMN ;
    }
    else if(strcmp(cstr,"bg-color")==0) {
        (*out) = LV_STYLE_BG_COLOR ;
    }
    else if(strcmp(cstr,"bg-color-filtered")==0) {
        (*out) = LV_STYLE_BG_COLOR_FILTERED ;
    }
    else if(strcmp(cstr,"bg-opa")==0) {
        (*out) = LV_STYLE_BG_OPA ;
    }
    else if(strcmp(cstr,"bg-grad-color")==0) {
        (*out) = LV_STYLE_BG_GRAD_COLOR ;
    }
    else if(strcmp(cstr,"bg-grad-color-filtered")==0) {
        (*out) = LV_STYLE_BG_GRAD_COLOR_FILTERED ;
    }
    else if(strcmp(cstr,"bg-grad-dir")==0) {
        (*out) = LV_STYLE_BG_GRAD_DIR ;
    }
    else if(strcmp(cstr,"bg-main-stop")==0) {
        (*out) = LV_STYLE_BG_MAIN_STOP ;
    }
    else if(strcmp(cstr,"bg-grad-stop")==0) {
        (*out) = LV_STYLE_BG_GRAD_STOP ;
    }
    else if(strcmp(cstr,"bg-img-src")==0) {
        (*out) = LV_STYLE_BG_IMG_SRC ;
    }
    else if(strcmp(cstr,"bg-img-opa")==0) {
        (*out) = LV_STYLE_BG_IMG_OPA ;
    }
    else if(strcmp(cstr,"bg-img-recolor")==0) {
        (*out) = LV_STYLE_BG_IMG_RECOLOR ;
    }
    else if(strcmp(cstr,"bg-img-recolor-filtered")==0) {
        (*out) = LV_STYLE_BG_IMG_RECOLOR_FILTERED ;
    }
    else if(strcmp(cstr,"bg-img-recolor-opa")==0) {
        (*out) = LV_STYLE_BG_IMG_RECOLOR_OPA ;
    }
    else if(strcmp(cstr,"bg-img-tiled")==0) {
        (*out) = LV_STYLE_BG_IMG_TILED ;
    }
    else if(strcmp(cstr,"border-color")==0) {
        (*out) = LV_STYLE_BORDER_COLOR ;
    }
    else if(strcmp(cstr,"border-color-filtered")==0) {
        (*out) = LV_STYLE_BORDER_COLOR_FILTERED ;
    }
    else if(strcmp(cstr,"border-opa")==0) {
        (*out) = LV_STYLE_BORDER_OPA ;
    }
    else if(strcmp(cstr,"border-width")==0) {
        (*out) = LV_STYLE_BORDER_WIDTH ;
    }
    else if(strcmp(cstr,"border-side")==0) {
        (*out) = LV_STYLE_BORDER_SIDE ;
    }
    else if(strcmp(cstr,"border-post")==0) {
        (*out) = LV_STYLE_BORDER_POST ;
    }
    else if(strcmp(cstr,"outline-width")==0) {
        (*out) = LV_STYLE_OUTLINE_WIDTH ;
    }
    else if(strcmp(cstr,"outline-color")==0) {
        (*out) = LV_STYLE_OUTLINE_COLOR ;
    }
    else if(strcmp(cstr,"outline-color-filtered")==0) {
        (*out) = LV_STYLE_OUTLINE_COLOR_FILTERED ;
    }
    else if(strcmp(cstr,"outline-opa")==0) {
        (*out) = LV_STYLE_OUTLINE_OPA ;
    }
    else if(strcmp(cstr,"outline-pad")==0) {
        (*out) = LV_STYLE_OUTLINE_PAD ;
    }
    else if(strcmp(cstr,"shadow-width")==0) {
        (*out) = LV_STYLE_SHADOW_WIDTH ;
    }
    else if(strcmp(cstr,"shadow-ofs-x")==0) {
        (*out) = LV_STYLE_SHADOW_OFS_X ;
    }
    else if(strcmp(cstr,"shadow-ofs-y")==0) {
        (*out) = LV_STYLE_SHADOW_OFS_Y ;
    }
    else if(strcmp(cstr,"shadow-spread")==0) {
        (*out) = LV_STYLE_SHADOW_SPREAD ;
    }
    else if(strcmp(cstr,"shadow-color")==0) {
        (*out) = LV_STYLE_SHADOW_COLOR ;
    }
    else if(strcmp(cstr,"shadow-color-filtered")==0) {
        (*out) = LV_STYLE_SHADOW_COLOR_FILTERED ;
    }
    else if(strcmp(cstr,"shadow-opa")==0) {
        (*out) = LV_STYLE_SHADOW_OPA ;
    }
    else if(strcmp(cstr,"img-opa")==0) {
        (*out) = LV_STYLE_IMG_OPA ;
    }
    else if(strcmp(cstr,"img-recolor")==0) {
        (*out) = LV_STYLE_IMG_RECOLOR ;
    }
    else if(strcmp(cstr,"img-recolor-filtered")==0) {
        (*out) = LV_STYLE_IMG_RECOLOR_FILTERED ;
    }
    else if(strcmp(cstr,"img-recolor-opa")==0) {
        (*out) = LV_STYLE_IMG_RECOLOR_OPA ;
    }
    else if(strcmp(cstr,"line-width")==0) {
        (*out) = LV_STYLE_LINE_WIDTH ;
    }
    else if(strcmp(cstr,"line-dash-width")==0) {
        (*out) = LV_STYLE_LINE_DASH_WIDTH ;
    }
    else if(strcmp(cstr,"line-dash-gap")==0) {
        (*out) = LV_STYLE_LINE_DASH_GAP ;
    }
    else if(strcmp(cstr,"line-rounded")==0) {
        (*out) = LV_STYLE_LINE_ROUNDED ;
    }
    else if(strcmp(cstr,"line-color")==0) {
        (*out) = LV_STYLE_LINE_COLOR ;
    }
    else if(strcmp(cstr,"line-color-filtered")==0) {
        (*out) = LV_STYLE_LINE_COLOR_FILTERED ;
    }
    else if(strcmp(cstr,"line-opa")==0) {
        (*out) = LV_STYLE_LINE_OPA ;
    }
    else if(strcmp(cstr,"arc-width")==0) {
        (*out) = LV_STYLE_ARC_WIDTH ;
    }
    else if(strcmp(cstr,"arc-rounded")==0) {
        (*out) = LV_STYLE_ARC_ROUNDED ;
    }
    else if(strcmp(cstr,"arc-color")==0) {
        (*out) = LV_STYLE_ARC_COLOR ;
    }
    else if(strcmp(cstr,"arc-color-filtered")==0) {
        (*out) = LV_STYLE_ARC_COLOR_FILTERED ;
    }
    else if(strcmp(cstr,"arc-opa")==0) {
        (*out) = LV_STYLE_ARC_OPA ;
    }
    else if(strcmp(cstr,"arc-img-src")==0) {
        (*out) = LV_STYLE_ARC_IMG_SRC ;
    }
    else if(strcmp(cstr,"text-color")==0) {
        (*out) = LV_STYLE_TEXT_COLOR ;
    }
    else if(strcmp(cstr,"text-color-filtered")==0) {
        (*out) = LV_STYLE_TEXT_COLOR_FILTERED ;
    }
    else if(strcmp(cstr,"text-opa")==0) {
        (*out) = LV_STYLE_TEXT_OPA ;
    }
    else if(strcmp(cstr,"text-font")==0) {
        (*out) = LV_STYLE_TEXT_FONT ;
    }
    else if(strcmp(cstr,"text-letter-space")==0) {
        (*out) = LV_STYLE_TEXT_LETTER_SPACE ;
    }
    else if(strcmp(cstr,"text-line-space")==0) {
        (*out) = LV_STYLE_TEXT_LINE_SPACE ;
    }
    else if(strcmp(cstr,"text-decor")==0) {
        (*out) = LV_STYLE_TEXT_DECOR ;
    }
    else if(strcmp(cstr,"text-align")==0) {
        (*out) = LV_STYLE_TEXT_ALIGN ;
    }
    else if(strcmp(cstr,"radius")==0) {
        (*out) = LV_STYLE_RADIUS ;
    }
    else if(strcmp(cstr,"clip-corner")==0) {
        (*out) = LV_STYLE_CLIP_CORNER ;
    }
    else if(strcmp(cstr,"opa")==0) {
        (*out) = LV_STYLE_OPA ;
    }
    else if(strcmp(cstr,"color-filter-dsc")==0) {
        (*out) = LV_STYLE_COLOR_FILTER_DSC ;
    }
    else if(strcmp(cstr,"color-filter-opa")==0) {
        (*out) = LV_STYLE_COLOR_FILTER_OPA ;
    }
    else if(strcmp(cstr,"anim-time")==0) {
        (*out) = LV_STYLE_ANIM_TIME ;
    }
    else if(strcmp(cstr,"anim-speed")==0) {
        (*out) = LV_STYLE_ANIM_SPEED ;
    }
    else if(strcmp(cstr,"transition")==0) {
        (*out) = LV_STYLE_TRANSITION ;
    }
    else if(strcmp(cstr,"blend-mode")==0) {
        (*out) = LV_STYLE_BLEND_MODE ;
    }
    else if(strcmp(cstr,"layout")==0) {
        (*out) = LV_STYLE_LAYOUT ;
    }
    else if(strcmp(cstr,"base-dir")==0) {
        (*out) = LV_STYLE_BASE_DIR ;
    }
    else if(strcmp(cstr,"-last-built-in-prop")==0) {
        (*out) = _LV_STYLE_LAST_BUILT_IN_PROP ;
    }
    else if(strcmp(cstr,"prop-any")==0) {
        (*out) = LV_STYLE_PROP_ANY ;
    }

    else {
        JS_ThrowReferenceError(ctx, "unkonw lv_style_prop pass in: %s", cstr) ;
        JS_FreeCString(ctx, cstr) ;
        return false ;
    }
    JS_FreeCString(ctx, cstr) ;
    return true ;
}
const char * lv_style_prop_const_to_str(lv_style_prop_t code) {
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
        case LV_STYLE_PROP_ANY: return "prop-any";
    }
    return "unkonw";
}
JSValue lv_style_prop_const_to_jsstr(JSContext *ctx, lv_style_prop_t code) {
    return JS_NewString(ctx, lv_style_prop_const_to_str(code));
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
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, js_lv_obj_class_id) ;
    return jsobj ;
}
static void js_lv_obj_finalizer(JSRuntime *rt, JSValue val){
    printf("js_lv_obj_finalizer()\n") ;
    lv_obj_t * thisobj = JS_GetOpaque(val, js_lv_obj_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_obj_class = {
    "lvgl.Obj",
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
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, js_lv_label_class_id) ;
    return jsobj ;
}
static void js_lv_label_finalizer(JSRuntime *rt, JSValue val){
    printf("js_lv_label_finalizer()\n") ;
    lv_label_t * thisobj = JS_GetOpaque(val, js_lv_label_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_label_class = {
    "lvgl.Label",
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
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, js_lv_arc_class_id) ;
    return jsobj ;
}
static void js_lv_arc_finalizer(JSRuntime *rt, JSValue val){
    printf("js_lv_arc_finalizer()\n") ;
    lv_arc_t * thisobj = JS_GetOpaque(val, js_lv_arc_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_arc_class = {
    "lvgl.Arc",
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
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, js_lv_bar_class_id) ;
    return jsobj ;
}
static void js_lv_bar_finalizer(JSRuntime *rt, JSValue val){
    printf("js_lv_bar_finalizer()\n") ;
    lv_bar_t * thisobj = JS_GetOpaque(val, js_lv_bar_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_bar_class = {
    "lvgl.Bar",
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
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, js_lv_btn_class_id) ;
    return jsobj ;
}
static void js_lv_btn_finalizer(JSRuntime *rt, JSValue val){
    printf("js_lv_btn_finalizer()\n") ;
    lv_btn_t * thisobj = JS_GetOpaque(val, js_lv_btn_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_btn_class = {
    "lvgl.Btn",
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
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, js_lv_btnmatrix_class_id) ;
    return jsobj ;
}
static void js_lv_btnmatrix_finalizer(JSRuntime *rt, JSValue val){
    printf("js_lv_btnmatrix_finalizer()\n") ;
    lv_btnmatrix_t * thisobj = JS_GetOpaque(val, js_lv_btnmatrix_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_btnmatrix_class = {
    "lvgl.BtnMatrix",
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
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, js_lv_canvas_class_id) ;
    return jsobj ;
}
static void js_lv_canvas_finalizer(JSRuntime *rt, JSValue val){
    printf("js_lv_canvas_finalizer()\n") ;
    lv_canvas_t * thisobj = JS_GetOpaque(val, js_lv_canvas_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_canvas_class = {
    "lvgl.Canvas",
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
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, js_lv_checkbox_class_id) ;
    return jsobj ;
}
static void js_lv_checkbox_finalizer(JSRuntime *rt, JSValue val){
    printf("js_lv_checkbox_finalizer()\n") ;
    lv_checkbox_t * thisobj = JS_GetOpaque(val, js_lv_checkbox_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_checkbox_class = {
    "lvgl.Checkbox",
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
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, js_lv_dropdown_class_id) ;
    return jsobj ;
}
static void js_lv_dropdown_finalizer(JSRuntime *rt, JSValue val){
    printf("js_lv_dropdown_finalizer()\n") ;
    lv_dropdown_t * thisobj = JS_GetOpaque(val, js_lv_dropdown_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_dropdown_class = {
    "lvgl.Dropdown",
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
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, js_lv_img_class_id) ;
    return jsobj ;
}
static void js_lv_img_finalizer(JSRuntime *rt, JSValue val){
    printf("js_lv_img_finalizer()\n") ;
    lv_img_t * thisobj = JS_GetOpaque(val, js_lv_img_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_img_class = {
    "lvgl.Img",
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
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, js_lv_line_class_id) ;
    return jsobj ;
}
static void js_lv_line_finalizer(JSRuntime *rt, JSValue val){
    printf("js_lv_line_finalizer()\n") ;
    lv_line_t * thisobj = JS_GetOpaque(val, js_lv_line_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_line_class = {
    "lvgl.Line",
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
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, js_lv_roller_class_id) ;
    return jsobj ;
}
static void js_lv_roller_finalizer(JSRuntime *rt, JSValue val){
    printf("js_lv_roller_finalizer()\n") ;
    lv_roller_t * thisobj = JS_GetOpaque(val, js_lv_roller_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_roller_class = {
    "lvgl.Roller",
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
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, js_lv_slider_class_id) ;
    return jsobj ;
}
static void js_lv_slider_finalizer(JSRuntime *rt, JSValue val){
    printf("js_lv_slider_finalizer()\n") ;
    lv_slider_t * thisobj = JS_GetOpaque(val, js_lv_slider_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_slider_class = {
    "lvgl.Slider",
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
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, js_lv_switch_class_id) ;
    return jsobj ;
}
static void js_lv_switch_finalizer(JSRuntime *rt, JSValue val){
    printf("js_lv_switch_finalizer()\n") ;
    lv_switch_t * thisobj = JS_GetOpaque(val, js_lv_switch_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_switch_class = {
    "lvgl.Switch",
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
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, js_lv_table_class_id) ;
    return jsobj ;
}
static void js_lv_table_finalizer(JSRuntime *rt, JSValue val){
    printf("js_lv_table_finalizer()\n") ;
    lv_table_t * thisobj = JS_GetOpaque(val, js_lv_table_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_table_class = {
    "lvgl.Table",
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
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, js_lv_textarea_class_id) ;
    return jsobj ;
}
static void js_lv_textarea_finalizer(JSRuntime *rt, JSValue val){
    printf("js_lv_textarea_finalizer()\n") ;
    lv_textarea_t * thisobj = JS_GetOpaque(val, js_lv_textarea_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_lv_textarea_class = {
    "lvgl.TextArea",
    .finalizer = js_lv_textarea_finalizer,
};

// AUTO GENERATE CODE END [DEFINE CLASS] --------




// AUTO GENERATE CODE START [METHOD LIST] --------
static JSValue js_lv_obj_add_flag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.addFlag() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.addFlag() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t f ;
    if(JS_ToUint32(ctx, (uint32_t *) &f, argv[0])!=0){
        THROW_EXCEPTION("arg f of method lvgl.Obj.addFlag() must be a number")
    }
    lv_obj_add_flag(thisobj, f) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_clear_flag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.clearFlag() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.clearFlag() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t f ;
    if(JS_ToUint32(ctx, (uint32_t *) &f, argv[0])!=0){
        THROW_EXCEPTION("arg f of method lvgl.Obj.clearFlag() must be a number")
    }
    lv_obj_clear_flag(thisobj, f) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_add_state(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.addState() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.addState() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t state ;
    if(JS_ToUint32(ctx, (uint32_t *) &state, argv[0])!=0){
        THROW_EXCEPTION("arg state of method lvgl.Obj.addState() must be a number")
    }
    lv_obj_add_state(thisobj, state) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_clear_state(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.clearState() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.clearState() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t state ;
    if(JS_ToUint32(ctx, (uint32_t *) &state, argv[0])!=0){
        THROW_EXCEPTION("arg state of method lvgl.Obj.clearState() must be a number")
    }
    lv_obj_clear_state(thisobj, state) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_has_flag(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.hasFlag() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.hasFlag() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t f ;
    if(JS_ToUint32(ctx, (uint32_t *) &f, argv[0])!=0){
        THROW_EXCEPTION("arg f of method lvgl.Obj.hasFlag() must be a number")
    }
    JSValue retval = JS_NewBool(ctx,lv_obj_has_flag(thisobj, f)) ;
    return retval ;
}

static JSValue js_lv_obj_has_flag_any(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.hasFlagAny() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.hasFlagAny() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t f ;
    if(JS_ToUint32(ctx, (uint32_t *) &f, argv[0])!=0){
        THROW_EXCEPTION("arg f of method lvgl.Obj.hasFlagAny() must be a number")
    }
    JSValue retval = JS_NewBool(ctx,lv_obj_has_flag_any(thisobj, f)) ;
    return retval ;
}

static JSValue js_lv_obj_get_state(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.state() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_obj_get_state(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_has_state(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.hasState() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.hasState() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t state ;
    if(JS_ToUint32(ctx, (uint32_t *) &state, argv[0])!=0){
        THROW_EXCEPTION("arg state of method lvgl.Obj.hasState() must be a number")
    }
    JSValue retval = JS_NewBool(ctx,lv_obj_has_state(thisobj, state)) ;
    return retval ;
}

static JSValue js_lv_obj_allocate_spec_attr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.allocateSpecAttr() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_allocate_spec_attr(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_is_valid(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.isValid() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_obj_is_valid(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_dpx(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.dpx() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.dpx() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t n ;
    if(JS_ToInt32(ctx, (int32_t *) &n, argv[0])!=0){
        THROW_EXCEPTION("arg n of method lvgl.Obj.dpx() must be a number")
    }
    JSValue retval = JS_NewInt32(ctx,lv_obj_dpx(thisobj, n)) ;
    return retval ;
}

static JSValue js_lv_obj_is_editable(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.isEditable() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_obj_is_editable(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_set_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Obj.setPos() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setPos() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method lvgl.Obj.setPos() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method lvgl.Obj.setPos() must be a number")
    }
    lv_obj_set_pos(thisobj, x, y) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setX() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setX() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method lvgl.Obj.setX() must be a number")
    }
    lv_obj_set_x(thisobj, x) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setY() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setY() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[0])!=0){
        THROW_EXCEPTION("arg y of method lvgl.Obj.setY() must be a number")
    }
    lv_obj_set_y(thisobj, y) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_size(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Obj.setSize() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setSize() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t w ;
    if(JS_ToInt32(ctx, (int32_t *) &w, argv[0])!=0){
        THROW_EXCEPTION("arg w of method lvgl.Obj.setSize() must be a number")
    }
    int16_t h ;
    if(JS_ToInt32(ctx, (int32_t *) &h, argv[1])!=0){
        THROW_EXCEPTION("arg h of method lvgl.Obj.setSize() must be a number")
    }
    lv_obj_set_size(thisobj, w, h) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_refr_size(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.refrSize() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_obj_refr_size(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_set_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setWidth() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setWidth() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t w ;
    if(JS_ToInt32(ctx, (int32_t *) &w, argv[0])!=0){
        THROW_EXCEPTION("arg w of method lvgl.Obj.setWidth() must be a number")
    }
    lv_obj_set_width(thisobj, w) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setHeight() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setHeight() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t h ;
    if(JS_ToInt32(ctx, (int32_t *) &h, argv[0])!=0){
        THROW_EXCEPTION("arg h of method lvgl.Obj.setHeight() must be a number")
    }
    lv_obj_set_height(thisobj, h) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_content_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setContentWidth() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setContentWidth() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t w ;
    if(JS_ToInt32(ctx, (int32_t *) &w, argv[0])!=0){
        THROW_EXCEPTION("arg w of method lvgl.Obj.setContentWidth() must be a number")
    }
    lv_obj_set_content_width(thisobj, w) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_content_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setContentHeight() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setContentHeight() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t h ;
    if(JS_ToInt32(ctx, (int32_t *) &h, argv[0])!=0){
        THROW_EXCEPTION("arg h of method lvgl.Obj.setContentHeight() must be a number")
    }
    lv_obj_set_content_height(thisobj, h) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_layout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setLayout() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setLayout() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t layout ;
    if(JS_ToUint32(ctx, (uint32_t *) &layout, argv[0])!=0){
        THROW_EXCEPTION("arg layout of method lvgl.Obj.setLayout() must be a number")
    }
    lv_obj_set_layout(thisobj, layout) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_is_layout_positioned(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.isLayoutPositioned() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_obj_is_layout_positioned(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_mark_layout_as_dirty(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.markLayoutAsDirty() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_mark_layout_as_dirty(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_update_layout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.updateLayout() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_update_layout(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_align(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setAlign() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setAlign() must be called as a lvgl.Obj method")
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
        THROW_EXCEPTION("lvgl.Obj.align() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.align() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_align_t align ;
    if(!lv_align_jsstr_to_const(ctx, argv[0], &align)) {
        return JS_EXCEPTION ;
    }
    int16_t x_ofs ;
    if(JS_ToInt32(ctx, (int32_t *) &x_ofs, argv[1])!=0){
        THROW_EXCEPTION("arg x_ofs of method lvgl.Obj.align() must be a number")
    }
    int16_t y_ofs ;
    if(JS_ToInt32(ctx, (int32_t *) &y_ofs, argv[2])!=0){
        THROW_EXCEPTION("arg y_ofs of method lvgl.Obj.align() must be a number")
    }
    lv_obj_align(thisobj, align, x_ofs, y_ofs) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_align_to(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<4) {
        THROW_EXCEPTION("lvgl.Obj.alignTo() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.alignTo() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_t * base = (lv_obj_t *)JS_GetOpaque(argv[0], js_lv_obj_class_id) ;
    if( !base ){
        THROW_EXCEPTION("arg base of method lvgl.Obj.alignTo() must be a beapi.lvgl.Obj")
    }
    lv_align_t align ;
    if(!lv_align_jsstr_to_const(ctx, argv[1], &align)) {
        return JS_EXCEPTION ;
    }
    int16_t x_ofs ;
    if(JS_ToInt32(ctx, (int32_t *) &x_ofs, argv[2])!=0){
        THROW_EXCEPTION("arg x_ofs of method lvgl.Obj.alignTo() must be a number")
    }
    int16_t y_ofs ;
    if(JS_ToInt32(ctx, (int32_t *) &y_ofs, argv[3])!=0){
        THROW_EXCEPTION("arg y_ofs of method lvgl.Obj.alignTo() must be a number")
    }
    lv_obj_align_to(thisobj, base, align, x_ofs, y_ofs) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_center(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.center() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_center(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_get_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.x() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_x(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.y() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_y(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_x_aligned(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.xAligned() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_x_aligned(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_y_aligned(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.yAligned() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_y_aligned(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.width() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_width(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.height() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_height(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_content_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.contentWidth() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_content_width(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_content_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.contentHeight() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_content_height(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_content_coords(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.contentCoords() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.contentCoords() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg area of method lvgl.Obj.contentCoords() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t area ;
    JSValue jsarea_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jsarea_x1)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.contentCoords() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x1, jsarea_x1)!=0) {
        THROW_EXCEPTION("property x1 of arg area is not a number")
    }
    JSValue jsarea_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jsarea_y1)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.contentCoords() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y1, jsarea_y1)!=0) {
        THROW_EXCEPTION("property y1 of arg area is not a number")
    }
    JSValue jsarea_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jsarea_x2)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.contentCoords() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x2, jsarea_x2)!=0) {
        THROW_EXCEPTION("property x2 of arg area is not a number")
    }
    JSValue jsarea_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jsarea_y2)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.contentCoords() missing property x, or is not a number")
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
        THROW_EXCEPTION("lvgl.Obj.selfWidth() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_self_width(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_self_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.selfHeight() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_self_height(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_refresh_self_size(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.refreshSelfSize() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_obj_refresh_self_size(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_refr_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.refrPos() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_refr_pos(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_move_to(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Obj.moveTo() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.moveTo() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method lvgl.Obj.moveTo() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method lvgl.Obj.moveTo() must be a number")
    }
    lv_obj_move_to(thisobj, x, y) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_move_children_by(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("lvgl.Obj.moveChildrenBy() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.moveChildrenBy() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x_diff ;
    if(JS_ToInt32(ctx, (int32_t *) &x_diff, argv[0])!=0){
        THROW_EXCEPTION("arg x_diff of method lvgl.Obj.moveChildrenBy() must be a number")
    }
    int16_t y_diff ;
    if(JS_ToInt32(ctx, (int32_t *) &y_diff, argv[1])!=0){
        THROW_EXCEPTION("arg y_diff of method lvgl.Obj.moveChildrenBy() must be a number")
    }
    bool ignore_floating = JS_ToBool(ctx, argv[2]) ;
    lv_obj_move_children_by(thisobj, x_diff, y_diff, ignore_floating) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_invalidate_area(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.invalidateArea() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.invalidateArea() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg area of method lvgl.Obj.invalidateArea() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t area ;
    JSValue jsarea_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jsarea_x1)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.invalidateArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x1, jsarea_x1)!=0) {
        THROW_EXCEPTION("property x1 of arg area is not a number")
    }
    JSValue jsarea_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jsarea_y1)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.invalidateArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y1, jsarea_y1)!=0) {
        THROW_EXCEPTION("property y1 of arg area is not a number")
    }
    JSValue jsarea_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jsarea_x2)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.invalidateArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x2, jsarea_x2)!=0) {
        THROW_EXCEPTION("property x2 of arg area is not a number")
    }
    JSValue jsarea_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jsarea_y2)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.invalidateArea() missing property x, or is not a number")
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
        THROW_EXCEPTION("lvgl.Obj.invalidate() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_invalidate(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_area_is_visible(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.areaIsVisible() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.areaIsVisible() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg area of method lvgl.Obj.areaIsVisible() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t area ;
    JSValue jsarea_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jsarea_x1)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.areaIsVisible() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x1, jsarea_x1)!=0) {
        THROW_EXCEPTION("property x1 of arg area is not a number")
    }
    JSValue jsarea_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jsarea_y1)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.areaIsVisible() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y1, jsarea_y1)!=0) {
        THROW_EXCEPTION("property y1 of arg area is not a number")
    }
    JSValue jsarea_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jsarea_x2)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.areaIsVisible() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x2, jsarea_x2)!=0) {
        THROW_EXCEPTION("property x2 of arg area is not a number")
    }
    JSValue jsarea_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jsarea_y2)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.areaIsVisible() missing property x, or is not a number")
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
        THROW_EXCEPTION("lvgl.Obj.isVisible() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_obj_is_visible(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_set_ext_click_area(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setExtClickArea() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setExtClickArea() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t size ;
    if(JS_ToInt32(ctx, (int32_t *) &size, argv[0])!=0){
        THROW_EXCEPTION("arg size of method lvgl.Obj.setExtClickArea() must be a number")
    }
    lv_obj_set_ext_click_area(thisobj, size) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_get_click_area(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.clickArea() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.clickArea() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg area of method lvgl.Obj.clickArea() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t area ;
    JSValue jsarea_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jsarea_x1)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.clickArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x1, jsarea_x1)!=0) {
        THROW_EXCEPTION("property x1 of arg area is not a number")
    }
    JSValue jsarea_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jsarea_y1)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.clickArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y1, jsarea_y1)!=0) {
        THROW_EXCEPTION("property y1 of arg area is not a number")
    }
    JSValue jsarea_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jsarea_x2)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.clickArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x2, jsarea_x2)!=0) {
        THROW_EXCEPTION("property x2 of arg area is not a number")
    }
    JSValue jsarea_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jsarea_y2)){
        THROW_EXCEPTION("arg area of method lvgl.Obj.clickArea() missing property x, or is not a number")
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
        THROW_EXCEPTION("lvgl.Obj.hitTest() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.hitTest() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg point of method lvgl.Obj.hitTest() must be a object{x,y}")
    }
    lv_point_t point ;
    JSValue jspoint_x = JS_GetPropertyStr(ctx, argv[0], "x") ;
    if(!JS_IsNumber(jspoint_x)){
        THROW_EXCEPTION("arg point of method lvgl.Obj.hitTest() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&point.x, jspoint_x)!=0) {
        THROW_EXCEPTION("property x of arg point is not a number")
    }
    JSValue jspoint_y = JS_GetPropertyStr(ctx, argv[0], "y") ;
    if(!JS_IsNumber(jspoint_y)){
        THROW_EXCEPTION("arg point of method lvgl.Obj.hitTest() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&point.y, jspoint_y)!=0) {
        THROW_EXCEPTION("property y of arg point is not a number")
    }
    JSValue retval = JS_NewBool(ctx,lv_obj_hit_test(thisobj, &point)) ;
    return retval ;
}

static JSValue js_lv_obj_set_scrollbar_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setScrollbarMode() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setScrollbarMode() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t mode ;
    if(JS_ToUint32(ctx, (uint32_t *) &mode, argv[0])!=0){
        THROW_EXCEPTION("arg mode of method lvgl.Obj.setScrollbarMode() must be a number")
    }
    lv_obj_set_scrollbar_mode(thisobj, mode) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_scroll_dir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setScrollDir() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setScrollDir() must be called as a lvgl.Obj method")
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
        THROW_EXCEPTION("lvgl.Obj.setScrollSnapX() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setScrollSnapX() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t align ;
    if(JS_ToUint32(ctx, (uint32_t *) &align, argv[0])!=0){
        THROW_EXCEPTION("arg align of method lvgl.Obj.setScrollSnapX() must be a number")
    }
    lv_obj_set_scroll_snap_x(thisobj, align) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_scroll_snap_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setScrollSnapY() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setScrollSnapY() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t align ;
    if(JS_ToUint32(ctx, (uint32_t *) &align, argv[0])!=0){
        THROW_EXCEPTION("arg align of method lvgl.Obj.setScrollSnapY() must be a number")
    }
    lv_obj_set_scroll_snap_y(thisobj, align) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_get_scrollbar_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.scrollbarMode() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_obj_get_scrollbar_mode(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_dir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.scrollDir() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = lv_dir_const_to_jsstr(ctx,lv_obj_get_scroll_dir(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_snap_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.scrollSnapX() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_obj_get_scroll_snap_x(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_snap_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.scrollSnapY() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_obj_get_scroll_snap_y(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.scrollX() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_scroll_x(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.scrollY() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_scroll_y(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_top(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.scrollTop() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_scroll_top(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_bottom(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.scrollBottom() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_scroll_bottom(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_left(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.scrollLeft() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_scroll_left(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_right(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.scrollRight() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_obj_get_scroll_right(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_scroll_end(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.scrollEnd() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.scrollEnd() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg end of method lvgl.Obj.scrollEnd() must be a object{x,y}")
    }
    lv_point_t end ;
    JSValue jsend_x = JS_GetPropertyStr(ctx, argv[0], "x") ;
    if(!JS_IsNumber(jsend_x)){
        THROW_EXCEPTION("arg end of method lvgl.Obj.scrollEnd() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&end.x, jsend_x)!=0) {
        THROW_EXCEPTION("property x of arg end is not a number")
    }
    JSValue jsend_y = JS_GetPropertyStr(ctx, argv[0], "y") ;
    if(!JS_IsNumber(jsend_y)){
        THROW_EXCEPTION("arg end of method lvgl.Obj.scrollEnd() missing property x, or is not a number")
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
        THROW_EXCEPTION("lvgl.Obj.scrollBy() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.scrollBy() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method lvgl.Obj.scrollBy() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method lvgl.Obj.scrollBy() must be a number")
    }
    bool anim_en = JS_ToBool(ctx, argv[2]) ;
    lv_obj_scroll_by(thisobj, x, y, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_scroll_to(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("lvgl.Obj.scrollTo() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.scrollTo() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method lvgl.Obj.scrollTo() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method lvgl.Obj.scrollTo() must be a number")
    }
    bool anim_en = JS_ToBool(ctx, argv[2]) ;
    lv_obj_scroll_to(thisobj, x, y, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_scroll_to_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Obj.scrollToX() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.scrollToX() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method lvgl.Obj.scrollToX() must be a number")
    }
    bool anim_en = JS_ToBool(ctx, argv[1]) ;
    lv_obj_scroll_to_x(thisobj, x, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_scroll_to_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Obj.scrollToY() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.scrollToY() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[0])!=0){
        THROW_EXCEPTION("arg y of method lvgl.Obj.scrollToY() must be a number")
    }
    bool anim_en = JS_ToBool(ctx, argv[1]) ;
    lv_obj_scroll_to_y(thisobj, y, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_scroll_to_view(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.scrollToView() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.scrollToView() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool anim_en = JS_ToBool(ctx, argv[0]) ;
    lv_obj_scroll_to_view(thisobj, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_scroll_to_view_recursive(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.scrollToViewRecursive() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.scrollToViewRecursive() must be called as a lvgl.Obj method")
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
        THROW_EXCEPTION("lvgl.Obj.isScrolling() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_obj_is_scrolling(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_update_snap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.updateSnap() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.updateSnap() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool anim_en = JS_ToBool(ctx, argv[0]) ;
    lv_obj_update_snap(thisobj, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_get_scrollbar_area(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Obj.scrollbarArea() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.scrollbarArea() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg hor of method lvgl.Obj.scrollbarArea() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t hor ;
    JSValue jshor_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jshor_x1)){
        THROW_EXCEPTION("arg hor of method lvgl.Obj.scrollbarArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&hor.x1, jshor_x1)!=0) {
        THROW_EXCEPTION("property x1 of arg hor is not a number")
    }
    JSValue jshor_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jshor_y1)){
        THROW_EXCEPTION("arg hor of method lvgl.Obj.scrollbarArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&hor.y1, jshor_y1)!=0) {
        THROW_EXCEPTION("property y1 of arg hor is not a number")
    }
    JSValue jshor_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jshor_x2)){
        THROW_EXCEPTION("arg hor of method lvgl.Obj.scrollbarArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&hor.x2, jshor_x2)!=0) {
        THROW_EXCEPTION("property x2 of arg hor is not a number")
    }
    JSValue jshor_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jshor_y2)){
        THROW_EXCEPTION("arg hor of method lvgl.Obj.scrollbarArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&hor.y2, jshor_y2)!=0) {
        THROW_EXCEPTION("property y2 of arg hor is not a number")
    }
    if(!JS_IsObject(argv[1])){
        THROW_EXCEPTION("arg ver of method lvgl.Obj.scrollbarArea() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t ver ;
    JSValue jsver_x1 = JS_GetPropertyStr(ctx, argv[1], "x1") ;
    if(!JS_IsNumber(jsver_x1)){
        THROW_EXCEPTION("arg ver of method lvgl.Obj.scrollbarArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&ver.x1, jsver_x1)!=0) {
        THROW_EXCEPTION("property x1 of arg ver is not a number")
    }
    JSValue jsver_y1 = JS_GetPropertyStr(ctx, argv[1], "y1") ;
    if(!JS_IsNumber(jsver_y1)){
        THROW_EXCEPTION("arg ver of method lvgl.Obj.scrollbarArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&ver.y1, jsver_y1)!=0) {
        THROW_EXCEPTION("property y1 of arg ver is not a number")
    }
    JSValue jsver_x2 = JS_GetPropertyStr(ctx, argv[1], "x2") ;
    if(!JS_IsNumber(jsver_x2)){
        THROW_EXCEPTION("arg ver of method lvgl.Obj.scrollbarArea() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&ver.x2, jsver_x2)!=0) {
        THROW_EXCEPTION("property x2 of arg ver is not a number")
    }
    JSValue jsver_y2 = JS_GetPropertyStr(ctx, argv[1], "y2") ;
    if(!JS_IsNumber(jsver_y2)){
        THROW_EXCEPTION("arg ver of method lvgl.Obj.scrollbarArea() missing property x, or is not a number")
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
        THROW_EXCEPTION("lvgl.Obj.scrollbarInvalidate() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_scrollbar_invalidate(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_readjust_scroll(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.readjustScroll() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.readjustScroll() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool anim_en = JS_ToBool(ctx, argv[0]) ;
    lv_obj_readjust_scroll(thisobj, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_remove_style_all(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.removeStyleAll() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_remove_style_all(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_fade_in(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Obj.fadeIn() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.fadeIn() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t time ;
    if(JS_ToUint32(ctx, (uint32_t *) &time, argv[0])!=0){
        THROW_EXCEPTION("arg time of method lvgl.Obj.fadeIn() must be a number")
    }
    uint32_t delay ;
    if(JS_ToUint32(ctx, (uint32_t *) &delay, argv[1])!=0){
        THROW_EXCEPTION("arg delay of method lvgl.Obj.fadeIn() must be a number")
    }
    lv_obj_fade_in(thisobj, time, delay) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_fade_out(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Obj.fadeOut() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.fadeOut() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t time ;
    if(JS_ToUint32(ctx, (uint32_t *) &time, argv[0])!=0){
        THROW_EXCEPTION("arg time of method lvgl.Obj.fadeOut() must be a number")
    }
    uint32_t delay ;
    if(JS_ToUint32(ctx, (uint32_t *) &delay, argv[1])!=0){
        THROW_EXCEPTION("arg delay of method lvgl.Obj.fadeOut() must be a number")
    }
    lv_obj_fade_out(thisobj, time, delay) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_parent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setParent() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setParent() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_t * undefined = (lv_obj_t *)JS_GetOpaque(argv[0], js_lv_obj_class_id) ;
    if( !undefined ){
        THROW_EXCEPTION("arg undefined of method lvgl.Obj.setParent() must be a beapi.lvgl.Obj")
    }
    lv_obj_set_parent(thisobj, undefined) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_move_to_index(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.moveToIndex() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.moveToIndex() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int32_t index ;
    if(JS_ToInt32(ctx, (int32_t *) &index, argv[0])!=0){
        THROW_EXCEPTION("arg index of method lvgl.Obj.moveToIndex() must be a number")
    }
    lv_obj_move_to_index(thisobj, index) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_get_screen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.screen() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_obj_get_screen(thisobj);
    if(lvobj){
        retval = js_lv_obj_wrapper(ctx, lvobj, js_lv_obj_class_id) ;
    } 
    return retval ;
}

static JSValue js_lv_obj_get_parent(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.parent() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_obj_get_parent(thisobj);
    if(lvobj){
        retval = js_lv_obj_wrapper(ctx, lvobj, js_lv_obj_class_id) ;
    } 
    return retval ;
}

static JSValue js_lv_obj_get_child(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.child() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.child() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int32_t id ;
    if(JS_ToInt32(ctx, (int32_t *) &id, argv[0])!=0){
        THROW_EXCEPTION("arg id of method lvgl.Obj.child() must be a number")
    }
    JSValue retval = JS_NULL ;
    void * lvobj = lv_obj_get_child(thisobj, id);
    if(lvobj){
        retval = js_lv_obj_wrapper(ctx, lvobj, js_lv_obj_class_id) ;
    } 
    return retval ;
}

static JSValue js_lv_obj_get_child_cnt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.childCnt() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_obj_get_child_cnt(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_get_index(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.index() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_obj_get_index(thisobj)) ;
    return retval ;
}

static JSValue js_lv_obj_set_flex_flow(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Obj.setFlexFlow() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setFlexFlow() must be called as a lvgl.Obj method")
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
        THROW_EXCEPTION("lvgl.Obj.setFlexAlign() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setFlexAlign() must be called as a lvgl.Obj method")
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
        THROW_EXCEPTION("lvgl.Obj.setFlexGrow() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setFlexGrow() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t grow ;
    if(JS_ToUint32(ctx, (uint32_t *) &grow, argv[0])!=0){
        THROW_EXCEPTION("arg grow of method lvgl.Obj.setFlexGrow() must be a number")
    }
    lv_obj_set_flex_grow(thisobj, grow) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_tile(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Obj.setTile() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setTile() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_t * tile_obj = (lv_obj_t *)JS_GetOpaque(argv[0], js_lv_obj_class_id) ;
    if( !tile_obj ){
        THROW_EXCEPTION("arg tile_obj of method lvgl.Obj.setTile() must be a beapi.lvgl.Obj")
    }
    bool anim_en = JS_ToBool(ctx, argv[1]) ;
    lv_obj_set_tile(thisobj, tile_obj, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_set_tile_id(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("lvgl.Obj.setTileId() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.setTileId() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t col_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &col_id, argv[0])!=0){
        THROW_EXCEPTION("arg col_id of method lvgl.Obj.setTileId() must be a number")
    }
    uint32_t row_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &row_id, argv[1])!=0){
        THROW_EXCEPTION("arg row_id of method lvgl.Obj.setTileId() must be a number")
    }
    bool anim_en = JS_ToBool(ctx, argv[2]) ;
    lv_obj_set_tile_id(thisobj, col_id, row_id, anim_en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_move_foreground(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.moveForeground() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_move_foreground(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_move_background(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.moveBackground() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_obj_move_background(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_obj_get_child_id(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Obj.childId() must be called as a lvgl.Obj method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_obj_get_child_id(thisobj)) ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_obj_proto_funcs[] = {
    JS_CFUNC_DEF("enableEvent", 0, js_lv_obj_enable_event),
    JS_CFUNC_DEF("disableEvent", 0, js_lv_obj_disable_event),
    JS_CFUNC_DEF("isScreen", 0, js_lv_obj_is_screen),
    JS_CFUNC_DEF("ptr", 0, js_lv_obj_ptr),
    JS_CFUNC_DEF("coords", 0, js_lv_obj_get_coords),
    JS_CFUNC_DEF("setCoords", 0, js_lv_obj_set_coords),
    JS_CFUNC_DEF("move", 0, js_lv_obj_move),
    JS_CFUNC_DEF("moveX", 0, js_lv_obj_move_x),
    JS_CFUNC_DEF("moveY", 0, js_lv_obj_move_y),
    JS_CFUNC_DEF("allStyles", 0, js_lv_obj_get_all_styles),
    JS_CFUNC_DEF("setStyle", 0, js_lv_obj_set_style),
    JS_CFUNC_DEF("style", 0, js_lv_obj_get_style),
    JS_CFUNC_DEF("addFlag", 0, js_lv_obj_add_flag),
    JS_CFUNC_DEF("clearFlag", 0, js_lv_obj_clear_flag),
    JS_CFUNC_DEF("addState", 0, js_lv_obj_add_state),
    JS_CFUNC_DEF("clearState", 0, js_lv_obj_clear_state),
    JS_CFUNC_DEF("hasFlag", 0, js_lv_obj_has_flag),
    JS_CFUNC_DEF("hasFlagAny", 0, js_lv_obj_has_flag_any),
    JS_CFUNC_DEF("state", 0, js_lv_obj_get_state),
    JS_CFUNC_DEF("hasState", 0, js_lv_obj_has_state),
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
    JS_CFUNC_DEF("removeStyleAll", 0, js_lv_obj_remove_style_all),
    JS_CFUNC_DEF("fadeIn", 0, js_lv_obj_fade_in),
    JS_CFUNC_DEF("fadeOut", 0, js_lv_obj_fade_out),
    JS_CFUNC_DEF("setParent", 0, js_lv_obj_set_parent),
    JS_CFUNC_DEF("moveToIndex", 0, js_lv_obj_move_to_index),
    JS_CFUNC_DEF("screen", 0, js_lv_obj_get_screen),
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

static JSValue js_lv_label_set_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Label.setText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Label.setText() must be called as a lvgl.Label method")
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
        THROW_EXCEPTION("lvgl.Label.setTextStatic() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Label.setTextStatic() must be called as a lvgl.Label method")
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
        THROW_EXCEPTION("lvgl.Label.setLongMode() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Label.setLongMode() must be called as a lvgl.Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t long_mode ;
    if(JS_ToUint32(ctx, (uint32_t *) &long_mode, argv[0])!=0){
        THROW_EXCEPTION("arg long_mode of method lvgl.Label.setLongMode() must be a number")
    }
    lv_label_set_long_mode(thisobj, long_mode) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_label_set_recolor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Label.setRecolor() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Label.setRecolor() must be called as a lvgl.Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool en = JS_ToBool(ctx, argv[0]) ;
    lv_label_set_recolor(thisobj, en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_label_set_text_sel_start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Label.setTextSelStart() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Label.setTextSelStart() must be called as a lvgl.Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t index ;
    if(JS_ToUint32(ctx, (uint32_t *) &index, argv[0])!=0){
        THROW_EXCEPTION("arg index of method lvgl.Label.setTextSelStart() must be a number")
    }
    lv_label_set_text_sel_start(thisobj, index) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_label_set_text_sel_end(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Label.setTextSelEnd() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Label.setTextSelEnd() must be called as a lvgl.Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t index ;
    if(JS_ToUint32(ctx, (uint32_t *) &index, argv[0])!=0){
        THROW_EXCEPTION("arg index of method lvgl.Label.setTextSelEnd() must be a number")
    }
    lv_label_set_text_sel_end(thisobj, index) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_label_get_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Label.text() must be called as a lvgl.Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_label_get_text(thisobj)) ;
    return retval ;
}

static JSValue js_lv_label_get_long_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Label.longMode() must be called as a lvgl.Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_label_get_long_mode(thisobj)) ;
    return retval ;
}

static JSValue js_lv_label_get_recolor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Label.recolor() must be called as a lvgl.Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_label_get_recolor(thisobj)) ;
    return retval ;
}

static JSValue js_lv_label_get_letter_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Label.letterPos() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Label.letterPos() must be called as a lvgl.Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t char_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &char_id, argv[0])!=0){
        THROW_EXCEPTION("arg char_id of method lvgl.Label.letterPos() must be a number")
    }
    if(!JS_IsObject(argv[1])){
        THROW_EXCEPTION("arg pos of method lvgl.Label.letterPos() must be a object{x,y}")
    }
    lv_point_t pos ;
    JSValue jspos_x = JS_GetPropertyStr(ctx, argv[1], "x") ;
    if(!JS_IsNumber(jspos_x)){
        THROW_EXCEPTION("arg pos of method lvgl.Label.letterPos() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&pos.x, jspos_x)!=0) {
        THROW_EXCEPTION("property x of arg pos is not a number")
    }
    JSValue jspos_y = JS_GetPropertyStr(ctx, argv[1], "y") ;
    if(!JS_IsNumber(jspos_y)){
        THROW_EXCEPTION("arg pos of method lvgl.Label.letterPos() missing property x, or is not a number")
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
        THROW_EXCEPTION("lvgl.Label.letterOn() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Label.letterOn() must be called as a lvgl.Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg pos_in of method lvgl.Label.letterOn() must be a object{x,y}")
    }
    lv_point_t pos_in ;
    JSValue jspos_in_x = JS_GetPropertyStr(ctx, argv[0], "x") ;
    if(!JS_IsNumber(jspos_in_x)){
        THROW_EXCEPTION("arg pos_in of method lvgl.Label.letterOn() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&pos_in.x, jspos_in_x)!=0) {
        THROW_EXCEPTION("property x of arg pos_in is not a number")
    }
    JSValue jspos_in_y = JS_GetPropertyStr(ctx, argv[0], "y") ;
    if(!JS_IsNumber(jspos_in_y)){
        THROW_EXCEPTION("arg pos_in of method lvgl.Label.letterOn() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&pos_in.y, jspos_in_y)!=0) {
        THROW_EXCEPTION("property y of arg pos_in is not a number")
    }
    JSValue retval = JS_NewUint32(ctx,lv_label_get_letter_on(thisobj, &pos_in)) ;
    return retval ;
}

static JSValue js_lv_label_is_char_under_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Label.isCharUnderPos() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Label.isCharUnderPos() must be called as a lvgl.Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg pos of method lvgl.Label.isCharUnderPos() must be a object{x,y}")
    }
    lv_point_t pos ;
    JSValue jspos_x = JS_GetPropertyStr(ctx, argv[0], "x") ;
    if(!JS_IsNumber(jspos_x)){
        THROW_EXCEPTION("arg pos of method lvgl.Label.isCharUnderPos() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&pos.x, jspos_x)!=0) {
        THROW_EXCEPTION("property x of arg pos is not a number")
    }
    JSValue jspos_y = JS_GetPropertyStr(ctx, argv[0], "y") ;
    if(!JS_IsNumber(jspos_y)){
        THROW_EXCEPTION("arg pos of method lvgl.Label.isCharUnderPos() missing property x, or is not a number")
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
        THROW_EXCEPTION("lvgl.Label.textSelectionStart() must be called as a lvgl.Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_label_get_text_selection_start(thisobj)) ;
    return retval ;
}

static JSValue js_lv_label_get_text_selection_end(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Label.textSelectionEnd() must be called as a lvgl.Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_label_get_text_selection_end(thisobj)) ;
    return retval ;
}

static JSValue js_lv_label_ins_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Label.insText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Label.insText() must be called as a lvgl.Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t pos ;
    if(JS_ToUint32(ctx, (uint32_t *) &pos, argv[0])!=0){
        THROW_EXCEPTION("arg pos of method lvgl.Label.insText() must be a number")
    }
    char * txt = (char *)JS_ToCString(ctx, argv[1]) ;
    lv_label_ins_text(thisobj, pos, txt) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, txt) ;
    return retval ;
}

static JSValue js_lv_label_cut_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Label.cutText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Label.cutText() must be called as a lvgl.Label method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t pos ;
    if(JS_ToUint32(ctx, (uint32_t *) &pos, argv[0])!=0){
        THROW_EXCEPTION("arg pos of method lvgl.Label.cutText() must be a number")
    }
    uint32_t cnt ;
    if(JS_ToUint32(ctx, (uint32_t *) &cnt, argv[1])!=0){
        THROW_EXCEPTION("arg cnt of method lvgl.Label.cutText() must be a number")
    }
    lv_label_cut_text(thisobj, pos, cnt) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_label_proto_funcs[] = {
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

static JSValue js_lv_arc_set_start_angle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Arc.setStartAngle() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.setStartAngle() must be called as a lvgl.Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t start ;
    if(JS_ToUint32(ctx, (uint32_t *) &start, argv[0])!=0){
        THROW_EXCEPTION("arg start of method lvgl.Arc.setStartAngle() must be a number")
    }
    lv_arc_set_start_angle(thisobj, start) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_end_angle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Arc.setEndAngle() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.setEndAngle() must be called as a lvgl.Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t end ;
    if(JS_ToUint32(ctx, (uint32_t *) &end, argv[0])!=0){
        THROW_EXCEPTION("arg end of method lvgl.Arc.setEndAngle() must be a number")
    }
    lv_arc_set_end_angle(thisobj, end) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_angles(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Arc.setAngles() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.setAngles() must be called as a lvgl.Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t start ;
    if(JS_ToUint32(ctx, (uint32_t *) &start, argv[0])!=0){
        THROW_EXCEPTION("arg start of method lvgl.Arc.setAngles() must be a number")
    }
    uint16_t end ;
    if(JS_ToUint32(ctx, (uint32_t *) &end, argv[1])!=0){
        THROW_EXCEPTION("arg end of method lvgl.Arc.setAngles() must be a number")
    }
    lv_arc_set_angles(thisobj, start, end) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_bg_start_angle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Arc.setBgStartAngle() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.setBgStartAngle() must be called as a lvgl.Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t start ;
    if(JS_ToUint32(ctx, (uint32_t *) &start, argv[0])!=0){
        THROW_EXCEPTION("arg start of method lvgl.Arc.setBgStartAngle() must be a number")
    }
    lv_arc_set_bg_start_angle(thisobj, start) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_bg_end_angle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Arc.setBgEndAngle() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.setBgEndAngle() must be called as a lvgl.Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t end ;
    if(JS_ToUint32(ctx, (uint32_t *) &end, argv[0])!=0){
        THROW_EXCEPTION("arg end of method lvgl.Arc.setBgEndAngle() must be a number")
    }
    lv_arc_set_bg_end_angle(thisobj, end) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_bg_angles(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Arc.setBgAngles() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.setBgAngles() must be called as a lvgl.Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t start ;
    if(JS_ToUint32(ctx, (uint32_t *) &start, argv[0])!=0){
        THROW_EXCEPTION("arg start of method lvgl.Arc.setBgAngles() must be a number")
    }
    uint16_t end ;
    if(JS_ToUint32(ctx, (uint32_t *) &end, argv[1])!=0){
        THROW_EXCEPTION("arg end of method lvgl.Arc.setBgAngles() must be a number")
    }
    lv_arc_set_bg_angles(thisobj, start, end) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_rotation(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Arc.setRotation() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.setRotation() must be called as a lvgl.Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t rotation ;
    if(JS_ToUint32(ctx, (uint32_t *) &rotation, argv[0])!=0){
        THROW_EXCEPTION("arg rotation of method lvgl.Arc.setRotation() must be a number")
    }
    lv_arc_set_rotation(thisobj, rotation) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Arc.setMode() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.setMode() must be called as a lvgl.Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t type ;
    if(JS_ToUint32(ctx, (uint32_t *) &type, argv[0])!=0){
        THROW_EXCEPTION("arg type of method lvgl.Arc.setMode() must be a number")
    }
    lv_arc_set_mode(thisobj, type) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Arc.setValue() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.setValue() must be called as a lvgl.Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t value ;
    if(JS_ToInt32(ctx, (int32_t *) &value, argv[0])!=0){
        THROW_EXCEPTION("arg value of method lvgl.Arc.setValue() must be a number")
    }
    lv_arc_set_value(thisobj, value) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_range(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Arc.setRange() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.setRange() must be called as a lvgl.Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t min ;
    if(JS_ToInt32(ctx, (int32_t *) &min, argv[0])!=0){
        THROW_EXCEPTION("arg min of method lvgl.Arc.setRange() must be a number")
    }
    int16_t max ;
    if(JS_ToInt32(ctx, (int32_t *) &max, argv[1])!=0){
        THROW_EXCEPTION("arg max of method lvgl.Arc.setRange() must be a number")
    }
    lv_arc_set_range(thisobj, min, max) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_set_change_rate(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Arc.setChangeRate() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.setChangeRate() must be called as a lvgl.Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t rate ;
    if(JS_ToUint32(ctx, (uint32_t *) &rate, argv[0])!=0){
        THROW_EXCEPTION("arg rate of method lvgl.Arc.setChangeRate() must be a number")
    }
    lv_arc_set_change_rate(thisobj, rate) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_arc_get_angle_start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.angleStart() must be called as a lvgl.Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_arc_get_angle_start(thisobj)) ;
    return retval ;
}

static JSValue js_lv_arc_get_angle_end(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.angleEnd() must be called as a lvgl.Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_arc_get_angle_end(thisobj)) ;
    return retval ;
}

static JSValue js_lv_arc_get_bg_angle_start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.bgAngleStart() must be called as a lvgl.Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_arc_get_bg_angle_start(thisobj)) ;
    return retval ;
}

static JSValue js_lv_arc_get_bg_angle_end(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.bgAngleEnd() must be called as a lvgl.Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_arc_get_bg_angle_end(thisobj)) ;
    return retval ;
}

static JSValue js_lv_arc_get_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.value() must be called as a lvgl.Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_arc_get_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_arc_get_min_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.minValue() must be called as a lvgl.Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_arc_get_min_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_arc_get_max_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.maxValue() must be called as a lvgl.Arc method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_arc_get_max_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_arc_get_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Arc.mode() must be called as a lvgl.Arc method")
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

static JSValue js_lv_bar_set_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Bar.setValue() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Bar.setValue() must be called as a lvgl.Bar method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int32_t value ;
    if(JS_ToInt32(ctx, (int32_t *) &value, argv[0])!=0){
        THROW_EXCEPTION("arg value of method lvgl.Bar.setValue() must be a number")
    }
    bool anim = JS_ToBool(ctx, argv[1]) ;
    lv_bar_set_value(thisobj, value, anim) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_bar_set_start_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Bar.setStartValue() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Bar.setStartValue() must be called as a lvgl.Bar method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int32_t start_value ;
    if(JS_ToInt32(ctx, (int32_t *) &start_value, argv[0])!=0){
        THROW_EXCEPTION("arg start_value of method lvgl.Bar.setStartValue() must be a number")
    }
    bool anim = JS_ToBool(ctx, argv[1]) ;
    lv_bar_set_start_value(thisobj, start_value, anim) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_bar_set_range(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Bar.setRange() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Bar.setRange() must be called as a lvgl.Bar method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int32_t min ;
    if(JS_ToInt32(ctx, (int32_t *) &min, argv[0])!=0){
        THROW_EXCEPTION("arg min of method lvgl.Bar.setRange() must be a number")
    }
    int32_t max ;
    if(JS_ToInt32(ctx, (int32_t *) &max, argv[1])!=0){
        THROW_EXCEPTION("arg max of method lvgl.Bar.setRange() must be a number")
    }
    lv_bar_set_range(thisobj, min, max) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_bar_set_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Bar.setMode() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Bar.setMode() must be called as a lvgl.Bar method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t mode ;
    if(JS_ToUint32(ctx, (uint32_t *) &mode, argv[0])!=0){
        THROW_EXCEPTION("arg mode of method lvgl.Bar.setMode() must be a number")
    }
    lv_bar_set_mode(thisobj, mode) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_bar_get_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Bar.value() must be called as a lvgl.Bar method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_bar_get_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_bar_get_start_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Bar.startValue() must be called as a lvgl.Bar method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_bar_get_start_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_bar_get_min_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Bar.minValue() must be called as a lvgl.Bar method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_bar_get_min_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_bar_get_max_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Bar.maxValue() must be called as a lvgl.Bar method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_bar_get_max_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_bar_get_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Bar.mode() must be called as a lvgl.Bar method")
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


static const JSCFunctionListEntry js_lv_btn_proto_funcs[] = {
} ;
#define __def_js_lv_btn_proto_funcs__

static JSValue js_lv_table_set_cell_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("lvgl.Table.setCellValue() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Table.setCellValue() must be called as a lvgl.Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t row ;
    if(JS_ToUint32(ctx, (uint32_t *) &row, argv[0])!=0){
        THROW_EXCEPTION("arg row of method lvgl.Table.setCellValue() must be a number")
    }
    uint16_t col ;
    if(JS_ToUint32(ctx, (uint32_t *) &col, argv[1])!=0){
        THROW_EXCEPTION("arg col of method lvgl.Table.setCellValue() must be a number")
    }
    char * txt = (char *)JS_ToCString(ctx, argv[2]) ;
    lv_table_set_cell_value(thisobj, row, col, txt) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, txt) ;
    return retval ;
}

static JSValue js_lv_table_set_row_cnt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Table.setRowCnt() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Table.setRowCnt() must be called as a lvgl.Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t row_cnt ;
    if(JS_ToUint32(ctx, (uint32_t *) &row_cnt, argv[0])!=0){
        THROW_EXCEPTION("arg row_cnt of method lvgl.Table.setRowCnt() must be a number")
    }
    lv_table_set_row_cnt(thisobj, row_cnt) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_table_set_col_cnt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Table.setColCnt() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Table.setColCnt() must be called as a lvgl.Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t col_cnt ;
    if(JS_ToUint32(ctx, (uint32_t *) &col_cnt, argv[0])!=0){
        THROW_EXCEPTION("arg col_cnt of method lvgl.Table.setColCnt() must be a number")
    }
    lv_table_set_col_cnt(thisobj, col_cnt) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_table_set_col_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Table.setColWidth() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Table.setColWidth() must be called as a lvgl.Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t col_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &col_id, argv[0])!=0){
        THROW_EXCEPTION("arg col_id of method lvgl.Table.setColWidth() must be a number")
    }
    int16_t w ;
    if(JS_ToInt32(ctx, (int32_t *) &w, argv[1])!=0){
        THROW_EXCEPTION("arg w of method lvgl.Table.setColWidth() must be a number")
    }
    lv_table_set_col_width(thisobj, col_id, w) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_table_add_cell_ctrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("lvgl.Table.addCellCtrl() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Table.addCellCtrl() must be called as a lvgl.Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t row ;
    if(JS_ToUint32(ctx, (uint32_t *) &row, argv[0])!=0){
        THROW_EXCEPTION("arg row of method lvgl.Table.addCellCtrl() must be a number")
    }
    uint16_t col ;
    if(JS_ToUint32(ctx, (uint32_t *) &col, argv[1])!=0){
        THROW_EXCEPTION("arg col of method lvgl.Table.addCellCtrl() must be a number")
    }
    uint8_t ctrl ;
    if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[2])!=0){
        THROW_EXCEPTION("arg ctrl of method lvgl.Table.addCellCtrl() must be a number")
    }
    lv_table_add_cell_ctrl(thisobj, row, col, ctrl) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_table_clear_cell_ctrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("lvgl.Table.clearCellCtrl() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Table.clearCellCtrl() must be called as a lvgl.Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t row ;
    if(JS_ToUint32(ctx, (uint32_t *) &row, argv[0])!=0){
        THROW_EXCEPTION("arg row of method lvgl.Table.clearCellCtrl() must be a number")
    }
    uint16_t col ;
    if(JS_ToUint32(ctx, (uint32_t *) &col, argv[1])!=0){
        THROW_EXCEPTION("arg col of method lvgl.Table.clearCellCtrl() must be a number")
    }
    uint8_t ctrl ;
    if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[2])!=0){
        THROW_EXCEPTION("arg ctrl of method lvgl.Table.clearCellCtrl() must be a number")
    }
    lv_table_clear_cell_ctrl(thisobj, row, col, ctrl) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_table_get_cell_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Table.cellValue() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Table.cellValue() must be called as a lvgl.Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t row ;
    if(JS_ToUint32(ctx, (uint32_t *) &row, argv[0])!=0){
        THROW_EXCEPTION("arg row of method lvgl.Table.cellValue() must be a number")
    }
    uint16_t col ;
    if(JS_ToUint32(ctx, (uint32_t *) &col, argv[1])!=0){
        THROW_EXCEPTION("arg col of method lvgl.Table.cellValue() must be a number")
    }
    JSValue retval = JS_NewString(ctx,lv_table_get_cell_value(thisobj, row, col)) ;
    return retval ;
}

static JSValue js_lv_table_get_row_cnt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Table.rowCnt() must be called as a lvgl.Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_table_get_row_cnt(thisobj)) ;
    return retval ;
}

static JSValue js_lv_table_get_col_cnt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Table.colCnt() must be called as a lvgl.Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_table_get_col_cnt(thisobj)) ;
    return retval ;
}

static JSValue js_lv_table_get_col_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Table.colWidth() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Table.colWidth() must be called as a lvgl.Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t col ;
    if(JS_ToUint32(ctx, (uint32_t *) &col, argv[0])!=0){
        THROW_EXCEPTION("arg col of method lvgl.Table.colWidth() must be a number")
    }
    JSValue retval = JS_NewInt32(ctx,lv_table_get_col_width(thisobj, col)) ;
    return retval ;
}

static JSValue js_lv_table_has_cell_ctrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("lvgl.Table.hasCellCtrl() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Table.hasCellCtrl() must be called as a lvgl.Table method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t row ;
    if(JS_ToUint32(ctx, (uint32_t *) &row, argv[0])!=0){
        THROW_EXCEPTION("arg row of method lvgl.Table.hasCellCtrl() must be a number")
    }
    uint16_t col ;
    if(JS_ToUint32(ctx, (uint32_t *) &col, argv[1])!=0){
        THROW_EXCEPTION("arg col of method lvgl.Table.hasCellCtrl() must be a number")
    }
    uint8_t ctrl ;
    if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[2])!=0){
        THROW_EXCEPTION("arg ctrl of method lvgl.Table.hasCellCtrl() must be a number")
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

static JSValue js_lv_textarea_add_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.TextArea.addText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.addText() must be called as a lvgl.TextArea method")
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
        THROW_EXCEPTION("lvgl.TextArea.delChar() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_textarea_del_char(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_del_char_forward(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.delCharForward() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_textarea_del_char_forward(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_set_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.TextArea.setText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.setText() must be called as a lvgl.TextArea method")
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
        THROW_EXCEPTION("lvgl.TextArea.setPlaceholderText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.setPlaceholderText() must be called as a lvgl.TextArea method")
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
        THROW_EXCEPTION("lvgl.TextArea.setCursorPos() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.setCursorPos() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int32_t pos ;
    if(JS_ToInt32(ctx, (int32_t *) &pos, argv[0])!=0){
        THROW_EXCEPTION("arg pos of method lvgl.TextArea.setCursorPos() must be a number")
    }
    lv_textarea_set_cursor_pos(thisobj, pos) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_set_cursor_click_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.TextArea.setCursorClickPos() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.setCursorClickPos() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool en = JS_ToBool(ctx, argv[0]) ;
    lv_textarea_set_cursor_click_pos(thisobj, en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_set_password_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.TextArea.setPasswordMode() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.setPasswordMode() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool en = JS_ToBool(ctx, argv[0]) ;
    lv_textarea_set_password_mode(thisobj, en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_set_one_line(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.TextArea.setOneLine() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.setOneLine() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool en = JS_ToBool(ctx, argv[0]) ;
    lv_textarea_set_one_line(thisobj, en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_set_accepted_chars(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.TextArea.setAcceptedChars() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.setAcceptedChars() must be called as a lvgl.TextArea method")
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
        THROW_EXCEPTION("lvgl.TextArea.setMaxLength() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.setMaxLength() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint32_t num ;
    if(JS_ToUint32(ctx, (uint32_t *) &num, argv[0])!=0){
        THROW_EXCEPTION("arg num of method lvgl.TextArea.setMaxLength() must be a number")
    }
    lv_textarea_set_max_length(thisobj, num) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_set_insert_replace(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.TextArea.setInsertReplace() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.setInsertReplace() must be called as a lvgl.TextArea method")
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
        THROW_EXCEPTION("lvgl.TextArea.setTextSelection() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.setTextSelection() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool en = JS_ToBool(ctx, argv[0]) ;
    lv_textarea_set_text_selection(thisobj, en) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_set_password_show_time(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.TextArea.setPasswordShowTime() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.setPasswordShowTime() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t time ;
    if(JS_ToUint32(ctx, (uint32_t *) &time, argv[0])!=0){
        THROW_EXCEPTION("arg time of method lvgl.TextArea.setPasswordShowTime() must be a number")
    }
    lv_textarea_set_password_show_time(thisobj, time) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_set_align(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.TextArea.setAlign() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.setAlign() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t align ;
    if(JS_ToUint32(ctx, (uint32_t *) &align, argv[0])!=0){
        THROW_EXCEPTION("arg align of method lvgl.TextArea.setAlign() must be a number")
    }
    lv_textarea_set_align(thisobj, align) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_get_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.text() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_textarea_get_text(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_get_placeholder_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.placeholderText() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_textarea_get_placeholder_text(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_get_label(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.label() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NULL ;
    void * lvobj = lv_textarea_get_label(thisobj);
    if(lvobj){
        retval = js_lv_obj_wrapper(ctx, lvobj, js_lv_obj_class_id) ;
    } 
    return retval ;
}

static JSValue js_lv_textarea_get_cursor_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.cursorPos() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_textarea_get_cursor_pos(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_get_cursor_click_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.cursorClickPos() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_textarea_get_cursor_click_pos(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_get_password_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.passwordMode() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_textarea_get_password_mode(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_get_one_line(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.oneLine() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_textarea_get_one_line(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_get_accepted_chars(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.acceptedChars() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_textarea_get_accepted_chars(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_get_max_length(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.maxLength() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_textarea_get_max_length(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_text_is_selected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.textIsSelected() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_textarea_text_is_selected(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_get_text_selection(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.textSelection() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_textarea_get_text_selection(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_get_password_show_time(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.passwordShowTime() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_textarea_get_password_show_time(thisobj)) ;
    return retval ;
}

static JSValue js_lv_textarea_clear_selection(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.clearSelection() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_textarea_clear_selection(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_cursor_right(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.cursorRight() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_textarea_cursor_right(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_cursor_left(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.cursorLeft() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_textarea_cursor_left(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_cursor_down(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.cursorDown() must be called as a lvgl.TextArea method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_textarea_cursor_down(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_textarea_cursor_up(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.TextArea.cursorUp() must be called as a lvgl.TextArea method")
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

static JSValue js_lv_slider_set_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Slider.setValue() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Slider.setValue() must be called as a lvgl.Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int32_t value ;
    if(JS_ToInt32(ctx, (int32_t *) &value, argv[0])!=0){
        THROW_EXCEPTION("arg value of method lvgl.Slider.setValue() must be a number")
    }
    bool anim = JS_ToBool(ctx, argv[1]) ;
    lv_slider_set_value(thisobj, value, anim) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_slider_set_left_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Slider.setLeftValue() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Slider.setLeftValue() must be called as a lvgl.Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int32_t value ;
    if(JS_ToInt32(ctx, (int32_t *) &value, argv[0])!=0){
        THROW_EXCEPTION("arg value of method lvgl.Slider.setLeftValue() must be a number")
    }
    bool anim = JS_ToBool(ctx, argv[1]) ;
    lv_slider_set_left_value(thisobj, value, anim) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_slider_set_range(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Slider.setRange() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Slider.setRange() must be called as a lvgl.Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int32_t min ;
    if(JS_ToInt32(ctx, (int32_t *) &min, argv[0])!=0){
        THROW_EXCEPTION("arg min of method lvgl.Slider.setRange() must be a number")
    }
    int32_t max ;
    if(JS_ToInt32(ctx, (int32_t *) &max, argv[1])!=0){
        THROW_EXCEPTION("arg max of method lvgl.Slider.setRange() must be a number")
    }
    lv_slider_set_range(thisobj, min, max) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_slider_set_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Slider.setMode() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Slider.setMode() must be called as a lvgl.Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t mode ;
    if(JS_ToUint32(ctx, (uint32_t *) &mode, argv[0])!=0){
        THROW_EXCEPTION("arg mode of method lvgl.Slider.setMode() must be a number")
    }
    lv_slider_set_mode(thisobj, mode) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_slider_get_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Slider.value() must be called as a lvgl.Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_slider_get_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_slider_get_left_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Slider.leftValue() must be called as a lvgl.Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_slider_get_left_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_slider_get_min_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Slider.minValue() must be called as a lvgl.Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_slider_get_min_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_slider_get_max_value(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Slider.maxValue() must be called as a lvgl.Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_slider_get_max_value(thisobj)) ;
    return retval ;
}

static JSValue js_lv_slider_is_dragged(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Slider.isDragged() must be called as a lvgl.Slider method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_slider_is_dragged(thisobj)) ;
    return retval ;
}

static JSValue js_lv_slider_get_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Slider.mode() must be called as a lvgl.Slider method")
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


static const JSCFunctionListEntry js_lv_switch_proto_funcs[] = {
} ;
#define __def_js_lv_switch_proto_funcs__

static JSValue js_lv_roller_set_options(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Roller.setOptions() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Roller.setOptions() must be called as a lvgl.Roller method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * options = (char *)JS_ToCString(ctx, argv[0]) ;
    uint8_t mode ;
    if(JS_ToUint32(ctx, (uint32_t *) &mode, argv[1])!=0){
        THROW_EXCEPTION("arg mode of method lvgl.Roller.setOptions() must be a number")
    }
    lv_roller_set_options(thisobj, options, mode) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, options) ;
    return retval ;
}

static JSValue js_lv_roller_set_selected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Roller.setSelected() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Roller.setSelected() must be called as a lvgl.Roller method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t sel_opt ;
    if(JS_ToUint32(ctx, (uint32_t *) &sel_opt, argv[0])!=0){
        THROW_EXCEPTION("arg sel_opt of method lvgl.Roller.setSelected() must be a number")
    }
    bool anim = JS_ToBool(ctx, argv[1]) ;
    lv_roller_set_selected(thisobj, sel_opt, anim) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_roller_set_visible_row_count(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Roller.setVisibleRowCount() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Roller.setVisibleRowCount() must be called as a lvgl.Roller method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t row_cnt ;
    if(JS_ToUint32(ctx, (uint32_t *) &row_cnt, argv[0])!=0){
        THROW_EXCEPTION("arg row_cnt of method lvgl.Roller.setVisibleRowCount() must be a number")
    }
    lv_roller_set_visible_row_count(thisobj, row_cnt) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_roller_get_selected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Roller.selected() must be called as a lvgl.Roller method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_roller_get_selected(thisobj)) ;
    return retval ;
}

static JSValue js_lv_roller_get_selected_str(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Roller.selectedStr() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Roller.selectedStr() must be called as a lvgl.Roller method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * buf = (char *)JS_ToCString(ctx, argv[0]) ;
    uint32_t buf_size ;
    if(JS_ToUint32(ctx, (uint32_t *) &buf_size, argv[1])!=0){
        THROW_EXCEPTION("arg buf_size of method lvgl.Roller.selectedStr() must be a number")
    }
    lv_roller_get_selected_str(thisobj, buf, buf_size) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, buf) ;
    return retval ;
}

static JSValue js_lv_roller_get_options(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Roller.options() must be called as a lvgl.Roller method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_roller_get_options(thisobj)) ;
    return retval ;
}

static JSValue js_lv_roller_get_option_cnt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Roller.optionCnt() must be called as a lvgl.Roller method")
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

static JSValue js_lv_checkbox_set_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Checkbox.setText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Checkbox.setText() must be called as a lvgl.Checkbox method")
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
        THROW_EXCEPTION("lvgl.Checkbox.setTextStatic() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Checkbox.setTextStatic() must be called as a lvgl.Checkbox method")
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
        THROW_EXCEPTION("lvgl.Checkbox.text() must be called as a lvgl.Checkbox method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_checkbox_get_text(thisobj)) ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_checkbox_proto_funcs[] = {
    JS_CFUNC_DEF("setText", 0, js_lv_checkbox_set_text),
    JS_CFUNC_DEF("setTextStatic", 0, js_lv_checkbox_set_text_static),
    JS_CFUNC_DEF("text", 0, js_lv_checkbox_get_text),
} ;
#define __def_js_lv_checkbox_proto_funcs__

static JSValue js_lv_line_set_y_invert(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Line.setYInvert() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Line.setYInvert() must be called as a lvgl.Line method")
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
        THROW_EXCEPTION("lvgl.Line.yInvert() must be called as a lvgl.Line method")
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

static JSValue js_lv_dropdown_set_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Dropdown.setText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Dropdown.setText() must be called as a lvgl.Dropdown method")
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
        THROW_EXCEPTION("lvgl.Dropdown.setOptions() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Dropdown.setOptions() must be called as a lvgl.Dropdown method")
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
        THROW_EXCEPTION("lvgl.Dropdown.setOptionsStatic() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Dropdown.setOptionsStatic() must be called as a lvgl.Dropdown method")
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
        THROW_EXCEPTION("lvgl.Dropdown.addOption() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Dropdown.addOption() must be called as a lvgl.Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * option = (char *)JS_ToCString(ctx, argv[0]) ;
    uint32_t pos ;
    if(JS_ToUint32(ctx, (uint32_t *) &pos, argv[1])!=0){
        THROW_EXCEPTION("arg pos of method lvgl.Dropdown.addOption() must be a number")
    }
    lv_dropdown_add_option(thisobj, option, pos) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, option) ;
    return retval ;
}

static JSValue js_lv_dropdown_clear_options(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Dropdown.clearOptions() must be called as a lvgl.Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_dropdown_clear_options(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_dropdown_set_selected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Dropdown.setSelected() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Dropdown.setSelected() must be called as a lvgl.Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t sel_opt ;
    if(JS_ToUint32(ctx, (uint32_t *) &sel_opt, argv[0])!=0){
        THROW_EXCEPTION("arg sel_opt of method lvgl.Dropdown.setSelected() must be a number")
    }
    lv_dropdown_set_selected(thisobj, sel_opt) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_dropdown_set_dir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Dropdown.setDir() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Dropdown.setDir() must be called as a lvgl.Dropdown method")
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
        THROW_EXCEPTION("lvgl.Dropdown.setSelectedHighlight() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Dropdown.setSelectedHighlight() must be called as a lvgl.Dropdown method")
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
        THROW_EXCEPTION("lvgl.Dropdown.text() must be called as a lvgl.Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_dropdown_get_text(thisobj)) ;
    return retval ;
}

static JSValue js_lv_dropdown_get_options(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Dropdown.options() must be called as a lvgl.Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_dropdown_get_options(thisobj)) ;
    return retval ;
}

static JSValue js_lv_dropdown_get_selected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Dropdown.selected() must be called as a lvgl.Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_dropdown_get_selected(thisobj)) ;
    return retval ;
}

static JSValue js_lv_dropdown_get_option_cnt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Dropdown.optionCnt() must be called as a lvgl.Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_dropdown_get_option_cnt(thisobj)) ;
    return retval ;
}

static JSValue js_lv_dropdown_get_selected_str(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Dropdown.selectedStr() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Dropdown.selectedStr() must be called as a lvgl.Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    char * buf = (char *)JS_ToCString(ctx, argv[0]) ;
    uint32_t buf_size ;
    if(JS_ToUint32(ctx, (uint32_t *) &buf_size, argv[1])!=0){
        THROW_EXCEPTION("arg buf_size of method lvgl.Dropdown.selectedStr() must be a number")
    }
    lv_dropdown_get_selected_str(thisobj, buf, buf_size) ;
    JSValue retval = JS_UNDEFINED ;
    JS_FreeCString(ctx, buf) ;
    return retval ;
}

static JSValue js_lv_dropdown_get_symbol(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Dropdown.symbol() must be called as a lvgl.Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewString(ctx,lv_dropdown_get_symbol(thisobj)) ;
    return retval ;
}

static JSValue js_lv_dropdown_get_selected_highlight(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Dropdown.selectedHighlight() must be called as a lvgl.Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_dropdown_get_selected_highlight(thisobj)) ;
    return retval ;
}

static JSValue js_lv_dropdown_get_dir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Dropdown.dir() must be called as a lvgl.Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = lv_dir_const_to_jsstr(ctx,lv_dropdown_get_dir(thisobj)) ;
    return retval ;
}

static JSValue js_lv_dropdown_open(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Dropdown.open() must be called as a lvgl.Dropdown method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    lv_dropdown_open(thisobj) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_dropdown_close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Dropdown.close() must be called as a lvgl.Dropdown method")
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

static JSValue js_lv_img_set_offset_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Img.setOffsetX() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Img.setOffsetX() must be called as a lvgl.Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method lvgl.Img.setOffsetX() must be a number")
    }
    lv_img_set_offset_x(thisobj, x) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_img_set_offset_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Img.setOffsetY() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Img.setOffsetY() must be called as a lvgl.Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[0])!=0){
        THROW_EXCEPTION("arg y of method lvgl.Img.setOffsetY() must be a number")
    }
    lv_img_set_offset_y(thisobj, y) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_img_set_angle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Img.setAngle() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Img.setAngle() must be called as a lvgl.Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t angle ;
    if(JS_ToInt32(ctx, (int32_t *) &angle, argv[0])!=0){
        THROW_EXCEPTION("arg angle of method lvgl.Img.setAngle() must be a number")
    }
    lv_img_set_angle(thisobj, angle) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_img_set_pivot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Img.setPivot() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Img.setPivot() must be called as a lvgl.Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method lvgl.Img.setPivot() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method lvgl.Img.setPivot() must be a number")
    }
    lv_img_set_pivot(thisobj, x, y) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_img_set_zoom(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Img.setZoom() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Img.setZoom() must be called as a lvgl.Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t zoom ;
    if(JS_ToUint32(ctx, (uint32_t *) &zoom, argv[0])!=0){
        THROW_EXCEPTION("arg zoom of method lvgl.Img.setZoom() must be a number")
    }
    lv_img_set_zoom(thisobj, zoom) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_img_set_antialias(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Img.setAntialias() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Img.setAntialias() must be called as a lvgl.Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    bool antialias = JS_ToBool(ctx, argv[0]) ;
    lv_img_set_antialias(thisobj, antialias) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_img_set_size_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Img.setSizeMode() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Img.setSizeMode() must be called as a lvgl.Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t mode ;
    if(JS_ToUint32(ctx, (uint32_t *) &mode, argv[0])!=0){
        THROW_EXCEPTION("arg mode of method lvgl.Img.setSizeMode() must be a number")
    }
    lv_img_set_size_mode(thisobj, mode) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_img_get_offset_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Img.offsetX() must be called as a lvgl.Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_img_get_offset_x(thisobj)) ;
    return retval ;
}

static JSValue js_lv_img_get_offset_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Img.offsetY() must be called as a lvgl.Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewInt32(ctx,lv_img_get_offset_y(thisobj)) ;
    return retval ;
}

static JSValue js_lv_img_get_angle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Img.angle() must be called as a lvgl.Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_img_get_angle(thisobj)) ;
    return retval ;
}

static JSValue js_lv_img_get_pivot(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.Img.pivot() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Img.pivot() must be called as a lvgl.Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg pivot of method lvgl.Img.pivot() must be a object{x,y}")
    }
    lv_point_t pivot ;
    JSValue jspivot_x = JS_GetPropertyStr(ctx, argv[0], "x") ;
    if(!JS_IsNumber(jspivot_x)){
        THROW_EXCEPTION("arg pivot of method lvgl.Img.pivot() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&pivot.x, jspivot_x)!=0) {
        THROW_EXCEPTION("property x of arg pivot is not a number")
    }
    JSValue jspivot_y = JS_GetPropertyStr(ctx, argv[0], "y") ;
    if(!JS_IsNumber(jspivot_y)){
        THROW_EXCEPTION("arg pivot of method lvgl.Img.pivot() missing property x, or is not a number")
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
        THROW_EXCEPTION("lvgl.Img.zoom() must be called as a lvgl.Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_img_get_zoom(thisobj)) ;
    return retval ;
}

static JSValue js_lv_img_get_antialias(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Img.antialias() must be called as a lvgl.Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_img_get_antialias(thisobj)) ;
    return retval ;
}

static JSValue js_lv_img_get_size_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Img.sizeMode() must be called as a lvgl.Img method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_img_get_size_mode(thisobj)) ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_img_proto_funcs[] = {
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

static JSValue js_lv_btnmatrix_get_popovers(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.BtnMatrix.popovers() must be called as a lvgl.BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewBool(ctx,lv_btnmatrix_get_popovers(thisobj)) ;
    return retval ;
}

static JSValue js_lv_btnmatrix_set_selected_btn(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.BtnMatrix.setSelectedBtn() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.BtnMatrix.setSelectedBtn() must be called as a lvgl.BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t btn_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &btn_id, argv[0])!=0){
        THROW_EXCEPTION("arg btn_id of method lvgl.BtnMatrix.setSelectedBtn() must be a number")
    }
    lv_btnmatrix_set_selected_btn(thisobj, btn_id) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_btnmatrix_set_btn_ctrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.BtnMatrix.setBtnCtrl() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.BtnMatrix.setBtnCtrl() must be called as a lvgl.BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t btn_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &btn_id, argv[0])!=0){
        THROW_EXCEPTION("arg btn_id of method lvgl.BtnMatrix.setBtnCtrl() must be a number")
    }
    uint8_t ctrl ;
    if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[1])!=0){
        THROW_EXCEPTION("arg ctrl of method lvgl.BtnMatrix.setBtnCtrl() must be a number")
    }
    lv_btnmatrix_set_btn_ctrl(thisobj, btn_id, ctrl) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_btnmatrix_clear_btn_ctrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.BtnMatrix.clearBtnCtrl() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.BtnMatrix.clearBtnCtrl() must be called as a lvgl.BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t btn_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &btn_id, argv[0])!=0){
        THROW_EXCEPTION("arg btn_id of method lvgl.BtnMatrix.clearBtnCtrl() must be a number")
    }
    uint8_t ctrl ;
    if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[1])!=0){
        THROW_EXCEPTION("arg ctrl of method lvgl.BtnMatrix.clearBtnCtrl() must be a number")
    }
    lv_btnmatrix_clear_btn_ctrl(thisobj, btn_id, ctrl) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_btnmatrix_set_btn_ctrl_all(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.BtnMatrix.setBtnCtrlAll() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.BtnMatrix.setBtnCtrlAll() must be called as a lvgl.BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t ctrl ;
    if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[0])!=0){
        THROW_EXCEPTION("arg ctrl of method lvgl.BtnMatrix.setBtnCtrlAll() must be a number")
    }
    lv_btnmatrix_set_btn_ctrl_all(thisobj, ctrl) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_btnmatrix_clear_btn_ctrl_all(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.BtnMatrix.clearBtnCtrlAll() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.BtnMatrix.clearBtnCtrlAll() must be called as a lvgl.BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t ctrl ;
    if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[0])!=0){
        THROW_EXCEPTION("arg ctrl of method lvgl.BtnMatrix.clearBtnCtrlAll() must be a number")
    }
    lv_btnmatrix_clear_btn_ctrl_all(thisobj, ctrl) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_btnmatrix_set_btn_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.BtnMatrix.setBtnWidth() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.BtnMatrix.setBtnWidth() must be called as a lvgl.BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t btn_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &btn_id, argv[0])!=0){
        THROW_EXCEPTION("arg btn_id of method lvgl.BtnMatrix.setBtnWidth() must be a number")
    }
    uint8_t width ;
    if(JS_ToUint32(ctx, (uint32_t *) &width, argv[1])!=0){
        THROW_EXCEPTION("arg width of method lvgl.BtnMatrix.setBtnWidth() must be a number")
    }
    lv_btnmatrix_set_btn_width(thisobj, btn_id, width) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_btnmatrix_set_one_checked(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.BtnMatrix.setOneChecked() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.BtnMatrix.setOneChecked() must be called as a lvgl.BtnMatrix method")
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
        THROW_EXCEPTION("lvgl.BtnMatrix.selectedBtn() must be called as a lvgl.BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    JSValue retval = JS_NewUint32(ctx,lv_btnmatrix_get_selected_btn(thisobj)) ;
    return retval ;
}

static JSValue js_lv_btnmatrix_get_btn_text(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<1) {
        THROW_EXCEPTION("lvgl.BtnMatrix.btnText() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.BtnMatrix.btnText() must be called as a lvgl.BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t btn_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &btn_id, argv[0])!=0){
        THROW_EXCEPTION("arg btn_id of method lvgl.BtnMatrix.btnText() must be a number")
    }
    JSValue retval = JS_NewString(ctx,lv_btnmatrix_get_btn_text(thisobj, btn_id)) ;
    return retval ;
}

static JSValue js_lv_btnmatrix_has_btn_ctrl(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.BtnMatrix.hasBtnCtrl() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.BtnMatrix.hasBtnCtrl() must be called as a lvgl.BtnMatrix method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t btn_id ;
    if(JS_ToUint32(ctx, (uint32_t *) &btn_id, argv[0])!=0){
        THROW_EXCEPTION("arg btn_id of method lvgl.BtnMatrix.hasBtnCtrl() must be a number")
    }
    uint8_t ctrl ;
    if(JS_ToUint32(ctx, (uint32_t *) &ctrl, argv[1])!=0){
        THROW_EXCEPTION("arg ctrl of method lvgl.BtnMatrix.hasBtnCtrl() must be a number")
    }
    JSValue retval = JS_NewBool(ctx,lv_btnmatrix_has_btn_ctrl(thisobj, btn_id, ctrl)) ;
    return retval ;
}

static JSValue js_lv_btnmatrix_get_one_checked(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.BtnMatrix.oneChecked() must be called as a lvgl.BtnMatrix method")
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

static JSValue js_lv_canvas_set_px_color(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("lvgl.Canvas.setPxColor() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Canvas.setPxColor() must be called as a lvgl.Canvas method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method lvgl.Canvas.setPxColor() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method lvgl.Canvas.setPxColor() must be a number")
    }
    uint16_t c ;
    if(JS_ToUint32(ctx, (uint32_t *) &c, argv[2])!=0){
        THROW_EXCEPTION("arg c of method lvgl.Canvas.setPxColor() must be a number")
    }
    lv_canvas_set_px_color(thisobj, x, y, (lv_color_t)c) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_canvas_set_px(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("lvgl.Canvas.setPx() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Canvas.setPx() must be called as a lvgl.Canvas method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method lvgl.Canvas.setPx() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method lvgl.Canvas.setPx() must be a number")
    }
    uint16_t c ;
    if(JS_ToUint32(ctx, (uint32_t *) &c, argv[2])!=0){
        THROW_EXCEPTION("arg c of method lvgl.Canvas.setPx() must be a number")
    }
    lv_canvas_set_px(thisobj, x, y, (lv_color_t)c) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_canvas_set_px_opa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<3) {
        THROW_EXCEPTION("lvgl.Canvas.setPxOpa() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Canvas.setPxOpa() must be called as a lvgl.Canvas method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    int16_t x ;
    if(JS_ToInt32(ctx, (int32_t *) &x, argv[0])!=0){
        THROW_EXCEPTION("arg x of method lvgl.Canvas.setPxOpa() must be a number")
    }
    int16_t y ;
    if(JS_ToInt32(ctx, (int32_t *) &y, argv[1])!=0){
        THROW_EXCEPTION("arg y of method lvgl.Canvas.setPxOpa() must be a number")
    }
    uint8_t opa ;
    if(JS_ToUint32(ctx, (uint32_t *) &opa, argv[2])!=0){
        THROW_EXCEPTION("arg opa of method lvgl.Canvas.setPxOpa() must be a number")
    }
    lv_canvas_set_px_opa(thisobj, x, y, opa) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_canvas_set_palette(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Canvas.setPalette() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Canvas.setPalette() must be called as a lvgl.Canvas method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint8_t id ;
    if(JS_ToUint32(ctx, (uint32_t *) &id, argv[0])!=0){
        THROW_EXCEPTION("arg id of method lvgl.Canvas.setPalette() must be a number")
    }
    uint16_t c ;
    if(JS_ToUint32(ctx, (uint32_t *) &c, argv[1])!=0){
        THROW_EXCEPTION("arg c of method lvgl.Canvas.setPalette() must be a number")
    }
    lv_canvas_set_palette(thisobj, id, (lv_color_t)c) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_canvas_blur_hor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Canvas.blurHor() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Canvas.blurHor() must be called as a lvgl.Canvas method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg area of method lvgl.Canvas.blurHor() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t area ;
    JSValue jsarea_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jsarea_x1)){
        THROW_EXCEPTION("arg area of method lvgl.Canvas.blurHor() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x1, jsarea_x1)!=0) {
        THROW_EXCEPTION("property x1 of arg area is not a number")
    }
    JSValue jsarea_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jsarea_y1)){
        THROW_EXCEPTION("arg area of method lvgl.Canvas.blurHor() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y1, jsarea_y1)!=0) {
        THROW_EXCEPTION("property y1 of arg area is not a number")
    }
    JSValue jsarea_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jsarea_x2)){
        THROW_EXCEPTION("arg area of method lvgl.Canvas.blurHor() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x2, jsarea_x2)!=0) {
        THROW_EXCEPTION("property x2 of arg area is not a number")
    }
    JSValue jsarea_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jsarea_y2)){
        THROW_EXCEPTION("arg area of method lvgl.Canvas.blurHor() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y2, jsarea_y2)!=0) {
        THROW_EXCEPTION("property y2 of arg area is not a number")
    }
    uint16_t r ;
    if(JS_ToUint32(ctx, (uint32_t *) &r, argv[1])!=0){
        THROW_EXCEPTION("arg r of method lvgl.Canvas.blurHor() must be a number")
    }
    lv_canvas_blur_hor(thisobj, &area, r) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_canvas_blur_ver(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Canvas.blurVer() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Canvas.blurVer() must be called as a lvgl.Canvas method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    if(!JS_IsObject(argv[0])){
        THROW_EXCEPTION("arg area of method lvgl.Canvas.blurVer() must be a object{x1,y1,x2,y2}")
    }
    lv_area_t area ;
    JSValue jsarea_x1 = JS_GetPropertyStr(ctx, argv[0], "x1") ;
    if(!JS_IsNumber(jsarea_x1)){
        THROW_EXCEPTION("arg area of method lvgl.Canvas.blurVer() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x1, jsarea_x1)!=0) {
        THROW_EXCEPTION("property x1 of arg area is not a number")
    }
    JSValue jsarea_y1 = JS_GetPropertyStr(ctx, argv[0], "y1") ;
    if(!JS_IsNumber(jsarea_y1)){
        THROW_EXCEPTION("arg area of method lvgl.Canvas.blurVer() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y1, jsarea_y1)!=0) {
        THROW_EXCEPTION("property y1 of arg area is not a number")
    }
    JSValue jsarea_x2 = JS_GetPropertyStr(ctx, argv[0], "x2") ;
    if(!JS_IsNumber(jsarea_x2)){
        THROW_EXCEPTION("arg area of method lvgl.Canvas.blurVer() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.x2, jsarea_x2)!=0) {
        THROW_EXCEPTION("property x2 of arg area is not a number")
    }
    JSValue jsarea_y2 = JS_GetPropertyStr(ctx, argv[0], "y2") ;
    if(!JS_IsNumber(jsarea_y2)){
        THROW_EXCEPTION("arg area of method lvgl.Canvas.blurVer() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&area.y2, jsarea_y2)!=0) {
        THROW_EXCEPTION("property y2 of arg area is not a number")
    }
    uint16_t r ;
    if(JS_ToUint32(ctx, (uint32_t *) &r, argv[1])!=0){
        THROW_EXCEPTION("arg r of method lvgl.Canvas.blurVer() must be a number")
    }
    lv_canvas_blur_ver(thisobj, &area, r) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}

static JSValue js_lv_canvas_fill_bg(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(argc<2) {
        THROW_EXCEPTION("lvgl.Canvas.fillBg() missing arg")
    }
    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;
    if(!lv_userdata) {
        THROW_EXCEPTION("lvgl.Canvas.fillBg() must be called as a lvgl.Canvas method")
    }
    lv_obj_t * thisobj = lv_userdata ;
    uint16_t color ;
    if(JS_ToUint32(ctx, (uint32_t *) &color, argv[0])!=0){
        THROW_EXCEPTION("arg color of method lvgl.Canvas.fillBg() must be a number")
    }
    uint8_t opa ;
    if(JS_ToUint32(ctx, (uint32_t *) &opa, argv[1])!=0){
        THROW_EXCEPTION("arg opa of method lvgl.Canvas.fillBg() must be a number")
    }
    lv_canvas_fill_bg(thisobj, (lv_color_t)color, opa) ;
    JSValue retval = JS_UNDEFINED ;
    return retval ;
}


static const JSCFunctionListEntry js_lv_canvas_proto_funcs[] = {
    JS_CFUNC_DEF("setPxColor", 0, js_lv_canvas_set_px_color),
    JS_CFUNC_DEF("setPx", 0, js_lv_canvas_set_px),
    JS_CFUNC_DEF("setPxOpa", 0, js_lv_canvas_set_px_opa),
    JS_CFUNC_DEF("setPalette", 0, js_lv_canvas_set_palette),
    JS_CFUNC_DEF("blurHor", 0, js_lv_canvas_blur_hor),
    JS_CFUNC_DEF("blurVer", 0, js_lv_canvas_blur_ver),
    JS_CFUNC_DEF("fillBg", 0, js_lv_canvas_fill_bg),
} ;
#define __def_js_lv_canvas_proto_funcs__

// AUTO GENERATE CODE END [METHOD LIST] --------



JSValue js_def_class(
        JSContext *ctx,
        const char * className,
        JSClassID js_class_id,
        JSClassDef * js_class_p,
        const char * cotrName,
        JSCFunction cotr,
        const JSCFunctionListEntry* funclst,
        uint16_t funcs,
        JSValue parentProto ,
        JSValue pkg
) {    
    JS_NewClass(JS_GetRuntime(ctx), js_class_id, js_class_p);

    JSValue proto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, proto, funclst, funcs);
    JS_SetClassProto(ctx, js_class_id, proto);
    JS_SetPropertyStr(ctx, proto, "__proto__", parentProto);

    JSValue jscotr = JS_NewCFunction2(ctx, cotr, cotrName, 1, JS_CFUNC_constructor, 0) ;
    JS_SetConstructor(ctx, jscotr, proto) ;

    JS_SetPropertyStr(ctx, pkg, className, jscotr);

    return proto ;
}



void require_vlgl_js_widgets(JSContext *ctx, JSValue lvgl) {

    JSValue EventEmitterProto = js_get_glob_prop(ctx, 3, "beapi", "EventEmitter", "prototype") ;

// AUTO GENERATE CODE START [REGISTER CLASS] --------
    // define js class lvgl.lv_obj    
    JSValue proto_lv_obj = js_def_class(ctx, "Obj", js_lv_obj_class_id, &js_lv_obj_class
                , "lvgl.Obj", js_lv_obj_constructor, js_lv_obj_proto_funcs, countof(js_lv_obj_proto_funcs), EventEmitterProto, lvgl) ;

    // define js class lvgl.lv_label    
    JSValue proto_lv_label = js_def_class(ctx, "Label", js_lv_label_class_id, &js_lv_label_class
                , "lvgl.Label", js_lv_label_constructor, js_lv_label_proto_funcs, countof(js_lv_label_proto_funcs), proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_arc    
    JSValue proto_lv_arc = js_def_class(ctx, "Arc", js_lv_arc_class_id, &js_lv_arc_class
                , "lvgl.Arc", js_lv_arc_constructor, js_lv_arc_proto_funcs, countof(js_lv_arc_proto_funcs), proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_bar    
    JSValue proto_lv_bar = js_def_class(ctx, "Bar", js_lv_bar_class_id, &js_lv_bar_class
                , "lvgl.Bar", js_lv_bar_constructor, js_lv_bar_proto_funcs, countof(js_lv_bar_proto_funcs), proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_btn    
    JSValue proto_lv_btn = js_def_class(ctx, "Btn", js_lv_btn_class_id, &js_lv_btn_class
                , "lvgl.Btn", js_lv_btn_constructor, js_lv_btn_proto_funcs, countof(js_lv_btn_proto_funcs), proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_btnmatrix    
    JSValue proto_lv_btnmatrix = js_def_class(ctx, "BtnMatrix", js_lv_btnmatrix_class_id, &js_lv_btnmatrix_class
                , "lvgl.BtnMatrix", js_lv_btnmatrix_constructor, js_lv_btnmatrix_proto_funcs, countof(js_lv_btnmatrix_proto_funcs), proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_canvas    
    JSValue proto_lv_canvas = js_def_class(ctx, "Canvas", js_lv_canvas_class_id, &js_lv_canvas_class
                , "lvgl.Canvas", js_lv_canvas_constructor, js_lv_canvas_proto_funcs, countof(js_lv_canvas_proto_funcs), proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_checkbox    
    JSValue proto_lv_checkbox = js_def_class(ctx, "Checkbox", js_lv_checkbox_class_id, &js_lv_checkbox_class
                , "lvgl.Checkbox", js_lv_checkbox_constructor, js_lv_checkbox_proto_funcs, countof(js_lv_checkbox_proto_funcs), proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_dropdown    
    JSValue proto_lv_dropdown = js_def_class(ctx, "Dropdown", js_lv_dropdown_class_id, &js_lv_dropdown_class
                , "lvgl.Dropdown", js_lv_dropdown_constructor, js_lv_dropdown_proto_funcs, countof(js_lv_dropdown_proto_funcs), proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_img    
    JSValue proto_lv_img = js_def_class(ctx, "Img", js_lv_img_class_id, &js_lv_img_class
                , "lvgl.Img", js_lv_img_constructor, js_lv_img_proto_funcs, countof(js_lv_img_proto_funcs), proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_line    
    JSValue proto_lv_line = js_def_class(ctx, "Line", js_lv_line_class_id, &js_lv_line_class
                , "lvgl.Line", js_lv_line_constructor, js_lv_line_proto_funcs, countof(js_lv_line_proto_funcs), proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_roller    
    JSValue proto_lv_roller = js_def_class(ctx, "Roller", js_lv_roller_class_id, &js_lv_roller_class
                , "lvgl.Roller", js_lv_roller_constructor, js_lv_roller_proto_funcs, countof(js_lv_roller_proto_funcs), proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_slider    
    JSValue proto_lv_slider = js_def_class(ctx, "Slider", js_lv_slider_class_id, &js_lv_slider_class
                , "lvgl.Slider", js_lv_slider_constructor, js_lv_slider_proto_funcs, countof(js_lv_slider_proto_funcs), proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_switch    
    JSValue proto_lv_switch = js_def_class(ctx, "Switch", js_lv_switch_class_id, &js_lv_switch_class
                , "lvgl.Switch", js_lv_switch_constructor, js_lv_switch_proto_funcs, countof(js_lv_switch_proto_funcs), proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_table    
    JSValue proto_lv_table = js_def_class(ctx, "Table", js_lv_table_class_id, &js_lv_table_class
                , "lvgl.Table", js_lv_table_constructor, js_lv_table_proto_funcs, countof(js_lv_table_proto_funcs), proto_lv_obj, lvgl) ;

    // define js class lvgl.lv_textarea    
    JSValue proto_lv_textarea = js_def_class(ctx, "TextArea", js_lv_textarea_class_id, &js_lv_textarea_class
                , "lvgl.TextArea", js_lv_textarea_constructor, js_lv_textarea_proto_funcs, countof(js_lv_textarea_proto_funcs), proto_lv_obj, lvgl) ;

// AUTO GENERATE CODE END [REGISTER CLASS] --------

    JS_Eval(ctx, (const char *)lv_obj_init_js, lv_obj_init_js_len, "[native]lv_obj_init", JS_EVAL_TYPE_GLOBAL|JS_EVAL_FLAG_STRIP) ;

    JS_FreeValue(ctx, EventEmitterProto);
}





void vlgl_js_widgets_init() {
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
// AUTO GENERATE CODE END [REGISTER CLASS ID] --------
}

JSClassID lv_obj_js_class_id() {
    return js_lv_obj_class_id ;
}