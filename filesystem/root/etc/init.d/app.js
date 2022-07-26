const confPath = "/home/become/config/boot.json"
exports.autorun = function() {
    if(!beapi.fs.isFileSync(confPath)) {
        return
    }
    try{
        let json = JSON.load(confPath)
        if(json["script"]) {
            eval(json["script"])
        }
        if(beapi.fs.isFileSync(json["path"])) {
            console.log("run path:", json["path"])
            require(json["path"])
            return true
        }
    }catch(e){
        console.log(e)
    }
    return false
}
exports.setAutoRunPath = function(path) {
    if(!path||!beapi.fs.existsSync(path)) {
        throw new Error("path not exists")
    }
    let json = JSON.load(confPath)
    json["path"] = path
    beapi.fs.writeFileSync(confPath, JSON.stringify(json,null,4))
}