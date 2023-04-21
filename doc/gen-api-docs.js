const fs = require("fs")
const {basename} = require("path")
const javadoc = require("javadoc")
const {updateCloud} = require("./api.js")


const c_sources = {
    'fs': {
        did: 8 ,
        title: '文件系统' ,
        src:__dirname+"/../main/modules/module_fs.c"
    } ,
    'wifi': {
        did: 9 ,
        title: 'WiFi' ,
        src:__dirname+"/../main/modules/module_wifi.c"
    } ,
    'gpio': {
        did: 10 ,
        title: 'GPIO' ,
        src:__dirname+"/../main/modules/module_gpio.c"
    } ,

    'lvgl': {
        did: 62 ,
        title: '全局函数' ,
        src:__dirname+"/../main/modules/lvgl/module_lvgl.c"
    } ,
    'gl': {
        did: 72 ,
        title: '绘图接口(GL)' ,
        src:__dirname+"/../main/modules/lvgl/be_gl_js.c" ,
        tpl:__dirname+"/tpl/lvgl-gl.md" ,
    } ,
}
function genApiDoc(doc) {

    let fullName = doc["@beapi"][0]
    let funcName = doc["@beapi"][0].split('.').pop()

    let md = `<div id="${funcName}"></div>\r\n\r\n`
    md+= '## ' + funcName + '('
    let mdParamList = ''

    let i = 0
    for(let param of doc["@param"]||[]) {
        let segs = param.trim().split(" ")
        let sign = segs.shift().trim()
        let desc = segs.join(" ")

        if(i++) {
            md+= ', '
        }
        md+= sign

        mdParamList+= "* " + sign + "\r\n"
        mdParamList+= " " + desc + "\r\n"
    }

    md+= ")\r\n\r\n"
    
    md+= "**" + fullName.trim() + "**\r\n\r\n"

    if(doc._) {
        md+= doc._.join("\r\n") + '\r\n'
    }

    if(doc["@param"]) {
        md+='参数:\r\n\r\n'
        md+= mdParamList + '\r\n'
    }

    md+='返回:\r\n\r\n'
    if(doc["@return"]) {
        let segs = doc["@return"][0].trim().split(" ")
        let sign = segs.shift().trim()
        let desc = segs.join(" ")

        md+= sign + '\r\n'
        md+= desc + '\r\n'
    }
    else {
        md+= '`undefine`\r\n'
    }

    return md + '\r\n'
}

async function parseFile(path) {

    let apiDesc = []

    try{
        var json = await javadoc.generate({
            include: [path],
            exclude: ["**/node_modules/**"],
            format: "json"
        })

        for(let doc of json.success[0]) {
            if(!doc["@beapi"]) {
                continue
            }

            console.log(doc)
            try{
                let md = genApiDoc(doc)
                console.log(md)

                apiDesc.push(md)
            }catch(e){
                console.error(e)
                continue
            }
        }

        let md = apiDesc.join("\r\n\r\n")

        return md
    }
    catch(e) {
        console.error(e)
    }
}

async function main() {

    let module = process.argv[2]
    if(!module) {
        console.log("usage:")
        console.log("node gen-api-docs.js <module_name> | all")
        return 
    }

    for(let pkg in c_sources) {
        if(module!='all' && module!=pkg) {
            continue
        }
        let {did, title, src, tpl} = c_sources[pkg]
        try{
            let md = await parseFile(src)

            if(tpl) {
                tpl = fs.readFileSync(tpl).toString()
                console.log(md)
                md = tpl.replace("${AUTO GEN}",md)
            }

            // let filename = basename(src)
            // let output = __dirname+"/api/api-"+filename+'.md'
            // fs.writeFileSync(output, md)
            // console.log("output to", output)

            await updateCloud(did,title,md)
            console.log("update to clound")
        } catch(e) {
            console.error(e)
        }
    }
}


main()