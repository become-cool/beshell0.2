const lv = require('lv')
const ProgramTools = require("./ProgramTools")

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