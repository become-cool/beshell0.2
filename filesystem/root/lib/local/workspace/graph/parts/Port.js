const lv = require('lv')
const conf = require('../conf.js')

class Port extends lv.Obj{
    constructor(parent) {
        super(parent)

        this.fromJson({
            width: 8*conf.PPM ,
            height: 8*conf.PPM ,

            children: [
                {
                    class: 'Obj' ,
                    width: 4.8*conf.PPM ,
                    height: 4.8*conf.PPM ,
                    center: true ,
                    style: {
                        radius: 4.8*conf.PPM/2 ,
                        "border-width": 1 ,
                        "border-color": lv.palette("grey") ,
                    }
                } ,
                
                {
                    class: 'Label' ,
                    width: -1 ,
                    height: 16 ,
                    center: true ,
                    style: {}
                }
            ]
        })
    }
}

module.exports = Port