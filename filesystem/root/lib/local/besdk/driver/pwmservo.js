class PWMServo {

    constructor(pinPwm, maxPos){
        this.pinPwm = pinPwm
        this.maxPos = maxPos || 270
    }
    
    begin(pinPwm, maxPos) {
        if(pinPwm!=undefined) {
            this.pinPwm = pinPwm
        }
        if(this.pinPwm!=undefined) {
            pin(this.pinPwm).setMode("output")
        }
        if(maxPos!=undefined) {
            this.maxPos = maxPos
        }
    }

    setValue(pos) {
        pin(this.pinPwm).pwm(0.025 + 0.1*pos/this.maxPos, 50)
    }
}


module.exports = PWMServo