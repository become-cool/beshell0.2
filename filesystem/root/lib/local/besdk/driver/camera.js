const DeviceDriver = require("./DeviceDriver")
class Camera extends DeviceDriver {
    constructor(){
        super("camera")
    }
    begin(opts) {
        return beapi.driver.camera.setup(opts)
    }
}
module.exports = Camera