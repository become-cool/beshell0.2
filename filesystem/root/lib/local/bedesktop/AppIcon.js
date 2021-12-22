const lv = require("lv")

class AppIcon extends lv.Column {
    constructor(parent) {
        super(parent)
        this.refs = lv.Obj.prototype.fromJson.call(this, {
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
                text: "" ,
                style: {
                    "text-align": "center" ,
                }
            }]
        })
    }

    fromJson(json) {
        if(json.text) {
            this.refs.label.setText(json.text)
        }

        if(json.src) {
            this.refs.icon.setSrc(json.src)
        }
        else if(json.symbol) {
            this.refs.icon.setSymbol(json.symbol)
        }
    }
}

module.exports = AppIcon