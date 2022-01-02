const lv = require('lv')
const ProgramTools = require("./ProgramTools")
const cardlib = require("./cards/cardlib")

class Program extends lv.CleanObj{

    zoom= 2
    tools = null
    workspace = null

    constructor(workspace) {
        super(workspace)
        this.workspace = workspace

        this.fromJson({
            width: "100%" ,
            height: "100%" ,
            visible: false ,
            children: [
                // {
                //     class: 'Label' ,
                //     center: true ,
                //     text: 'Program' ,
                //     style: {
                //         "text-color": lv.rgb(80,80,80)
                //     }
                // }
            ]
        })
        
        let menu = new lv.Menu(this)
        menu.addItem("xxxx")
        menu.popup(100,100)
        global.m = menu


        let clz = cardlib[0].cards[1]
        for(let i=1;i<=5; i++) {
            let card = new clz(this, workspace.model.vm)
            card.setCoords(50, 5+i*50-50)
            // card.slots.what.input.setText(i)
        }
        clz = cardlib[0].cards[2]
        for(let i=1;i<=5; i++) {
            let card = new clz(this, workspace.model.vm)
            card.setCoords(150, 5+i*50-50)
            // card.slots.what.input.setText(i)
        }

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