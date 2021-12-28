if(typeof beapi.lvgl.defaultDisplay!='function') {
    console.log("desktop.js dependent by display.js")
}
else {

    (async function worker() {
        await require("lv/prequire") ()
        await require("desktop/prequire") ()
        await require("workspace/prequire") ()

        const Desktop = require("/lib/local/desktop/Desktop")
        global.desktop = new Desktop(beapi.lvgl.defaultDisplay())

    })()
}

