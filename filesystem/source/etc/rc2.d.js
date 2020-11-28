try{
    require("./init.d/repl.js")

    require("./init.d/banner.js")()

    require("./init.d/wifi.js")

    require("./init.d/app.js").autoBoot()

    console.log('BeShell is ready.')
}
catch(e) {
    console.log(e.message)
    console.log(e.stack)
}