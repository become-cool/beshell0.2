const configPath = "/etc/config/wifi.json"

WiFi.save = function() {
    let status = WiFi.getStatus()
    if(!status.ssid)
        return
    let json
    try{
        json = fs.readFileSync(configPath)
        json = JSON.parse(json)
    }catch(e) {
        console.log(e)
    }
    if(!json || !json.sta) {
        json = {
            "sta": {
                "ssid": "" ,
                "password": "" ,
                "useDHCP": true ,
                "gateway": "" ,
                "netmask": "" ,
                "ip": ""
            } ,
        
            "ap": {
                "ssid": "BECOME-${UUID}" ,
                "password": "" ,
            }
        }
    }

    json.ssid = status.ssid
    json.password = status.password

    fs.writeFileSync(configPath, JSON.stringify(json,null,4))
}

function autoConnect() {
    let json = fs.readFileSync(configPath)
    json = JSON.parse(json)

    let status = WiFi.getStatus()

    // sta
    if(status.status=='disconnected') {

        console.log("connect to ssid:", json.ssid, json.password)

        if(json.ssid) {
            WiFi.connect(json.ssid, json.password||""/*, null, function(err){
                if(err) {
                    console.log("connect wifi error:", err)
                }
                else {
                    "has connected to wifi "
                }
            }*/)
        }
    }
    else {
        console.log("wifi status", status.status)
    }

    // ap
    let apstatus = WiFi.getAPStatus()
    let ssid = json.ap.ssid.replace("${UUID}", utils.uuid().substr(0,4))
    if( !apstatus.started || apstatus.ssid!=ssid ){
        WiFi.startAP(ssid, json.ap.password)
    }

}

exports.autoConnect = autoConnect

autoConnect()