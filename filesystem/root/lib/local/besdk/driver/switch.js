const Pin = require("../pin")

class Switch extends beapi.EventEmitter {

    constructor(){
        super()
        this._callback = (val)=>{
            this.emit( "change", val!=this._mode )
            this.emit( (val==this._mode? "off": "on"), val )
        }
        this._pinInput = null
        this._mode = 1  // 1: input pullup, 0: input pulldown
    }

    /**
     * 
     * @param {Pin} pinInput 
     * @param {Pin|'vcc'|'ground'} pinOutput = 'ground'
     */
    begin(pinInput, pinOutput) {

        this.deinit()

        // 转为 Pin 对象
        pinInput = pin( pinInput )
        if(pinOutput) {
            try{
                pinOutput = pinOutput && pin(pinOutput)
            }catch(e) {}
        }
        // pinOutput 默认值 ground
        else {
            pinOutput = 'ground'
        }

        this._mode = 1  

        if((pinOutput.gpio==35 || pinOutput.gpio==39)) {
            // 交换 input / output pin
            let tmp = pinInput
            pinInput = pinOutput
            pinOutput = tmp
        }

        if( pinOutput=='vcc' || pinOutput.gpio==34 || pinOutput.gpio==36 ) {   // 34/36 被固定上拉
            this._mode = 0
        }
        // gpio2 is input-pullup only
        if(pinInput.gpio==2 || pinInput.gpio==35 || pinInput.gpio==39) {
            this._mode = 0
        }
        
        if(pinOutput instanceof Pin){
            pinOutput
                .setMode("output")
                .write(this._mode? 0: 1)
        }
        
        this._pinInput = pinInput
                            .setMode(this._mode? "input-pullup": "input-pulldown")
                            .watch(this._callback, "both")
    }

    deinit() {
        if(this._pinInput) {
            this._pinInput.unwatch(this._callback, "both")
            this._pinInput = null
        }
    }

    isOn() {
        if(!(this._pinInput instanceof Pin))
            return false
        return this._pinInput.read() != this._mode
    }
}


module.exports = Switch