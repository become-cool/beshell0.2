

function parseCSource(src) {

    

    let array = []
    for(let res of [...src.matchAll(/\s*([\w\-_]+)(\s*=\s*[^\,]+)\s*\,/g)]) {
        array.push(res[1])
    }
    return array
}

module.exports = [
    {
        prefix: 'LV_FLEX_FLOW_' ,
        type: 'lv_flex_flow_t' ,
        name: 'lv_flex_flow' ,
        end: 'LV_FLEX_FLOW_COLUMN_WRAP_REVERSE+1' ,
        def: [
            "LV_FLEX_FLOW_ROW" ,
            "LV_FLEX_FLOW_COLUMN" ,
            "LV_FLEX_FLOW_ROW_WRAP" ,
            "LV_FLEX_FLOW_ROW_REVERSE" ,
            "LV_FLEX_FLOW_ROW_WRAP_REVERSE" ,
            "LV_FLEX_FLOW_COLUMN_WRAP" ,
            "LV_FLEX_FLOW_COLUMN_REVERSE" ,
            "LV_FLEX_FLOW_COLUMN_WRAP_REVERSE"
        ] ,
    }

    , {
        prefix: 'LV_FLEX_ALIGN_' ,
        type: 'lv_flex_align_t' ,
        name: 'lv_flex_align' ,
        end: 'LV_FLEX_ALIGN_SPACE_BETWEEN+1' ,
        def: [
            "LV_FLEX_ALIGN_START",
            "LV_FLEX_ALIGN_END",
            "LV_FLEX_ALIGN_CENTER",
            "LV_FLEX_ALIGN_SPACE_EVENLY",
            "LV_FLEX_ALIGN_SPACE_AROUND",
            "LV_FLEX_ALIGN_SPACE_BETWEEN"
        ]
    }

    , {
        prefix: 'LV_EVENT_' ,
        type: 'lv_event_code_t' ,
        name: 'lv_event_code' ,
        end: '_LV_EVENT_LAST' ,
        def: [
            "LV_EVENT_ALL",
            "LV_EVENT_PRESSED",
            "LV_EVENT_PRESSING",
            "LV_EVENT_PRESS_LOST",
            "LV_EVENT_SHORT_CLICKED",
            "LV_EVENT_LONG_PRESSED",
            "LV_EVENT_LONG_PRESSED_REPEAT",
            "LV_EVENT_CLICKED",
            "LV_EVENT_RELEASED",
            "LV_EVENT_SCROLL_BEGIN",
            "LV_EVENT_SCROLL_END",
            "LV_EVENT_SCROLL",
            "LV_EVENT_GESTURE",
            "LV_EVENT_KEY",
            "LV_EVENT_FOCUSED",
            "LV_EVENT_DEFOCUSED",
            "LV_EVENT_LEAVE",
            "LV_EVENT_HIT_TEST",
            "LV_EVENT_COVER_CHECK",
            "LV_EVENT_REFR_EXT_DRAW_SIZE",
            "LV_EVENT_DRAW_MAIN_BEGIN",
            "LV_EVENT_DRAW_MAIN",
            "LV_EVENT_DRAW_MAIN_END",
            "LV_EVENT_DRAW_POST_BEGIN",
            "LV_EVENT_DRAW_POST",
            "LV_EVENT_DRAW_POST_END",
            "LV_EVENT_DRAW_PART_BEGIN",
            "LV_EVENT_DRAW_PART_END",
            "LV_EVENT_VALUE_CHANGED",
            "LV_EVENT_INSERT",
            "LV_EVENT_REFRESH",
            "LV_EVENT_READY",
            "LV_EVENT_CANCEL",
            "LV_EVENT_DELETE",
            "LV_EVENT_CHILD_CHANGED",
            "LV_EVENT_CHILD_CREATED",
            "LV_EVENT_CHILD_DELETED",
            "LV_EVENT_SCREEN_UNLOAD_START",
            "LV_EVENT_SCREEN_LOAD_START",
            "LV_EVENT_SCREEN_LOADED",
            "LV_EVENT_SCREEN_UNLOADED",
            "LV_EVENT_SIZE_CHANGED",
            "LV_EVENT_STYLE_CHANGED",
            "LV_EVENT_LAYOUT_CHANGED",
            "LV_EVENT_GET_SELF_SIZE",
        ]
    }


    , {
        prefix: 'LV_DIR_' ,
        type: 'lv_dir_t' ,
        name: 'lv_dir' ,
        end: 'LV_DIR_ALL+1' ,
        def: [
            "LV_DIR_NONE",
            "LV_DIR_LEFT",
            "LV_DIR_RIGHT",
            "LV_DIR_TOP",
            "LV_DIR_BOTTOM",
            "LV_DIR_HOR",
            "LV_DIR_VER",
            "LV_DIR_ALL",
        ]
    }

    , {
        prefix: 'LV_ALIGN_' ,
        type: 'lv_align_t' ,
        name: 'lv_align' ,
        end: 'LV_ALIGN_OUT_RIGHT_BOTTOM+1' ,
        def: [
            "LV_ALIGN_DEFAULT",
            "LV_ALIGN_TOP_LEFT",
            "LV_ALIGN_TOP_MID",
            "LV_ALIGN_TOP_RIGHT",
            "LV_ALIGN_BOTTOM_LEFT",
            "LV_ALIGN_BOTTOM_MID",
            "LV_ALIGN_BOTTOM_RIGHT",
            "LV_ALIGN_LEFT_MID",
            "LV_ALIGN_RIGHT_MID",
            "LV_ALIGN_CENTER",

            "LV_ALIGN_OUT_TOP_LEFT",
            "LV_ALIGN_OUT_TOP_MID",
            "LV_ALIGN_OUT_TOP_RIGHT",
            "LV_ALIGN_OUT_BOTTOM_LEFT",
            "LV_ALIGN_OUT_BOTTOM_MID",
            "LV_ALIGN_OUT_BOTTOM_RIGHT",
            "LV_ALIGN_OUT_LEFT_TOP",
            "LV_ALIGN_OUT_LEFT_MID",
            "LV_ALIGN_OUT_LEFT_BOTTOM",
            "LV_ALIGN_OUT_RIGHT_TOP",
            "LV_ALIGN_OUT_RIGHT_MID",
            "LV_ALIGN_OUT_RIGHT_BOTTOM",
        ]
    }

    ,{
        prefix: 'LV_BORDER_SIDE_' ,
        type: 'lv_border_side_t' ,
        name: 'lv_border_side' ,
        end: 'LV_BORDER_SIDE_INTERNAL+1' ,
        def: parseCSource(`
            LV_BORDER_SIDE_NONE     = 0x00,
            LV_BORDER_SIDE_BOTTOM   = 0x01,
            LV_BORDER_SIDE_TOP      = 0x02,
            LV_BORDER_SIDE_LEFT     = 0x04,
            LV_BORDER_SIDE_RIGHT    = 0x08,
            LV_BORDER_SIDE_FULL     = 0x0F,
            LV_BORDER_SIDE_INTERNAL = 0x10, /**< FOR matrix-like objects (e.g. Button matrix)*/
        `)
        
    }

    ,{
        prefix: 'LV_BASE_DIR_' ,
        type: 'lv_base_dir_t' ,
        name: 'lv_base_dir' ,
        end: 'LV_BASE_DIR_WEAK+1' ,
        def: parseCSource(`
        enum {
            LV_BASE_DIR_LTR      = 0x00,
            LV_BASE_DIR_RTL      = 0x01,
            LV_BASE_DIR_AUTO     = 0x02,
        
            LV_BASE_DIR_NEUTRAL  = 0x20,
            LV_BASE_DIR_WEAK     = 0x21,
        };
        `)
        
    }


    , {
        prefix: 'LV_STYLE_' ,
        type: 'lv_style_prop_t' ,
        name: 'lv_style_prop' ,
        end: '_LV_STYLE_LAST_BUILT_IN_PROP' ,
        def: parseCSource(`
            LV_STYLE_PROP_INV                = 0,
        
            /*Group 0*/
            LV_STYLE_WIDTH                   = 1 | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_MIN_WIDTH               = 2 | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_MAX_WIDTH               = 3 | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_HEIGHT                  = 4 | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_MIN_HEIGHT              = 5 | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_MAX_HEIGHT              = 6 | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_X                       = 7 | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_Y                       = 8 | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_ALIGN                   = 9 | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_TRANSFORM_WIDTH         = 10 | LV_STYLE_PROP_EXT_DRAW,
            LV_STYLE_TRANSFORM_HEIGHT        = 11 | LV_STYLE_PROP_EXT_DRAW,
            LV_STYLE_TRANSLATE_X             = 12 | LV_STYLE_PROP_LAYOUT_REFR | LV_STYLE_PROP_PARENT_LAYOUT_REFR,
            LV_STYLE_TRANSLATE_Y             = 13 | LV_STYLE_PROP_LAYOUT_REFR | LV_STYLE_PROP_PARENT_LAYOUT_REFR,
            LV_STYLE_TRANSFORM_ZOOM          = 14 | LV_STYLE_PROP_EXT_DRAW | LV_STYLE_PROP_LAYOUT_REFR | LV_STYLE_PROP_PARENT_LAYOUT_REFR,
            LV_STYLE_TRANSFORM_ANGLE         = 15 | LV_STYLE_PROP_EXT_DRAW | LV_STYLE_PROP_LAYOUT_REFR | LV_STYLE_PROP_PARENT_LAYOUT_REFR,
        
            /*Group 1*/
            LV_STYLE_PAD_TOP                 = 16 | LV_STYLE_PROP_EXT_DRAW | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_PAD_BOTTOM              = 17 | LV_STYLE_PROP_EXT_DRAW | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_PAD_LEFT                = 18 | LV_STYLE_PROP_EXT_DRAW | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_PAD_RIGHT               = 19 | LV_STYLE_PROP_EXT_DRAW | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_PAD_ROW                 = 20 | LV_STYLE_PROP_EXT_DRAW | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_PAD_COLUMN              = 21 | LV_STYLE_PROP_EXT_DRAW | LV_STYLE_PROP_LAYOUT_REFR,
        
            /*Group 2*/
            LV_STYLE_BG_COLOR                = 32,
            LV_STYLE_BG_COLOR_FILTERED       = 32 | LV_STYLE_PROP_FILTER,
            LV_STYLE_BG_OPA                  = 33,
            LV_STYLE_BG_GRAD_COLOR           = 34,
            LV_STYLE_BG_GRAD_COLOR_FILTERED  = 34 | LV_STYLE_PROP_FILTER,
            LV_STYLE_BG_GRAD_DIR             = 35,
            LV_STYLE_BG_MAIN_STOP            = 36,
            LV_STYLE_BG_GRAD_STOP            = 37,
        
            LV_STYLE_BG_IMG_SRC              = 38 | LV_STYLE_PROP_EXT_DRAW,
            LV_STYLE_BG_IMG_OPA              = 39,
            LV_STYLE_BG_IMG_RECOLOR          = 40,
            LV_STYLE_BG_IMG_RECOLOR_FILTERED = 40 | LV_STYLE_PROP_FILTER,
            LV_STYLE_BG_IMG_RECOLOR_OPA      = 41,
            LV_STYLE_BG_IMG_TILED            = 42,
        
            /*Group 3*/
            LV_STYLE_BORDER_COLOR            = 48,
            LV_STYLE_BORDER_COLOR_FILTERED   = 48 | LV_STYLE_PROP_FILTER,
            LV_STYLE_BORDER_OPA              = 49,
            LV_STYLE_BORDER_WIDTH            = 50 | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_BORDER_SIDE             = 51,
            LV_STYLE_BORDER_POST             = 52,
        
            LV_STYLE_OUTLINE_WIDTH           = 58 | LV_STYLE_PROP_EXT_DRAW,
            LV_STYLE_OUTLINE_COLOR           = 59,
            LV_STYLE_OUTLINE_COLOR_FILTERED  = 59 | LV_STYLE_PROP_FILTER,
            LV_STYLE_OUTLINE_OPA             = 60 | LV_STYLE_PROP_EXT_DRAW,
            LV_STYLE_OUTLINE_PAD             = 61 | LV_STYLE_PROP_EXT_DRAW,
        
            /*Group 4*/
            LV_STYLE_SHADOW_WIDTH            = 64 | LV_STYLE_PROP_EXT_DRAW,
            LV_STYLE_SHADOW_OFS_X            = 65 | LV_STYLE_PROP_EXT_DRAW,
            LV_STYLE_SHADOW_OFS_Y            = 66 | LV_STYLE_PROP_EXT_DRAW,
            LV_STYLE_SHADOW_SPREAD           = 67 | LV_STYLE_PROP_EXT_DRAW,
            LV_STYLE_SHADOW_COLOR            = 68,
            LV_STYLE_SHADOW_COLOR_FILTERED   = 68 | LV_STYLE_PROP_FILTER,
            LV_STYLE_SHADOW_OPA              = 69 | LV_STYLE_PROP_EXT_DRAW,
        
            LV_STYLE_IMG_OPA                 = 70,
            LV_STYLE_IMG_RECOLOR             = 71,
            LV_STYLE_IMG_RECOLOR_FILTERED    = 71 | LV_STYLE_PROP_FILTER,
            LV_STYLE_IMG_RECOLOR_OPA         = 72,
        
            LV_STYLE_LINE_WIDTH              = 73 | LV_STYLE_PROP_EXT_DRAW,
            LV_STYLE_LINE_DASH_WIDTH         = 74,
            LV_STYLE_LINE_DASH_GAP           = 75,
            LV_STYLE_LINE_ROUNDED            = 76,
            LV_STYLE_LINE_COLOR              = 77,
            LV_STYLE_LINE_COLOR_FILTERED     = 77 | LV_STYLE_PROP_FILTER,
            LV_STYLE_LINE_OPA                = 78,
        
            /*Group 5*/
            LV_STYLE_ARC_WIDTH               = 80 | LV_STYLE_PROP_EXT_DRAW,
            LV_STYLE_ARC_ROUNDED             = 81,
            LV_STYLE_ARC_COLOR               = 82,
            LV_STYLE_ARC_COLOR_FILTERED      = 82 | LV_STYLE_PROP_FILTER,
            LV_STYLE_ARC_OPA                 = 83,
            LV_STYLE_ARC_IMG_SRC             = 84,
        
            LV_STYLE_TEXT_COLOR              = 87 | LV_STYLE_PROP_INHERIT,
            LV_STYLE_TEXT_COLOR_FILTERED     = 87 | LV_STYLE_PROP_INHERIT | LV_STYLE_PROP_FILTER,
            LV_STYLE_TEXT_OPA                = 88 | LV_STYLE_PROP_INHERIT,
            LV_STYLE_TEXT_FONT               = 89 | LV_STYLE_PROP_INHERIT | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_TEXT_LETTER_SPACE       = 90 | LV_STYLE_PROP_INHERIT | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_TEXT_LINE_SPACE         = 91 | LV_STYLE_PROP_INHERIT | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_TEXT_DECOR              = 92 | LV_STYLE_PROP_INHERIT,
            LV_STYLE_TEXT_ALIGN              = 93 | LV_STYLE_PROP_INHERIT | LV_STYLE_PROP_LAYOUT_REFR,
        
            /*Group 6*/
            LV_STYLE_RADIUS                  = 96,
            LV_STYLE_CLIP_CORNER             = 97,
            LV_STYLE_OPA                     = 98 | LV_STYLE_PROP_INHERIT,
            LV_STYLE_COLOR_FILTER_DSC        = 99,
            LV_STYLE_COLOR_FILTER_OPA        = 100,
            LV_STYLE_ANIM_TIME               = 101,
            LV_STYLE_ANIM_SPEED              = 102,
            LV_STYLE_TRANSITION              = 103,
            LV_STYLE_BLEND_MODE              = 104,
            LV_STYLE_LAYOUT                  = 105 | LV_STYLE_PROP_LAYOUT_REFR,
            LV_STYLE_BASE_DIR                = 106 | LV_STYLE_PROP_INHERIT | LV_STYLE_PROP_LAYOUT_REFR,
        
            _LV_STYLE_LAST_BUILT_IN_PROP     = 111,
        
            LV_STYLE_PROP_ANY                = 0xFFFF
        `)
        , vars: [
            'LV_STYLE_FLEX_FLOW' ,
            'LV_STYLE_FLEX_MAIN_PLACE' ,
            'LV_STYLE_FLEX_CROSS_PLACE' ,
            'LV_STYLE_FLEX_TRACK_PLACE' ,
            'LV_STYLE_FLEX_GROW' ,
        ]
    }

    , {
        prefix: 'LV_SCR_LOAD_ANIM_' ,
        type: 'lv_scr_load_anim_t' ,
        name: 'lv_scr_load_anim' ,
        def: [
            'LV_SCR_LOAD_ANIM_NONE' ,
            'LV_SCR_LOAD_ANIM_OVER_LEFT' ,
            'LV_SCR_LOAD_ANIM_OVER_RIGHT' ,
            'LV_SCR_LOAD_ANIM_OVER_TOP' ,
            'LV_SCR_LOAD_ANIM_OVER_BOTTOM' ,
            'LV_SCR_LOAD_ANIM_MOVE_LEFT' ,
            'LV_SCR_LOAD_ANIM_MOVE_RIGHT' ,
            'LV_SCR_LOAD_ANIM_MOVE_TOP' ,
            'LV_SCR_LOAD_ANIM_MOVE_BOTTOM' ,
            'LV_SCR_LOAD_ANIM_FADE_ON' ,
        ]
    }


    , {
        prefix: 'LV_STATE_' ,
        type: 'lv_state_t' ,
        name: 'lv_state' ,
        def: parseCSource(`
        enum {
            LV_STATE_DEFAULT     =  0x0000,
            LV_STATE_CHECKED     =  0x0001,
            LV_STATE_FOCUSED     =  0x0002,
            LV_STATE_FOCUS_KEY   =  0x0004,
            LV_STATE_EDITED      =  0x0008,
            LV_STATE_HOVERED     =  0x0010,
            LV_STATE_PRESSED     =  0x0020,
            LV_STATE_SCROLLED    =  0x0040,
            LV_STATE_DISABLED    =  0x0080,
        
            LV_STATE_USER_1      =  0x1000,
            LV_STATE_USER_2      =  0x2000,
            LV_STATE_USER_3      =  0x4000,
            LV_STATE_USER_4      =  0x8000,
        
            LV_STATE_ANY = 0xFFFF,
        };
        `)
    }


    , {
        prefix: 'LV_OBJ_FLAG_' ,
        type: 'lv_obj_flag_t' ,
        name: 'lv_obj_flag' ,
        def: parseCSource(`
            LV_OBJ_FLAG_HIDDEN          = (1L << 0),  /**< Make the object hidden. (Like it wasn't there at all)*/
            LV_OBJ_FLAG_CLICKABLE       = (1L << 1),  /**< Make the object clickable by the input devices*/
            LV_OBJ_FLAG_CLICK_FOCUSABLE = (1L << 2),  /**< Add focused state to the object when clicked*/
            LV_OBJ_FLAG_CHECKABLE       = (1L << 3),  /**< Toggle checked state when the object is clicked*/
            LV_OBJ_FLAG_SCROLLABLE      = (1L << 4),  /**< Make the object scrollable*/
            LV_OBJ_FLAG_SCROLL_ELASTIC  = (1L << 5),  /**< Allow scrolling inside but with slower speed*/
            LV_OBJ_FLAG_SCROLL_MOMENTUM = (1L << 6),  /**< Make the object scroll further when "thrown"*/
            LV_OBJ_FLAG_SCROLL_ONE      = (1L << 7),  /**< Allow scrolling only one snappable children*/
            LV_OBJ_FLAG_SCROLL_CHAIN    = (1L << 8),  /**< Allow propagating the scroll to a parent*/
            LV_OBJ_FLAG_SCROLL_ON_FOCUS = (1L << 9),  /**< Automatically scroll object to make it visible when focused*/
            LV_OBJ_FLAG_SNAPPABLE       = (1L << 10), /**< If scroll snap is enabled on the parent it can snap to this object*/
            LV_OBJ_FLAG_PRESS_LOCK      = (1L << 11), /**< Keep the object pressed even if the press slid from the object*/
            LV_OBJ_FLAG_EVENT_BUBBLE    = (1L << 12), /**< Propagate the events to the parent too*/
            LV_OBJ_FLAG_GESTURE_BUBBLE  = (1L << 13), /**< Propagate the gestures to the parent*/
            LV_OBJ_FLAG_ADV_HITTEST     = (1L << 14), /**< Allow performing more accurate hit (click) test. E.g. consider rounded corners.*/
            LV_OBJ_FLAG_IGNORE_LAYOUT   = (1L << 15), /**< Make the object position-able by the layouts*/
            LV_OBJ_FLAG_FLOATING        = (1L << 16), /**< Do not scroll the object when the parent scrolls and ignore layout*/
        
            LV_OBJ_FLAG_LAYOUT_1        = (1L << 23), /**< Custom flag, free to use by layouts*/
            LV_OBJ_FLAG_LAYOUT_2        = (1L << 24), /**< Custom flag, free to use by layouts*/
        
            LV_OBJ_FLAG_WIDGET_1        = (1L << 25), /**< Custom flag, free to use by widget*/
            LV_OBJ_FLAG_WIDGET_2        = (1L << 26), /**< Custom flag, free to use by widget*/
            LV_OBJ_FLAG_USER_1          = (1L << 27), /**< Custom flag, free to use by user*/
            LV_OBJ_FLAG_USER_2          = (1L << 28), /**< Custom flag, free to use by user*/
            LV_OBJ_FLAG_USER_3          = (1L << 29), /**< Custom flag, free to use by user*/
            LV_OBJ_FLAG_USER_4          = (1L << 30), /**< Custom flag, free to use by user*/
        `)
    }

    ,{
        prefix: 'LV_TEXT_ALIGN_' ,
        type: 'lv_text_align_t' ,
        name: 'lv_text_align' ,
        end: 'LV_TEXT_ALIGN_RIGHT+1' ,
        def: [
            'LV_TEXT_ALIGN_AUTO' ,
            'LV_TEXT_ALIGN_LEFT' ,
            'LV_TEXT_ALIGN_CENTER' ,
            'LV_TEXT_ALIGN_RIGHT' ,
        ]   
    }

    ,{
        prefix: 'LV_PALETTE_' ,
        type: 'lv_palette_t' ,
        name: 'lv_palette' ,
        end: 'LV_PALETTE_NONE+1' ,
        def: [
            'LV_PALETTE_RED' ,
            'LV_PALETTE_PINK' ,
            'LV_PALETTE_PURPLE' ,
            'LV_PALETTE_DEEP_PURPLE' ,
            'LV_PALETTE_INDIGO' ,
            'LV_PALETTE_BLUE' ,
            'LV_PALETTE_LIGHT_BLUE' ,
            'LV_PALETTE_CYAN' ,
            'LV_PALETTE_TEAL' ,
            'LV_PALETTE_GREEN' ,
            'LV_PALETTE_LIGHT_GREEN' ,
            'LV_PALETTE_LIME' ,
            'LV_PALETTE_YELLOW' ,
            'LV_PALETTE_AMBER' ,
            'LV_PALETTE_ORANGE' ,
            'LV_PALETTE_DEEP_ORANGE' ,
            'LV_PALETTE_BROWN' ,
            'LV_PALETTE_BLUE_GREY' ,
            'LV_PALETTE_GREY' ,
            '_LV_PALETTE_LAST' ,
            'LV_PALETTE_NONE' ,
        ]   
    } 

    , {
        prefix: 'LV_LABEL_LONG_' ,
        type: 'lv_label_long_mode_t' ,
        name: 'lv_label_long_mode' ,
        end: 'LV_LABEL_LONG_CLIP+1' ,
        def: [
            'LV_LABEL_LONG_WRAP' ,              /**< Keep the object width, wrap the too long lines and expand the object height*/
            'LV_LABEL_LONG_DOT' ,               /**< Keep the size and write dots at the end if the text is too long*/
            'LV_LABEL_LONG_SCROLL' ,            /**< Keep the size and roll the text back and forth*/
            'LV_LABEL_LONG_SCROLL_CIRCULAR' ,   /**< Keep the size and roll the text circularly*/
            'LV_LABEL_LONG_CLIP' ,              /**< Keep the size and clip the text out of it*/
        ]   
    }
    , {
        prefix: 'LV_KEYBOARD_MODE_' ,
        type: 'lv_keyboard_mode_t' ,
        name: 'lv_keyboard_mode' ,
        end: 'LV_KEYBOARD_MODE_USER_4+1' ,
        def: [
            'LV_KEYBOARD_MODE_TEXT_LOWER' ,
            'LV_KEYBOARD_MODE_TEXT_UPPER' ,
            'LV_KEYBOARD_MODE_SPECIAL' ,
            'LV_KEYBOARD_MODE_NUMBER' ,
            'LV_KEYBOARD_MODE_USER_1' ,
            'LV_KEYBOARD_MODE_USER_2' ,
            'LV_KEYBOARD_MODE_USER_3' ,
            'LV_KEYBOARD_MODE_USER_4' ,
        ]   
    }

    , {
        prefix: 'LV_IMG_CF_' ,
        type: 'lv_img_cf_t' ,
        name: 'lv_img_cf' ,
        end: 'LV_IMG_CF_USER_ENCODED_7+1' ,
        def: [
            "LV_IMG_CF_UNKNOWN",

            "LV_IMG_CF_RAW",
            "LV_IMG_CF_RAW_ALPHA",
            "LV_IMG_CF_RAW_CHROMA_KEYED",

            "LV_IMG_CF_TRUE_COLOR",
            "LV_IMG_CF_TRUE_COLOR_ALPHA",
            "LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED",

            "LV_IMG_CF_INDEXED_1BIT",
            "LV_IMG_CF_INDEXED_2BIT",
            "LV_IMG_CF_INDEXED_4BIT",
            "LV_IMG_CF_INDEXED_8BIT",

            "LV_IMG_CF_ALPHA_1BIT",
            "LV_IMG_CF_ALPHA_2BIT",
            "LV_IMG_CF_ALPHA_4BIT",
            "LV_IMG_CF_ALPHA_8BIT",

            "LV_IMG_CF_RESERVED_15",
            "LV_IMG_CF_RESERVED_16",
            "LV_IMG_CF_RESERVED_17",
            "LV_IMG_CF_RESERVED_18",
            "LV_IMG_CF_RESERVED_19",
            "LV_IMG_CF_RESERVED_20",
            "LV_IMG_CF_RESERVED_21",
            "LV_IMG_CF_RESERVED_22",
            "LV_IMG_CF_RESERVED_23",

            "LV_IMG_CF_USER_ENCODED_0",
            "LV_IMG_CF_USER_ENCODED_1",
            "LV_IMG_CF_USER_ENCODED_2",
            "LV_IMG_CF_USER_ENCODED_3",
            "LV_IMG_CF_USER_ENCODED_4",
            "LV_IMG_CF_USER_ENCODED_5",
            "LV_IMG_CF_USER_ENCODED_6",
            "LV_IMG_CF_USER_ENCODED_7",
        ]
    }

    , {
        prefix: 'LV_SCROLL_SNAP_' ,
        type: 'lv_scroll_snap_t' ,
        name: 'lv_scroll_snap' ,
        end: 'LV_SCROLL_SNAP_CENTER+1' ,
        def: [
            'LV_SCROLL_SNAP_NONE' ,
            'LV_SCROLL_SNAP_START' ,
            'LV_SCROLL_SNAP_END' ,
            'LV_SCROLL_SNAP_CENTER' ,
        ]
    }
]


function constC2JS(src, prefix) {
    for(let res of [...src.matchAll(/\s*([\w\-_]+)\s*=\s*([\dA-Fa-fxX]+)/g)]) {
        let name = res[1].substr(prefix.length).toLowerCase()
        console.log("'"+name+"':",res[2], ",")
    }
}