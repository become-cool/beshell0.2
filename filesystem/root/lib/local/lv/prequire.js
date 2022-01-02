module.exports = function(){
    return prequire([
        "./obj.js",
        "./list.js",
        "./color.js",
        "./loader.js",
        "./animate.js",
        "./index.js",
    ], require)
}