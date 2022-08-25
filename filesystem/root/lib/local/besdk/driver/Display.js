const DeviceDriver = require("./DeviceDriver")
class Display extends DeviceDriver {
    constructor(name){
        super("")
        this.name = name || this.constructor.name
    }
    setup(opts) {
        this.optsSetup = opts
        this.disp = beapi.lvgl.createDisplay(this.name, opts||{}) 
        return !!this.disp
    }
    register(insName) {
        super.register(insName)
        if(!be.disp){
            be.disp = []
        }
        be.disp.push(this.disp)
    }
    width() {
        return (this.optsSetup?.width) || 0
    }
    height() {
        return (this.optsSetup?.height) || 0
    }
}
module.exports = Display