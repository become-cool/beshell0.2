const lv = require('lv')
const Part = require('./Part')

const ScreenWidth = 77.6
const ScreenHeight = 58 // 54

class BePad extends Part{
    static config = require("./part.lib/003.js")
    constructor(parent) {
        super(parent)

        this.loadConf(BePad.config)

        this.refs = this.fromJson({
            children: [
                {
                    center: true ,
                    bubble: true ,
                    ref: "screen" ,
                    style:{
                        "border-width": 0 ,
                        "bg-color": lv.rgb(0,0,0) ,
                        radius: 0 ,
                    }
                }
            ]
        })
    }

    repaint(zoom) {
        super.repaint(zoom)
        this.refs.screen.setWidth(ScreenWidth*zoom)
        this.refs.screen.setHeight(ScreenHeight*zoom)
    }
}

module.exports = BePad