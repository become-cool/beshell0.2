const DeviceDriver = require("./DeviceDriver")
class Camera extends DeviceDriver {
    constructor(){
        super("camera")
    }
    setup(opts) {
        if(!beapi.driver.camera.setup(opts)){
            return false
        }
        if( opts.streamPort ) {
            beapi.driver.camera.startStream(opts.streamPort)
        }
    }
}
module.exports = Camera