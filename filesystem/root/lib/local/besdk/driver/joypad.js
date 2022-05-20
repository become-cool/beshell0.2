class Joypad extends beapi.EventEmitter {
    constructor(bus,sda,clk,addr) {
        super()
        this.bus=bus||0
        this.sda=sda
        this.clk=clk
        this.addr=addr
        this._timer=-1
        this.raw = 0
        if(this.bus!=0 && this.bus!=1) {
            throw new Error("i2c bus num must be 0 or 1")
        }
    }
    scan(addr) {
        return beapi.i2c.ping(this.bus, addr||this.addr)
    }
    setup() {
        beapi.i2c.setup(this.bus, this.sda, this.clk)
        return this.scan(this.addr)
    }
    register() {
        this.indev = new lv.InDevNav("joypad", this.bus, this.addr)
    }
    end() {
        if(this._timer>-1) {
            clearTimeout(this._timer)
            this._timer = -1
        }
    }
    read() {
        if( this.addr!=51 && this.addr!=52 ) {
            throw new Error("not set i2c device addr")
        }
        this.raw = beapi.i2c.readUInt8(this.bus, this.addr)
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