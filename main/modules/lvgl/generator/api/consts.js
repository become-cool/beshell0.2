

function parseCSource(src) {
    let array = []
    for(let res of [...src.matchAll(/\s*([\w\-_]+)(\s*=\s*[\dA-Fa-fxX]+)\s*\,?/g)]) {
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
        enum {
            LV_BORDER_SIDE_NONE     = 0x00,
            LV_BORDER_SIDE_BOTTOM   = 0x01,
            LV_BORDER_SIDE_TOP      = 0x02,
            LV_BORDER_SIDE_LEFT     = 0x04,
            LV_BORDER_SIDE_RIGHT    = 0x08,
            LV_BORDER_SIDE_FULL     = 0x0F,
            LV_BORDER_SIDE_INTERNAL = 0x10, /**< FOR matrix-like objects (e.g. Button matrix)*/
        };
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
        typedef enum {
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
        } lv_style_prop_t;
        `)
        
    }



    

]


function constC2JS(src, prefix) {
    for(let res of [...src.matchAll(/\s*([\w\-_]+)\s*=\s*([\dA-Fa-fxX]+)/g)]) {
        let name = res[1].substr(prefix.length).toLowerCase()
        console.log("'"+name+"':",res[2], ",")
    }
}
