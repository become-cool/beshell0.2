const DeviceDriver = require("./DeviceDriver")
class ShiftReg extends DeviceDriver {
    constructor(){
        super("shift")
    }
    setup(opt) {
        if(isNaN(opt.clk)) {
            throw new Error("missing clk pin")
        }
        this.pin_clk = opt.clk
        pin(this.pin_clk).setMode("output")

        if(isNaN(opt.data)) {
            throw new Error("missing data pin")
        }
        this.pin_data = opt.data
        pin(this.pin_data).setMode("input")

        if(!isNaN(opt.pl)) {
            this.pin_pl = opt.pl
            pin(this.pin_pl).setMode("output")
        }

        if(!isNaN(opt.ce)) {
            this.pin_ce = opt.ce
            pin(this.pin_ce).setMode("output")
        }

        this.us = isNaN(opt.us)?100: opt.us
    }
    read(bits) {
        return beapi.driver.common.shift_read(this.pin_data,this.pin_clk,this.pin_pl,this.pin_ce,bits||8,this.us)
    }
}
module.exports = ShiftReg