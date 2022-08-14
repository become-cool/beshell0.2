const DeviceDriver = require("./DeviceDriver")
require("../pin.js")

class LED extends DeviceDriver{

    constructor(){
        super("led")
    }

    /**
     * 
     * @param p {number} 正极 pin， undefined 表示使用电源正 
     * @param n {number} 负极 pin， undefined 表示使用电源负 
     */
    setup(pp, np, freq, resolution) {
        if(typeof p=='object') {
            var {p,n,freq,resolution} = p
        }
        if(p!=undefined){
            this.pp = p
            beapi.gpio.pinMode(pp, "output", true)
        }
        if(n!=undefined) {
            this.np = n
            beapi.gpio.pinMode(np, "output", true)
        }

        if( this.pp ) {
            pin(this.pp).pwm(0, freq||200, resolution||14)
        }
        else if( this.np ) {
            pin(this.np).pwm(0, freq||200, resolution||14)
        }
    }

    /**
     * 设置 led 的亮度
     * 
     * @param {float} value 0 - 255
     */
    setValue(value, bGamma) {

        if(bGamma)
            value = beapi.utils.gamma8Correct(value)
        value/= 255

        // pwm -> ground
        if( this.np==undefined && this.pp!=undefined ) {
            pin(this.pp).pwm(value)
        }
        // pwm -> pwm
        else if( this.np!=undefined && this.pp!=undefined ) {
            pin(this.pp).pwm(value)
            beapi.gpio.digitalWrite(this.np, 0)
        }
        // source -> pwm
        else if( this.np!=undefined && this.pp==undefined ) {
            pin(this.np).pwm(1-value)
        }
        // source -> ground
        else if( this.np==undefined && this.pp==undefined ) {
            // nothing todo
        }
    }

    deinit() {
        if( this.pp ) {
            pin(this.pp).pwmStop()
            delete this.pp
        }
        if( this.np ) {
            pin(this.np).pwmStop()
            delete this.np
        }
    }
}


module.exports = LED