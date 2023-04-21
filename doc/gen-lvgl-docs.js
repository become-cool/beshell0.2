const fs = require("fs")
const {basename} = require("path")
const javadoc = require("javadoc")
const {updateCloud, createDocCloud, api_host} = require("./api.js")

const LibConst = require("../generator/api/consts.js")
const LibStruct = require("../generator/api/struct.js")
const {StyleProps, StyleConstProps} = require("../generator/api/style.js")

const LibWidget = require("../generator/lv_widgets.meta.js")
const dids_widget = {
    Obj: 35,
    Label: 36,
    Arc: 37,
    Bar: 38,
    Btn: 39,
    BtnMatrix: 40,
    Canvas: 41,
    Checkbox: 42,
    Dropdown: 43,
    Img: 44,
    Line: 45,
    Roller: 46,
    Slider: 47,
    Switch: 48,
    Table: 49,
    TextArea: 50,
    MsgBox: 51,
    Keyboard: 52,
    TileView: 53,
    TabView: 54,
    Spinner: 55,
    List: 56,
    Group: 57
}

const did_style = 24
const did_event = 25
const did_struct = 26
const did_const = 58

const number_types = [
    // signed
    'int8_t','int16_t','int32_t','lv_coord_t' ,
    // unsigned
    'uint8_t','uint16_t','uint32_t','lv_color_t','lv_opa_t', "lv_border_side_t"
]

const bool_types = ['lv_res_t', 'lv_anim_enable_t']
const string_types = ['char *', 'const char *']

function iconvType(ctype) {

    let jsType = ctype
    let signType = ctype
    let type = 'scalar'

    ctype = ctype.trim().replace(/\*$/,'').trim()

    if(LibConst.byName[ctype]) {
        jsType = `[${ctype}](http://${api_host}/doc/${did_const}#${ctype})`
        signType = 'string'
        type = 'const'
    }
    else if(number_types.includes(ctype)) {
        jsType = `number`
        signType = 'number'
    }
    else if(bool_types.includes(ctype)) {
        jsType = `bool`
        signType = 'bool'
    }
    else if(string_types.includes(ctype)) {
        jsType = `string`
        signType = 'string'
    }
    else if(LibStruct.byName[ctype]) {
        let struct = LibStruct.byName[ctype]
        jsType = `[beapi.lvgl.${struct.jsclass}](http://${api_host}/doc/${did_struct}#${struct.type})`
        signType = jsType
    }
    else if(ctype=='void') {
        jsType = `undefined`
        signType = 'undefined'
    }
    else if(ctype=='lv_style_t *' || ctype=='lv_style_t'){
        jsType = `[beapi.lvgl.Style](http://doc.become.cool/doc/${did_style})`
        signType = `[beapi.lvgl.Style](http://doc.become.cool/doc/${did_style})`
        type = 'struct'
    }
    else if(ctype=='lv_obj_t *' || ctype=='lv_obj_t'){
        jsType = `[beapi.lvgl.Obj](http://doc.become.cool/doc/35)`
        signType = '[beapi.lvgl.Obj](http://doc.become.cool/doc/35)'
        type = 'struct'
    }

    return [jsType,signType,type]
}


async function createWidgetDoc() {
    const widgets_did = 23
    let widgets = Object.keys(LibWidget.byName)
    let mapDids = {}

    for(let widgetName of widgets) {
        let res = await createDocCloud(widgets_did, widgetName, "")
        if(res.data) {
            mapDids[widgetName] = res.data
        }
    }

    console.log(mapDids)
}

function print_help() {
    console.log("usage:")
    console.log("node gen-lvgl-docs.js widget <widget_name>")
}

function toCamelCase(cstyle) {
    return cstyle.split('_').map((seg,i)=>{
        if(i>0) {
            return seg[0].toUpperCase() + seg.slice(1)
        }
        else {
            return seg
        }
    }).join('')
}

