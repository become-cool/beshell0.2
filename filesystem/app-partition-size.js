const fs = require("fs")

let align = 64
let appstat = fs.statSync(__dirname + "/../build/beshell.bin")
let appsize = Math.ceil(appstat.size/1024/align) *1024*align

align = 8
let rootstat = fs.statSync(__dirname + "/img/root.raw")
let rootsize = Math.ceil(rootstat.size/1024/align) *1024*align

console.log("appsize", appsize)
console.log("rootsize", rootsize)

module.exports = [appsize, rootsize]