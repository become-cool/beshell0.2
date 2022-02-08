const fs = require('fs')
const ast = require("./lvgl-ast.json")
const {fileInsert} = require("../source_insert.js")

const writeSource = true

const mapClassDef = {}


function defineJSClass(baseName, thisType, parentClass) {
    let className = baseName.split('_').map(s=>{
        if(s) {
            return s[0].toUpperCase() + s.substr(1)
        } else {
            return '_'
        }
    }).join('')

    let ctypePrefix = 'lv_'+baseName + '_'
    if(!thisType) {
        thisType = ctypePrefix + 't *'
    }

    if(!parentClass) {
        parentClass = 'WAMSObject'
    }

    let meta = {
        className ,
        ctypePrefix, 
        ctypeName: ctypePrefix + 't *' ,
        methods: [] ,
        thisType ,
        parentClass,
    }

    if(!mapClassDef[ctypePrefix]) {
        mapClassDef[ctypePrefix] = meta
    }
    return meta
}
// defineJSClass('disp_draw_buf')
// defineJSClass('disp_drv')
// defineJSClass('disp')
// defineJSClass('animimg', 'lv_obj_t *', 'EventEmitter').widget = true
// defineJSClass('calendar', 'lv_obj_t *', 'EventEmitter').widget = true
// defineJSClass('chart', 'lv_obj_t *', 'EventEmitter').widget = true
// defineJSClass('colorwheel', 'lv_obj_t *', 'EventEmitter').widget = true
// defineJSClass('imgbtn', 'lv_obj_t *', 'EventEmitter').widget = true
// defineJSClass('led', 'lv_obj_t *', 'EventEmitter').widget = true
// defineJSClass('list', 'lv_obj_t *', 'EventEmitter').widget = true
// defineJSClass('menu', 'lv_obj_t *', 'EventEmitter').widget = true
// defineJSClass('meter', 'lv_obj_t *', 'EventEmitter').widget = true
// defineJSClass('msgbox', 'lv_obj_t *', 'EventEmitter').widget = true
// defineJSClass('span', 'lv_obj_t *', 'EventEmitter').widget = true
// defineJSClass('spinbox', 'lv_obj_t *', 'EventEmitter').widget = true
// defineJSClass('spinner', 'lv_obj_t *', 'EventEmitter').widget = true
// defineJSClass('tabview', 'lv_obj_t *', 'EventEmitter').widget = true
// defineJSClass('tileview', 'lv_obj_t *', 'EventEmitter').widget = true
// defineJSClass('led', 'lv_obj_t *', 'EventEmitter').widget = true
// defineJSClass('win', 'lv_obj_t *', 'EventEmitter').widget = true



function walk_ast(nodeLst, callback) {
    let prev = null
    for(let node of nodeLst) {
        if(/^_?lv_/.test(node.name)) {
            if( callback(node, prev)!=false && node.inner instanceof Array) {
                walk_ast(node.inner, callback)
            }
        }
        prev = node
    }
}


const mapExportFunctions = {}
const mapUnexportFunctions = {}  // static 函数


// 第一次遍历 ast ，搜集处理搜索 lv 函数
function exportApiFunctions() {

    let codeExportFunctions = ''

    walk_ast(ast.inner, (node, prev)=>{
        if(node.kind=="FunctionDecl") {
            genFunc(node)
            return false
        }
    })
    
    function genFunc(node) {
        if(node.storageClass=='static') {
            mapUnexportFunctions[node.name] = 1
            return
        }
        if(mapExportFunctions[node.name]) {
            return
        }
        
        codeExportFunctions+= `set(EXPORTED_FUNCTIONS "\${EXPORTED_FUNCTIONS}, _${node.name}")\r\n`
        
        if(!node.type.qualType) {
            return
        }
        let res = node.type.qualType.trim().match(/(.+)\(.+\)/)
        let funcDef = {
            name: node.name ,
            returnType: res[1].trim() ,
            args: []
        }
    
        for(let child of node.inner||[]) {
            if(child.kind!='ParmVarDecl') {
                continue
            }
            funcDef.args.push({
                type: child.type.qualType.replace(/^struct _/,'') ,
                name: child.name
            })
        }
    
        mapExportFunctions[node.name] = funcDef
    }    
    
    // wasm_wrapper.cmake
    if(writeSource){
        fileInsert(__dirname + "/../../wasm/wasm_wrapper.cmake", codeExportFunctions, 'EXPORTED FUNCTIONS','#')
    }
}



