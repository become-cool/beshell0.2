const fs = require("fs")

const lvapi_obj = require("./api/lv_obj.js")

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

const MapC2JS_IntConver = {
    "uint32_t": conver_uint32 ,
    "int32_t": conver_int32 ,
    "uint16_t": conver_uint16 ,
    "int16_t": conver_int16 ,
    "lv_coord_t": conver_int16 ,
    "lv_obj_flag_t": conver_uint32 ,
    "lv_state_t": conver_uint16 ,
    "lv_align_t": conver_uint8 ,
    "bool": conver_bool ,
    "lv_anim_enable_t": conver_bool,
    "lv_color_t": conver_uint16 ,
    "lv_scrollbar_mode_t": conver_uint8,
    "lv_scroll_snap_t": conver_uint8,
    "lv_dir_t": conver_uint8,
    "lv_area_t *": conver_area ,
    "lv_point_t *": conver_point ,
    "lv_obj_t *": conver_obj,
    "_lv_obj_t *": conver_obj,
}

// 在栈上创建、填充的 struct 参数，做为指针传递给 lvgl c api
const ArrStructArgTypes = ['lv_area_t *', 'lv_point_t *']



const MapJS2C_IntConver = {
    "uint32_t": "JS_NewUint32" ,
    "int32_t": "JS_NewInt32" ,
    "uint16_t": "JS_NewUint32" ,
    "int16_t": "JS_NewInt32" ,
    "lv_coord_t": "JS_NewInt32" ,
    "lv_state_t": "JS_NewUint32" ,
    "lv_obj_flag_t": "JS_NewUint32" ,
    "lv_scrollbar_mode_t": "JS_NewUint32" ,
    "lv_scroll_snap_t": "JS_NewUint32" ,
    "lv_dir_t": "JS_NewUint32" ,
}

function gen_lv_class(cName, className, api){

    let codeFuncDef = ""
    let codeMethedList = `static const JSCFunctionListEntry js_${cName}_proto_funcs[] = {\r\n`

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
        codeFuncDef+= `    lv_obj_t * thisobj = JS_GetOpaque(this_val, js_${cName}_class_id) ;\r\n`
        codeFuncDef+= `    if(!thisobj) {\r\n`
        codeFuncDef+= `        THROW_EXCEPTION("${className}.${jsmethod}() must be called as a ${className} method")\r\n`
        codeFuncDef+= `    }\r\n`

        let cargvLst = ''
        if(argLstConf) {
            for(let i=0;i<argLstConf.length;i++) {
                let [argtype, cargv] = argLstConf[i]
                if(MapC2JS_IntConver[argtype]) {
                    codeFuncDef+= MapC2JS_IntConver[argtype](cargv, i, className, jsmethod)
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
            }
        }

        // js method 返回 this
        if(returnType=="this") {
            codeFuncDef+= `    ${cfunc}(thisobj${cargvLst}) ;\r\n`
            codeFuncDef+= `    JS_DupValue(ctx, this_val) ;\r\n`
        }
        else if(returnType=="void") {
            codeFuncDef+= `    ${cfunc}(thisobj${cargvLst}) ;\r\n`
            codeFuncDef+= `    return JS_UNDEFINED ;\r\n`
        }
        else if(returnType=="_lv_obj_t *"||returnType=="lv_obj_t *") {
            codeFuncDef+= `    return lv_obj_get_user_data(${cfunc}(thisobj${cargvLst})) || JS_NULL ;\r\n`
        }
        else if(returnType=="bool") {
            codeFuncDef+= `    return JS_NewBool(ctx,${cfunc}(thisobj${cargvLst})) ;\r\n`
        }
        else if(MapJS2C_IntConver[returnType]) {
            codeFuncDef+= `    return ${MapJS2C_IntConver[returnType]}(ctx,${cfunc}(thisobj${cargvLst})) ;\r\n`
        }
        else {
            console.error("unknow return type",returnType, "for function", cfunc)
        }

        codeFuncDef+= "}\r\n\r\n"

        codeMethedList+= `    JS_CFUNC_DEF("${jsmethod}", 0, js_${cfunc}),\r\n`
    }

    codeMethedList+= "} ;\r\n\r\n"

    return codeFuncDef + "\r\n" + codeMethedList

}

let src_mark_start = "// AUTO GENERATE CODE START --------"
let src_mark_end = "// AUTO GENERATE CODE END --------"
let src_path = __dirname + "/widgets.template.c"
let dist_path = __dirname + "/../widgets.c"

function main() {
    let code = ""
    code+= gen_lv_class("lv_obj", "lvgl.Obj", lvapi_obj)

    let src = fs.readFileSync(src_path).toString()
    let markStartAt = src.indexOf(src_mark_start)
    let markEndAt = src.indexOf(src_mark_end)
    if(markStartAt<0 || markEndAt<0) {
        console.log(code)    
        console.log("not found mark start or end")
        return
    }

    code = src.substr(0, markStartAt) + src_mark_start + "\r\n" + code + src.substr(markEndAt) 
    // console.log(code)

    fs.writeFileSync(dist_path, code)

}
main()