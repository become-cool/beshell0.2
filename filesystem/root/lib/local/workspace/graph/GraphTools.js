const lv = require('lv')
const PartLib = require('./PartLib')

class GraphTools extends lv.Column{
    constructor(parent, workspace) {
        super(parent)

        // this.workspace = workspace

        this.refs = this.fromJson({
            width: "100%" ,
            height: -1 ,
            align: "top-right" ,
            style: {
                "pad-row": 5
            }, 
            children: [
                {
                    class: "Label" ,
                    text: lv.symbol.plus ,
                    flag: ["clickable"] ,
                    clicked: ()=>{
                        this.partLib.isVisible()?
                            this.partLib.hide() :
                            this.partLib.show()
                    }
                } ,
                // {
                //     class: "Label" ,
                //     text: lv.symbol.list ,
                // } ,
                {
                    class: "Label" ,
                    text: lv.symbol.edit ,
                    ref: "props" ,
                    visible: false ,
                } ,
                {
                    class: "Label" ,
                    text: lv.symbol.refresh ,
                    ref: "rotate" ,
                    visible: false ,
                    flag: ["clickable"] ,
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

        this.partLib = new PartLib(workspace)
        this.partLib.hide()

        
        workspace.graph.on("graph-active-changed", (activePart)=>{
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

    // graph = null
    partLib = null
}

module.exports = GraphTools