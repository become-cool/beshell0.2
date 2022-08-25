const lv = require("lv")
const Dashboard = require("./dashboard/Dashboard.js")
const ScrApps = require("./ScrApps.js")
const Setting = require("./setting/Setting.js")


class Desktop extends lv.Obj {

    _dashboard = null

    activePanel = 1
    scrollSnap = 40

    _switchingPanel = false

    constructor() {
        try{
            super()
            
            lv.loadScreen(this)
            this.clearFlag("scrollable")
            this.setStyle("bg-color", lv.rgb(242))

            this.updateLayout()
            // let height = this.height()

            // this._wallpaperOuter = new lv.CleanObj(this)
            // this._wallpaperOuter.setWidth(width)
            // this._wallpaperOuter.setHeight("100%")
            // this._wallpaperOuter.setStyle("bg-opa",0)
            // this._wallpaper = new lv.Img(this._wallpaperOuter)
            // this._wallpaper.setSrc("/home/become/bg.png")

            // this._panels = new lv.CleanObj(this)
            // this._panels.setWidth("100%")
            // this._panels.setHeight("100%")
            // this._panels.setStyle("bg-opa",0)
            // this._panels.scrollToY(-height, false)
            // this._panels.addFlag("scrollable")

            // this._sysApps = new ScrSysApps(this._panels)
            // this._sysApps.setWidth(width)
            // this._sysApps.setHeight("100%")

            this._apps = new ScrApps(this, this)
            this._apps.setY(0)

            this._apps.on("setting-popup()", ()=>{
                if(!this.setting) {
                    this.setting = new Setting()
                }
                lv.loadScreen(this.setting)
            })

            this._dashboard = new Dashboard(this, this)
            this._dashboard.setY(0)

            this.on("gesture",(e, target, dir)=>{
                console.log("gesture",dir)
                if(dir=="top") {
                    this.unlockScreen(true)
                } else if(dir=="bottom") {
                    this.lockScreen(true)
                }
            })

            this.on("ipt.btn.press", (key)=>{
                console.log(key, "press")
                this.unlockScreen(true)
            })
            this.holdKeys()

            // console.log("Desktop constructor")
            be.bus.on("return-desktop", ()=>{
                lv.loadScreen(this)
            })

        }catch(e){
            console.log(e)
        }
    }
    _lockDur = 200
    lockScreen(anim) {
        if(anim) {
            this._dashboard.animate({'y': 0},this._lockDur,undefined,()=>{
                this._apps.hide()
            })
        }
        else {
            this._dashboard.setY(0)
            this._apps.hide()
        }
        this.holdKeys()
    }
    unlockScreen(anim) {
        this._apps.show()
        let height = this.height()
        if(anim) {
            this._dashboard.animate({'y': -height},this._lockDur)
        }
        else {
            this._dashboard.setY(-height)
        }
        this.releaseKeys()
    }
}
module.exports = Desktop