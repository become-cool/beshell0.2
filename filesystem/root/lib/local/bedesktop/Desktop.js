const lv = require("lv")
const Dashboard = require("./dashboard/Dashboard.js")
const ScrApps = require("./ScrApps")
const ScrSysApps = require("./ScrSysApps")

module.exports = class Desktop {

    _disp = null
    _dashboard = null

    constructor(disp) {
        try{
            this._disp = disp

            this._tv = new lv.TileView()
            lv.loadScreen(this._tv)

            this._sysApps = new ScrSysApps(this._tv.addTile(0,0,lv.dir.HOR))
            
            this._dashboard = this._tv.addTile(1,0,lv.dir.HOR)
            new Dashboard(this._dashboard)

            this._apps = this._tv.addTile(2,0,lv.dir.HOR)
            new ScrApps(this._apps)

            setTimeout(() =>this._tv.setTile(this._dashboard,false), 1000)
            
            global.desktop = this
        }catch(e){
            console.log(e)
        }
    }
}