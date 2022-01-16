if(typeof beapi.lvgl.defaultDisplay!='function') {
    console.log("desktop.js dependent by display.js")
}
else {
    (async function worker() {
        let screen = beapi.lvgl.defaultDisplay().activeScreen()
        if(screen.splash) {
            screen.loading.clearFlag("hidden")
            let [lvlst,lvreq] = require("lv/prequire")
            let [dtplst,dtpreq] = require("desktop/prequire")

            let loaded = 0
            let total = lvlst.length + dtplst.length

            await prequire(lvlst,lvreq,(l,t,p)=>{
                if(l==t) { return }
                screen.loading.setValue((loaded++)*100/total)
            })
            await prequire(dtplst,dtpreq,(l,t,p)=>{
                if(l==t) { return }
                screen.loading.setValue((loaded++)*100/total)
            })
            screen.loading.setValue(100)
            await sleep(100)
        }
        else {
            await prequire(... require("lv/prequire"))
            await prequire(... require("desktop/prequire"))
        }

        const Desktop = require("desktop/Desktop")
        global.desktop = new Desktop(beapi.lvgl.defaultDisplay())
    })()
}

