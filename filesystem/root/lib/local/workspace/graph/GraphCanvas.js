const lv = require('lv')
const BePad = require('./parts/BePad')

class GraphCanvas extends lv.Canvas{
    constructor(parent) {
        super(parent)

        this.fromJson({
            width: 297*5 ,
            height: 210*5 ,
            children: [
                {
                    class: BePad
                }
            ]
        })

        this.clearFlag("scrollable")
        this.draggable()
    }
}

module.exports = GraphCanvas