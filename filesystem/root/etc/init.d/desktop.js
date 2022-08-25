for(let dev of be.disp) {
    if(dev.desktop!=false) {
        exports.disp = dev
        break
    }
}

exports.scripts = [
    require("desktop/prequire")
    , [["besdk/driver/BeJoypad"], require]
]

exports.init = function() {
    console.log("require desktop")
    const Desktop = require("desktop/Desktop")
    be.desktop = new Desktop()
}