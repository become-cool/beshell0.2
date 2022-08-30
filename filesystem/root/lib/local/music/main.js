const Player = require("./Player.js")

_singleton = null
exports.singleton = function() {
    if(!_singleton) {
        _singleton = new Player
    }
    
    lv.loadScreen(_singleton)
    global.app = _singleton
}