function updateWidgetDoc(wname, wdid) {

    let widget = LibWidget.byName[wname]
    let methods = widget.methods

    if(wname=='Obj') {
        var parent = 'beapi.EventEmitter'
    }
    else {
        var parent = 'beapi.lvgl.Obj'
    }

    let md_methosd = ''
    for(let methodName in methods) {
        let [, argv, returnType] = methods[methodName]
        // console.log(methodName,argv,returnType)

        let jsMethodName = methodName.replace(widget.typeName+'_', '')
        jsMethodName = toCamelCase(jsMethodName)

        // console.log(methodName+'()')

        let md_args = ''
        let argsSign = []
        if(argv.length) {

            for(let [ctype,argName] of argv) {

                let [jsType,signType] = iconvType(ctype)

                md_args+= `* ${argName}: ${jsType}\r\n\r\n`
                argsSign.push(`${argName}:${signType}`)
            }
        }

        
        md_methosd+= `<h3 id="${jsMethodName}"></h3>\r\n\r\n`
        md_methosd+= '-----\r\n\r\n'
        md_methosd+= `### ${jsMethodName}(${argsSign.join(', ')})\r\n\r\n`
        md_methosd+= `> 该方法是对 LVGL C API \`${methodName}()\` 的包装\r\n\r\n`

        md_methosd+= `参数:\r\n\r\n`
        md_methosd+= md_args || '该函数没有参数\r\n\r\n'
        
        md_methosd+= `返回值:\r\n`

        let [jsReturnType,jsReturnSign,type] = iconvType(returnType)

        md_methosd+= `${jsReturnSign}\r\n\r\n`
        if(type=='const') {
            md_methosd+= `该函数的返回值是一个字符串常量: ${jsReturnType}\r\n\r\n`
        }
    }

    let md = `\`beapi.lvgl.${wname}\` 类是对 LVGL 的 Widget \`${widget.typeName}_t\` 的包装

**继承自: ${parent}**

## 方法:

${md_methosd || '该 Widget 没有定义方法'}
` 

    return updateCloud(dids_widget[wname],wname,md)
    
}


function updateConst() {
    let md = ''

    for(let consDef of LibConst) {
        md+= `<h3 id="${consDef.type}"></h3>\r\n\r\n-----\r\n\r\n`

        md+= `### 常量类型 ${consDef.type}\r\n\r\n`
        md+= `可选值(js 字符串):\r\n\r\n`
        
        for(let name of consDef.def) {

            let value = name.replace(consDef.prefix, '').replace(/_/g, '-').toLowerCase()
            
            md+= `* "${value}"\r\n`
            md+= `LVGL C 常量: \`${name}\`\r\n\r\n`
        }

    }

    return updateCloud(did_const,'常量',md)

}

function updateStruct() {

    let md = 'BeShell 将一些 LCGL C api 用到的结构体绑定到了对应的 JS类。\r\n\r\n'

    for(let struct of LibStruct) {

        let props = ''
        let getter = ''
        let setter = ''
        for(let [ctype,name] of struct.members) {

            let [jsType, jsSign, type] = iconvType(ctype)

            props+= `* ${name}: ${jsSign}\r\n`
            if(type=='const'){
                props+= ` > 该属性是一个字符串常量: ${jsType}\r\n`
            }
            props+= '\r\n'

            let setterName = toCamelCase('set_'+name)
            let getterName = toCamelCase(name)

            setter+= `#### ${setterName}(value: ${jsSign})\r\n\r\n`
            setter+= `设置内部属性: \`${name}\`\r\n\r\n`

            setter+= `#### ${getterName}(): ${jsSign}\r\n\r\n`
            setter+= `返回内部属性: \`${name}\`\r\n\r\n`
        }

        md+= `<h3 id="${struct.jsclass}"></h3><h3 id="${struct.type}"></h3>

## beapi.lvgl.${struct.jsclass}

LVGL C struct \`${struct.type}\`

### 内部属性:

${props}

### 方法:

${setter}

${getter}

`
    }
    
    return updateCloud(did_struct,'结构对象(Struct)',md)
}


function updateStyle() {

    let md = ''

    for(let style of StyleProps) {
        if(!style.name) {
            continue
        }

        let name = style.name.toLowerCase().replace(/_/g, "-")

        md+= `* **${name}** (${style.style_type})\r\n`
        md+= ` ${style.dsc}\r\n\r\n`
    }

    let tpl = fs.readFileSync(__dirname+'/tpl/lvgl-style.md').toString()
    md = tpl.replace('${ALL STYLES}', md)

    // 
    return updateCloud(did_style,'样式(Style)',md)

}

function updateEvent() {
}

async function main() {
    if(process.argv.length<3) {
        print_help()
        return
    }
    if(process.argv[2]=='widget') {

        if( process.argv[3] == 'all' ) {
            for(let wname in dids_widget) {
                console.log(wname, dids_widget[wname])
                await updateWidgetDoc(wname, dids_widget[wname])
            }
            return
        }

        else if( dids_widget[process.argv[3]] ) {
            await updateWidgetDoc(process.argv[3], dids_widget[process.argv[3]])
            return
        }

        console.log("unknow widget:", process.argv[3])

        return
    }

    else if(process.argv[2]=='const') {
        await updateConst()
        return
    }

    else if(process.argv[2]=='struct') {
        await updateStruct()
        return
    }

    else if(process.argv[2]=='style') {
        await updateStyle()
        return
    }

    else if(process.argv[2]=='event') {
        await updateEvent()
        return
    }

    print_help()

}

main()