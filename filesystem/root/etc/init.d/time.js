
beapi.utils.setTimezoneOffset(-8*60)

const sntpUrl = "udp://203.107.6.88:123"

function sntp_request(retry) {
    console.log("require time from",sntpUrl)
    beapi.mg.sntpRequest(sntpUrl,(t)=>{ // ntp.aliyun.com
        if(t>0) {
            console.log("internet time:",t)
            beapi.utils.setTime(t)
        } else {
            retry = parseInt(retry)
            if(!isNaN(retry)) {
                console.log("sntp request failed, retry ...")
                setTimeout(()=>sntp_request(retry-1), 3000)
            }
        }
    })
}

if(!process.simulate) {
    const wifi = require("besdk/wifi")
    wifi.on("ip.got", ()=>{
        sntp_request(10)
    })
}