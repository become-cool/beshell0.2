const DeviceDriver = require("./DeviceDriver")
class FakeIndev extends DeviceDriver {
    constructor(){
        super("fake-indev")
    }
    setup() {
        this.indev = new beapi.lvgl.InDevPointer('fake')
        return !!this.indev
    }
    register(insName) {
        super.register(insName)
        be.indev.push(this.indev)
    }
}
module.exports = FakeIndev