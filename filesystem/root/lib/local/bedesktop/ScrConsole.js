const lv = require("lv")
class ScrConsole extends lv.Obj {
    constructor(parent) {
        super(parent)
        
        let {btn1,btn2,btn3,rect1} = this.refs = lv.fromJson([
            {
                class: "Btn" ,
                text: "btn1" ,
                ref: 'btn1' ,
            } ,
            {
                class: "Btn" ,
                text: "btn2" ,
                ref: 'btn2' ,
            } ,
            {
                class: "Obj" ,
                ref: 'rect1' ,
                center: "center" ,
                children: [
                    {
                        class: "Btn" ,
                        text: "btn3" ,
                        ref: 'btn3' ,
                    }
                ]
            }
        ], this)

        global.btn1 = btn1
        global.btn2 = btn2
        global.btn3 = btn3
        global.rect1 = rect1
        
    }
}
module.exports = ScrConsole