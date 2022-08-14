require("../pin")

class PWMMotor {

    constructor(pinPwm){
        this.pinPwm = pinPwm
    }

    setup(s) {
        if(typeof s=='object') {
            var {s} = s
        }
        if(s!=undefined) {
            this.pinPwm = s
        }
        if(this.pinPwm!=undefined) {
            pin(this.pinPwm).setMode("output")
        }
    }

    setValue(value, ccw) {
        if(this.pinPwm==undefined) 
            return
        if(value>1)
            value = 1
        if(value<0)
            value = 0
        let v =  0.075 + value * ((!!ccw)? 0.05: -0.05)
        pin(this.pinPwm).pwm(v, 50)
    }
}


module.exports = PWMMotor