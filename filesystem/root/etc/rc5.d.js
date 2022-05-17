(async function(){
    try{
        require("./init.d/repl.js")
        sleep(0)

        require("./init.d/banner.js")()
        sleep(0)


        if(!process.simulate) {
            global.WiFi = require("besdk/wifi")
            WiFi.autostart()

            require("./init.d/beconsoled.js")
            sleep(0)
        }
        
        await require("./init.d/display.js")()
        sleep(0)
        
        // BePad (003)
        // if(beapi.utils.partId()==3) {
            await require("./init.d/desktop.js")()
            sleep(0)
        // }

        let rcpath = process.env.HOME + "/.beshellrc.js"
        if(beapi.fs.isFileSync(rcpath)) {
            require(rcpath)
            sleep(0)
        }

        console.log('BeShell is ready.')
        
        require("./init.d/app").autoBoot()
        sleep(0)
    }
    catch(e) {
        console.log(e)
    }
})()