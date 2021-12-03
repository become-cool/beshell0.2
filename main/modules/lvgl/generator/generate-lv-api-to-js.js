const fs = require("fs")

function create_conver_base(ctype, js_conver_func) {
    return function (cargv,i,className,jsmethod){
        code = `    ${ctype} ${cargv} ;\r\n`
        code+= `    if(!${js_conver_func}(ctx, &${cargv}, argv[${i}])){\r\n`
        code+= `        THROW_EXCEPTION("arg ${cargv} of method ${className}.${jsmethod}() must be a number")\r\n`
        code+= `    }\r\n`
        return code
    }
}

const conver_uint8 = create_conver_base("uint8_t", "JS_ToUint32")
const conver_int16 = create_conver_base("int16_t", "JS_ToInt32")
const conver_uint16 = create_conver_base("uint16_t", "JS_ToUint32")
const conver_int32 = create_conver_base("int32_t", "JS_ToInt32")
const conver_uint32 = create_conver_base("uint32_t", "JS_ToUint32")


function conver_bool(cargv,i){
    return `    bool ${cargv} = JS_ToBool(ctx, argv[${i}]) ;\r\n`
}

function conver_int_prop(propName, cargv,i,className,jsmethod) {
    code = `    JSValue js${cargv}_${propName} = JS_GetPropertyStr(ctx, argv[${i}], "${propName}") ;\r\n`
    code+= `    if(!JS_IsNumber(js${cargv}_${propName})){\r\n`
    code+= `        THROW_EXCEPTION("arg ${cargv} of method ${className}.${jsmethod}() missing property x, or is not a number")\r\n`
    code+= `    }\r\n`
    code+= `    ${cargv}.${propName} = js${cargv}_${propName} ;\r\n`
    return code
}

function conver_point(cargv,i,className,jsmethod) {
    code = `    if(!JS_IsObject(argv[${i}])){\r\n`
    code+= `        THROW_EXCEPTION("arg ${cargv} of method ${className}.${jsmethod}() must be a object{x,y}")\r\n`
    code+= `    }\r\n`
    code+= `    lv_point_t ${cargv} ;\r\n`
    code+= conver_int_prop('x', cargv,i,className,jsmethod)
    code+= conver_int_prop('y', cargv,i,className,jsmethod)
    return code
}
function conver_area(cargv,i,className,jsmethod) {
    code = `    if(!JS_IsObject(argv[${i}])){\r\n`
    code+= `        THROW_EXCEPTION("arg ${cargv} of method ${className}.${jsmethod}() must be a object{x1,y1,x2,y2}")\r\n`
    code+= `    }\r\n`
    code+= `    lv_area_t ${cargv} ;\r\n`
    code+= conver_int_prop('x1', cargv,i,className,jsmethod)
    code+= conver_int_prop('y1', cargv,i,className,jsmethod)
    code+= conver_int_prop('x2', cargv,i,className,jsmethod)
    code+= conver_int_prop('y2', cargv,i,className,jsmethod)
    return code
}
function conver_obj(cargv,i,className,jsmethod) {
    code = `    lv_obj_t * ${cargv} = (lv_obj_t *)JS_GetOpaque(argv[${i}], js_lv_obj_class_id) ;\r\n`
    code+= `    if( !${cargv} ){\r\n`
    code+= `        THROW_EXCEPTION("arg ${cargv} of method ${className}.${jsmethod}() must be a beapi.lvgl.Obj")\r\n`
    code+= `    }\r\n`
    return code
}
function conver_string(cargv,i) {
    return `    char * ${cargv} = JS_ToCString(ctx, argv[${i}]) ;\r\n`
}

