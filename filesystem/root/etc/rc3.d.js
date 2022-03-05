(async function(){
    try{        
        require("./init.d/repl.js")

        require("./init.d/banner.js")()

        if(!process.simulate) {
            global.WiFi = require("besdk/wifi")
            WiFi.autostart()
        }

        await require("./init.d/display.js")()
        
        require("lv")
        await sleep(0)
        
        console.log('BeShell is ready.')

        if(beapi.reset_app) {
            let pkgpath = beapi.reset_app + "/package.json"
            if( !beapi.fs.existsSync(pkgpath) ) {
                console.log("path is not a app folder", beapi.reset_app)
            }
            else {
                let pkg = JSON.load(pkgpath)
                let mainpath = beapi.reset_app + '/' + (pkg.main || "index.js")
                require(mainpath)
            }
        }

    }
    catch(e) {
        console.log(e.message)
        console.log(e.stack)
    }
})()