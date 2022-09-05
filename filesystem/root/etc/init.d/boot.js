const confPath = "/home/become/config/boot.json"
exports.autorun = async function() {
    let json = {}
    try{
        if(beapi.fs.isFileSync(confPath)) {
            json = JSON.load(confPath)
        }
    }catch(e){
        console.log(e)
    }
    json.__proto__ =  {
        desktop: "desktop/Desktop"
    }
    
    if(json["script"]) {
        try{
            eval(json["script"])
        }catch(e){ console.log(e) }
    }
    
    if(json["app"] && beapi.fs.isFileSync(json["app"])) {
        try{
            console.log("run app:", json["app"])
            require(json["app"])
            return
        }catch(e){ console.log(e) }
    }
    
    if( be.disp?.length && json.desktop ){
        const Desktop = require(json.desktop)
        be.desktop = new Desktop()
    }

    return
}
function setAutoScript(path) {
    if(!path||!beapi.fs.existsSync(path)) {
        throw new Error("path not exists")
    }
    let json = JSON.load(confPath)
    json["path"] = path
    beapi.fs.writeFileSync(confPath, JSON.stringify(json,null,4))
}
exports.setAutoScript = setAutoScript

exports.rebootToScript = function(path) {
    setAutoScript(path)
    beapi.nvs.setNextBootLevel(6)
    process.reboot()
}
