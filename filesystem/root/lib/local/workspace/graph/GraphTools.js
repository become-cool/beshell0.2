const lv = require('lv')
const PartLib = require('./PartLib')

class GraphTools extends lv.Column{
    constructor(parent, workspace) {
        super(parent)

        // this.workspace = workspace

        this.fromJson({
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
                {
                    class: "Label" ,
                    text: lv.symbol.edit ,
                } ,
                {
                    class: "Label" ,
                    text: lv.symbol.list ,
                } ,
                {
                    class: "Label" ,
                    text: lv.symbol.trash ,
                } ,
            ]
        })

        this.partLib = new PartLib(workspace)
        // this.partLib.hide()
    }

    // graph = null
    partLib = null
}

module.exports = GraphTools