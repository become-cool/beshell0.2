class PWMServo {

    constructor(pinPwm, maxPos){
        this.pinPwm
        this.maxPos = 270
    }
    
    setup(s, max) {
        if(typeof s=='object') {
            var {s,max} = s
        }
        if(s!=undefined) {
            this.pinPwm = s
        }
        if(this.pinPwm!=undefined) {
            pin(this.pinPwm).setMode("output")
        }
        if(max!=undefined) {
            this.maxPos = max
        }
    }

    setValue(pos) {
        pin(this.pinPwm).pwm(0.025 + 0.1*pos/this.maxPos, 50)
    }
}


module.exports = PWMServo