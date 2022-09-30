const DeviceDriver = require("./DeviceDriver")
class Button extends DeviceDriver {
    constructor() {
        super("btn")
    }
    setup(opts){
        this.i = opts.i
        this.o = opts?.o
        this.t = opts?.t || 0
        this.k = opts?.k

        if(this.o!=undefined) {
            pin(this.o).setMode("output").write( this.t )
        }
        pin(this.i).setMode("input").pull(this.t?"down":"up").watch((lv)=>{
            console.log(this.i,lv)
        })
    }
    register(insName) {
        super.register(insName)
        // this.indev = new lv.InDevNav("joypad", this.bus||0, this.addr||51)
        // be.indev.push(this.indev)
    }
}

module.exports = Button