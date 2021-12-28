const lv = require('lv')

const GraphWidth = 300*2
const GraphHeight = 200*2

class GraphCanvas extends lv.CleanObj{

    docOutterX = 20
    docOutterY = 20

    constructor(parent, workspace) {
        super(parent)

        this.fromJson({
            width: GraphWidth ,
            height: GraphHeight ,
            center: true ,
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

        this.workspace = workspace
        global.graph = this
    }

    workspace = null
    zoom = 2

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
        let doxX = (gx-this.coordX() - this.width()/2) / this.zoom
        let doxY = (gy-this.coordY() - this.height()/2) / this.zoom
        doxX = Math.round(doxX)
        doxY = Math.round(doxY)
        return [ doxX, doxY ]
    }
    docToGlobal(docX, docY) {
        return [
            Math.round(docX * this.zoom + this.width()/2 + this.coordX())  ,
            Math.round(docY * this.zoom + this.height()/2 + this.coordY()) 
        ]
    }
}

module.exports = GraphCanvas