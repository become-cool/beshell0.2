if(typeof beapi.lvgl.defaultDisplay!='function') {
    console.log("desktop.js dependent by display.js")
}
else {
    require('/lib/local/bedesktop').main(beapi.lvgl.defaultDisplay())
}