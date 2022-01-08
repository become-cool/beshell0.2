const lv = require('lv')
const ProgramTools = require("./ProgramTools")
const basic = require("./cards/basic")
const ui = require("./cards/ui")

class Program extends lv.CleanObj{

    zoom= 2
    tools = null
    workspace = null
    model = null

    constructor(workspace) {
        super(workspace)
        this.workspace = workspace
        this.model = workspace.model

        this.fromJson({
            width: "100%" ,
            height: "100%" ,
            visible: false ,
            children: []
        })

        let clz = basic.Print
        for(let i=1;i<=1; i++) {
            let card = new clz(this, this)
            card.setCoords(50, 5+i*50-50)
            // card.slots.what.input.setText(i)
        }
        clz = ui.UIGetText
        for(let i=1;i<=1; i++) {
            let card = new clz(this, this)
            card.setCoords(150, 5+i*50-50)
            // card.slots.what.input.setText(i)
        }

        //
        let card = new ui.UIEvent(this, this)
        card.setCoords(100,50)
        card = new basic.IsNumber(this, this)
        card.setCoords(100,150)
    }

    setZoom(zoom) {
        this.zoom = zoom
    }
    viewTools(toolbar) {
        if(!this.tools) {
            this.tools = new ProgramTools(toolbar, this.workspace)
        }
        return this.tools
    }

    enter() {

    }
    leave() {
        if(this.tools) {
            this.tools.close()
        }
    }
}

module.exports = Program