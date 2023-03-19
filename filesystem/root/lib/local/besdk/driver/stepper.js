const DeviceDriver = require("./DeviceDriver")
class Stepper extends DeviceDriver {

    constructor(name){
        super("")
        this.name = name || this.constructor.name
        this.proxy('run')
        this.proxy('runSteps')
        this.proxy('runTo')
        this.proxy('stop')
        this.proxy('freq')
        this.proxy('setFreq')
        this.proxy('accel')
        this.proxy('setAccel')
        this.proxy('isRunning')
        this.proxy('isStopping')
        this.proxy('pos')
        this.proxy('setPos')
        this.proxy('setPassing')
        this.proxy('clearPassing')
        this.proxy('on')
        this.proxy('race')
    }
    setup(opts) {
        this.optsSetup = opts
        this.dvr = new beapi.driver.SoftStepper(opts.step,opts.dir,opts.limit) 
        return !! this.dvr
    }

    proxy(methodName) {
        this[methodName] = (...argv)=>{
            if(!this.dvr) {
                throw new Error("invork setup() first")
            }
            return this.dvr[methodName] (...argv)
        }
    }
    info() {
        return {
            pos: this.dvr.pos() ,
            freq: this.dvr.freq() ,
            freqRT: this.dvr.freq(true) ,
            accel: this.dvr.accel() ,
            running: this.dvr.isRunning() ,
            stopping: this.dvr.isStopping() ,
        }
    }
    runTime() { return this.dvr.stopTime() - this.dvr.startTime() }
}
module.exports = Stepper