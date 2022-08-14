const fs = require("fs")

let align = 64
let appstat = fs.statSync(__dirname + "/../build/beshell.bin")
let appsize = Math.ceil(appstat.size/1024/align) *1024*align

align = 8
let rootstat = fs.statSync(__dirname + "/img/fs-root.img")
let rootsize = Math.ceil(rootstat.size/1024/align) *1024*align

console.log("appsize:", appstat.size, '->', appsize)
console.log("rootsize:", rootstat.size, '->', rootsize)

module.exports = [appsize, rootsize]