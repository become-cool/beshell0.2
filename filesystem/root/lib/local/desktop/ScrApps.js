const lv = require("lv")
const AppIcon = require("./AppIcon")
const NewAppDialog = require("workspace/NewAppDialog")

let selectedAppFolder = null

module.exports = class ScrApps extends lv.Obj {
    constructor(parent) {
        super(parent)

        this.refs = this.fromJson({

            width: "100%" ,
            height: "100%" ,
            clear: true ,

            children:[{
                width: "100%" ,
                height: "100%" ,
                clear: true ,
                flex: "row-wrap" ,
                ref: "appPad" ,
                style: {
                    "pad-row": 16 ,
                    "pad-column": 16 ,
                    "pad-top": 8 ,
                    "pad-left": 8 ,
                } ,
    
                children: [
                    {
                        class: "Obj" ,
                        width: 48 ,
                        height: 48 ,
                        clear: true ,
                        ref: "btnNew" ,
                        children: [
                            {
                                class: "Btn" ,
                                width: 32 ,
                                height: 32 ,
                                center: true ,
                                children: [{
                                    class: "Label" ,
                                    text: lv.symbol.plus ,
                                    center: true ,
                                }] ,
    
                                clicked: ()=>{
                                    try{
                                        this.dlgNewApp().start()
                                    }catch(e){
                                        console.log(e)
                                        console.log(e.stack)
                                    }
                                }
                            }
                        ]
                    }
                ]
            }],
        })

        this.loadAppIconFromLib("/home/become")
    }

    dlgNewApp() {
        if(!this._dlgNewApp) {
            this._dlgNewApp = new NewAppDialog(lv.active())
            this._dlgNewApp.on("new-app",(appPath)=>{
                this.loadAppIcon(appPath)
            })
        }
        else {
            this._dlgNewApp.setParent(lv.active())
        }
        return this._dlgNewApp        
    }

    loadAppIconFromLib(libpath) {
        let last = null
        for(let item of beapi.fs.readdirSync(libpath) ){
            if(item=='.'||item=='..') {
                continue
            }
            let folder = libpath + "/" +item 
            if(beapi.fs.existsSync(folder+"/package.json")) {
                this.loadAppIcon(folder)
                last = folder
            }
        }
        if(last) {
            setTimeout(()=>{
                require("workspace").start( last )
            },500)
        }
    }

    loadAppIcon(path) {
        try{
            let pkgjson = JSON.load(path+'/package.json')
            
            let appico = new AppIcon(this.refs.appPad)
            let longPressed = false
            appico.fromJson({
                text: pkgjson.title||"Unname" ,
                src: pkgjson.icon||"/lib/icon/32/folder.png" ,
                clicked: ()=>{
                    if(longPressed) {
                        longPressed = false
                        return
                    }
                    console.log("xxx")
                    require("workspace").start( path )
                } ,
                longPressed: ()=>{
                    this.popupAppMenu(appico, path)
                    longPressed = true
                }
            })

            console.log("load app icon",path)

            this.refs.btnNew.moveToIndex(this.refs.appPad.childCnt()-1)

        }catch(e){
            lv.msg.error("加载文件出错")
            return
        }
    }

    popupAppMenu(appico, path) {

        selectedAppFolder = path

        if(!this._appMenu) {
            this._appMenu = new AppMenu(this)
            this._appMenu.hide()
        }
        let {x,y} = lv.inputPoint()

        this._appMenu.updateLayout()

        this._appMenu.show()
        let mx = this.width() - this._appMenu.width()
        let my = this.height() - this._appMenu.height()
        if(x>mx) x = mx
        if(y>my) y = my
        this._appMenu.setCoords(x, y)
    }
}

class AppMenu extends lv.List {
    constructor(parent) {
        super(parent)
        this.setWidth(140)
        this.setHeight(170)
        this.addItem(lv.symbol.gps, "开机启动", ()=>{
        })
        this.addItem(lv.symbol.edit, "编辑", ()=>{
            require("workspace").start( selectedAppFolder )
        })
        this.addItem(lv.symbol.trash, "删除", ()=>{
            console.log("ccc")
        })
        this.addItem(lv.symbol.close, "取消", ()=>{
            this.hide()
        })
    }

    addItem(symbol, text, cb) {
        let btn = this.addBtn(symbol, text)
        btn.on("clicked", ()=>{
            this.hide()
            cb && cb()
        })
        btn.child(1).as(lv.Label).setFont("msyh")
    }
}