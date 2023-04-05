const lv = require("lv")
const I2CDevice = require("./I2CDevice")
class Joypad extends I2CDevice {
    constructor() {
        super("joypad")
        this.raw = 0
    }
    scan(addr) {
        return beapi.i2c.ping(this.bus, addr||this.addr)
    }
    setup(opts){
        this.bus = opts?.i2c || 0
        this.addr = opts?.addr

        this.indev = new lv.InDevNav("joypad", this.bus||0, this.addr||51)
        be.indev.push(this.indev)
        try{
            this.indev.registerToLvgl()
        }catch(e) {
            console.log(e)
        }
    }
    read() {
        if( this.addr!=51 && this.addr!=52 ) {
            throw new Error("not set i2c device addr")
        }
        this.raw = beapi.i2c.readU8(this.bus, this.addr)
        let btns = {
            up: !!(this.raw & 0x01) ,
            down: !!(this.raw & 0x02) ,
            left: !!(this.raw & 0x04) ,
            right: !!(this.raw & 0x08) ,
            a: !!(this.raw & 0x010) ,
            b: !!(this.raw & 0x020) ,
            start: !!(this.raw & 0x40) ,
            select: !!(this.raw & 0x80) ,
        }
        return btns
    }
}
module.exports = Joypad