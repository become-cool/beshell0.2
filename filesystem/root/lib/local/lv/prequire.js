module.exports = function(){
    return prequire([
        "./obj.js",
        "./list.js",
        "./draggable.js",
        "./color.js",
        "./loader.js",
        "./index.js",
    ], require)
}