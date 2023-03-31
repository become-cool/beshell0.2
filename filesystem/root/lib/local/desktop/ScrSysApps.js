const lv = require("lv")
const AppIcon = require("./AppIcon")

module.exports = class ScrSysApps extends lv.Obj {

    constructor(parent) {
        super(parent)
        
        this.refs = this.fromJson({
            width: "100%" ,
            height: "100%" ,
            clear: true ,
            flex: "row-wrap" ,
            style: {
                "pad-row": 16 ,
                "pad-column": 16 ,
                "pad-top": 8 ,
                "pad-left": 8 ,
            } ,

            children: [
                {
                    class:AppIcon ,
                    "text": "文件" ,
                    ref: "app1" ,
                    "src": "/lib/icon/32/folder.png"
                }
                , {
                    class:AppIcon ,
                    "text": "相册" ,
                    ref: "app2" ,
                    "src": "/lib/icon/32/album.png"
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
                }
                , {
                    class:AppIcon ,
                    "text": "设置" ,
                    "src": "/lib/icon/32/setting.png"
                }
                , {
                    class:AppIcon ,
                    "text": "相机" ,
                    "src": "/lib/icon/32/camera.png"
                }
            ]
        })
    }
}