try{
    require("/lib/base/utils.js")
    
    require("./init.d/repl.js")

    require("./init.d/banner.js")()

    global.WiFi = require("besdk/wifi")
    try{
        WiFi.restore()
    }catch(e) {
        console.error(e)
    }

    require("./init.d/beconsoled.js")

    require("./init.d/app.js").autoBoot()

    let hasDisplay = false
    switch(beapi.utils.partId()) {
        // bepad(003)
        case 3 :
            hasDisplay = true
            console.log("init spi")
            beapi.spi.setup(1,14,13,-1);
            console.log("init spi diplay")
            beapi.lvgl.createDisplay("ST7789", {dc:17, cs:15, spi:1, width:320, height:240, freq:80000000}) 
            break
        // bevision(003)
        case 19:
            break
    }
    if(!hasDisplay) {
        beapi.lvgl.createDisplay("VIRTUAL", {width:320, height:240}) 
    }

    console.log('BeShell is ready.')
}
catch(e) {
    console.log(e.message)
    console.log(e.stack)
}