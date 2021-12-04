const fs = require("fs")

function create_conver_base(ctype, js_conver_func) {
    return function (cargv,i,className,jsmethod){
        code = `    ${ctype} ${cargv} ;\r\n`
        code+= `    if(${js_conver_func}(ctx, &${cargv}, argv[${i}])!=0){\r\n`
        code+= `        THROW_EXCEPTION("arg ${cargv} of method ${className}.${jsmethod}() must be a number")\r\n`
        code+= `    }\r\n`
        return code
    }
}


function create_conver_mappingconst(ctype, js_conver_func) {
    return function (cargv,i,className,jsmethod){
        return `    ${ctype} ${cargv} ;
    if(!${js_conver_func}(ctx, argv[${i}], &${cargv})) {
        return JS_EXCEPTION ;
    }
`
    }
}


const conver_int8 = create_conver_base("int8_t", "JS_ToInt32")
const conver_uint8 = create_conver_base("uint8_t", "JS_ToUint32")
const conver_int16 = create_conver_base("int16_t", "JS_ToInt32")
const conver_uint16 = create_conver_base("uint16_t", "JS_ToUint32")
const conver_int32 = create_conver_base("int32_t", "JS_ToInt32")
const conver_uint32 = create_conver_base("uint32_t", "JS_ToUint32")
const conver_flex_flow = create_conver_mappingconst('lv_flex_flow_t', 'lv_flex_flow_jsstr_to_code')
const conver_flex_align = create_conver_mappingconst('lv_flex_align_t', 'lv_flex_align_jsstr_to_code')


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
    "int8_t": conver_int8 ,
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
    "lv_flex_align_t": conver_flex_align,
    "lv_flex_flow_t": conver_flex_flow,
    "lv_style_selector_t": conver_uint32,
    "lv_label_long_mode_t": conver_uint8,   
    "lv_arc_mode_t": conver_uint8,
    "lv_bar_mode_t": conver_uint8,
    "lv_table_cell_ctrl_t": conver_uint8,
    "lv_text_align_t": conver_uint8,
    "lv_slider_mode_t": conver_uint8,
    "lv_roller_mode_t": conver_uint8,
    "lv_img_cf_t": conver_uint8,
    "lv_img_size_mode_t": conver_uint8,
    "lv_btnmatrix_ctrl_t": conver_uint8,
    "lv_img_src_t": conver_uint8,
    "lv_opa_t": conver_uint8,
}

function free_argv_string(cargv){
    return `    JS_FreeCString(ctx, ${cargv}) ;\r\n`
}

const MapFreeArgTypes = {
    "char *": free_argv_string ,
    "const char *": free_argv_string ,
}

function arglst_modifier_color(argvName) {
    return "(lv_color_t)"+argvName
}
// 在栈上创建、填充的 struct 参数，做为指针传递给 lvgl c api
function arglst_modifier_pass_addr(argvName) {
    return "&"+argvName
}

const MapModifyArgLst = {
    "lv_color_t": arglst_modifier_color ,
    "lv_area_t *": arglst_modifier_pass_addr ,
    "lv_point_t *": arglst_modifier_pass_addr ,
}





