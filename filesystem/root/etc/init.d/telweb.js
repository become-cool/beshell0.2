
if(beapi.wifi.hasInited()) {

    const telweb = require("telweb")


    let addr8022 = "http://0.0.0.0:8022"
    let addr80 = "http://0.0.0.0:"+(process.simulate?'8088':'80')

    be.server.http8022 = telweb.start(addr8022)
    be.server.http80 = telweb.start(addr80)

    console.log("telweb start on addr:")
    console.log(addr8022)
    console.log(addr80)

}