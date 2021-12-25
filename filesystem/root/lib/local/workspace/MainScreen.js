const lv = require("lv")
const ToolBar = require("./ToolBar")
const GraphCanvas = require("./graph/GraphCanvas")

class MainScreen extends lv.Obj {
    constructor(parent) {
        super(parent)

        this.refs = this.fromJson({
            style: {
                // "bg-color": lv.rgb(249,244,241)
                "bg-color": lv.rgb(249,249,249)
            } ,
            children: [

                {
                    class: GraphCanvas ,
                } ,

                {
                    class: ToolBar ,
                    ref: "toolBar" ,
                } ,
            ]
        })

        this.clearFlag("scrollable")
    }

    appFolder = ""
}

let singleton = null

MainScreen.start = function(appFolder) {
    if(!singleton) {
        singleton = new MainScreen()
        global.workspace = singleton
    }

    lv.loadScreen(singleton)
    
    singleton.appFolder = appFolder
    console.log(appFolder)
}



module.exports = MainScreen