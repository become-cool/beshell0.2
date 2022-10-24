const DeviceDriver = require("./DeviceDriver")
class PianoKeyboard extends DeviceDriver {
    constructor() {
        super("piano")
    }
    keys = [
        53,54,55,56,57,58,59,60,
        61,62,63,64,0,0,0,0,
        // 65,66,67,68,69,70,71,72,
        // 73,74,75,76,0,0,0,0,
    ]
    setup(opts){
        this.cs = opts.cs
        this.pl = opts.pl
        this.i2s = opts.i2s||0
        // console.log(opts)
        pin(opts.pl).setMode("output")
        this.devId = beapi.spi.addDevice(opts.spi,this.cs,1000000,0)
        // console.log("piano:",this.devId)
        // beapi.midi.playWithKeyboard(this.devId,this.pl)
    }
}
module.exports = PianoKeyboard