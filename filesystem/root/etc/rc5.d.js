require("besdk")

; (async function(){
    try{
        require("./init.d/repl.js")
        sleep(0)

        require("./init.d/banner.js")()
        sleep(0)

        if(!process.simulate) {
            global.WiFi = require("besdk/wifi")
            WiFi.autostart()
        }

        require("./init.d/time.js")
        sleep(0)

        require("./init.d/beconsoled.js")
        sleep(0)

        let setupConf = require("./init.d/setup.js") ()

        if(be.dev?.disp?.length){
            let desktop = require("./init.d/desktop.js")

            await require("./init.d/display.js") ( desktop.disp? desktop.scripts: [] )

            if(desktop.disp) {
                desktop.init( setupConf )
            }
        }

        let rcpath = process.env.HOME + "/.beshellrc.js"
        if(beapi.fs.isFileSync(rcpath)) {
            require(rcpath)
            sleep(0)
        }

        console.log('BeShell is ready.')
        
        if(require("./init.d/app").autorun()) {
            if(setupConf.autorun) {
                require(setupConf.autorun)
            }
        }
        sleep(0)
    }
    catch(e) {
        console.log(e)
    }
})()