module.exports = async function() {
    if(typeof beapi.lvgl.defaultDisplay!='function') {
        console.log("desktop.js dependent by display.js")
        return
    }

    let screen = beapi.lvgl.defaultDisplay().activeScreen()
    if(screen.splash) {
        screen.loading.clearFlag("hidden")
        let [lvlst,lvreq] = require("lv/prequire")
        let [dtplst,dtpreq] = require("desktop/prequire")

        let loaded = 0
        let total = lvlst.length + dtplst.length + otherlst.length
        function require_cb(l,t,p) {
            if(l==t) { return }
            screen.loading.setValue((loaded++)*100/total)
        }

        await prequire(lvlst,lvreq,require_cb)
        await prequire(dtplst,dtpreq,require_cb)
        await prequire(otherlst,require,require_cb)
        screen.loading.setValue(100)
        await sleep(100)
    }
    else {
        await prequire(... require("lv/prequire"))
        await prequire(... require("desktop/prequire"))
    }

    const Desktop = require("desktop/Desktop")
    global.desktop = new Desktop(beapi.lvgl.defaultDisplay())
}

