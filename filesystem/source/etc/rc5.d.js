
console.log("require ./init.d/repl.js")
require("./init.d/repl.js")

console.log("./init.d/banner.js")
require("./init.d/banner.js")()

require("./init.d/wifi.js")

require('./init.d/beconsoled.js')

console.log('BeShell is ready.')
