require("./init.d/repl.js")
repl.setEcho(false, false)

require("./init.d/beconsoled.js")

require("./init.d/banner.js")()

require("./init.d/wifi.js")

console.log('BeShell is ready.')
console.log(`run<<EmitReady('beshell','${process.versions.beshell}')`)