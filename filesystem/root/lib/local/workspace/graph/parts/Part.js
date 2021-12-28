const lv = require('lv')
const Port = require('./Port')

class Part extends lv.CleanObj{

    ports = []

    docW = 0
    docH = 0
    docX = 0
    docY = 0

    snap = 4

    constructor(graph) {
        super(graph)

        this.fromJson({
            center: true ,
            style: {
                "bg-opa": 255 ,
            } ,
            children: [] ,
            // pressed(){
            // }
        })

        global.bepad = this

        this.draggable(null ,
            pos=>{
                let [docX, docY] = graph.globalToDoc(pos.x, pos.y)
                docX = Math.round(docX/this.snap) * this.snap
                docY = Math.round(docY/this.snap) * this.snap
                ; ([pos.x, pos.y] = graph.docToGlobal(docX, docY))
            } ,
            ()=>{
                this.updateDocPos()
            })
    }

    updateDocPos() {
        let zoom = this.graph().zoom
        this.docX = this.xAligned() / zoom
        this.docY = this.yAligned() / zoom
    }

    graph() {
        return this.parent()
    }
    
    config() {
        return this.constructor.config
    }

    loadConf(config) {
        if(config.width) {
            this.docW = config.width
        }
        if(config.height) {
            this.docH = config.height
        }
        if(config.ports) {
            this.loadPortConf(config.ports)
        }
        if(config["bg-color"]) {
            this.setStyle('bg-color',config["bg-color"])
        }
    }

    repaint(zoom) {
        this.setWidth(this.docW*zoom)
        this.setHeight(this.docH*zoom)
        this.setX(this.docX*zoom)
        this.setY(this.docY*zoom)
        
        for(let port of this.ports) {
            port.repaint(zoom)
        }
    }
    
    loadPortConf(arrConf) {
        for(let portConf of arrConf) {
            this.ports.push(new Port(this, portConf))
        }
    }
}

module.exports = Part