const MapJS2C_Conver = {
    "char *": "JS_NewString",   
    "int8_t": "JS_NewInt32" ,
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
    "lv_arc_mode_t": "JS_NewUint32",
    "lv_bar_mode_t":"JS_NewUint32",
    "lv_table_cell_ctrl_t":"JS_NewUint32",
    "lv_text_align_t":"JS_NewUint32",
    "lv_slider_mode_t":"JS_NewUint32",
    "lv_roller_mode_t":"JS_NewUint32",
    "lv_img_cf_t":"JS_NewUint32",
    "lv_img_size_mode_t":"JS_NewUint32",
    "lv_btnmatrix_ctrl_t":"JS_NewUint32",
    "lv_img_src_t":"JS_NewUint32",
    "lv_opa_t":"JS_NewUint32",
    "lv_color_t":"(uint16_t)JS_NewUint32",
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

                cargvLst+= `, ` + (MapModifyArgLst[argtype]? MapModifyArgLst[argtype](cargv): cargv)

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
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
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


function generateConstMapping(arrconst, prefix, ctype, cname, lastval) {
    
    let mappingCode = ""
    let nameLstCode = ""
    let i = 0
    for(let constName of arrconst) {
        let name = constName.substr(prefix.length).toLowerCase()
        mappingCode+= `    ${i? 'else ':''}if(strcmp(cstr,"${name}")==0) {\r\n`
        mappingCode+= `        (*out) = ${constName} ;\r\n`
        mappingCode+= `    }\r\n`

        if(i) {
            nameLstCode+= ", "
        }
        nameLstCode+= `"${name}"`
        i++
    }

    let code = `
const char * ${cname}_names[] = { ${nameLstCode} } ;
bool ${cname}_jsstr_to_code(JSContext *ctx, JSValue jsstr, ${ctype}* out) {
    char * cstr = JS_ToCString(ctx, jsstr) ;
${mappingCode}
    else {
        JS_ThrowReferenceError(ctx, "unkonw ${cname} pass in: %s", cstr) ;
        JS_FreeCString(ctx, cstr) ;
        return false ;
    }
    JS_FreeCString(ctx, cstr) ;
    return true ;
}
JSValue ${cname}_code_to_jsstr(JSContext *ctx, ${ctype} code) {
    if(code>=${lastval}) {
        return JS_NewString(ctx, "unkonw");
    }
    return JS_NewString(ctx, ${cname}_names[code]);
}
`
    return code
}


const methodlst_mark_start = "// AUTO GENERATE CODE START [METHOD LIST] --------"
const methodlst_mark_end = "// AUTO GENERATE CODE END [METHOD LIST] --------"
const defclass_mark_start = "// AUTO GENERATE CODE START [DEFINE CLASS] --------"
const defclass_mark_end = "// AUTO GENERATE CODE END [DEFINE CLASS] --------"
const registerclass_mark_start = "// AUTO GENERATE CODE START [REGISTER CLASS] --------"
const registerclass_mark_end = "// AUTO GENERATE CODE END [REGISTER CLASS] --------"
const registerclass_id_mark_start = "// AUTO GENERATE CODE START [REGISTER CLASS ID] --------"
const registerclass_id_mark_end = "// AUTO GENERATE CODE END [REGISTER CLASS ID] --------"
const const_mapping_mark_start = "// AUTO GENERATE CODE START [CONST MAPPING] --------"
const const_mapping_mark_end = "// AUTO GENERATE CODE END [CONST MAPPING] --------"

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
    code+= gen_lv_class("lv_arc", "lvgl.Arc", require("./api/lv_arc.js"))
    code+= gen_lv_class("lv_bar", "lvgl.Bar", require("./api/lv_bar.js"))
    code+= gen_lv_class("lv_btn", "lvgl.Btn", require("./api/lv_btn.js"))
    code+= gen_lv_class("lv_table", "lvgl.Table", require("./api/lv_table.js"))
    code+= gen_lv_class("lv_textarea", "lvgl.TextArea", require("./api/lv_textarea.js"))
    code+= gen_lv_class("lv_slider", "lvgl.Slider", require("./api/lv_slider.js"))
    code+= gen_lv_class("lv_switch", "lvgl.Switch", require("./api/lv_switch.js"))
    code+= gen_lv_class("lv_roller", "lvgl.Roller", require("./api/lv_roller.js"))
    code+= gen_lv_class("lv_checkbox", "lvgl.Checkbox", require("./api/lv_checkbox.js"))
    code+= gen_lv_class("lv_line", "lvgl.Line", require("./api/lv_line.js"), {
                "lv_line_set_points": "setPoints"
    })
    code+= gen_lv_class("lv_dropdown", "lvgl.Dropdown", require("./api/lv_dropdown.js"))
    code+= gen_lv_class("lv_img", "lvgl.Img", require("./api/lv_img.js"))
    code+= gen_lv_class("lv_btnmatrix", "lvgl.BtnMatrix", require("./api/lv_btnmatrix.js"))
    code+= gen_lv_class("lv_canvas", "lvgl.Canvas", require("./api/lv_canvas.js"))


    
    let src = fs.readFileSync(dist_path).toString()
    src = srcInsert(src, code, methodlst_mark_start, methodlst_mark_end)

    src = srcInsert(src, generateClassDefine(), defclass_mark_start, defclass_mark_end)
    src = srcInsert(src, generateClassRegister(), registerclass_mark_start, registerclass_mark_end)
    src = srcInsert(src, generateClassIDRegister(), registerclass_id_mark_start, registerclass_id_mark_end)

    code = generateConstMapping(require("./api/const_event"), 'LV_EVENT_', 'lv_event_code_t', 'lv_event', '_LV_EVENT_LAST')
    code+= generateConstMapping(require("./api/const_flex_flow"), 'LV_FLEX_FLOW_', 'lv_flex_flow_t', 'lv_flex_flow', 'LV_FLEX_FLOW_COLUMN_WRAP_REVERSE+1')
    code+= generateConstMapping(require("./api/const_flex_align"), 'LV_FLEX_ALIGN_', 'lv_flex_align_t', 'lv_flex_align', 'LV_FLEX_ALIGN_SPACE_BETWEEN+1')
    src = srcInsert(src, code, const_mapping_mark_start, const_mapping_mark_end)

    fs.writeFileSync(dist_path, src)

    console.log()

}
main()