
exports.setup = function(preqLst) {
    preqLst.push(
        require("desktop/prequire")
        , [["besdk/driver/joypad"], require]
    )
}

exports.begin = async function() {
    if(typeof beapi.lvgl.defaultDisplay!='function') {
        console.log("desktop.js dependent by display.js")
        return
    }
    let disp = beapi.lvgl.defaultDisplay()

    const Joypad = require("besdk/driver/joypad")    
    disp.joypad1 = new Joypad(0, 4, 5, 0x33)
    if(disp.joypad1.setup()) {
        disp.joypad1.register()
        console.log("found joypad1(0x33)")
    } else {
        console.log("not found joypad1(0x33)")
    }
    disp.keys = new lv.KeysRouter(disp)

    const Desktop = require("desktop/Desktop")
    global.desktop = new Desktop(disp)
}