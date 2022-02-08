const LstStuct = [
    {
        name: 'draw_rect_dsc' ,
        type: 'lv_draw_rect_dsc_t' ,
        jsclass: 'DrawRectDsc' ,
        members: [
            ["lv_coord_t", "radius"] ,
            // ["lv_blend_mode_t", "blend_mode"] ,
            /*Background*/
            ["lv_color_t", "bg_color"] ,
            ["lv_color_t", "bg_grad_color"] ,
            ["uint8_t", "bg_main_color_stop"] ,
            ["uint8_t", "bg_grad_color_stop"] ,
            ["lv_opa_t", "bg_opa"] ,
            // ["lv_grad_dir_t", "bg_grad_dir"] ,
            /*Background img*/
            // ["const void *", "bg_img_src"] ,
            // ["const void *", "bg_img_symbol_font"] ,
            ["lv_color_t", "bg_img_recolor"] ,
            ["lv_opa_t", "bg_img_opa"] ,
            ["lv_opa_t", "bg_img_recolor_opa"] ,
            ["uint8_t", "bg_img_tiled"] ,
            /*Border*/
            ["lv_color_t", "border_color"] ,
            ["lv_coord_t", "border_width"] ,
            ["lv_opa_t", "border_opa"] ,
            ["uint8_t", "border_post"] ,
            ["lv_border_side_t", "border_side"] ,
            /*Outline*/
            ["lv_color_t", "outline_color"] ,
            ["lv_coord_t", "outline_width"] ,
            ["lv_coord_t", "outline_pad"] ,
            ["lv_opa_t", "outline_opa"] ,
            /*Shadow*/
            ["lv_color_t", "shadow_color"] ,
            ["lv_coord_t", "shadow_width"] ,
            ["lv_coord_t", "shadow_ofs_x"] ,
            ["lv_coord_t", "shadow_ofs_y"] ,
            ["lv_coord_t", "shadow_spread"] ,
            ["lv_opa_t", "shadow_opa"] ,
        ] ,
        init: 'lv_draw_rect_dsc_init'
    } ,

    {
        name: 'draw_arc_dsc' ,
        type: 'lv_draw_arc_dsc_t' ,
        jsclass: 'DrawArcDsc' ,
        members: [
            ['lv_color_t', 'color'] ,
            ['lv_coord_t', 'width'] ,
            // ['const void *', 'img_src'] ,
            ['lv_opa_t', 'opa'] ,
            // ['lv_blend_mode_t', 'blend_mode'] ,
            ['uint8_t', 'rounded'] ,
        ] ,
        init: 'lv_draw_arc_dsc_init'
    } ,
    
    {
        name: 'draw_line_dsc' ,
        type: 'lv_draw_line_dsc_t' ,
        jsclass: 'DrawLineDsc' ,
        members: [
            ['lv_color_t', 'color'] ,
            ['lv_coord_t', 'width'] ,
            ['lv_coord_t', 'dash_width'] ,
            ['lv_coord_t', 'dash_gap'] ,
            // ['const void *', 'img_src'] ,
            ['lv_opa_t', 'opa'] ,
            // ['lv_blend_mode_t', 'blend_mode'] ,
            ['uint8_t', 'round_start'] ,
            ['uint8_t', 'round_end'] ,
            ['uint8_t', 'raw_end'] ,
        ] ,
        init: 'lv_draw_line_dsc_init'
    } ,
    
    {
        name: 'area' ,
        type: 'lv_area_t' ,
        jsclass: 'Area' ,
        args: [
            ['lv_coord_t', 'x1'] ,
            ['lv_coord_t', 'y1'] ,
            ['lv_coord_t', 'x2'] ,
            ['lv_coord_t', 'y2'] ,
        ] ,
        members: [
            ['lv_coord_t', 'x1'] ,
            ['lv_coord_t', 'y1'] ,
            ['lv_coord_t', 'x2'] ,
            ['lv_coord_t', 'y2'] ,
        ] ,
        // init: 'memset'
    } ,


    // mask
    // {
    //     name: 'draw_mask_radius_param' ,
    //     type: 'lv_draw_mask_radius_param_t' ,
    //     jsclass: 'DrawMaskRadius' ,
    //     members: [
    //         ['lv_coord_t', 'x1'] ,
    //         ['lv_coord_t', 'y1'] ,
    //         ['lv_coord_t', 'x2'] ,
    //         ['lv_coord_t', 'y2'] ,
    //     ] ,
    //     init: 'lv_draw_mask_radius_init'
    // } ,

    // 

    
]


const int_types = ['int8_t','int16_t','lv_coord_t']
const uint_types = ['uint8_t','uint16_t','lv_color_t','lv_opa_t', "lv_border_side_t"]

function toJsName(cname) {
    let jsname = ''
    for(let seg of cname.split('_')) {
        jsname+= seg[0].toUpperCase() + seg.substr(1)
    }
    return jsname
}


