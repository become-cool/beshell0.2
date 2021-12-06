if(typeof beapi.lvgl.defaultDisplay!='function') {
    console.log("desktop.js dependent by display.js")
}
else {
    setTimeout(function(){
        const Desktop = require("/lib/local/bedesktop/Desktop")
        global.desktop = new Desktop(beapi.lvgl.defaultDisplay())
    }, 0)
}