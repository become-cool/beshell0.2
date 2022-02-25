try{    
    require("./init.d/repl.js")

    require("./init.d/banner.js")()

    console.log('BeShell is ready.')
}
catch(e) {
    console.log(e.message)
    console.log(e.stack)
}