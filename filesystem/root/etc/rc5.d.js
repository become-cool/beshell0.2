try{
    require("/lib/base/utils.js")
    
    require("./init.d/repl.js")

    require("./init.d/banner.js")()

    require("./init.d/wifi.js")

    require("./init.d/beconsoled.js")

    require("./init.d/app.js").autoBoot()

    console.log('BeShell is ready.')
}
catch(e) {
    console.log(e.message)
    console.log(e.stack)
}