const MapC2JS_Conver = {
    "uint8_t": conver_uint8 ,
    "int16_t": conver_int16 ,
    "uint16_t": conver_uint16 ,
    "int32_t": conver_int32 ,
    "uint32_t": conver_uint32 ,
    "lv_coord_t": conver_int16 ,
    "lv_obj_flag_t": conver_uint32 ,
    "lv_state_t": conver_uint16 ,
    "lv_align_t": conver_uint8 ,
    "bool": conver_bool ,
    "char *": conver_string ,
    "const char *": conver_string ,
    "lv_anim_enable_t": conver_bool,
    "lv_color_t": conver_uint16 ,
    "lv_scrollbar_mode_t": conver_uint8,
    "lv_scroll_snap_t": conver_uint8,
    "lv_dir_t": conver_uint8,
    "lv_area_t *": conver_area ,
    "lv_point_t *": conver_point ,
    "lv_obj_t *": conver_obj,
    "_lv_obj_t *": conver_obj,
    "lv_flex_align_t": conver_uint8,
    "lv_flex_flow_t": conver_uint8,
    "lv_style_selector_t": conver_uint32,
    "lv_label_long_mode_t": conver_uint8,   
}

function free_argv_string(cargv){
    return `    JS_FreeCString(ctx, ${cargv}) ;\r\n`
}

const MapFreeArgTypes = {
    "char *": free_argv_string ,
    "const char *": free_argv_string ,
}

// 在栈上创建、填充的 struct 参数，做为指针传递给 lvgl c api
const ArrStructArgTypes = ['lv_area_t *', 'lv_point_t *']



const MapJS2C_Conver = {
    "uint8_t": "JS_NewUint32" ,
    "int16_t": "JS_NewInt32" ,
    "uint16_t": "JS_NewUint32" ,
    "int32_t": "JS_NewInt32" ,
    "uint32_t": "JS_NewUint32" ,
    "lv_coord_t": "JS_NewInt32" ,
    "lv_state_t": "JS_NewUint32" ,
    "lv_obj_flag_t": "JS_NewUint32" ,
    "lv_scrollbar_mode_t": "JS_NewUint32" ,
    "lv_scroll_snap_t": "JS_NewUint32" ,
    "lv_dir_t": "JS_NewUint32" ,
    "lv_flex_align_t": "JS_NewUint32",
    "lv_flex_flow_t": "JS_NewUint32",
    "lv_style_selector_t": "JS_NewUint32",
    "lv_label_long_mode_t": "JS_NewUint32",   
    "char *": "JS_NewString",   
}

