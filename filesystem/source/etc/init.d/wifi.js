const configPath = "/etc/config/wifi.json"

WiFi.save = function() {
    let status = WiFi.getStatus()
    if(!status.sta.ssid)
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
                "ssid": "" ,
                "password": "" ,
                "gateway": "192.168.4.1" ,
                "netmask": "255.255.255.0" ,
                "ip": "192.168.4.1"
            }
        }
    }

    json.sta.ssid = status.sta.ssid
    json.sta.password = status.sta.password

    fs.writeFileSync(configPath, JSON.stringify(json,null,4))
}

function autoConnect() {
    let json = fs.readFileSync(configPath)
    json = JSON.parse(json)

    let status = WiFi.getStatus()

    // sta
    if(status.sta.status=='disconnected') {
        if(json.sta.ssid) {
            WiFi.connect(json.sta.ssid, json.sta.password||"")
        }
    }

    // ap

}

exports.autoConnect = autoConnect()

autoConnect()