const lv = require('lv')
const Part = require('./Part')
const {PPM} = require('../conf')

class BePad extends Part{
    constructor(parent) {
        super(parent)

        this.loadConf("./part.lib/003.js")

        this.fromJson({
            children: [
                {
                    width: 77.6*PPM ,
                    height: 54*PPM ,
                    center: true ,
                    bubble: true ,
                    style:{
                        "border-width": 0 ,
                        "bg-color": lv.rgb(0,0,0) ,
                        radius: 0 ,
                    }
                }
            ]
        })

        this.clearFlag("scrollable")
    }
}

module.exports = BePad