const DeviceDriver = require("./DeviceDriver")
module.exports = class ES8156 extends DeviceDriver {
    constructor(){
        super("ES8156")
    }
    setup(opts) {
        this.opts = opts
        beapi.driver.ES8156Setup(opts.i2c,opts.addr||0x8)
        beapi.driver.holdDMA("audio",30*1024)
    }
    register(insName) {
        super.register(insName)
        // be.indev.push(this.indev)
    }
    printAllREG() {
        for(let r=0;r<=0x25;r++) {
            console.log("REGISTER", f(r), "=", f(beapi.i2c.readU8(0,0x8,r)))
        }
    }
}

function f(d) {
    d = d.toString(16).toUpperCase()
    if(d.length<2) {
        d = '0' + d
    }
    return '0x' + d
}
