const fs = require("fs")
const {execSync} = require("child_process")



function packdir(from, to, packpath, ignores) {
    if(!fs.existsSync(to)) {
        fs.mkdirSync(to, {recursive:true})
    }

    for(let item of fs.readdirSync(from)) {
        let itemFull = from +"/"+ item
        let vfspath = packpath+"/"+item
        if(item=='.' || item=='..' || (ignores&&ignores.includes(vfspath))){
            continue
        }
        let stat = fs.statSync(itemFull)
        if (stat.isDirectory()) {
            packdir(itemFull, to+'/'+item, vfspath, ignores)
        }
        else if(stat.isFile()) {
            if(item.substr(-3).toLowerCase()==".js") {
                let binFull = itemFull+".bin"
                if( fs.existsSync(binFull) ) {
                    console.log("skip source:", itemFull)
                    continue
                }
                // compile(itemFull, to+'/'+item, packpath, packpath+"/"+item)
            }
            fs.copyFileSync(itemFull, to+'/'+item)
            console.log(itemFull)
        }
        else {
            console.error("unknow type", itemFull)
        }
    }
}

// function compile(scriptPath, dist, dirname, filename) {

//     let script = `(function(require,__filename,__dirname) {
//     var exports={};
//     var module = {exports};
//     ${fs.readFileSync(scriptPath).toString()} ;
//     return module.exports
// })(Module.__mkrequire('${dirname}'),'${filename}','${dirname}');`

//     // fs.writeFileSync(dist,script)

// }


execSync(`rm -rf "${__dirname}/tmp/"`)

packdir(__dirname+"/root", __dirname+"/tmp/root", "", ["/home/become"])
packdir(__dirname+"/home", __dirname+"/tmp/home", "/home")