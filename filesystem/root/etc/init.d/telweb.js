
if(beapi.wifi.hasInited() || process.simulate) {

    const telweb = require("telweb")

    let addr80 = "http://0.0.0.0:"+(process.simulate?'8088':'80')
    let addr8022 = "http://0.0.0.0:8022"
    let addr43322 = "https://0.0.0.0:43322"

    be.server.http80 = telweb.start(addr80)
    be.server.http8022 = telweb.start(addr8022)
    be.server.http43322 = telweb.start(addr43322, true)

    console.log("telweb start on addr:")
    console.log(addr80)
    console.log(addr8022)
    console.log(addr43322)

}