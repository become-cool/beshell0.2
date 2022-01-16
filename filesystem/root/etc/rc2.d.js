try{
    require("/lib/base/utils.js")
    
    require("./init.d/repl.js")

    require("./init.d/banner.js")()

    // global.WiFi = require("besdk/wifi")
    // try{
    //     WiFi.restore()
    // }catch(e) {
    //     console.error(e)
    // }

    // require("./init.d/app.js").autoBoot()

    console.log('BeShell is ready.')
}
catch(e) {
    console.log(e.message)
    console.log(e.stack)
}