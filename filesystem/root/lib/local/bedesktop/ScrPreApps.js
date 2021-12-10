const lv = require("lv")

module.exports = class ScrPreApps extends beapi.lvgl.Obj {

    constructor(parent) {
        super(parent)
        
        this.refs = lv.fromJson([
            {
                class:"Btn" ,
                "text": "xxxxx" ,
                center: true ,
                ref: "btn1"
            }
        ], this)
    }
}