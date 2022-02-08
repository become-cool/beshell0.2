const fs = require('fs')
const {fileInsert} = require("../source_insert.js")
const WidgetsMeta = require("../lv_widgets.meta.js")
const lvStructDef = require("./lvgl-struct.json")
const lvConstDef = require("./lvgl-const.json")

const mapExportWidgets = {}
const mapExportStructs = {
    'lv_disp_draw_buf_t': 'DispDrawBuf' ,
    'lv_disp_drv_t': 'DispDrv' ,
    'lv_disp_t': 'Disp' ,
    'lv_obj_class_t': 'ObjClass' ,
}
const mapCTypeAlias = {
    'struct _lv_obj_t': 'Obj'
}
let codeExports = ''

let collectArgTypes = []

const redefineFunctions = {
    lv_obj_set_width(width) {
        Module._lv_obj_set_width(this.ptr, size(width))
    } ,
    lv_obj_set_height(height) {
        Module._lv_obj_set_height(this.ptr, size(height))
    } ,
}


// 转换为驼峰命令风格，去掉前导get
function transMethodName(cfuncName, ctypePrefix) {
    let jsmethod = cfuncName.substr(ctypePrefix.length)
    let arr = jsmethod.split("_")
    jsmethod = arr.shift()
    if(jsmethod=='get') {
        jsmethod = arr.shift()
    }
    for(let n of arr) {
        jsmethod+= n[0].toUpperCase() + n.substr(1)
    }

    return jsmethod
}

function ctypeToJSClass(ctype) {
    if(!ctype.match(/ \*$/)) {
        return null
    }
    let returnCType = ctype.substr(0,ctype.length-2)
    return mapExportWidgets[returnCType] || mapExportStructs[returnCType] || mapCTypeAlias[returnCType] || null
}

function generateMethod(methodDef, ctypePrefix, thisType) {

    let methodName = transMethodName(methodDef.name, ctypePrefix)

    if( redefineFunctions[methodDef.name] ) {
        let codeFunc = redefineFunctions[methodDef.name].toString().substr(methodDef.name.length)
        return "\r\n    " + methodName + codeFunc
    }

    let args = []
    let argTypes = []
    let formalArgs = []

    let first = methodDef.args[0]
    if( first && first.type == thisType ) {
        args.push('this.ptr')
        argTypes.push('"number"')
    }
    let needCCall = false
    for(let argDef of methodDef.args.slice(1)) {
        // struct pointer
        if( !! ctypeToJSClass(argDef.type) ) {
            args.push(argDef.name+'.ptr')
            argTypes.push(`"pointer"`)
        }
        // const
        else if(lvConstDef[argDef.type]) {
            args.push(
                `constMapping.${lvConstDef[argDef.type].jsname}.value(${argDef.name})`
            )
            argTypes.push(`"number"`)
        }
        // string
        else if(argDef.type=="char *" || argDef.type=="const char *") {
            args.push(argDef.name)
            argTypes.push(`"string"`)
            needCCall = true
        }
        else {
            args.push(argDef.name)
            argTypes.push(`"number"`)
        }
        
        formalArgs.push(argDef.name)
    }
    args = args.join(', ')
    formalArgs = formalArgs.join(', ')
    argTypes = argTypes.join(', ')


    if(methodDef.returnType=="char *" || methodDef.returnType=="const char *") {
        var ccallReturnType = "string"
        needCCall = true
    }
    else if(methodDef.returnType=="bool") {
        var ccallReturnType = "bool"
        needCCall = true
    }
    else {
        var ccallReturnType = "number"
    }

    let code = `
    ${methodName}(${formalArgs}) {`

    let returnWrapperClass = ctypeToJSClass(methodDef.returnType)

    if(needCCall) {
        var codeCall = `Module.ccall("${methodDef.name}", "${ccallReturnType}", [${argTypes}], [${args}])`
    }
    else {
        var codeCall = `Module._${methodDef.name}(${args})`
    }

    // 无返回值
    if(methodDef.returnType=='void') {
        code+= `\r\n        ${codeCall}\r\n`
    }
    // js wrap widget/struct
    else if(returnWrapperClass) {
        code+= `\r\n        return WASMObject.wrap(${returnWrapperClass}, ${codeCall})\r\n`
    }
    else {
        code+= `\r\n        return ${codeCall}\r\n`
    }

    code+= "    }"
    return code
}

function generateWidgets() {

    let code = ""

    for(let wgtDef of WidgetsMeta) {

        mapExportWidgets[wgtDef.typeName+'_t'] = wgtDef.className

        if(wgtDef.className=='Obj') {
            code+= `class ${wgtDef.className} extends EventEmitter {
    constructor(parent) {
        super()
        this._createWidget(parent)
    }`
        }
        else {
            code+= `class ${wgtDef.className} extends Obj {`
        }

        code+= `
    _createWidget(parent) {
        if(!parent instanceof Obj) {
            throw new Error("parent must be a lv.Obj")
        }
        this.ptr = Module._${wgtDef.typeName}_create(parent?parent.ptr:null)
    }`
        // methods 
        let ctypePrefix = wgtDef.typeName + '_'
        for(let cfuncName in wgtDef.methods) {
            let [, argvDef, returnType] = wgtDef.methods[cfuncName]
            let methodDef = {
                name: cfuncName ,
                returnType,
                args: [{type:'lv_obj_t *',name:'obj'}] ,
            }
            for(let [type, name] of argvDef) {
                methodDef.args.push({ name, type })
            }
            code+= generateMethod(methodDef,ctypePrefix,'lv_obj_t *')
        }

        code+= "\r\n}\r\n"

        codeExports+= `    exports.${wgtDef.className} = ${wgtDef.className}\r\n`
    }

    fileInsert(__dirname + "/../../wasm/src/lvgl.js", code, 'LVGL.JS WIDGETS')
}


function generateStructs() {
    let code = ""
    for(let ctype of Object.keys(mapExportStructs)) {
        if(!lvStructDef[ctype]) {
            throw new Error("unknow type "+ctype)
        }
        let clzDef = lvStructDef[ctype]
        code+= `class ${clzDef.className} extends WASMObject {
    constructor() {
        super()
        this.ptr=Module._${clzDef.ctypePrefix}create()
    }`


        for(let methodDef of clzDef.methods){
            code+= generateMethod(methodDef, clzDef.ctypePrefix, clzDef.thisType)
        }

        code+= "\r\n}\r\n"
        
        codeExports+= `    exports.${clzDef.className} = ${clzDef.className}\r\n`
    }

    fileInsert(__dirname + "/../../wasm/src/lvgl.js", code, 'LVGL.JS STRUCTS')
}

function generateConstNameMapping() {
    let code = ''
    for(let ctype in lvConstDef) {
        code+= `    ${lvConstDef[ctype].jsname} : {
        value(name) {
            return Module.ccall("${lvConstDef[ctype].valueFunc}", "number", ["string"], [name])
        } ,
        name(value) {
            return Module.ccall("${lvConstDef[ctype].nameFunc}", "string", ["number"], [value])
        }
    } ,
`
    }

    fileInsert(__dirname + "/../../wasm/src/lvgl.js", code, 'LVGL.JS CONST MAPPING')
}

;(function main() {
    generateWidgets()

    generateStructs()

    generateConstNameMapping()

    fileInsert(__dirname + "/../../wasm/src/lvgl.js", codeExports, 'EXPORT JS CLASS')

    let data = fs.readFileSync(__dirname + "/../../wasm/src/lvgl.js")
    fs.writeFileSync(__dirname + "/../../wasm/build/lvgl.js", data)
}) () ;