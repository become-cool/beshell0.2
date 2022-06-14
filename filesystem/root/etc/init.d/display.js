let _defaultDisp = null
let _scrSplash = null
exports.setup = function(preqLst){
    return
    let hasDisplay = false

    beapi.lvgl.defaultDisplay = function() {
        return _defaultDisp ;
    }
    switch(beapi.utils.partId()) {
        // bepad(003)
        case 3 :
            hasDisplay = true
            beapi.spi.setup(1,14,13,12);
            _defaultDisp = beapi.lvgl.createDisplay("ST7789", {dc:17, cs:15, spi:1, width:320, height:240, freq:80000000}) 

            // input
            _defaultDisp.touch = new beapi.lvgl.InDevPointer("xpt2046",1,21)
            break
        // bevision(019)
        case 19:
            break
    }
    if(!hasDisplay) {
        // _defaultDisp = beapi.lvgl.createDisplay("VIRTUAL", {width:320, height:240})
    }

    if(_defaultDisp) {
        _scrSplash = _defaultDisp.activeScreen()
        _scrSplash.splash = true
        _scrSplash.setStyle("bg-color", beapi.lvgl.rgb(80))
        let logo = new beapi.lvgl.Label(_scrSplash)
        logo.setText("BECOME")
        logo.setStyle("text-color", beapi.lvgl.rgb(255))
        logo.setFont("m24")
        logo.center()

        let loading = new beapi.lvgl.Bar(_scrSplash)
        loading.setAlign("bottom-mid")
        loading.setY(-20)
        loading.setWidth(200)
        loading.setHeight(5)
        loading.setValue(0)
        loading.addFlag("hidden")
        _scrSplash.loading = loading
    }

    preqLst.push(require("lv/prequire"))
}

exports.prequire = async function(lst) {
    let total = 0
    for(let [files,] of lst) {
        total+= files.length
    }
    if(_scrSplash) {
        _scrSplash.loading.clearFlag("hidden")
        _scrSplash.loading.setValue(0)
    }
    
    let loaded = 0 

    for(let [files,req] of lst) {
        await prequire(files,req,require_cb)
    }

    _scrSplash && _scrSplash.loading.setValue(100)

    await sleep(100)

    function require_cb(l,t,p) {
        if(l==t) { return }
        _scrSplash && _scrSplash.loading.setValue((loaded++)*100/total)
    }
}