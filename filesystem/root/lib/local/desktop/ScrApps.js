const lv = require("lv")
const AppIcon = require("./AppIcon")
try{
    var NewAppDialog = require("workspace/NewAppDialog")
}catch(e){}

let selectedAppFolder = null

module.exports = class ScrApps extends lv.Obj {
    constructor(parent,desktop) {
        super(parent)
        this.group = new lv.Group()
        this.fromJson({

            width: "100%" ,
            height: "100%" ,
            clear: true ,

            children:[{
                width: "100%" ,
                height: "100%" ,
                clear: true ,
                flex: "row-wrap" ,
                ref: "grid" ,
                style: {
                    "pad-column": 10 ,
                    "pad-row": 12 ,
                    "pad-top": 12 ,
                    "pad-left": 9 ,
                } ,
                children: [
                    // {
                    //     class:AppIcon ,
                    //     "text": "设置" ,
                    //     "src": "/lib/icon/32/setting.png" ,
                    //     group: this.group ,
                    //     ref: 'settings' ,
                    //     clicked: ()=>{
                    //         this.emit("setting-popup()")
                    //     }
                    // } ,
                    // {
                    //     class:AppIcon ,
                    //     "text": "模型" ,
                    //     "src": "/lib/icon/32/arm.png",
                    //     group: this.group ,
                    //     ref: 'files'
                    // } ,
                    {
                        class:AppIcon ,
                        "text": "文件" ,
                        "src": "/lib/icon/32/files.png",
                        group: this.group ,
                        ref: 'files'
                    }
                    , {
                        class:AppIcon ,
                        "text": "相册" ,
                        "src": "/lib/icon/32/album.png" ,
                        group: this.group ,
                        ref: 'album'
                    }
                    , {
                        class:AppIcon ,
                        "text": "音乐" ,
                        "src": "/lib/icon/32/music.png",
                        group: this.group ,
                        create () {
                            require("/lib/local/music/PlayerUI.js").singleton()
                        }
                    }
                    , {
                        class:AppIcon ,
                        "text": "游戏" ,
                        "src": "/lib/icon/32/game.png" ,
                        group: this.group ,
                        create () {
                            require("/lib/local/game/Player.js").singleton()
                        }
                    }
                    , {
                        class:AppIcon ,
                        "text": "相机" ,
                        "src": "/lib/icon/32/camera.png" ,
                        group: this.group ,
                    }
                    , {
                        class:AppIcon ,
                        "text": "天气" ,
                        "src": "/lib/icon/32/weather.png" ,
                        group: this.group ,
                        create() {
                            require("/lib/local/holoclock/index.js")
                        }
                    }
                ]
            }],
        },this)

        this.loadAppIconFromLib("/home/become")
        
        this.holdKeys()
        this.on("ipt.btn.press",key=>{
            if(key=='esc') {
                desktop.lockScreen(true)
            }
            else if(key=='left') {
                this.group.focusPrev()
            }
            else if(key=='right'||key=='tab') {
                this.group.focusNext()
            }
            else if(key=='enter') {
                let icon = this.group.focused()
                icon && icon.run()
                console.log(icon)
            }
        })

        this.styleFocused = new lv.Style({
            "border-width": 1,
            "border-color": lv.palette("green"),
            "radius": 3,
        })
        for(let child of this.grid.children()){
            child.addStyle(this.styleFocused, 2) // LV_STATE_FOCUSED: 2
        }


        lv.Group.focusObj(this.grid.child(0))
    }

    dlgNewApp() {
        if(NewAppDialog && !this._dlgNewApp) {
            this._dlgNewApp = new NewAppDialog(lv.active())
            this._dlgNewApp.on("new-app",(appPath, startWorkspace)=>{
                this.loadAppIcon(appPath)
                if(startWorkspace) {
                    setTimeout(()=>{
                        require("workspace").start( appPath )
                    }, 0)
                }
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
                // require("workspace").start( last )
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
                    require("workspace").start( path )
                } ,
                longPressed: ()=>{
                    this.popupAppMenu(path)
                    longPressed = true
                }
            })

            // console.log("load app icon",path)

            this.refs.btnNew.moveToIndex(this.refs.appPad.childCnt()-1)

        }catch(e){
            lv.msg.error("加载文件出错")
            return
        }

        setTimeout(()=>{
            require("music/main.js").singleton()
        },1000)
    }

    popupAppMenu(path) {

        selectedAppFolder = path

        if(!this._appMenu) {
            this._appMenu = new lv.Menu(null, {
                items: [
                    { value:"运行", font:"msyh", callback:()=>{
                    }} ,
                    { value:"开机时运行", font:"msyh", callback:()=>{
                    }} ,
                    { value:"工作台", font:"msyh", callback:()=>{
                        require("workspace").start( path )
                    }} ,
                    { value:"删除", font:"msyh", callback:()=>{
                        
                    }} ,
                ]
            })
            this._appMenu.hide()
        }
        this._appMenu.popup()
    }
}
