const DeviceDriver = require("./DeviceDriver")

class I2CDevice extends DeviceDriver {
    setup(bus,sda,scl,addr) {
        if(typeof bus=='object') {
            var {bus,sda,scl,addr} = bus
        }
        if(bus!=0 && bus!=1) {
            throw new Error("i2c bus num must be 0 or 1")
        }
        this.bus=bus
        this.sda=sda
        this.scl=scl
        this.addr=addr
        beapi.i2c.setup(this.bus, this.sda, this.scl)
        return this.scan(this.addr)
    }
}
module.exports = I2CDevice