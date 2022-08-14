const DeviceDriver = require("./DeviceDriver")
class LEDRGB extends DeviceDriver {

    constructor(){
        super("led-rgb")
    }

    /**
     * @param opt:{
     *     r?: GPIONum ,
     *     g?: GPIONum ,
     *     b?: GPIONum ,
     * }
     * @param {number} pr 红色 pin， undefined 表示使用电源负 
     * @param {number} pg 绿色 pin， undefined 表示使用电源负 
     * @param {number} pb 蓝色 pin， undefined 表示使用电源负 
     * @param {number} pp 共阳 pin， undefined 表示使用电源正 
     */
    begin(pr, pg, pb, pp) {
        if(pp!=undefined){
            this.pp = pp
            pinMode(pp, "output", true)
        }
        if(pr!=undefined) {
            this.pr = pr
            pinMode(pr, "output", true)
        }
        if(pg!=undefined) {
            this.pg = pg
            pinMode(pg, "output", true)
        }
        if(pb!=undefined) {
            this.pb = pb
            pinMode(pb, "output", true)
        }
    }

    /**
     * 设置 led 的亮度
     * 
     * @param {int} r,g,b value: 0 - 255
     */
    setValue(r, g, b) {
        if( this.pr!=undefined ) {
            analogWrite(this.pr, 1-r/255)
        }
        if( this.pg!=undefined ) {
            analogWrite(this.pg, 1-g/255)
        }
        if( this.pb!=undefined ) {
            analogWrite(this.pb, 1-b/255)
        }
        if( this.pp!=undefined ) {
            analogWrite(this.pp, 1)
        }
    }
}


module.exports = LEDRGB