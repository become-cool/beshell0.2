const DeviceDriver = require("./DeviceDriver")
class XPT2046 extends DeviceDriver {
    constructor(){
        super("XPT2046")
    }
    setup(opts) {
        this.indev = new beapi.lvgl.InDevPointer( this.name, opts.spi, opts.cs )
        return !!this.indev
    }
}
module.exports = XPT2046