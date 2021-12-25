const lv = require("lv")
const Dashboard = require("./dashboard/Dashboard.js")
const ScrApps = require("./ScrApps")
const ScrSysApps = require("./ScrSysApps")

class Desktop extends lv.Obj {

    _dashboard = null

    constructor(disp) {
        try{
            super()

            console.log("Desktop")
            
            lv.loadScreen(this)

            this._tv = new lv.TileView(this)

            this._sysApps = new ScrSysApps(this._tv.addTile(0,0,lv.dir.HOR))
            
            let tileDashboard = this._tv.addTile(1,0,lv.dir.HOR)
            this._dashboard = new Dashboard(tileDashboard)

            let tileApps = this._tv.addTile(2,0,lv.dir.HOR)
            this._apps = new ScrApps(tileApps)

            // setTimeout(() =>this._tv.setTile(tileApps,false), 1000)

            global.desktop = this
        }catch(e){
            console.log(e)
            console.log(e.stack)
        }
    }
}
module.exports = Desktop