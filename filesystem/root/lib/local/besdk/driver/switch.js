const DeviceDriver = require("./DeviceDriver")
const Pin = require("../pin")

class Switch extends DeviceDriver {

    constructor(){
        super('switch')
        this._callback = (val)=>{
            this.emit( "change", val!=this._trigger )
            this.emit( (val==this._trigger? "on": "off"), val )
        }
        this._pinInput = null
        this._trigger = 1  // 0: input pullup; 1: input pulldown
    }

    /**
     * @param opt: {
     *  i:GPIONum,
     *  o:GPIONum|"vcc"|"ground",
     *  triger?: 1|0
     * }
     */
    begin(i, o, triger) {
        if(typeof i=='object') {
            var {i, o, triger} = i
        }
        this._triger = !!triger

        this.deinit()

        // 转为 Pin 对象
        i = pin( i )
        if(o) {
            if(o=='vcc') {
                pin(o).setMode("output").write(this._trigger)
            }
            else if(o=='ground') {
                pin(o).setMode("output").write(this._trigger)
            }
            else {
                pin(o).setMode("output").write(this._trigger)
            }
        }
        
        this._pinInput = i.setMode(this._trigger? "input-pulldown": "input-pullup")
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
        return this._pinInput.read() != this._trigger
    }
}


module.exports = Switch