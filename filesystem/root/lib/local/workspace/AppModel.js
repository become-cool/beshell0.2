const VM = require("./program/VM")
const BePad = require("./graph/parts/BePad")

const mapWidgetNameTpl = {
    TextArea:'input%i',
    List:'list%i',
    Switch:'switch%i',
    Slider:'slider%i',
    Roller:'roller%i',
    Img:'img%i',
    Dropdown:'dropdown%i',
    Checkbox:'checkbox%i',
    Arc:'arc%i',
    Bar:'bar%i',
    Line:'line%i',
    Btn:'btn%i',
    Label:'text%i',
    Obj:'rect%i',
}


class AppModel extends beapi.EventEmitter{
    
    host = null
    parts =[]
    widgets = {}
    cards = []
    vm= new VM(this)

    _widgetsLastId = {}

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

    widgetNames(classFilter) {
        if(!classFilter){
            return Object.keys(this.widgets)
        } else {
            
        }
    }

    addWidget (widget) {
        if(!widget.name){
            this.makeWidgetName(widget)
        }
        this.widgets[widget.name] = widget
        this.emit("model.widget.new", widget)
        widget.on("*", (eventName)=>{
            this.vm.emit('ui', widget.name, eventName)
        })
    }

    makeWidgetName(widget) {
        let clzname = widget.constructor.name
        let nameTpl = mapWidgetNameTpl[clzname]
        if(!nameTpl) {
            console.log("unknow widget class for AppModel", clzname)
            nameTpl = clzname + '%i'
        }
        if(this._widgetsLastId[clzname]==undefined){
            this._widgetsLastId[clzname] = 0
        }
        widget.name = nameTpl.replace("%i", this._widgetsLastId[clzname] ++ )
    }
}

module.exports = AppModel