try{
    require("/lib/base/utils.js")
    
    require("./init.d/repl.js")

    require("./init.d/banner.js")()

    require("./init.d/wifi.js")

    require("./init.d/beconsoled.js")

    require("./init.d/app.js").autoBoot()

    beapi.spi.setup(1,14,13,-1);
    global.display = beapi.lvgl.createDisplay("ST7789", {dc:17, cs:15, spi:1, width:320, height:240, freq:80000000}) 

    console.log('BeShell is ready.')
}
catch(e) {
    console.log(e.message)
    console.log(e.stack)
}