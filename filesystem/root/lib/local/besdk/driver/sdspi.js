const DeviceDriver = require("./DeviceDriver")
class SDSPI extends DeviceDriver {
    constructor(){
        super("sdspi")
    }
    setup(opts) {
        return beapi.driver.mountSD(opts.spi, opts.cs, opts.mount)
    }
}
module.exports = SDSPI