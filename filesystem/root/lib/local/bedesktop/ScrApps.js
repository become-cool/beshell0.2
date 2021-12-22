const lv = require("lv")
const AppIcon = require("./AppIcon")

module.exports = class ScrApps extends lv.Obj {
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
                    "symbol": lv.symbol.plus
                } ,
                {
                    class:AppIcon ,
                    "symbol": lv.symbol.plus
                } ,
                {
                    class:AppIcon ,
                    "symbol": lv.symbol.plus
                } ,
                {
                    class:AppIcon ,
                    "symbol": lv.symbol.plus
                } ,
                {
                    class:AppIcon ,
                    "symbol": lv.symbol.plus
                } ,
                {
                    class:AppIcon ,
                    "symbol": lv.symbol.plus
                } ,
                {
                    class:AppIcon ,
                    "symbol": lv.symbol.plus
                } ,
                {
                    class: "Obj" ,
                    width: 48 ,
                    height: 48 ,
                    clear: true ,
                    children: [
                        {
                            class: "Btn" ,
                            width: 32 ,
                            height: 32 ,
                            center: true ,
                            children: [{
                                class: "Label" ,
                                text: lv.symbol.plus ,
                                center: true ,
                            }]
                        }
                    ]
                }
            ]
        })
    }
}