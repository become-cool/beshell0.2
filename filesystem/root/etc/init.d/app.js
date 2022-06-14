exports.autoBoot = function() {
    try{
    let json = JSON.load("/etc/config/boot.json")
        if(json["script"]) {
            eval(json["script"])
        }
        if(json["path"]) {
            console.log("run path:", json["path"])
            require(json["path"])
        }
    }catch(e){
        console.log(e)
    }
}
exports.setAutoBootPath = function(path) {
    if(!path||!beapi.fs.existsSync(path)) {
        throw new Error("path not exists")
    }
    let json = JSON.load("/etc/config/boot.json")
    json["path"] = path
    beapi.fs.writeFileSync("/etc/config/boot.json", JSON.stringify(json,null,4))
}