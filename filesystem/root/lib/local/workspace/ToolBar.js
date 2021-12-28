const lv = require("lv")
const GraphTools = require("./graph/GraphTools")

class ToolBar extends lv.Obj {
    constructor(parent, workspace) {
        super(parent)

        this.workspace = workspace

        this.refs = this.fromJson({
            clear: true ,
            width: 20 ,
            height: "100%" ,
            style:{
                "border-width": 1 ,
                "border-color": lv.palette("blue") ,
                "bg-color": lv.palette("blue") ,
                "bg-opa": 128 ,
                "align": "left-mid" ,
            } ,
            children: [
                {
                    ref: "graphTools" ,
                    class: GraphTools ,
                    args:[workspace]
                } ,

                {
                    class: "Column" ,
                    width: "100%" ,
                    height: -1 ,
                    center: true ,
                    children: [
                        {
                            class: "Label" ,
                            text: lv.symbol.power
                        } ,
                        {
                            class: "Label" ,
                            font: "s16" ,
                            text: lv.symbol.search
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
                            text: lv.symbol.settings ,
                        } ,
                    ]
                } ,
            ]
        })
    }

    workspace = null
}

module.exports = ToolBar