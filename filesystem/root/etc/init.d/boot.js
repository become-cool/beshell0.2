const confDir = "/home/become/config"
const confPath = confDir + "/boot.json"

module.exports = JSON.load(confPath) || {}

module.exports.__proto__ = {
    desktop: "desktop/Desktop" ,

    async autorun () {

        let app = beapi.nvs.readString("rst-app",true)
        if(app) {
            try{
                console.log("[reset] run app:", app)
                require(app)
                return
            }catch(e){
                console.log(e)
                process.reboot()
            }
        }
        
        if(this["script"]) {
            try{
                console.log("auto run script", this["script"])
                require(this["script"])
            }catch(e){ console.log(e) }
        }

        if(this["app"]) {
            try{
                console.log("run app:", this["app"])
                require(this["app"])
                return
            }catch(e){ console.log(e) }
        }
        
        if( be.disp?.length && this.desktop && !beapi.nvs.readUint8("rst-nodesktop",true) ){
            console.log("desktop:", this.desktop)
            const Desktop = require(this.desktop)
            be.desktop = new Desktop()
        }
    } ,

    setAutoScript(path) {
        if(!path||!beapi.fs.existsSync(path)) {
            throw new Error("path not exists")
        }
        this.script = path
        beapi.fs.mkdirSync(confDir)
        beapi.fs.writeFileSync(confPath, JSON.stringify(this,null,4))
    } ,

    rebootToApp (path, nowifi) {
        beapi.nvs.writeString("rst-app", path)
        if(nowifi) {
            beapi.nvs.writeUint8("rst-nowifi", 1)
        }
        process.reboot()
    } ,


}

