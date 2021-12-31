const lv = require('lv')
const AppModel = require("./AppModel")
const ToolBar = require("./ToolBar")
const Zoomer = require("./Zoomer")
const GraphCanvas = require("./graph/GraphCanvas")
const UI = require("./ui/UI")
const Program = require("./program/Program")

let _singleton = null

class Workspace extends lv.CleanObj {
    constructor() {
        super()
        this.fromJson({
            style: {
                "bg-color": lv.rgb(249,249,249) ,
            }
        })
        
        this.model = new AppModel(this)
        
        this.graph = new GraphCanvas(this, this)
        this.ui = new UI(this, this)
        this.program = new Program(this, this)

        this.zoomer = new Zoomer(this)
        this.zoomer.setAlign("right-mid")

        this.toolbar = new ToolBar(this)

        this.model.createHostFromDevice(this.graph)

        this.graph.setZoom(this.zoomer.value)

        this.setActiveView(this.program)   
    }

    model = null
    graph = null
    zoom = null
    toolbar = null

    activeView = null

    appFolder = ""

    start( appFolder ) {
        this.appFolder = appFolder
        lv.loadScreen(this)
    }

    setActiveView(view) {
        if(this.activeView==view) {
            return
        }
        this.emit("ws-active-view-changed", view, this.activeView)
        if(this.activeView) {
            this.activeView.leave()
            this.activeView.hide()
        }
        if(view) {
            view.enter()
            view.show()
        }
        this.activeView = view
    }

    static singleton() {
        if(!_singleton)
            _singleton = new Workspace()
        return _singleton
    }
}

module.exports = Workspace
