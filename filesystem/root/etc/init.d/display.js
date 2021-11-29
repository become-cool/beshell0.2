let hasDisplay = false
switch(beapi.utils.partId()) {
    // bepad(003)
    case 3 :
        hasDisplay = true
        console.log("init spi")
        beapi.spi.setup(1,14,13,-1);
        console.log("init spi diplay")
        beapi.lvgl.createDisplay("ST7789", {dc:17, cs:15, spi:1, width:320, height:240, freq:80000000}) 
        break
    // bevision(003)
    case 19:
        break
}
if(!hasDisplay) {
    // beapi.lvgl.createDisplay("VIRTUAL", {width:320, height:240}) 
}