// 第二次遍历 ast , 生成 struct/union/enum wrapper api 函数
function lvStructToJSClass() {

    let codeStructApiDef = ''
    let codeConstCApiFuncs = ''
    let mapConstDef = {}

    walk_ast(ast.inner, (node, prev)=>{
        if(node.kind=='TypedefDecl' && node.isReferenced==true && prev /*&& prev.completeDefinition==true*/) {
            // typedef struct/union {} lv_xxxx_t
            if(prev.completeDefinition==true) {
                genStruct(node, prev)
            }
            // enmu
            else if(prev.kind=='EnumDecl') {
                genEnum(node, prev)
            }
            return false
        }
    })


    function genEnum(typedef, def) {

        let prefixeName = typedef.name.replace(/_t$/,'_').toUpperCase()
        let regexp = new RegExp("^(_?)"+prefixeName)

        let baseName = typedef.name.replace(/_t$/, '').replace(/^(_?)lv_/,'$1')

        let codeToConst = ''
        let codeToName = ''

        mapConstDef[typedef.name] = {
            jsname: baseName.toUpperCase(),
            nameFunc: `lv_${baseName}_to_name` ,
            valueFunc: `lv_${baseName}_to_const` ,
        }

        for(let node of def.inner||[]) {
            if(node.kind!='EnumConstantDecl') {
                continue
            }
            let name = node.name.replace(regexp, "$1").replace(/_/g,"-").replace(/^\-/g,"_").toLowerCase()
            codeToName+= `        case ${node.name}: return "${name}" ;\r\n`
            codeToConst+= `    ${codeToConst?'else ':''}if(strcmp("${name}", n)==0){ return ${node.name} ; }\r\n`
        }

        codeConstCApiFuncs+= `EMSCRIPTEN_KEEPALIVE char * lv_${baseName}_to_name(uint32_t v) {
    switch(v) {
${codeToName}\
        default: return "<unknow>" ;
    }
}
EMSCRIPTEN_KEEPALIVE uint32_t lv_${baseName}_to_value(char * n) {
${codeToConst}\
    else {
        return (uint32_t)-1 ;
    }
}
`
    }

    function genStruct(typedef, def) {

        if(codeStructApiDef) {
            codeStructApiDef+= "\r\n\r\n"
        }

        codeStructApiDef+= `// struct ${typedef.name} wasm api ----------\r\n`

        let nameNoT = typedef.name.replace(/_t$/, '')
        let funcName = nameNoT + "_create"
        
        if(mapClassDef[nameNoT+'_']) {
            var clsMeta = mapClassDef[nameNoT+'_']
        }
        else {

            let baseName = nameNoT.replace(/^(_?)lv_/,'$1')
            var clsMeta = defineJSClass(baseName)
        }

        if(!mapExportFunctions[funcName]) {

            // lv原生函数为 static , 未导出
            if(mapUnexportFunctions[funcName]) {
                funcName = nameNoT + "_create_"
            }

            codeStructApiDef+= `EMSCRIPTEN_KEEPALIVE ${typedef.name} * ${funcName}() {
        ${typedef.name} * p = malloc(sizeof(${typedef.name})) ;
        memset(p, 0, sizeof(${typedef.name})) ;
        return p ;
    }
    `
        }

        if(def.inner) {

            let codeGetter = '' ;
            let codeSetter = '' ;
        
            let prevField = null
            for(let field of def.inner) {
                if(field.kind=='FieldDecl') {
                    // 嵌套 struct/union
                    if(/\(anonymous (union|struct) at/.test(field.type.qualType)) {
                        if(prevField.kind=="RecordDecl") {
                            // todo 递归生成内嵌 struct/union
                        }
                    }
                    // 普通字段
                    else {

                        let type = field.type.qualType

                        // 函数指针类型
                        if( /\(\*\)/.test(type) ) {
                            type = 'void *'
                        }

                        // 有长度的字符串
                        let resArrayField = type.match(/\[(\d+)\]/)
                        let memlen = 0 ;
                        if(resArrayField) {
                            memlen = resArrayField[1]
                            type = type.replace(/\[(\d+)\]/, '*')
                        }

                        // getter ====
                        funcName = `${nameNoT}_get_${field.name}`
                        if(!mapExportFunctions[funcName] && !mapUnexportFunctions[funcName]) {
                            codeGetter+= `EMSCRIPTEN_KEEPALIVE ${type} ${funcName}(${typedef.name} * p) { return p->${field.name} ; }\r\n`

                            clsMeta.methods.push({
                                name: funcName ,
                                args: [{type:typedef.name+' *', name: 'p'}] ,
                                returnType: type
                            })
                        }
        
                        // setter ====
                        funcName = `${nameNoT}_set_${field.name}`
                        if(!mapExportFunctions[funcName] && !mapUnexportFunctions[funcName]) {
                            if(resArrayField) {

                            }
                            else {
                                codeSetter+= `EMSCRIPTEN_KEEPALIVE void ${funcName}(${typedef.name} * p, ${type} v) { p->${field.name} = v ; }\r\n`
                            }
                            
                            clsMeta.methods.push({
                                name: funcName ,
                                args: [{type:typedef.name+' *', name: 'p'}, {type, name: field.name}] ,
                                returnType: 'void'
                            })
                        }


                    }
                }
                prevField = field
            }
        
            if(codeGetter) {
                codeStructApiDef+= `// fields getter\r\n`
                codeStructApiDef+= codeGetter
            }
            if(codeSetter) {
                codeStructApiDef+= `// fields setter\r\n`
                codeStructApiDef+= codeSetter
            }
        }
    }

    if(writeSource){
        // struct_wrapper.c
        fileInsert(__dirname + "/../../wasm/src/struct_wrapper.c", codeStructApiDef, 'API STRUCT')
        
        // const_mapping.c
        fileInsert(__dirname + "/../../wasm/src/const_mapping.c", codeConstCApiFuncs, 'CONST MAPPING')

        // lvgl-const.json
        fs.writeFileSync(__dirname + '/lvgl-const.json', JSON.stringify(mapConstDef, null, 4))
    }
}


// 将 lvgl c function 根据命名规则归类到 js class
function allotClassDef(functionName) {
    let found = null
    for(let ctypePrefix in mapClassDef) {
        if( functionName.substr(0, ctypePrefix.length) == ctypePrefix ){
            if(!found) {
                found = ctypePrefix
                continue
            }
            // 最长匹配
            else {
                if(ctypePrefix.length>found.length) {
                    found = ctypePrefix
                }
            }
        }
    }
    return found? mapClassDef[found]: null
}
function allotMethods() {

    for(let funcDef of Object.values(mapExportFunctions)) {
        let first = funcDef.args[0]
        if(first) {
            let classDef = allotClassDef(funcDef.name)
            if(classDef && classDef.thisType == first.type) {
                classDef.methods.push(funcDef)
            }
        }
    }

}

// 根据 mapClassDef 生成 js class 代码
function lvApiToJSClass() {
    // 生成 JS Class 
    let libClasses = {}
    for(let clsDef of Object.values(mapClassDef)) {
        let ctype = clsDef.ctypePrefix+'t'
        libClasses[ctype] = {
            className: clsDef.className ,
            ctype ,
            ctypePrefix: clsDef.ctypePrefix ,
            thisType: clsDef.thisType ,
            methods: clsDef.methods
        }
    }
    if(writeSource) {
        fs.writeFileSync(__dirname+"/lvgl-struct.json", JSON.stringify(libClasses,null,4))
    }
}


;(function main() {

    exportApiFunctions()

    lvStructToJSClass()

    allotMethods()
    
    lvApiToJSClass()

}) () ;