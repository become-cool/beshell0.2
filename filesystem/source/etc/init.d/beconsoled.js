require("besdk")

const CMD_CALLBACK = 6

/**
 * /home/become 目录下的模型列表
 */
function module_list(){
    let lst = []
    for(let filename of fs.readdirSync(process.env.HOME)){
        let path = process.env.HOME + '/' + filename
        let stat = fs.statSync(path)
        if(!stat.isDir) {
            continue
        }

        let packagePath = path + '/package.json'
        if(!fs.isFileSync(packagePath)) {
            continue
        }
        try{
            var pkg = JSON.load(packagePath)
        }catch(e){
            continue
        }
        if(!pkg.become) {
            continue
        }

        if(!pkg.become.version){
            pkg.become.version = pkg.version
        }
        pkg.become.folder = path
        lst.push(pkg.become)
    }
    return lst
}


function watching_callback(val, pin){
    telnet.send(0, CMD_CALLBACK, `EmitPinChanged('${pin.gpio}', ${val})`)
}
function watch(gpio) {
    if(!pin(gpio).isListening('both', watching_callback)){
        pin(gpio).watch(watching_callback, "both")
    }
}
function unwatch(gpio) {
    pin(gpio).unwatch(watching_callback, "both")
}

global.beconsoled  = {
    module_list, watch, unwatch, watching_callback
}
