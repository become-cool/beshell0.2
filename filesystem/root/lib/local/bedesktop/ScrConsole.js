const lv = require("lv")
class ScrConsole extends lv.Obj {
    constructor(parent) {
        super(parent)
        
        this.refs = lv.fromJson({
            class: "Row",
            align: "center" ,
            // grow: 1,
            name: "row" ,
            children: [
                {
                    class: "Btn" ,
                    text: "<<system apps" ,
                    ref: 'btnsysapp' ,
                } ,
                {
                    class: "Btn" ,
                    text: "user apps>>" ,
                    ref: 'btnapp' ,
                }
            ]
        }, this)
    }
}
module.exports = ScrConsole