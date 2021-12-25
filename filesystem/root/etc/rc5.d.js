(async function(){
    try{
        require("/lib/base/utils.js")
        sleep(0)

        require("./init.d/repl.js")
        sleep(0)

        require("./init.d/banner.js")()
        sleep(0)


        if(!process.simulate) {
            global.WiFi = require("besdk/wifi")
            try{
                // WiFi.restore()
            }catch(e) {
                console.error(e)
            }
            require("./init.d/beconsoled.js")
            sleep(0)

            // require("/lib/local/telweb/main.js")
            // sleep(0)
        }
        
        require("./init.d/display.js")
        sleep(0)
        
        require("./init.d/desktop.js")
        sleep(0)

        let rcpath = process.env.HOME + "/.beshellrc"
        if(beapi.fs.isFileSync(rcpath)) {
            require(rcpath)
            sleep(0)
        }

        console.log('BeShell is ready.')
        
        require("./init.d/app.js").autoBoot()
        sleep(0)
    }
    catch(e) {
        console.log(e.message)
        console.log(e.stack)
    }
})()