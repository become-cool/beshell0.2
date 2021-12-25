const lv = require("lv")

class ToolBar extends lv.Obj {
    constructor(parent) {
        super(parent)

        this.fromJson({
            clear: true ,
            width: 20 ,
            height: "100%" ,
            style:{
                "border-width": 1 ,
                "border-color": lv.palette("blue") ,
                "align": "left-mid" ,
            } ,
            children: [

                {
                    class: "Column" ,
                    width: "100%" ,
                    height: -1 ,
                    center: true ,
                    children: [
                        {
                            class: "Label" ,
                            text: lv.symbol.power
                        }
                    ]
                } ,

                {
                    class: "Column" ,
                    width: 18 ,
                    height: -1 ,
                    align: "bottom-right" ,
                    style: {
                        "pad-row": 5
                    }, 
                    children: [
                        {
                            class: "Label" ,
                            text: lv.symbol.play ,
                        } ,
                        {
                            class: "Label" ,
                            text: lv.symbol.save ,
                        } ,
                        {
                            class: "Label" ,
                            text: lv.symbol.settings ,
                        } ,
                        {
                            class: "Label" ,
                            text: lv.symbol.power ,
                        } ,
                    ]
                } ,
                
                
            ]
        })
    }
}

module.exports = ToolBar