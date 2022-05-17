const lv = require("lv")
const AppIcon = require("./AppIcon")
try{
    var NewAppDialog = require("workspace/NewAppDialog")
}catch(e){}

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
                        class:AppIcon ,
                        "text": "设置" ,
                        "src": "/lib/icon/32/setting.png"
                    } , 
                    {
                        class:AppIcon ,
                        "text": "文件" ,
                        "src": "/lib/icon/32/folder.png"
                    }
                    , {
                        class:AppIcon ,
                        "text": "音乐" ,
                        "src": "/lib/icon/32/music.png"
                    }
                    , {
                        class:AppIcon ,
                        "text": "游戏" ,
                        "src": "/lib/icon/32/game.png"
                    } ,
                    // {
                    //     class: "Obj" ,
                    //     width: 48 ,
                    //     height: 48 ,
                    //     clear: true ,
                    //     ref: "btnNew" ,
                    //     children: [
                    //         {
                    //             class: "Btn" ,
                    //             width: 32 ,
                    //             height: 32 ,
                    //             center: true ,
                    //             children: [{
                    //                 class: "Label" ,
                    //                 text: lv.symbol.plus ,
                    //                 center: true ,
                    //             }] ,
    
                    //             clicked: ()=>{
                    //                 try{
                    //                     this.dlgNewApp().start()
                    //                 }catch(e){
                    //                     console.log(e)
                    //                 }
                    //             }
                    //         }
                    //     ]
                    // }
                ]
            }],
        })

        this.loadAppIconFromLib("/home/become")
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
