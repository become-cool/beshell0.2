const DeviceDriver = require("./DeviceDriver")
class Stepper extends DeviceDriver {

    constructor(name){
        super("")
        this.name = name || this.constructor.name
        this.proxy('run')
        this.proxy('runSteps')
        this.proxy('runTo')
        this.proxy('stop')
        this.proxy('getSteps')
        this.proxy('setFreq')
        this.proxy('freq')
        this.proxy('isRunning')
        this.proxy('pos')
        this.proxy('setPos')
        this.proxy('setPassingBy')
        this.proxy('clearPassingBy')
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
}
module.exports = Stepper