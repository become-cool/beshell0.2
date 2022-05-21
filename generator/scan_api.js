const fs = require("fs")
const inc_path = __dirname + "/../components/lvgl/src"


function findHeadFile(dir, callback) {
    for(let name of fs.readdirSync(dir)) {
        let path = dir+'/'+name
        let stats = fs.statSync(path)
        if( stats.isFile() ){
            if(path.substr(-2).toLowerCase()=='.h') {
                // console.log(path) 
                callback(path)
            }
        }
        else if( stats.isDirectory() ){
            findHeadFile(path, callback)
        }
    }
}

function tidyType(type) {
    return type
            .trim()
            .replace(/^const +/,"")
            .replace(/^struct +_/, "")
}

const lstIgnoreFuncs = [
    "lv_obj_event_base" ,
]

function main() {
    if(process.argv.length<3) {
        console.error("node scan_api.js <func_prefix>")
        return
    }

    let thisType = "lv_obj_t *"
    if(process.argv.length>3) {
        thisType = process.argv[3].trim()
    }

    let func_prefix = process.argv[2]

    let funclst = []

    let out = "module.exports = {\r\n"
    function parseHead(path) {
        let src = fs.readFileSync(path).toString()
        for(let defres of src.matchAll(/([^\(\)\"\s=\,]+( +\*)?) +(lv_[a-z_]+)\(([^\(\)\"]+)\)/g)){
            let [def, returntype, _, funcname, arglstdef] = defres

            if(func_prefix && funcname.substr(0,func_prefix.length+1)!=func_prefix+"_") {
                continue
            }

            if(funclst.includes(funcname)) {
                continue
            }

            let isStatic = 'false'
            returntype = tidyType(returntype)
            try{
                arglst = arglstdef.trim().split(",")
                            .map(argdef=>{
                                let segs = argdef.split(/ +/)
                                if(segs.length<2)
                                    throw new Error()
                                let v = segs.pop().trim()
                                let t = tidyType(segs.join(' '))
                                return [t, v]
                            })
                            // .filter(arg=>arg!=null)
                
                // 第一个参数做为 this 对象
                if(arglst[0] && arglst[0][0] == thisType) {
                    arglst.shift()
                }
                // 静态方法
                else {
                    isStatic = 'true'
                }

                arglst = arglst.map(([t,v])=>{
                    return `["${t}","${v}"]`
                })

            }catch(e){
                continue
            }

            funclst.push(funcname)

            out+= `    "${funcname}": [null, [${arglst.join(",")}], "${returntype}", ${isStatic}],\r\n`
        }
    }


    // parseHead(__dirname + '/lv.inc/core/lv_obj_pos.h')
    findHeadFile(inc_path, parseHead)

    out+= "}"
    
    console.log(out)
}
main()