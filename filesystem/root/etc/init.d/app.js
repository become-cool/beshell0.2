exports.autoBoot = function() {
    try{
    let json = JSON.load("/etc/config/boot.json")
        if(json["active-app"]) {
            console.log("run app:", json["active-app"])
            require(json["active-app"])
        }
    }catch(e){
        console.log(e.message)
        console.log(e.stack)
    }
}
exports.setAutoBootPath = function(path) {
    if(!path||!beapi.fs.existsSync(path)) {
        throw new Error("path not exists")
    }
    let json = JSON.load("/etc/config/boot.json")
    json["active-app"] = path
    beapi.fs.writeFileSync("/etc/config/boot.json", JSON.stringify(json,null,4))
}