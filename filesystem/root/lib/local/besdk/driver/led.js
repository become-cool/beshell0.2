require("../pin.js")

class LED {

    constructor(){
    }

    /**
     * 
     * @param {number} 正极 pin， undefined 表示使用电源正 
     * @param {number} 负极 pin， undefined 表示使用电源负 
     */
    begin(pp, np, freq, resolution) {
        if(pp!=undefined){
            this.pp = pp
            beapi.gpio.pinMode(pp, "output", true)
        }
        if(np!=undefined) {
            this.np = np
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