function argToMember(meta, mem_ctype, mem_name, func_name, argv) {
    let code = `    {\r\n`
    if(uint_types.includes(mem_ctype)) {
        code+= `        uint32_t num ;\r\n`
        code+= `        if( JS_ToUint32(ctx, &num, ${argv})!=0 ) {\r\n`
        code+= `            THROW_EXCEPTION("arg %s of %s() is invalid", "${mem_name}", "${func_name}") \r\n`
        code+= `        } \r\n`
        if(mem_ctype=='lv_color_t') {
            code+= `        cvar->${mem_name}.full = num ; \r\n`
        }
        else{
            code+= `        cvar->${mem_name} = (${mem_ctype})num ; \r\n`
        }

    }
    else if(int_types.includes(mem_ctype)) {
        code+= `        int32_t num ;\r\n`
        code+= `        if( JS_ToInt32(ctx, &num, ${argv})!=0 ) {\r\n`
        code+= `            THROW_EXCEPTION("arg %s of %s() is invalid", "${mem_name}", "${func_name}") \r\n`
        code+= `        } \r\n`
        code+= `        cvar->${mem_name} = (${mem_ctype})num ; \r\n`
    }
    else {
        console.log("unknow type", mem_ctype, "for", meta.type)
    }
    
    code+= `    } \r\n`
    // code+= `    dn(cvar->${mem_name})\r\n`

    return code
}


function memberToJS(meta, mem_ctype, mem_name, cvar) {
    if(uint_types.includes(mem_ctype)) {
        
        if(mem_ctype=='lv_color_t') {
            return `JS_NewUint32(ctx, (uint32_t)${cvar}->${mem_name}.full)`
        } else {
            return `JS_NewUint32(ctx, (uint32_t)${cvar}->${mem_name})`
        }
    }
    else if(int_types.includes(mem_ctype)) {
        return `JS_NewInt32(ctx, (int32_t)${cvar}->${mem_name})`
    }
    else {
        console.log("unknow type", mem_ctype, "for", meta.type)
        return 'JS_UNDEFINED'
    }
}

exports.generateDefine = function() {
    let code = ''
    for(let meta of LstStuct) {
        code+= `
// wrap lvgl struct ${meta.type}
JSClassID js_lv_${meta.name}_class_id ;
static JSValue js_lv_${meta.name}_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    JSValue proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto)) {
        JS_FreeValue(ctx, proto) ;
        return JS_EXCEPTION ;
    }
    JSValue jsobj = JS_NewObjectProtoClass(ctx, proto, js_lv_${meta.name}_class_id) ;
    JS_FreeValue(ctx, proto) ;

    ${meta.type} * cvar = malloc(sizeof(${meta.type})) ;
`
        if(meta.init=='memset') {
            code+= `    memset(cvar, 0, sizeof(${meta.type})) ;\r\n`
        }
        else if(meta.init) {
            code+= `    ${meta.init}(cvar) ;\r\n`
        }

        // 构造函数参数
        if(meta.args && meta.args.length) {
            code+= `\r\n    CHECK_ARGC(${meta.args.length})\r\n`
            for(let i=0; i<meta.args.length; i++) {
                code+= argToMember(meta, meta.args[i][0], meta.args[i][1], `lv.${meta.jsclass}`, `argv[${i}]`)
            }
        }

        code+= `
    JS_SetOpaque(jsobj, cvar) ;
    return jsobj ;
}
static void js_lv_${meta.name}_finalizer(JSRuntime *rt, JSValue this_val){
    ${meta.type} * cvar = JS_GetOpaque(this_val, js_lv_${meta.name}_class_id) ;
    if(cvar) {
        // printf("free(${meta.type}*)\\n") ;
        free(cvar) ;
    }
}
static JSClassDef js_lv_${meta.name}_class = {
    "${meta.jsclass}",
    .finalizer = js_lv_${meta.name}_finalizer,
} ;
`


        let code_func_setter = ''
        let code_func_getter = ''
        for(let [mem_ctype,mem_name] of meta.members) {

            let jsname = toJsName(mem_name)
            let setterName = 'set' + jsname
            let getterName = jsname[0].toLowerCase() + jsname.substr(1)

            
            code_func_setter+= `    JS_CFUNC_DEF("${setterName}", 0, js_lv_${meta.name}_set_${mem_name}),\r\n`
            code_func_getter+= `    JS_CFUNC_DEF("${getterName}", 0, js_lv_${meta.name}_get_${mem_name}),\r\n`

            code+=
`static JSValue js_lv_${meta.name}_set_${mem_name}(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ${meta.type} * cvar = JS_GetOpaque(this_val, js_lv_${meta.name}_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("${setterName}() must be call with a lv.${meta.jsclass} object")
    }
${argToMember(meta, mem_ctype, mem_name, setterName, 'argv[0]')}
    return JS_UNDEFINED ;
}
static JSValue js_lv_${meta.name}_get_${mem_name}(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    ${meta.type} * cvar = JS_GetOpaque(this_val, js_lv_${meta.name}_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("${setterName}() must be call with a lv.${jsname} object")
    }
    return ${memberToJS(meta,mem_ctype,mem_name,'cvar')} ;
}
`

        }



        code+= `
static const JSCFunctionListEntry js_lv_${meta.name}_proto_funcs[] = {
${code_func_setter}
${code_func_getter}
} ;

`
    }
    return code
}

