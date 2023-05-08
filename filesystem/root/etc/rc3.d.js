require("besdk")

(async function(){
    try{        
        require("./init.d/repl.js")
    
        require("./init.d/telweb.js")

        require("./init.d/banner.js")()

        if(process.platform=='esp32' && beapi.wifi.hasInited()) {
            require("besdk/wifi").autostart()
        }

        require("./init.d/setup.js") ()
        
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
        console.log(e)
    }
})()