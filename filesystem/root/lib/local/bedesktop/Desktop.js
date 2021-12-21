const lv = require("lv")
const Dashboard = require("./dashboard/Dashboard.js")
// const ScrApps = require("./ScrApps")
const ScrSysApps = require("./sys/ScrSysApps")

module.exports = class Desktop {

    _disp = null
    _dashboard = null

    constructor(disp) {
        this._disp = disp

        let tv = new lv.TileView()
        lv.loadScreen(tv)

        this._sysApps = new ScrSysApps(tv.addTile(0,0,lv.dir.RIGHT))
        
        this._dashboard = tv.addTile(1,0,lv.dir.HOR)
        new Dashboard(this._dashboard)

        this._apps = tv.addTile(2,0,lv.dir.LEFT)
        let btn3 = new lv.Btn(this._apps)
        btn3.setText("Apps")
        btn3.center()


        // let img = new lv.Img(this._apps)
        // img.set

        tv.setTile(this._dashboard,false)
        global.desktop = this
    }
}