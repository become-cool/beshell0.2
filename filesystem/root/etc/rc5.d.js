require("besdk")
require("besdk/http")

; (async function(){
    try{
        require("./init.d/repl.js")
        sleep(0)
    
        require("./init.d/telweb.js")
        sleep(0)

        require("./init.d/banner.js")()
        sleep(0)


        if(!process.simulate && beapi.wifi.hasInited()) {
            require("besdk/wifi").autostart()
        }

        require("./init.d/time.js")
        sleep(0)

        require("./init.d/beconsoled.js")
        sleep(0)

        require("./init.d/setup.js") ()

        let rcpath = process.env.HOME + "/.beshellrc.js"
        if(beapi.fs.isFileSync(rcpath)) {
            require(rcpath)
            sleep(0)
        }

        console.log('BeShell is ready.')

        sleep(0)
        let bootJson = require("./init.d/boot.js")
        await bootJson.autorun()
    }
    catch(e) {
        console.log(e)
    }
})()