function gen_lv_class(cName, className, api, extraMethod) {

    let codeFuncDef = ""
    let codeMethedList = `static const JSCFunctionListEntry js_${cName}_proto_funcs[] = {\r\n`

    for(let cfuncName in extraMethod) {
        codeMethedList+= `    JS_CFUNC_DEF("${extraMethod[cfuncName]}", 0, ${cfuncName}),\r\n`
    }

    for(let cfunc in api) {

        let [jsmethod, argLstConf, returnType] = api[cfunc]

        if(!jsmethod) {
            if( cfunc.substr(0, cName.length+1) == (cName+'_') ){
                jsmethod = cfunc.substr(cName.length+1)
                let arr = jsmethod.split("_")
                jsmethod = arr.shift()
                for(let n of arr) {
                    jsmethod+= n[0].toUpperCase() + n.substr(1)
                }
            }
            else {
                console.log("not set method name , and do not auto generate.", cName, cfunc)
            }
        }
        
        codeFuncDef+= `static JSValue js_${cfunc}(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {\r\n`
        if(argLstConf) {
            codeFuncDef+= `    if(argc<${Object.keys(argLstConf).length}) {\r\n`
            codeFuncDef+= `        THROW_EXCEPTION("${className}.${jsmethod}() missing arg")\r\n`
            codeFuncDef+= `    }\r\n`
        }
        codeFuncDef+= `    lv_obj_t * thisobj = JS_GetOpaqueInternal(this_val) ;\r\n`
        codeFuncDef+= `    if(!thisobj) {\r\n`
        codeFuncDef+= `        THROW_EXCEPTION("${className}.${jsmethod}() must be called as a ${className} method")\r\n`
        codeFuncDef+= `    }\r\n`

        codeFreeArgvs = ''

        let cargvLst = ''
        if(argLstConf) {
            for(let i=0;i<argLstConf.length;i++) {
                let [argtype, cargv] = argLstConf[i]
                if(MapC2JS_Conver[argtype]) {
                    codeFuncDef+= MapC2JS_Conver[argtype](cargv, i, className, jsmethod)
                }

                else {
                    console.error("unknow arg type", argtype, "for function", cfunc)
                    continue
                }

                // 复合类型
                if( ArrStructArgTypes.includes(argtype) ) {
                    cargvLst+= `, &${cargv}`
                }
                else {
                    cargvLst+= `, ${cargv}`
                }

                if(MapFreeArgTypes[argtype]){
                    codeFreeArgvs+= MapFreeArgTypes[argtype](cargv) ;
                }
            }
        }

        // js method 返回 this
        if(returnType=="this") {
            codeFuncDef+= `    ${cfunc}(thisobj${cargvLst}) ;\r\n`
            codeFuncDef+= `    JSValue retval = JS_DupValue(ctx, this_val) ;\r\n`
        }
        else if(returnType=="void") {
            codeFuncDef+= `    ${cfunc}(thisobj${cargvLst}) ;\r\n`
            codeFuncDef+= `    JSValue retval = JS_UNDEFINED ;\r\n`
        }
        else if(returnType=="_lv_obj_t *"||returnType=="lv_obj_t *") {
            codeFuncDef+= `    void * retptr = lv_obj_get_user_data(${cfunc}(thisobj${cargvLst}));\r\n`
            codeFuncDef+= `    if(!retptr){\r\n`
            codeFuncDef+= `        return JS_NULL; \r\n`
            codeFuncDef+= `    } \r\n`
            codeFuncDef+= `    JSValue retval = JS_MKPTR(JS_TAG_OBJECT, retptr) ; \r\n`
        }
        else if(returnType=="bool") {
            codeFuncDef+= `    JSValue retval = JS_NewBool(ctx,${cfunc}(thisobj${cargvLst})) ;\r\n`
        }
        else if(MapJS2C_Conver[returnType]) {
            codeFuncDef+= `    JSValue retval = ${MapJS2C_Conver[returnType]}(ctx,${cfunc}(thisobj${cargvLst})) ;\r\n`
        }
        else {
            console.error("unknow return type",returnType, "for function", cfunc)
        }


        codeFuncDef+= codeFreeArgvs
        codeFuncDef+= "    return retval ;\r\n"
        codeFuncDef+= "}\r\n\r\n"

        codeMethedList+= `    JS_CFUNC_DEF("${jsmethod}", 0, js_${cfunc}),\r\n`
    }

    codeMethedList+= "} ;\r\n"
    codeMethedList+= `#define __def_js_${cName}_proto_funcs__\r\n\r\n`

    return codeFuncDef + "\r\n" + codeMethedList

}

const LstClasses = [
    "Obj",
    "Label",
    "Arc",
    "Bar",
    "Btn",
    "BtnMatrix",
    "Canvas",
    "Checkbox",
    "Dropdown",
    "Img",
    "Line",
    "Roller",
    "Slider",
    "Switch",
    "Table",
    "TextArea",
]

function generateClassDefine() {
    let code = ""
    for(let clzName of LstClasses) {
        let ctypeName = "lv_" + clzName.toLowerCase()
        code+=
` // beapi.lvgl.${clzName} --
static JSClassID js_${ctypeName}_class_id ;
static JSValue js_${ctypeName}_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    ${ctypeName}_t * cobj = ${ctypeName}_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, js_${ctypeName}_class_id) ;
    if(cparent) {
        JS_DupValue(ctx, argv[0]) ;
        JS_DupValue(ctx, jsobj) ;
    }
    return jsobj ;
}
static void js_${ctypeName}_finalizer(JSRuntime *rt, JSValue val){
    printf("js_${ctypeName}_finalizer()\\n") ;
    ${ctypeName}_t * thisobj = JS_GetOpaque(val, js_${ctypeName}_class_id) ;
    lv_obj_del(thisobj) ;
}
static JSClassDef js_${ctypeName}_class = {
    "lvgl.${clzName}",
    .finalizer = js_${ctypeName}_finalizer,
};

`
    }
    return code
}
function generateClassRegister() {

    let code = ""
    for(let clzName of LstClasses) {
        let ctypeName = "lv_" + clzName.toLowerCase()
        if(clzName=="Obj")
            var parentProtoName = "EventEmitterProto"
        else
            var parentProtoName = "proto_lv_obj"
        code+= 
`    // define js class lvgl.${ctypeName}    
    JSValue proto_${ctypeName} = js_def_class(ctx, "${clzName}", js_${ctypeName}_class_id, &js_${ctypeName}_class
                , "lvgl.${clzName}", js_${ctypeName}_constructor, js_${ctypeName}_proto_funcs, countof(js_${ctypeName}_proto_funcs), ${parentProtoName}, lvgl) ;

`
    }

    return code

}
function generateClassIDRegister() {
    let code = ""
    for(let clzName of LstClasses) {
        let ctypeName = "lv_" + clzName.toLowerCase()
        code+= `    JS_NewClassID(&js_${ctypeName}_class_id);\r\n`
    }
    return code
}

