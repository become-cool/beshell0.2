const lv = require("lv")
class ScrConsole extends lv.Obj {
    constructor(parent) {
        super(parent)

        this.setFlexFlow("column")
        
        let objs = lv.fromJson([
            {
                class: 'Row' ,
                grow: 1 ,
                ref: "row1" ,
                style: {
                    "border-width": 2
                }
            } ,
            {
                class: 'Row' ,
                ref: "row2" ,
                height: 32 ,
                children: [
                    {
                        class: 'Btn' ,
                        text: 'btn1' ,
                        ref: "btn1" ,
                        width: 100 ,
                    } ,
                    {
                        class: 'Btn' ,
                        text: 'btn2' ,
                        ref: "btn2" ,
                    }
                ]
            } ,
        ], this)

        for(let name in objs) {
            global[name] = objs[name]
        }
        
    }
}
module.exports = ScrConsole