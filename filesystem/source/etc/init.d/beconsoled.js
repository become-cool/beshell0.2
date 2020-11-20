require("besdk")
require("besdk/pin.js")

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

telnet.callback = function(funcName, ...arglst){
    for(let i=0; i<arglst.length; i++) {
        arglst[i] = JSON.stringify(arglst[i])
    }
    telnet.send(0, CMD_CALLBACK, `${funcName}(${arglst.join(',')})`)
}


let __debounce_timers = {}
watchPins((gpio, val)=>{
    if(__debounce_timers[gpio]!=undefined)
        return
    __debounce_timers[gpio] = setTimeout(()=>{
        delete __debounce_timers[gpio]
        if( digitalRead(gpio) == val ) {
            telnet.callback('EmitPinChanged', gpio, val)
        }
    }, 10)
})

global.beconsoled  = {
    module_list
}
