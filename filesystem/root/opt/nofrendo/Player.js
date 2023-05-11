const RomList = require("./RomList.js")

let _singleton
exports.singleton = function() {
    if(!_singleton) {
        _singleton = new RomList
    }
    lv.loadScreen(_singleton)
    global.game = _singleton
}

