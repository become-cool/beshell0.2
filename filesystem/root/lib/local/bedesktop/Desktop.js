const lv = beapi.lvgl
const ScrConsole = require("./ScrConsole")
const ScrApps = require("./ScrApps")
const ScrPreApps = require("./ScrPreApps")

module.exports = class Desktop {

    _disp = null
    _scrConsole = null

    constructor(disp) {
        // if(!disp instanceof lv.Display) {
        //     throw Error("arg disp must be a beapi.lvgl.Display") ;
        // }
        this._disp = disp
        this._scrConsole = new ScrConsole()
        this._scrApps = new ScrApps()
        this._scrPreApps = new ScrPreApps()

        this._scrConsole.refs.btn1.on("clicked", ()=>{
            lv.loadScreen(this._scrPreApps, "move-right", 100)
        })
        this._scrConsole.refs.btn2.on("clicked", ()=>{
            lv.loadScreen(this._scrApps, "move-left", 100)
        })

        
        this._scrApps.refs.btn1.on("clicked", ()=>{
            lv.loadScreen(this._scrConsole, "move-right", 100)
        })
        this._scrPreApps.refs.btn1.on("clicked", ()=>{
            lv.loadScreen(this._scrConsole, "fade-on", 100)
        })

        lv.loadScreen(this._scrConsole)
        global.scr1 = this._scrConsole
        global.scr2 = this._scrApps
        global.scr3 = this._scrPreApps
        global.disp = disp
        global.lv = beapi.lvgl
    }
}