

if(beapi.wifi.hasInited() || process.platform=='linux') {

    const telweb = require("telweb")

    // let 
    let addr8022 = "http://0.0.0.0:8022"
    let addr43322 = "https://0.0.0.0:43322"

    be.server.http8022 = telweb.start(addr8022)
    be.server.http43322 = telweb.start(addr43322, true)

    console.log("telweb start on addr:")
    console.log(addr8022)
    console.log(addr43322)

    const path80 = '/home/become/www/main.js'
    if(beapi.fs.isFileSync(path80)) {
        setTimeout(()=>{
            try{
                console.log("require",path80)
                require(path80)
            } catch(e) {
                console.error(e)
            }
        },0)
    }
}