const DeviceDriver = require("./DeviceDriver")
class ShiftReg extends DeviceDriver {
    constructor(){
        super("shift")
    }
    setup(opt) {
        if(isNaN(opt.clk)) {
            throw new Error("missing clk pin")
        }
        if(isNaN(opt.data)) {
            throw new Error("missing data pin")
        }
        this.opt=opt
        pin(opt.data).setMode("input")
        pin(opt.clk).setMode("output")
        if(!isNaN(opt.pl)) {
            pin(opt.pl).setMode("output")
        }
        if(!isNaN(opt.ce)) {
            pin(opt.ce).setMode("output")
        }
    }
    read(bits) {
        return beapi.driver.common.shift_read(this.opt.data,this.opt.clk,this.opt.pl,this.opt.ce,bits||8)
    }
}
module.exports = ShiftReg