const DeviceDriver = require("./DeviceDriver")
class SDSPI extends DeviceDriver {
    constructor(){
        super("sdspi")
    }
    setup(opts) {
        this.opts = opts
        this.mounted = beapi.driver.mountSD(opts.spi, opts.cs, opts.mount, opts.khz)
        return this.mounted
    }
}
module.exports = SDSPI