const lv = require('lv')

const RAD = 3

class Port extends lv.CleanObj{
    constructor(parent, conf) {
        super(parent, conf)

        this.conf = conf
        // if(conf.x) {
        //     this.setX(conf.x*PPM)
        // }
        // if(conf.y) {
        //     this.setY(conf.y*PPM)
        // }

        this.refs = this.fromJson({
            style: {
                // radius: 3*PPM ,
                "border-width": 1 ,
                "border-color": lv.palette("blue-grey") ,
            } ,
            center: true ,
            children: [
                // {
                //     class: 'CleanObj' ,
                //     x: 1*PPM ,
                //     y: 1*PPM ,
                //     width: 6*PPM ,
                //     height: 6*PPM ,
                //     bubble: true ,
                //     ref: "port" ,
                //     style: {
                //     }
                // } ,
                
                // {
                //     class: 'Label' ,
                //     width: -1 ,
                //     height: 16 ,
                //     center: true ,
                //     style: {}
                // }
            ]
        })

        this.draggable()
    }


    repaint(zoom) {
        this.setWidth(RAD*2*zoom)
        this.setHeight(RAD*2*zoom)
        this.setStyle("radius", RAD*zoom)
        this.setX(this.conf.x*zoom)
        this.setY(this.conf.y*zoom)
    }
}

module.exports = Port