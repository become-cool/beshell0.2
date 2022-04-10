const fs = require('fs')
const {fileInsert} = require("../source_insert.js")
const WidgetsMeta = require("./widgets-meta.js")
const lvStructDef = require("./lvgl-struct.json")
const lvConstDef = require("./lvgl-const.json")
const lvConstAPI = require("../api/consts.js")

delete WidgetsMeta.map.Obj.methods.lv_obj_set_coords

const mapExportWidgets = {
}
const mapExportStructs = {
    'lv_disp_draw_buf_t': 'DispDrawBuf' ,
    'lv_disp_drv_t': 'DispDrv' ,
    'lv_disp_t': 'Disp' ,
    'lv_obj_class_t': 'ObjClass' ,
    'lv_indev_drv_t': 'IndevDrv' ,
    'lv_indev_data_t': 'IndevData' ,
    'lv_point_t': 'Point' ,
}
const mapCTypeAlias = {
    'struct _lv_obj_t': 'Obj' ,
    'lv_style_t': 'Style' ,
}
const lvExtraDef = {
    Obj: `
    public setCoordX(val:number) {
        Module._lv_obj_set_coord_x(this.ptr, val)
    }
    public setCoordY(val:number) {
        Module._lv_obj_set_coord_y(this.ptr, val)
    }
    public setCoords(x:number,y:number) {
        Module._lv_obj_set_coords_redefine(this.ptr, x, y)
    }
    public coordX() {
        return Module._lv_obj_get_coord_x(this.ptr)
    }
    public coordY() {
        return Module._lv_obj_get_coord_y(this.ptr)
    }
    public coords() {
        return [Module._lv_obj_get_coord_x(this.ptr), Module._lv_obj_get_coord_y(this.ptr)]
    }
` ,

    Disp: `
    public sharedKeyboard?: Keyboard
    public popupSharedKeyboard(txtarea:TextArea) {
        if(!this.sharedKeyboard){
            this.sharedKeyboard = new Keyboard(this.actScr())
        }
        else {
            this.sharedKeyboard.setParent(this.actScr())
        }
        this.sharedKeyboard.popup(txtarea)
    }
`
}

const redefineFunctions = {
    lv_obj_set_width: `public setWidth(width:string|number) {
        Module._lv_obj_set_width(this.ptr, size(width))
    }` ,
    lv_obj_set_height: `public setHeight(height:string|number) {
        Module._lv_obj_set_height(this.ptr, size(height))
    }` ,
}

const createWidgetMethods = {
    Btn: `
    public label?: Label
    constructor(parent: Obj|null, ptr=0) {
        super(parent,ptr)
        if(ptr==0) {
            this.label = new Label(this)
            this.label.setHeight(-1)
            this.label.setWidth("100%")
            this.label.setStyle("text-align","center")
            this.label.align("center",0,0)
        }
    }
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_btn_create(parent?parent.ptr:null)
        this.setWidth(80)
        this.setHeight(20)
        this.registerPointer()
    }
    text() {
        return this.label?.text()
    }
    setText(text: string) {
        this.label?.setText(text)
    }
    // font() {
    //     return this.label?.font()
    // }
    // setFont(font: string) {
    //     this.label?.setFont(font)
    // }
    `,
    
    TextArea: `
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._lv_textarea_create(parent?parent.ptr:null)
        this.registerPointer()
        this.on("clicked",()=>{
            this.disp().popupSharedKeyboard(this)
        })
    }`
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
    ctype = ctype.replace(/^struct /, '')
    ctype = ctype.replace(/^_/, '')
    let returnCType = ctype.substr(0,ctype.length-2)
    return mapExportWidgets[returnCType] || mapExportStructs[returnCType] || mapCTypeAlias[returnCType] || null
}

function generateMethod(methodDef, ctypePrefix, thisType) {

    let methodName = transMethodName(methodDef.name, ctypePrefix)

    if( redefineFunctions[methodDef.name] ) {
        return "\r\n    " + redefineFunctions[methodDef.name]
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
        let argClassName = ctypeToJSClass(argDef.type)

        // struct pointer
        if( argClassName  ) {
            args.push(argDef.name+'.ptr')
            argTypes.push(`"pointer"`)
            formalArgs.push(argDef.name+":"+argClassName)
        }
        // const
        else if(lvConstDef[argDef.type]) {
            args.push(`constMapping.${lvConstDef[argDef.type].jsname}.value(${argDef.name})`)
            argTypes.push(`"number"`)
            formalArgs.push(argDef.name+":string")
        }
        // string
        else if(argDef.type=="char *" || argDef.type=="const char *") {
            args.push(argDef.name)
            argTypes.push(`"string"`)
            formalArgs.push(argDef.name+":string")
            needCCall = true
        }
        else {
            args.push(argDef.name)
            argTypes.push(`"number"`)
            formalArgs.push(argDef.name+":any")
        }
        
    }
    args = args.join(', ')
    formalArgs = formalArgs.join(', ')
    argTypes = argTypes.join(', ')

    let returnWrapperClass = ctypeToJSClass(methodDef.returnType)

    let methodReturnType = 'any'
    if(methodDef.returnType=="char *" || methodDef.returnType=="const char *") {
        var ccallReturnType = "string"
        needCCall = true
        methodReturnType = 'string'
    }
    else if(methodDef.returnType=="bool") {
        var ccallReturnType = "bool"
        needCCall = true
        methodReturnType = 'boolean'
    }
    else if(methodDef.returnType=="lv_obj_t *") {
        var ccallReturnType = "number"
        methodReturnType = 'Obj'
    }
    else {
        var ccallReturnType = "number"
        methodReturnType = 'any'
    }

    let code = `
    public ${methodName}(${formalArgs}): $methodReturnType {`

    if(needCCall) {
        var codeCall = `Module.ccall("${methodDef.name}", "${ccallReturnType}", [${argTypes}], [${args}])`
    }
    else {
        var codeCall = `Module._${methodDef.name}(${args})`
    }

    // 无返回值
    if(methodDef.returnType=='void') {
        code+= `\r\n        ${codeCall}\r\n`
        methodReturnType = 'void'
    }
    // js wrap widget/struct
    else if(returnWrapperClass) {
        code+= `\r\n        return WASMObject.wrap(${codeCall}, ${returnWrapperClass})\r\n`
        methodReturnType = returnWrapperClass
    }
    else if(lvConstDef[methodDef.returnType]){
        code+= `\r\n        return constMapping.${lvConstDef[methodDef.returnType].jsname}.name(${codeCall})\r\n`
    }
    else {
        code+= `\r\n        return ${codeCall}\r\n`
    }

    code+= "    }"
    
    return code.replace("$methodReturnType", methodReturnType)
}

