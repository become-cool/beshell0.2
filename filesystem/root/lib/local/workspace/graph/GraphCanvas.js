const lv = require('lv')
const Zoomer = require("../comm/Zoomer")
const GraphTools = require("./GraphTools")

const GraphWidth = 300*2
const GraphHeight = 200*2

class GraphCanvas extends lv.CleanObj{

    activePart = null

    docOutterX = 20
    docOutterY = 20

    zoomer = null

    workspace = null
    tools = null

    constructor(workspace) {
        super(workspace)
        this.workspace = workspace

        this.fromJson({
            width: GraphWidth ,
            height: GraphHeight ,
            center: true ,
            visible: false ,
            style: {
                // "bg-color": lv.rgb(249,244,241)
                "bg-color": lv.rgb(249,249,249) ,
                "bg-opa": 255 ,
            }
        })

        this.draggable(null, (newpos)=>{
            let parent = this.parent()
            let minX = -(this.width()-parent.width()) - this.docOutterX
            let minY = -(this.height()-parent.height()) - this.docOutterX
            if( newpos.x<minX || newpos.x>this.docOutterX) {
                newpos.x = false
            }
            if( newpos.y<minY || newpos.y>this.docOutterX) {
                newpos.y = false
            }
        })

        this.on("clicked", ()=>{
            if(this.activePart) {
                this.emit("graph.active.changed", null, this.activePart)
                this.activePart.setSelected(false)
                this.activePart = null
            }
        })

        this.zoomer = new Zoomer(workspace)
        this.zoomer.setAlign("right-mid")
        this.zoomer.on("value-changed",()=>{
            this.setZoom(this.zoomer.value)
        })
        this.setZoom(this.zoomer.value)
    }

    setZoom(zoom) {
        this.zoom = zoom

        this.setWidth(GraphWidth * zoom)
        this.setHeight(GraphHeight * zoom)

        for(let part of this.workspace.model.parts) {
            part.repaint(zoom)
        }
    }

    // 为屏幕上的全局坐标 转换为 graph 的局部坐标
    globalToDoc(gx, gy) {
        let doxX = (gx-this.coordX() - this.width()/2) / this.zoomer.value
        let doxY = (gy-this.coordY() - this.height()/2) / this.zoomer.value
        doxX = Math.round(doxX)
        doxY = Math.round(doxY)
        return [ doxX, doxY ]
    }
    docToGlobal(docX, docY) {
        return [
            Math.round(docX * this.zoomer.value + this.width()/2 + this.coordX())  ,
            Math.round(docY * this.zoomer.value + this.height()/2 + this.coordY()) 
        ]
    }

    setActivePart(part) {
        this.emit("graph.active.changed", part, this.activePart)
        if(this.activePart) {
            this.activePart.setSelected(false)
        }
        part.setSelected(true)
        this.activePart = part
    }

    viewTools(toolbar) {
        if(!this.tools) {
            this.tools = new GraphTools(toolbar, this.workspace)
        }
        return this.tools
    }
    enter() {
        this.zoomer.show()
    }
    leave() {
        if(this.tools) {
            this.tools.partLib.hide()
        }
        this.zoomer.hide()
    }

    serialize() {
        return {
            x: this.x(),
            y: this.y(),
            zoom: this.zoom,
        }
    }
}

module.exports = GraphCanvas