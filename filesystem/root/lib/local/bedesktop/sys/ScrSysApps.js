const lv = require("lv")

class AppIcon extends lv.Column {
    constructor(parent) {
        super(parent)
        this.refs = this.fromJson({
            width:48 ,
            height:-1 ,
            children: [{
                class: "Obj" ,
                clear: true ,
                width:"100%" ,
                height:48 ,
                ref: "outer" ,
                children: [{
                    class: 'Img' ,
                    ref:"icon" ,
                    center: true ,
                }]
            }, {
                class: 'Label' ,
                ref: 'label' ,
                font: "msyh" ,
                width:"100%" ,
                longMode: "scroll-circular" ,
                style: {
                    "text-align": "center" ,
                }
            }]
        })
    }

    setText(txt) {
        this.refs.label.setText(txt)
    }
    setSrc(src) {
        this.refs.icon.setSrc(src)
    }
}

module.exports = class ScrSysApps extends beapi.lvgl.Obj {

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
                    "src": "/lib/icon/folder.png"
                }
                , {
                    class:AppIcon ,
                    "text": "相册" ,
                    ref: "app2" ,
                    "src": "/lib/icon/album.png"
                }
                , {
                    class:AppIcon ,
                    "text": "音乐" ,
                    ref: "btn1" ,
                    "src": "/lib/icon/music.png"
                }
                , {
                    class:AppIcon ,
                    "text": "游戏" ,
                    ref: "btn1" ,
                    "src": "/lib/icon/game.png"
                }
                , {
                    class:AppIcon ,
                    "text": "设置" ,
                    ref: "btn1" ,
                    "src": "/lib/icon/setting.png"
                }
                , {
                    class:AppIcon ,
                    "text": "相机" ,
                    ref: "btn1" ,
                    "src": "/lib/icon/camera.png"
                }
                , {
                    class:AppIcon ,
                    "text": "示波器" ,
                    ref: "btn1" ,
                    "src": "/lib/icon/oscilloscope.png"
                }
                , {
                    class:AppIcon ,
                    "text": "应用" ,
                    ref: "btn1" ,
                    "src": "/lib/icon/store.png"
                }
            ]
        })
    }
}