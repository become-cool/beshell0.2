const DeviceDriver = require("./DeviceDriver")
class Camera extends DeviceDriver {
    constructor(){
        super("camera")
    }
    setup(opts) {
        return beapi.driver.camera.setup(opts)
    }
}
module.exports = Camera