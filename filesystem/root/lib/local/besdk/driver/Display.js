const DeviceDriver = require("./DeviceDriver")
class Display extends DeviceDriver {
    constructor(){
        super("")
        this.name = this.constructor.name
    }
    setup(opts) {
        this.indev = beapi.lvgl.createDisplay(this.name, opts||{}) 
        return !!this.indev
    }
    register(insName) {
        super.register(insName)
        if(!be.disp){
            be.disp = []
        }
        be.disp.push(this)
    }
}
module.exports = Display