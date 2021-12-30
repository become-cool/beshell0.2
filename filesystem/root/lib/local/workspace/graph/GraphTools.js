const lv = require('lv')

const CardStack = require("../comm/CardStack")
const {BeMotor, BeServo} = require('./parts/BeMotor')
const {BeScreen130, BeScreen096} = require('./parts/BeScreen')
const lstPartLib = [
    BeMotor, BeServo,
    BeScreen096, BeScreen130,
]
// this.addCart(BeMotor)
// this.addCart(BeServo)
// this.addCart(BeScreen130)
// this.addCart(BeScreen096)
// // this.addCart("LED")
// // this.addCart("RGB LED")
// // this.addCart("姿态传感器")
// // this.addCart("颜色传感器")
// // this.addCart("气压计")
// // this.addCart("红外线收发器")
// // this.addCart("超声波")
// // this.addCart("扬声器")
// // this.addCart("麦克风")
// // this.addCart("温度传感器")
// // this.addCart("湿度传感器")

class GraphTools extends lv.Column{
    
    // graph = null
    partLib = null

    constructor(parent, workspace) {
        super(parent)

        // this.workspace = workspace

        this.refs = this.fromJson({
            width: "100%" ,
            height: -1 ,
            align: "top-mid" ,
            style: {
                "pad-row": 5 ,
                "flex-cross-place": "center" ,
            }, 
            children: [
                {
                    class: "Label" ,
                    text: lv.symbol.plus ,
                    clicked: ()=>{
                        this.partLib.isVisible()?
                            this.partLib.hide() :
                            this.partLib.show()
                    }
                } ,
                // {
                //     class: "Img" ,
                //     src: '/lib/icon/16/tree.png' ,
                // } ,
                {
                    class: "Img" ,
                    src: '/lib/icon/16/props.png' ,
                    ref: "props" ,
                    visible: false ,
                } ,
                {
                    class: "Label" ,
                    text: lv.symbol.refresh ,
                    ref: "rotate" ,
                    visible: false ,
                    clicked: ()=>{
                        if(workspace.graph.activePart) {
                            workspace.graph.activePart.rotate()
                        }
                    }
                } ,
                {
                    class: "Label" ,
                    text: lv.symbol.trash ,
                    ref: "remove" ,
                    visible: false ,
                } ,
            ]
        })

        this.partLib = new CardStack(workspace, workspace)
        this.partLib.hide()
        for(let clz of lstPartLib) {
            this.partLib.addCart(clz.config.title, clz=>{
                let part = workspace.model.createPart(clz)
                part.repaint(workspace.zoomer.value)

                workspace.graph.setActivePart(part)
                return part
            }, clz)
        }

        
        workspace.graph.on("graph.active.changed", (activePart)=>{
            if(activePart) {
                this.refs.props.show()
                this.refs.rotate.show()
                this.refs.remove.show()
            }
            else {
                this.refs.props.hide()
                this.refs.rotate.hide()
                this.refs.remove.hide()

                this.partLib.hide()
            }
        })
    }
}

module.exports = GraphTools