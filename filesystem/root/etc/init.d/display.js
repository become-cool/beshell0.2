module.exports = function(){
    let hasDisplay = false

    let _defaultDisp = null
    beapi.lvgl.defaultDisplay = function() {
        return _defaultDisp ;
    }
    switch(beapi.utils.partId()) {
        // bepad(003)
        case 3 :
            hasDisplay = true
            beapi.spi.setup(1,14,13,-1);
            _defaultDisp = beapi.lvgl.createDisplay("ST7789", {dc:17, cs:15, spi:1, width:320, height:240, freq:80000000}) 
            break
        // bevision(003)
        case 19:
            break
    }
    if(!hasDisplay) {
            _defaultDisp = beapi.lvgl.createDisplay("VIRTUAL", {width:320, height:240})
    }

    let screen = _defaultDisp.activeScreen()
    screen.splash = true
    screen.setStyle("bg-color", beapi.lvgl.rgb(80))
    let logo = new beapi.lvgl.Label(screen)
    logo.setText("BECOME")
    logo.setStyle("text-color", beapi.lvgl.rgb(255))
    logo.setFont("m24")
    logo.center()

    let loading = new beapi.lvgl.Bar(screen)
    loading.setAlign("bottom-mid")
    loading.setY(-20)
    loading.setWidth(200)
    loading.setHeight(5)
    loading.setValue(0)
    loading.addFlag("hidden")
    screen.loading = loading
}