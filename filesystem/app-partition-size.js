const fs = require("fs")

let stat = fs.statSync(__dirname + "/../build/beshell.bin")
let appsize = Math.ceil(stat.size/1024/128) *1024*128

module.exports = appsize

