const lv = require('lv')
const Part = require('./Part')
const config = require("./part.lib/motor.js")

class BeMotor extends Part{
    static config = config.motor
    constructor(parent) {
        super(parent)
        this.loadPortConf(this.config().ports)
    }
}

class BeServo extends BeMotor{
    static config = config.servo
}

exports.BeMotor = BeMotor
exports.BeServo = BeServo