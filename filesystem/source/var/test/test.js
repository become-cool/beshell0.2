console.log("this is test.js")

console.log(__filename)
console.log(__dirname)

module.exports = {a: 123}

require("./a.js")