const lv = require("lv")

class AppIcon extends lv.Column {
    constructor(parent) {
        super(parent)
        this.refs = lv.Obj.prototype.fromJson.call(this, {
            width:48 ,
            height:-1 ,
            flag: ["clickable"] ,
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
                    clicked: ()=>{
                        this.run()
                    } ,
                }]
            }, {
                class: 'Label' ,
                ref: 'label' ,
                font: "msyh" ,
                width:"100%" ,
                longMode: "scroll-circular" ,
                text: "" ,
                clicked: ()=>{
                    this.run()
                } ,
                style: {
                    "text-align": "center" ,
                }
            }]
        })

        this.addFlag("clickable")
        this.on('clicked',()=>{
            console.log("clicked")
        })
    }

    fromJson(json,refs) {
        if(json.text) {
            this.refs.label.setText(json.text)
            delete json.text
        }

        if(json.src) {
            this.refs.icon.setSrc(json.src)
            delete json.src
        }
        else if(json.symbol) {
            this.refs.icon.setSymbol(json.symbol)
            delete json.symbol
        }
        
        if(json.clicked) {
            this.refs.icon.on("clicked", json.clicked)
            delete json.clicked
        }
        if(json.longPressed) {
            this.refs.icon.on("long-pressed", json.longPressed)
            delete json.longPressed
        }
        if(json.longPressed) {
        }
        lv.Obj.prototype.fromJson.call(this, json, refs)

        this.create = json.create
    }
    run() {
        beapi.lvgl.Group.focusObj(this)
        if(this.create) {
            let box = beapi.lvgl.msg.info("正在启动APP ...",'',false)
            setTimeout(()=>{
                this.create()
                box.hide()
            },100)
        }
        else {
            beapi.lvgl.msg.info("coming soon",'APP还没有上架',false)
        }
    }
}

module.exports = AppIcon