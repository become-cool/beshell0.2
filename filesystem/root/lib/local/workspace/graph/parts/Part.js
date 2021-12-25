const lv = require('lv')
const {PPM} = require('../conf')
const Port = require('./Port')

class Part extends lv.Obj{
    constructor(parent) {
        super(parent)

        this.fromJson({
            style: {
                radius: 0 ,
            } ,
            children: []
        })

        global.bepad = this

        this.draggable()
    }

    loadConf(path) {
        this.conf = require(path)
        if(this.conf.width) {
            this.setWidth(this.conf.width*PPM)
        }
        if(this.conf.height) {
            this.setHeight(this.conf.height*PPM)
        }
        if(this.conf.width && this.conf.height) {
            this.drawRect()
        }
        if(this.conf.ports) {
            this.loadPortConf(this.conf.ports)
        }
        if(this.conf["bg-color"]) {
            this.setStyle('bg-color',this.conf["bg-color"])

        }
        console.log(JSON.stringify(this.conf))
    }

    drawRect() {
        // this.setStyle('border-width',1)
    }
    
    loadPortConf(conf) {
    }
}

module.exports = Part
