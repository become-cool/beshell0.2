const DeviceDriver = require("./DeviceDriver")
class XPT2046 extends DeviceDriver {
    constructor(){
        super("XPT2046")
    }
    setup(opts) {
        this.indev = new beapi.lvgl.InDevPointer( this.name, opts.spi, opts.cs )
        let setConf = (prop,method) => {
            if(opts[prop]!=undefined) {
                this.indev[method]( opts[prop] )
            } 
        }
        setConf('maxX','setMaxX')
        setConf('maxY','setMaxY')
        setConf('offsetX','setOffsetX')
        setConf('offsetY','setOffsetY')
        setConf('swapXY','setSwapXY')
        setConf('invX','setInvX')
        setConf('invY','setInvY')
        return !!this.indev
    }
    register(insName) {
        super.register(insName)
        be.indev.push(this.indev)
    }
}
module.exports = XPT2046