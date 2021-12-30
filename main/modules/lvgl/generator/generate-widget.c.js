const fs = require("fs")
const WidgetsMeta = require("./lv_widgets.meta.js")
const StructWrapper = require("./lv_struct_wrapper.js")
// console.log(WidgetsMeta)

function create_conver_base(ctype, js_conver_func, api_arg_type) {
    return function (cargv,i,className,jsmethod, defaultVal){
        if(defaultVal==undefined) {
            return `    ${ctype} ${cargv} ;
    if(${js_conver_func}(ctx, (${api_arg_type} *) &${cargv}, argv[${i}])!=0){
        THROW_EXCEPTION("arg ${cargv} of method ${className}.${jsmethod}() must be a number")
    }
`
        }
        else {
            return `    ${ctype} ${cargv} = ${defaultVal} ;
    if(argc>${i}){
        if(${js_conver_func}(ctx, (${api_arg_type} *) &${cargv}, argv[${i}])!=0){
            THROW_EXCEPTION("arg ${cargv} of method ${className}.${jsmethod}() must be a number")
        }
    }
`
        }
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


const conver_int8 = create_conver_base("int8_t", "JS_ToInt32", "int32_t")
const conver_uint8 = create_conver_base("uint8_t", "JS_ToUint32", "uint32_t")
const conver_int16 = create_conver_base("int16_t", "JS_ToInt32", "int32_t")
const conver_uint16 = create_conver_base("uint16_t", "JS_ToUint32", "uint32_t")
const conver_int32 = create_conver_base("int32_t", "JS_ToInt32", "int32_t")
const conver_uint32 = create_conver_base("uint32_t", "JS_ToUint32", "uint32_t")

function conver_bool(cargv,i,className,jsmethod, defaultVal){
    if(defaultVal) {
        return `    bool ${cargv} = ${defaultVal} ;
    if(argc>${i}) {
        ${cargv} = JS_ToBool(ctx, argv[${i}]) ;
    }
`
    }
    else {
        return `    bool ${cargv} = JS_ToBool(ctx, argv[${i}]) ;\r\n`
    }
}

function conver_int_prop(propName, cargv,i,className,jsmethod) {
    return `    JSValue js${cargv}_${propName} = JS_GetPropertyStr(ctx, argv[${i}], "${propName}") ;
    if(!JS_IsNumber(js${cargv}_${propName})){
        THROW_EXCEPTION("arg ${cargv} of method ${className}.${jsmethod}() missing property x, or is not a number")
    }
    if(JS_ToInt32(ctx, (int32_t*)&${cargv}.${propName}, js${cargv}_${propName})!=0) {
        THROW_EXCEPTION("property ${propName} of arg ${cargv} is not a number")
    }
`
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
    code = `    lv_obj_t * ${cargv} = (lv_obj_t *)JS_GetOpaqueInternal(argv[${i}]) ;\r\n`
    code+= `    if( !${cargv} ){\r\n`
    code+= `        THROW_EXCEPTION("arg ${cargv} of method ${className}.${jsmethod}() must be a beapi.lvgl.Obj")\r\n`
    code+= `    }\r\n`
    return code
}
function conver_style(cargv,i,className,jsmethod) {
    return `    if( !qjs_instanceof(ctx, argv[${i}], js_lv_style_class_id) ) {
        THROW_EXCEPTION("arg style of method ${className}.${jsmethod}() must be a lvgl.Style")
    }
    lv_style_t * ${cargv} = JS_GetOpaque(argv[${i}], js_lv_style_class_id) ;
    if(!${cargv}) {
        THROW_EXCEPTION("arg style of method ${className}.${jsmethod}() is invalid")
    }
`
}
function conver_string(cargv,i) {
    return `    char * ${cargv} = (char *)JS_ToCString(ctx, argv[${i}]) ;\r\n`
}

function struct_wrapper(structType, class_id, classFullName) {
    return function(cargv,i,className,jsmethod) {
        cargv = cargv.replace(/^&/, "")
        return `    ${structType} * ${cargv} = JS_GetOpaque(argv[${i}], ${class_id});
    if(!${cargv}) {
        THROW_EXCEPTION("arg ${cargv} of method ${className}.${jsmethod}() must be a ${classFullName} object")
    }
`
    }
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
    "lv_align_t": create_conver_mappingconst('lv_align_t', 'lv_align_jsstr_to_const') ,
    "lv_dir_t": create_conver_mappingconst('lv_dir_t', 'lv_dir_jsstr_to_const'),
    "bool": conver_bool ,
    "char *": conver_string ,
    "const char *": conver_string ,
    "lv_anim_enable_t": conver_bool,
    "lv_color_t": conver_uint16 ,
    "lv_scrollbar_mode_t": conver_uint8,
    "lv_scroll_snap_t": conver_uint8,
    "lv_area_t *": conver_area ,
    "lv_point_t *": conver_point ,
    "lv_obj_t *": conver_obj,
    "_lv_obj_t *": conver_obj,
    "lv_flex_align_t": create_conver_mappingconst('lv_flex_align_t', 'lv_flex_align_jsstr_to_const'),
    "lv_flex_flow_t": create_conver_mappingconst('lv_flex_flow_t', 'lv_flex_flow_jsstr_to_const'),
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
    "lv_state_t": conver_uint32,
    "lv_part_t": conver_uint32,
    "lv_style_selector_t": conver_uint32,
    "lv_base_dir_t": create_conver_mappingconst('lv_base_dir_t', 'lv_base_dir_jsstr_to_const'),
    "lv_border_side_t": create_conver_mappingconst('lv_border_side_t', 'lv_border_side_jsstr_to_const'),
    "lv_style_prop_t": create_conver_mappingconst('lv_style_prop_t', 'lv_style_prop_jsstr_to_const'),
    "lv_obj_flag_t": create_conver_mappingconst('lv_obj_flag_t', 'lv_obj_flag_jsstr_to_const'),
    "lv_label_long_mode_t": create_conver_mappingconst('lv_label_long_mode_t', 'lv_label_long_mode_jsstr_to_const'),
    "lv_keyboard_mode_t": create_conver_mappingconst('lv_keyboard_mode_t', 'lv_keyboard_mode_jsstr_to_const'),
    "lv_style_t *": conver_style ,
    "lv_draw_rect_dsc_t *": struct_wrapper('lv_draw_rect_dsc_t', 'js_lv_draw_rect_dsc_class_id', 'lv.DrawRectDsc') ,
    "lv_draw_arc_dsc_t *": struct_wrapper('lv_draw_arc_dsc_t', 'js_lv_draw_arc_dsc_class_id', 'lv.DrawArcDsc') ,

    
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
    "lv_dir_t": "lv_dir_const_to_jsstr" ,
    "lv_align_t": 'lv_align_const_to_jsstr' ,
    "lv_flex_align_t": "lv_flex_align_const_to_jsstr",
    "lv_flex_flow_t": "lv_flex_flow_const_to_jsstr",
    "lv_style_selector_t": "JS_NewUint32",
    "lv_label_long_mode_t": "JS_NewUint32",
    "lv_keyboard_mode_t": "JS_NewUint32",
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
    "lv_state_t": 'JS_NewUint32',
    "lv_part_t": 'JS_NewUint32',
    "lv_style_selector_t": 'JS_NewUint32',
    "lv_base_dir_t": 'lv_base_dir_const_to_jsstr',
    "lv_border_side_t": 'lv_border_side_const_to_jsstr',
    "lv_style_prop_t": 'lv_style_prop_const_to_jsstr',
    "lv_obj_flag_t": 'lv_obj_flag_const_to_jsstr',
    "lv_obj_t *": '',
    "_lv_obj_t *": '',
}

function mk_lvobj_converfunc(type) {
    return function(cfunc, cargvLst) {
        return `    JSValue retval = JS_NULL ;
    void * lvobj = ${cfunc}(thisobj${cargvLst});
    if(lvobj) {
        retval = js_lv_obj_wrapper(ctx, lvobj, JS_UNDEFINED, js_lv_${type}_class_id) ;
        JS_DupValue(ctx, retval) ;
    }
`
    }
}

const MapJS2C_ConverFunc = {
    "lv_color_t": function(cfunc, cargvLst){
        return `    JSValue retval = JS_NewUint32(ctx,${cfunc}(thisobj${cargvLst}).full) ;\r\n`
    },
    "lv_obj_t *": mk_lvobj_converfunc("obj") ,
    "_lv_obj_t *": mk_lvobj_converfunc("obj") ,
    "lv_label_t *": mk_lvobj_converfunc("label") ,
    "lv_btn_t *": mk_lvobj_converfunc("btn") ,
}


function gen_lv_methods(clzConf) {

    let codeFuncDef = ""
    let codeMethedList = `static const JSCFunctionListEntry js_${clzConf.typeName}_proto_funcs[] = {\r\n`

    for(let cfuncName in clzConf.cusMethods) {
        codeMethedList+= `    JS_CFUNC_DEF("${clzConf.cusMethods[cfuncName]}", 0, ${cfuncName}),\r\n`
    }

    for(let cfunc in clzConf.methods) {

        let [jsmethod, argLstConf, returnType] = clzConf.methods[cfunc]

        if(!jsmethod) {
            if( cfunc.substr(0, clzConf.typeName.length+1) == (clzConf.typeName+'_') ){
                jsmethod = cfunc.substr(clzConf.typeName.length+1)
                let arr = jsmethod.split("_")
                jsmethod = arr.shift()
                if(jsmethod=='get') {
                    jsmethod = arr.shift()
                }
                for(let n of arr) {
                    jsmethod+= n[0].toUpperCase() + n.substr(1)
                }
            }
            else {
                console.log("not set method name , and do not auto generate.", clzConf.typeName, cfunc)
            }
        }
        
        codeFuncDef+= `static JSValue js_${cfunc}(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {\r\n`
        if(argLstConf.length) {

            let argcMin = 0
            for(let argconf of argLstConf) {
                if(argconf[2]==undefined) {  // 参数没有默认值
                    argcMin++ ;
                }
            }
            if(argcMin) {
                codeFuncDef+= `    if(argc<${argcMin}) {\r\n`
                codeFuncDef+= `        THROW_EXCEPTION("${clzConf.className}.${jsmethod}() missing arg")\r\n`
                codeFuncDef+= `    }\r\n`
            }
        }
        codeFuncDef+= `    void * lv_userdata = JS_GetOpaqueInternal(this_val) ;\r\n`
        codeFuncDef+= `    if(!lv_userdata) {\r\n`
        codeFuncDef+= `        THROW_EXCEPTION("${clzConf.className}.${jsmethod}() must be called as a ${clzConf.className} method")\r\n`
        codeFuncDef+= `    }\r\n`
        codeFuncDef+= `    lv_obj_t * thisobj = lv_userdata ;\r\n`

        codeFreeArgvs = ''

        let cargvLst = ''
        if(argLstConf) {
            for(let i=0;i<argLstConf.length;i++) {
                let [argtype, cargv, defaultVal] = argLstConf[i]
                if(MapC2JS_Conver[argtype]) {
                    codeFuncDef+= MapC2JS_Conver[argtype](cargv, i, clzConf.className, jsmethod, defaultVal)
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
        else if(returnType=="bool") {
            codeFuncDef+= `    JSValue retval = JS_NewBool(ctx,${cfunc}(thisobj${cargvLst})) ;\r\n`
        }
        else if(MapJS2C_ConverFunc[returnType]) {
            codeFuncDef+= MapJS2C_ConverFunc[returnType](cfunc, cargvLst)
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
    codeMethedList+= `#define __def_js_${clzConf.typeName}_proto_funcs__\r\n\r\n`

    return codeFuncDef + "\r\n" + codeMethedList
}

function generateClassDefine() {
    let code = ""
    for(let clzConf of WidgetsMeta) {
        code+=
` // beapi.lvgl.${clzConf.className} --
static JSClassID js_${clzConf.typeName}_class_id ;
`

        if(!clzConf.cusConstructor) {
            code+= `static JSValue js_${clzConf.typeName}_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    lv_obj_t * cparent = NULL ;
    if(argc>=1 && !JS_IsUndefined(argv[0]) && !JS_IsNull(argv[0])) {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lvgl.Obj object")
        cparent = JS_GetOpaqueInternal(argv[0]) ;
    }
    ${clzConf.ctypeName} cobj = ${clzConf.typeName}_create(cparent) ;
    JSValue jsobj = js_lv_obj_wrapper(ctx, cobj, new_target, js_${clzConf.typeName}_class_id) ;
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
    JS_DupValue(ctx, jsobj) ;
    return jsobj ;
}
`
        }

        code+= `static void js_${clzConf.typeName}_finalizer(JSRuntime *rt, JSValue val){
    printf("js_${clzConf.typeName}_finalizer()\\n") ;
    // ${clzConf.typeName}_t * thisobj = JS_GetOpaque(val, js_${clzConf.typeName}_class_id) ;
    // lv_obj_del(thisobj) ;
}
static JSClassDef js_${clzConf.typeName}_class = {
    "${clzConf.fullClassName}",
    .finalizer = js_${clzConf.typeName}_finalizer,
};

`
    }
    return code
}
function generateClassRegister() {

    let code = ""
    for(let clzConf of WidgetsMeta) {
        if(clzConf.className=="Obj")
            var parentProtoName = "EventEmitterProto"
        else
            var parentProtoName = "proto_lv_obj"
        code+= 
`    // define js class lvgl.${clzConf.typeName}    
    JSValue proto_${clzConf.typeName} = qjs_def_class(ctx, "${clzConf.className}", js_${clzConf.typeName}_class_id, &js_${clzConf.typeName}_class
                , "${clzConf.fullClassName}", js_${clzConf.typeName}_constructor, js_${clzConf.typeName}_proto_funcs, countof(js_${clzConf.typeName}_proto_funcs), ${parentProtoName}, lvgl) ;

`
    }

    return code

}
function generateClassIDRegister() {
    let code = ""
    for(let clzConf of WidgetsMeta) {
        code+= `    JS_NewClassID(&js_${clzConf.typeName}_class_id);\r\n`
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


function generateConstMapping(defConst) {
    let mappingCode = ""
    let mappingName = ""
    let mappingByVar = ""
    let i = 0
    for(let constName of defConst.def) {
        let name = constName.substr(defConst.prefix.length).toLowerCase().replace(/_/g,'-')
        mappingCode+= `    ${i? 'else ':''}if(strcmp(name,"${name}")==0) {\r\n`
        mappingCode+= `        (*out) = ${constName} ;\r\n`
        mappingCode+= `    }\r\n`

        mappingName+= `        case ${constName}: return "${name}";\r\n`
        i++
    }

    i = 0
    for(let varName of defConst.vars||[]) {
        let name = varName.substr(defConst.prefix.length).toLowerCase().replace(/_/g,'-')
        mappingByVar = `    ${i? 'else ':''}if(code==${varName}) {\r\n`
        mappingByVar+= `        return "${name}" ;\r\n`
        mappingByVar+= `    }\r\n`

        mappingCode+= `    else if(strcmp(name,"${name}")==0) {\r\n`
        mappingCode+= `        (*out) = ${varName} ;\r\n`
        mappingCode+= `    }\r\n`
    }

    let code = `
bool ${defConst.name}_str_to_const(const char * name, ${defConst.type}* out) {
    ${mappingCode}
    else {
        return false ;
    }
    return true ;
}
bool ${defConst.name}_jsstr_to_const(JSContext *ctx, JSValue jsstr, ${defConst.type}* out) {
    char * cstr = (char *)JS_ToCString(ctx, jsstr) ;
    bool suc = ${defConst.name}_str_to_const(cstr, out) ;
    JS_FreeCString(ctx, cstr) ;
    return suc ;
}
const char * ${defConst.name}_const_to_str(${defConst.type} code) {
${mappingByVar}
    switch(code) {
${mappingName}
        default:
            return "unkonw";
    }
}
JSValue ${defConst.name}_const_to_jsstr(JSContext *ctx, ${defConst.type} code) {
    return JS_NewString(ctx, ${defConst.name}_const_to_str(code));
}
`
    return code
}


function generateConstDeclare() {
    let code = ""
    let libConst = require("./api/consts.js")
    for(defConst of libConst) {
        code+= `
bool ${defConst.name}_str_to_const(const char * str, ${defConst.type}* out) ;
bool ${defConst.name}_jsstr_to_const(JSContext *ctx, JSValue jsstr, ${defConst.type}* out) ;
const char *  ${defConst.name}_const_to_str(${defConst.type} code) ;
JSValue ${defConst.name}_const_to_jsstr(JSContext *ctx, ${defConst.type} code) ;
`
    }
    return code
}

function generateConsts() {
    let code = ''
    let libConst = require("./api/consts.js")
    for(defConst of libConst) {
        code+= generateConstMapping(defConst)
    }
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

const const_mapping_header_mark_start = "// AUTO GENERATE CODE START [CONST CONVERT] --------"
const const_mapping_header_mark_end = "// AUTO GENERATE CODE END [CONST CONVERT] --------"

function start(name) {
    return `// AUTO GENERATE CODE START [${name.toUpperCase()}] --------`
}
function end(name) {
    return `// AUTO GENERATE CODE END [${name.toUpperCase()}] --------`
}


let dist_src_path = __dirname + "/../widgets_gen.c"
let dist_header_path = __dirname + "/../widgets_gen.h"

let struct_wrapper_src_path = __dirname + "/../be_lv_struct_wrapper.c"

function main() {
    let code = ""


    for(let clzConf of WidgetsMeta) {
        code+= gen_lv_methods(clzConf)
    }
    
    let src = fs.readFileSync(dist_src_path).toString()
    src = srcInsert(src, code, methodlst_mark_start, methodlst_mark_end)

    src = srcInsert(src, generateClassDefine(), defclass_mark_start, defclass_mark_end)
    src = srcInsert(src, generateClassRegister(), registerclass_mark_start, registerclass_mark_end)
    src = srcInsert(src, generateClassIDRegister(), registerclass_id_mark_start, registerclass_id_mark_end)

    src = srcInsert(src, generateConsts(), const_mapping_mark_start, const_mapping_mark_end)

    fs.writeFileSync(dist_src_path, src)

    // header file
    src = fs.readFileSync(dist_header_path).toString()
    src = srcInsert(src, generateConstDeclare(), const_mapping_header_mark_start, const_mapping_header_mark_end)
    fs.writeFileSync(dist_header_path, src)

    // struct wrapper
    src = fs.readFileSync(struct_wrapper_src_path).toString()
    src = srcInsert(src, StructWrapper.generateDefine(), start('CLASS DEFINE'), end('CLASS DEFINE'))   
    src = srcInsert(src, StructWrapper.generateInit(), start('CLASS ID INIT'), end('CLASS ID INIT'))   
    src = srcInsert(src, StructWrapper.generateRequire(), start('CLASS REQUIRE'), end('CLASS REQUIRE'))    
    fs.writeFileSync(struct_wrapper_src_path, src)


}
main()