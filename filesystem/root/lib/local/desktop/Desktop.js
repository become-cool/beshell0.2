const lv = require("lv")
const Dashboard = require("./dashboard/Dashboard.js")
const ScrApps = require("./ScrApps")
const Joypad = require("besdk/driver/joypad")


class Desktop extends lv.Obj {

    _dashboard = null

    activePanel = 1
    scrollSnap = 40

    _switchingPanel = false

    constructor(disp) {
        try{
            super()

            this.setupInputDev()
            
            lv.loadScreen(this)
            this.clearFlag("scrollable")
            this.setStyle("bg-color", lv.rgb(242))

            this.updateLayout()
            let height = this.height()

            // this._wallpaperOuter = new lv.CleanObj(this)
            // this._wallpaperOuter.setWidth(width)
            // this._wallpaperOuter.setHeight("100%")
            // this._wallpaperOuter.setStyle("bg-opa",0)
            // this._wallpaper = new lv.Img(this._wallpaperOuter)
            // this._wallpaper.setSrc("/home/become/bg.png")

            this._panels = new lv.CleanObj(this)
            this._panels.setWidth("100%")
            this._panels.setHeight("100%")
            this._panels.setStyle("bg-opa",0)

            // this._sysApps = new ScrSysApps(this._panels)
            // this._sysApps.setWidth(width)
            // this._sysApps.setHeight("100%")


            this._dashboard = new Dashboard(this._panels, this)
            this._dashboard.setY(0)

            this._apps = new ScrApps(this._panels, this)
            this._apps.setY(height)

            this.on("gesture",(e, target, dir)=>{
                // console.log("<<<------------>>>", e, target, dir)
                if(dir=="up") {
                    this.setActivePanel(this.activePanel+1, true)
                }
                else if(dir=="down") {
                    this.setActivePanel(this.activePanel-1, true)
                }
            })

        }catch(e){
            console.log(e)
        }
    }
    setActivePanel(i, anim) {
        let height = this.height()
        this.activePanel = i
        this._panels.scrollToY(i*height, !!anim)

        // if(this._wallpaper) {
        //     let ww = this._wallpaper.width() - this.width()
        //     if(ww>0) {
        //         this._wallpaperOuter.scrollToX(i*(ww/2), !!anim)
        //     }
        // }
    }

    setupInputDev() {
        this.joypad1 = new Joypad(0, 4, 5, 0x33)
        if(this.joypad1.setup()) {
            this.joypad1.register()
            console.log("found joypad1(0x33)")
        } else {
            console.log("not found joypad1(0x33)")
        }
    }
}
module.exports = Desktop