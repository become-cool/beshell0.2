
beapi.utils.setTimezoneOffset(-8*60)

if(!process.simulate) {
    const wifi = require("besdk/wifi")
    wifi.on("ip.got", ()=>{
        let retry = 5
        function sntp_request() {
            beapi.mg.sntpRequest("udp://ntp.aliyun.com:123",(t)=>{
                if(t>0) {
                    beapi.utils.setTime(t)
                } else {
                    if(retry--) {
                        console.log("sntp request failed, retry ...")
                        setTimeout(sntp_request, 3000)
                    }
                }
            })
        }
        sntp_request()
    })
}