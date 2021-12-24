let __flyweight = {}

class Pin extends beapi.EventEmitter {
    constructor(gpio) {
        if(!Pin.isPin(gpio))
            throw new Error("Not a valid pin number: "+gpio)
        if(__flyweight[gpio])
            return __flyweight[gpio]

        super()
        this.gpio = gpio
        this.mode = ""
        __flyweight[gpio] = this
    }

    /**
     * @param {function} callback 
     * @param {"rising"|"falling"|"both"|object|undefined} options="both"
     */
    watch(callback, options) {
        if(!options)
            options = "both"
        if(typeof options=="string") {
            options = {
                mode: options ,
                debounce: 20 ,
                once: false ,
            }
        }
        if(options.debounce>0) {
            let oricb = callback
            let timer = -1
            callback = (val, pin)=>{
                if(timer>-1) 
                    return
                timer = setTimeout(()=>{
                    timer = -1
                    if(beapi.gpio.digitalRead(pin.gpio)==val)
                        oricb(val, pin)
                }, options.debounce)
            }
            callback.__origin = oricb
        }
        if(!!options.once)
            this.once(options.mode, callback)
        else 
            this.on(options.mode,callback)
        this.resetISR()
        return this
    }
    unwatch(callback, mode) {
        if(!mode) {
            this.off("rising", callback, true)
            this.off("falling", callback, true)
            this.off("both", callback, true)
        }
        else {
            this.off(mode, callback, true)
        }
        this.resetISR()
        return this
    }
    clear() {
        this.off("rising")
        this.off("falling")
        this.off("both")
        this.resetISR()
        return this
    }

    resetISR() {
        let mode = 0 ;
        if(this._handles["both"]&&this._handles["both"].length) {
            mode = 3 // both
        }
        if(this._handles["rising"]&&this._handles["rising"].length) {
            mode|= 1 // rising
        }
        if(this._handles["falling"]&&this._handles["falling"].length) {
            mode|= 2 // falling
        }
        beapi.gpio.setPinISR(this.gpio, mode)
    }

    setMode(mode) {
        beapi.gpio.pinMode(this.gpio, mode)
        this.mode = mode
        return this
    }

    pull(mode) {
        beapi.gpio.pinPull(this.gpio, mode)
        return this
    }

    write(val) {
        return beapi.gpio.digitalWrite(this.gpio, val)
    }

    read() {
        return beapi.gpio.digitalRead(this.gpio)
    }

    // freq * resolution^2 最大值为 LEDC_APB_CLK (80 MHz)
    pwm (value, freq, resolution) {
        if(value<0 || value>1){
            throw new Error("Invalid value (0-1)")
        }
        if(freq!=undefined && resolution!=undefined) {
            beapi.gpio.pwmConfig( this.gpio, freq, resolution, Math.round(value*(2**resolution)) )
        }
        else {
            beapi.gpio.pwmWrite( this.gpio, value )
        }
    }

    pwmStop() {
        beapi.gpio.pwmStop(this.gpio)
    }
}

Pin.isPin = function(pinNum){
    if(!Number.isInteger(pinNum))
        return
    return (pinNum>=0 && pinNum<=5)
            || (pinNum>=12 || pinNum<=19)
            || (pinNum>=21 || pinNum<=23)
            || (pinNum>=25 || pinNum<=27)
            || (pinNum>=32 || pinNum<=36)
            || pinNum == 39
}

let _pinsISRCallbacks = []
beapi.gpio.setPinISRCallback(function(gpio){
    let val = beapi.gpio.digitalRead(gpio)
    for(let cb  of _pinsISRCallbacks) {
        cb(gpio, val)
    }
    if(__flyweight[gpio]) {
        __flyweight[gpio].emit(val? "rising": "falling", val, __flyweight[gpio])
        __flyweight[gpio].emit("both", val, __flyweight[gpio])
    }
})

global.watchPins = function(callback) {
    _pinsISRCallbacks.push(callback)
}

global.pin = function(gpio) {
    if( gpio instanceof Pin )
        return gpio
    return new Pin(gpio)
}
// pinMode 会清除 isr 类型设置
// 可能和 pinMode() 的 c 实现有关
let _pinMode = beapi.gpio.pinMode
beapi.gpio.pinMode = function(gpio, mode) {
    _pinMode(gpio, mode)
    pin(gpio).resetISR()
}
module.exports = Pin