exports.generateRequire = function() {
    let code = ''
    for(let meta of LstStuct) {
        code+= `    qjs_def_class(ctx, "${meta.jsclass}", js_lv_${meta.name}_class_id, &js_lv_${meta.name}_class
                , "lvgl.${meta.jsclass}", js_lv_${meta.name}_constructor, js_lv_${meta.name}_proto_funcs, countof(js_lv_${meta.name}_proto_funcs), JS_UNDEFINED, lvgl) ;\r\n\r\n`
    }
    return code
}
exports.generateInit = function() {
    let code = ''
    for(let meta of LstStuct) {
        code+= `    JS_NewClassID(&js_lv_${meta.name}_class_id);\r\n`
    }
    return code
}
exports.generateDeclare = function() {
    let code = ''
    for(let meta of LstStuct) {
    }
    return code
}

exports.LstStuct = LstStuct





// exports.generateDefine = function() {

//     let code = ''
//     for(let meta of LstStuct) {

//     code+= `JSValue be_lv_jsobj_to_struct_${meta.name}(JSContext * ctx, JSValue jsobj, ${meta.type} * cvar) {
//     ${meta.init}(cvar) ;
//     uint32_t propCnt ;
//     JSPropertyEnum *dscs;
//     if (JS_GetOwnPropertyNames(ctx, &dscs, &propCnt, jsobj, JS_GPN_STRING_MASK | JS_GPN_ENUM_ONLY) < 0){
//         THROW_EXCEPTION("arg is invalid")
//     }
//     JSValue key ;
//     const char * ckey ;
//     int32_t num_int ;
//     uint32_t num_uint ;
//     for(int p=0;p<propCnt;p++) {
//         key = JS_AtomToValue(ctx, dscs[p].atom);
//         ckey = JS_ToCString(ctx, key) ;
//         JSValue val = JS_GetProperty(ctx, jsobj, dscs[p].atom) ;
// `
//         let midx = 0 ;
//         for(let [mem_ctype,mem_name] of meta.members) {
//             let js_mem_name = mem_name.replace(/_/g,'-')
//             code+= `        ${(midx++)?"else ":''}if( strcmp("${js_mem_name}", ckey)==0 ){ \r\n`
//             if((['uint8_t','uint16_t','lv_color_t','lv_opa_t']).includes(mem_ctype)) {
//                 code+= `            if( JS_ToUint32(ctx, &num_uint, val)!=0 ) {\r\n`
//                 code+= `                THROW_EXCEPTION("member %s of ${meta.type} is invalid", "${js_mem_name}") \r\n`
//                 code+= `            } \r\n`
//                 if(mem_ctype=='lv_color_t') {
//                     code+= `            cvar->${mem_name}.full = num_uint ; \r\n`
//                 }
//                 else{
//                     code+= `            cvar->${mem_name} = (${mem_ctype})num_uint ; \r\n`
//                 }
//                 code+= `            dn(cvar->${mem_name})\r\n`
//             }
//             else if((['int8_t','int16_t','lv_coord_t']).includes(mem_ctype)) {
//                 code+= `            if( JS_ToInt32(ctx, &num_int, val)!=0 ) {\r\n`
//                 code+= `                THROW_EXCEPTION("member %s of ${meta.type} is invalid", "${js_mem_name}") \r\n`
//                 code+= `            } \r\n`
//                 code+= `            cvar->${mem_name} = (${mem_ctype})num_int ; \r\n`
//                 code+= `            dn(cvar->${mem_name})\r\n`
//             }
//             else {
//                 console.log("unknow type", mem_ctype, "for", meta.type)
//             }
//             code+= `        }\r\n`

//         }

//         code+= `        JS_FreeValue(ctx, val) ;
//         JS_FreeCString(ctx, ckey) ;
//     }
//     return JS_TRUE ;
// }
// `   }

//     return code
// }
// exports.generateDeclare = function() {
//     let code = ''
//     for(let meta of LstStuct) {
//         code+= `JSValue be_lv_jsobj_to_struct_${meta.name}(JSContext * ctx, JSValue jsobj, ${meta.type} * cvar) ;\r\n\r\n`
//     }
//     return code
// }