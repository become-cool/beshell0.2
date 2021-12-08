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
        // this._scrApps = new ScrApps()
        // this._scrPreApps = new ScrPreApps()

        this._disp.loadScreen(this._scrConsole)
        global.scr = this._scrConsole
    }
}