try{
    require("/lib/base/utils.js")

    require("./init.d/repl.js")

    require("./init.d/banner.js")()


    if(!process.simulate) {
        global.WiFi = require("besdk/wifi")
        try{
            // WiFi.restore()
        }catch(e) {
            console.error(e)
        }
        require("./init.d/beconsoled.js")
    
        // require("/lib/local/webtelnet/main.js")
    }
    
    require("./init.d/display.js")
    
    require("./init.d/desktop.js")

    require("./init.d/app.js").autoBoot()

    console.log('BeShell is ready.')

    let rcpath = process.env.HOME + "/.beshellrc"
    if(beapi.fs.isFileSync(rcpath)) {
        require(rcpath)
    }
}
catch(e) {
    console.log(e.message)
    console.log(e.stack)
}