for(let part of be.dev.disp) {
    if(part.desktop!=false) {
        exports.disp = part
        break
    }
}

exports.scripts = [
    require("desktop/prequire")
    , [["besdk/driver/BeJoypad"], require]
]

exports.init = function(setupConf) {
    console.log("require desktop")
    if(setupConf.desktop) {
        console.log(setupConf.desktop)
        require(setupConf.desktop)
    }
    else {
        const Desktop = require("desktop/Desktop")
        global.desktop = new Desktop()
    }
}