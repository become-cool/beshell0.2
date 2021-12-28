const lv = require('lv')
const AppModel = require("./AppModel")
const ToolBar = require("./ToolBar")
const Zoomer = require("./Zoomer")
const GraphCanvas = require("./graph/GraphCanvas")

let singleton = null

class Workspace extends lv.CleanObj {
    constructor(parent) {
        super(parent)

        ; ({  graph: this.graph ,
            zoomer: this.zoomer ,
            toolbar: this.toolbar ,
        } = this.fromJson({
            style: {} ,
            children: [
                {
                    class: GraphCanvas ,
                    args: [this] ,
                    ref: "graph" ,
                } ,

                {
                    class: ToolBar , 
                    args: [this] ,
                    ref: "toolbar" ,
                } ,
                {
                    class: Zoomer ,
                    ref: "zoomer" ,
                    align: "right-mid" ,
                    height:180 ,
                    valueChanged: (value)=>{
                        this.graph.setZoom( value )
                    } ,
                } ,
            ]
        }))

        this.model = new AppModel(this)
        this.model.createHostFromDevice(this.graph)

        this.graph.setZoom(this.zoomer.value)

        this.clearFlag("scrollable")
    }

    model = null
    graph = null
    zoom = null
    toolbar = null

    appFolder = ""

    start( appFolder ) {
        this.appFolder = appFolder
        lv.loadScreen(this)
    }

    static singleton() {
        if(!singleton)
            singleton = new Workspace()
        return singleton
    }
}

module.exports = Workspace