function srcInsert(src, code, startMart, endMark) {
    
    let markStartAt = src.indexOf(startMart)
    if(markStartAt<0) {
        console.log(src)    
        throw new Error ("not found mark "+startMart)
    }
    let markEndAt = src.indexOf(endMark)
    if(markEndAt<0) {
        console.log(src)    
        throw new Error ("not found mark "+endMark)
    }
    
    return src.substr(0, markStartAt) + startMart + "\r\n" + code + src.substr(markEndAt) 
}


const ArrEvents = [
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
function generateEventsMapping() {
    
    let mappingCode = ""
    let nameLstCode = ""
    let i = 0
    for(let constName of ArrEvents) {
        let name = constName.replace(/^LV_EVENT_/, "").toLowerCase()
        mappingCode+= `    ${i? 'else ':''}if(strcmp(cstr,"${name}")==0) {\r\n`
        mappingCode+= `        code = ${constName} ;\r\n`
        mappingCode+= `    }\r\n`

        if(i) {
            nameLstCode+= ", "
        }
        nameLstCode+= `"${name}"`
        i++
    }

    return `
const char * lv_event_names[] = { ${nameLstCode} } ;
lv_event_code_t lv_event_jsstr_to_code(JSContext *ctx, JSValue jsstr) {
    char * cstr = JS_ToCString(ctx, jsstr) ;
    lv_event_code_t code = _LV_EVENT_LAST ;
${mappingCode}
    JS_FreeCString(ctx, cstr) ;
    return code ;
}
` ;
}


const methodlst_mark_start = "// AUTO GENERATE CODE START [METHOD LIST] --------"
const methodlst_mark_end = "// AUTO GENERATE CODE END [METHOD LIST] --------"
const defclass_mark_start = "// AUTO GENERATE CODE START [DEFINE CLASS] --------"
const defclass_mark_end = "// AUTO GENERATE CODE END [DEFINE CLASS] --------"
const registerclass_mark_start = "// AUTO GENERATE CODE START [REGISTER CLASS] --------"
const registerclass_mark_end = "// AUTO GENERATE CODE END [REGISTER CLASS] --------"
const registerclass_id_mark_start = "// AUTO GENERATE CODE START [REGISTER CLASS ID] --------"
const registerclass_id_mark_end = "// AUTO GENERATE CODE END [REGISTER CLASS ID] --------"
const event_mapping_mark_start = "// AUTO GENERATE CODE START [EVENT MAPPING] --------"
const event_mapping_mark_end = "// AUTO GENERATE CODE END [EVENT MAPPING] --------"

let dist_path = __dirname + "/../widgets.c"

function main() {
    let code = ""

    code+= gen_lv_class(
                "lv_obj", "lvgl.Obj"
                , require("./api/lv_obj.js")
                , {
                    "js_lv_obj_enable_event": "enableEvent" ,
                    "js_lv_obj_disable_event": "disableEvent" ,
                }
    )
    code+= gen_lv_class("lv_label", "lvgl.Label", require("./api/lv_label.js"))

    let src = fs.readFileSync(dist_path).toString()
    src = srcInsert(src, code, methodlst_mark_start, methodlst_mark_end)

    src = srcInsert(src, generateClassDefine(), defclass_mark_start, defclass_mark_end)
    src = srcInsert(src, generateClassRegister(), registerclass_mark_start, registerclass_mark_end)
    src = srcInsert(src, generateClassIDRegister(), registerclass_id_mark_start, registerclass_id_mark_end)
    src = srcInsert(src, generateEventsMapping(), event_mapping_mark_start, event_mapping_mark_end)

    fs.writeFileSync(dist_path, src)

    console.log()

}
main()