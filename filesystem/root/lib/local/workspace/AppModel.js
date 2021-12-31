const VM = require("./program/VM")
const BePad = require("./graph/parts/BePad")

class AppModel extends beapi.EventEmitter{
    
    host = null
    parts =[]
    widgets = []
    cards = []
    vm= new VM

    constructor(workspace) {
        super()
        this.workspace = workspace
    }

    createHostFromDevice(){
        this.host = new BePad(this.workspace.graph)
        this.parts.push(this.host)  
        
        this.emit("model.part.new", this.host)
    }

    createPart(partClass) {
        let part = new partClass(this.workspace.graph)
        this.parts.push(part)
        
        this.emit("model.part.new", part)

        return part
    }

    addWidget (widget) {
        this.widgets.push(widget)
        this.emit("model.widget.new", widget)
    }
}

module.exports = AppModel