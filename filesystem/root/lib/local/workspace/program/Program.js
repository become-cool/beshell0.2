const lv = require('lv')

class Program extends lv.CleanObj{

    zoom= 2
    tools = null

    constructor(parent) {
        super(parent)

        this.fromJson({
            width: "100%" ,
            height: "100%" ,
            visible: false ,
            children: [
                {
                    class: 'Label' ,
                    center: true ,
                    text: 'Program' ,
                    style: {
                        "text-color": lv.rgb(80,80,80)
                    }
                }
            ]
        })
    }

    setZoom(zoom) {
        this.zoom = zoom
    }
    viewTools(toolbar) {
        if(!this.tools) {
            // this.tools = new GraphTools(toolbar, this.workspace)
        }
        return this.tools
    }
}

module.exports = Program