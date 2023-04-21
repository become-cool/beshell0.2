const fs = require("fs")
const {StyleProps, StyleConstProps} = require("./api/style.js")


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

// bool lv_style_prop_jsstr_to_const(JSContext *ctx, JSValue jsstr, lv_style_prop_t* out) ;
// const char * lv_style_prop_const_to_str(lv_style_prop_t code) ;
// JSValue lv_style_prop_const_to_jsstr(JSContext *ctx, lv_style_prop_t code) ;

function generateStyleValueFuncs() {

    let src_styles_int = ""
    let src_styles_uint = ""
    let src_styles_color = ""
    let src_styles_ptr = ""
    let src_styles_convert_c2js = ""
    let src_styles_convert_js2c = ""
    
    for(let prop of StyleProps) {
        if(!prop.name) {
            continue
        }
        // console.log(prop.style_type, prop.var_type)
        if( StyleConstProps.includes(prop.var_type) ) {
            let name = prop.var_type.replace(/^lv_/,'').replace(/_t$/,'')
            let c2jsName = `lv_${name}_const_to_jsstr`
            src_styles_convert_c2js+= `        case LV_STYLE_${prop.name}:
            return ${c2jsName}(ctx, value.num) ;
`
            
            let js2cName = `lv_${name}_jsstr_to_const`
            src_styles_convert_js2c+= `        case LV_STYLE_${prop.name}:
            return ${js2cName}(ctx, jsval, &(value->num)) ;
`

        } else if(prop.style_type=='num') {
            if(prop.var_type=="lv_coord_t") {
                src_styles_int+= `        case LV_STYLE_${prop.name}: \r\n`
            }
            else {
                src_styles_uint+= `        case LV_STYLE_${prop.name}: \r\n`
            }
        }
        else if(prop.style_type=='color') {
            src_styles_color+= `        case LV_STYLE_${prop.name}: \r\n`
        }
        else if(prop.style_type=='ptr') {
            src_styles_ptr+= `        case LV_STYLE_${prop.name}: \r\n`
        }
    }

    return `
JSValue lv_style_value_to_js(JSContext * ctx, lv_style_prop_t prop, lv_style_value_t value) {
    if( prop==LV_STYLE_FLEX_FLOW ){
        return lv_flex_flow_const_to_jsstr(ctx, value.num) ;
    }
    else if( prop==LV_STYLE_FLEX_MAIN_PLACE
        || prop==LV_STYLE_FLEX_CROSS_PLACE
        || prop==LV_STYLE_FLEX_TRACK_PLACE
    ) {
        return lv_flex_align_const_to_jsstr(ctx, value.num) ;
    }
    else if( prop==LV_STYLE_FLEX_GROW ) {
        return JS_NewUint32(ctx, value.num) ;
    }
    switch(prop) {
${src_styles_convert_c2js}
${src_styles_int}
            return JS_NewInt32(ctx, (int32_t)value.num) ;
${src_styles_uint}
            return JS_NewUint32(ctx, value.num) ;
${src_styles_color}
            return JS_NewUint32(ctx, value.color.full) ;
        default:
            return JS_NewString(ctx, "unknow type") ;
    }
}


bool lv_style_js_to_value(JSContext * ctx, lv_style_prop_t prop, JSValue jsval, lv_style_value_t * value) {
    if( prop==LV_STYLE_FLEX_FLOW ){
        return lv_flex_flow_jsstr_to_const(ctx, jsval, &(value->num)) ;
    }
    else if( prop==LV_STYLE_FLEX_MAIN_PLACE
        || prop==LV_STYLE_FLEX_CROSS_PLACE
        || prop==LV_STYLE_FLEX_TRACK_PLACE
    ) {
        return lv_flex_align_jsstr_to_const(ctx, jsval, &(value->num)) ;
    }
    else if( prop==LV_STYLE_FLEX_GROW ) {
        return JS_ToUint32(ctx, (uint32_t*)&(value->num), jsval)==0 ;
    }
    switch(prop) {
${src_styles_convert_js2c}
${src_styles_int}
            return JS_ToInt32(ctx, &(value->num), jsval)==0 ;
${src_styles_uint}
            return JS_ToUint32(ctx, (uint32_t*)&(value->num), jsval)==0 ;
${src_styles_color}
            return JS_ToUint32(ctx, &(value->color.full), jsval)==0 ;
        default:
            return false ;
    }
}
`
    
}



const mark_start = "// AUTO GENERATE CODE START [STYLE VALUE SETTER/GETTER] --------"
const mark_end = "// AUTO GENERATE CODE END [STYLE VALUE SETTER/GETTER] --------"




let dist_path = __dirname + "/../style.c"

function main() {

    // console.log(generateStyleValueFuncs())
    
    let src = fs.readFileSync(dist_path).toString()
    src = srcInsert(src, generateStyleValueFuncs(), mark_start, mark_end)
    fs.writeFileSync(dist_path, src)
}

main()