function generateWidgets() {

    let code = ""

    for(let wgtDef of WidgetsMeta) {

        mapExportWidgets[wgtDef.typeName+'_t'] = wgtDef.className

        if(wgtDef.className=='Obj') {
            code+= `export class ${wgtDef.className} extends EventEmitter {
    public id = ""
    public name = ""
    constructor(parent: Obj|null, ptr=0) {
        super()
        this.on("#EVENT.ADD#", (name:string)=>{
            try{ Module._lv_obj_enable_event(this.ptr,constMapping.EVENT_CODE.value(name)) }catch(e){}
        })
        this.on("#EVENT.CLEAR#", (name:string)=>{
            try{ Module._lv_obj_disable_event(this.ptr,constMapping.EVENT_CODE.value(name)) }catch(e){}
        })
        if(ptr) {
            this.ptr = ptr
            this.registerPointer()
        }
        else {
            this._createWidget(parent)
        }
    }
    `
        }
        else {
            code+= `export class ${wgtDef.className} extends Obj {`
        }

        if(lvExtraDef[wgtDef.className]) {
            code+= lvExtraDef[wgtDef.className]
        }

        if(createWidgetMethods[wgtDef.className]) {
            code+= createWidgetMethods[wgtDef.className]
        }
        else {
            code+= `
    protected _createWidget(parent: Obj|null) {
        this.ptr = Module._${wgtDef.typeName}_create(parent?parent.ptr:null)
        this.registerPointer()
    }`

        }
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

        // codeExports+= `    exports.${wgtDef.className} = ${wgtDef.className}\r\n`
    }

    return fileInsert(__dirname + "/../../wasm/src/lvgl.ts", code, 'LVGL.JS WIDGETS')
}


function generateStructs() {
    let code = ""
    for(let ctype in mapExportStructs) {
        if(!lvStructDef[ctype]) {
            throw new Error("unknow type "+ctype)
        }
        let clzDef = lvStructDef[ctype]
        code+= `export class ${clzDef.className} extends WASMObject {
    constructor() {
        super()
        this.ptr=Module._${clzDef.ctypePrefix}create()
    }`

        if(lvExtraDef[clzDef.className]) {
            code+= lvExtraDef[clzDef.className]
        }

        for(let methodDef of clzDef.methods){
            code+= generateMethod(methodDef, clzDef.ctypePrefix, clzDef.thisType)
        }

        code+= "\r\n}\r\n"
        
        // codeExports+= `    exports.${clzDef.className} = ${clzDef.className}\r\n`
    }

    return fileInsert(__dirname + "/../../wasm/src/lvgl.ts", code, 'LVGL.JS STRUCTS')
}

function generateConstNameMapping() {
    let code = "const constMapping = {\r\n"
    for(let ctype in lvConstDef) {
        code+= `    ${lvConstDef[ctype].jsname} : {
        value(name: string): number {
            let value = Module.ccall("${lvConstDef[ctype].valueFunc}", "number", ["string"], [name])
            if(value<0) {
                throw new Error("unknow const ${lvConstDef[ctype].jsname} value:"+name)
            }
            return value
        } ,
        name(value: number): string { return Module.ccall("${lvConstDef[ctype].nameFunc}", "string", ["number"], [value]) }
    } ,
`
    }
    code+= "}\r\n\r\n"
    
    for(let defConst of lvConstAPI) {
        let ctype = defConst.type
        code+= `export type ${lvConstDef[ctype].jsname} = `
        let values = defConst.def.map(name=>{
            let value = name.substr(defConst.prefix.length).toLowerCase().replace(/_/g,'-')
            return '"'+value+'"'
        })
        code+= values.join(" | ") + "\r\n"
    }

    return fileInsert(__dirname + "/../../wasm/src/lvgl.ts", code, 'LVGL.JS CONST MAPPING')
}

;(function main() {
    generateWidgets()

    generateStructs()

    let data = generateConstNameMapping()

    // export all widgets list
    let code = Object.values(mapExportWidgets).join(", \r\n")+"\r\n"
    fileInsert(__dirname + "/../../wasm/src/lvgl.ts", code, 'LVGL.JS WIDGET NAMES')

    // write to build
    fs.writeFileSync(__dirname + "/../../wasm/build/lvgl.ts", data)
}) () ;