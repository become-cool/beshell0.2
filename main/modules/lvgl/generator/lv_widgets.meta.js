const lstClasses = [

    // core
    {
        className: "Obj" ,
        cusMethods: {
            "js_lv_obj_enable_event": "enableEvent" ,
            "js_lv_obj_disable_event": "disableEvent" ,
            "js_lv_obj_is_screen": "isScreen" ,
            "js_lv_obj_get_coord_x": "coordX" ,
            "js_lv_obj_get_coord_y": "coordY" ,
            "js_lv_obj_get_coords": "coords" ,
            "js_lv_obj_set_coord_x": "setCoordX" ,
            "js_lv_obj_set_coord_y": "setCoordY" ,
            "js_lv_obj_set_coords": "setCoords" ,
            "js_lv_obj_move": "move" ,
            "js_lv_obj_move_x": "moveX" ,
            "js_lv_obj_move_y": "moveY" ,
            "js_lv_obj_get_all_style_values": "allStyleValues" ,
            "js_lv_obj_set_style": "setStyle" ,
            "js_lv_obj_get_style": "style" ,
            "js_lv_obj_refresh_style": "refreshStyle" ,
            "js_lv_obj_get_local_style": "localStyle" ,
            "js_lv_obj_get_font_height": "fontHeight" ,
            "js_lv_obj_as": "as" ,
            "js_lv_obj_ptr": "ptr" ,
            "js_lv_obj_abort_scroll": "abortScroll" ,
        }
    },
    {
        className: "Label" ,
        cusMethods: {
            "js_lv_label_set_font": "setFont" ,
        }
    } ,
    "Arc",
    "Bar",
    "Btn",
    "BtnMatrix",
    {
        className: "Canvas" ,
        cusMethods: {
            "js_lv_canvas_malloc": "malloc" ,
            // "js_lv_canvas_free": "free" ,
        }
    } ,
    "Checkbox",
    "Dropdown",
    {
        className: "Img" ,
        cusMethods: {
            "js_lv_img_set_src": "setSrc" ,
            "js_lv_img_set_symbol": "setSymbol" ,
        }
    } ,
    {
        className: "Line" ,
        cusMethods: {
            "lv_line_set_points": "setPoints"
        }
    } ,
    "Roller",
    "Slider",
    "Switch",
    "Table",
    "TextArea",

    // extra
    {
        className: "MsgBox" ,
        cusConstructor: true ,
    } ,
    "Keyboard" ,
    "TileView",
    {
        className: "List" ,
        ctypeName: "lv_obj_t *" ,
        cusMethods: {
            "js_lv_list_add_btn": "addBtn" ,
            // "js_lv_list_add_text": "addText" ,
        }

    },
]


const WidgetsMeta = []

for(let clzconf of lstClasses) {
    if(typeof clzconf=='string') {
        clzconf = {
            className: clzconf ,
        }
    }

    clzconf.cusConstructor = !!clzconf.cusConstructor

    if(!clzconf.typeName) {
        clzconf.typeName = "lv_"+clzconf.className.toLowerCase()
    }
    if(!clzconf.ctypeName) {
        clzconf.ctypeName = clzconf.typeName + "_t *"
    }
    if(!clzconf.fullClassName) {
        clzconf.fullClassName = "lvgl."+clzconf.className
    }

    if(!clzconf.methods) {
        clzconf.methods = require(__dirname+"/api/"+clzconf.typeName+".js")
    }
    if(!clzconf.cusMethods) {
        clzconf.cusMethods = []
    }
    
    WidgetsMeta.push(clzconf)
}

module.exports = WidgetsMeta