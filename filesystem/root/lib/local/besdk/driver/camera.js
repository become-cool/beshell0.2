const DeviceDriver = require("./DeviceDriver")
class Camera extends DeviceDriver {
    constructor(){
        super("camera")
    }
    setup(opts) {
        if(!beapi.driver.camera.setup(opts)){
            return false
        }
        if( opts.stream ) {
            if(opts.stream.http) {
                beapi.driver.camera.startHTTPStream(opts.stream.http)
            }
            if(opts.stream.tcp) {
                beapi.driver.camera.startTCPStream(opts.stream.tcp)
            }
        }
    }
}
module.exports = Camera