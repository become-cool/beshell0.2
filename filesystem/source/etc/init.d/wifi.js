const configPath = "/etc/config/wifi.json"
function defaultConfig() {
    return {
            "sta": {
            "ssid": "" ,
            "password": "" ,
            "useDHCP": true
        } ,

        "ap": {
            "ssid": "BECOME-${UUID}" ,
            "password": "" ,
        }
    }
}

WiFi.save = function() {
    let status = WiFi.getStatus()
    if(!status.ssid)
        return
    try{
        var json = fs.readFileSync(configPath)
        json = JSON.parse(json)
    }catch(e) {
        console.log(e)
    }
    if(!json || !json.sta) {
        json = defaultConfig()
    }
    json.sta.ssid = status.ssid
    json.sta.password = status.password
    fs.writeFileSync(configPath, JSON.stringify(json,null,4))
}
WiFi.saveAP = function() {
    let status = WiFi.getAPStatus()
    if(!status.ssid)
        return
    try{
        var json = fs.readFileSync(configPath)
        json = JSON.parse(json)
    }catch(e) {
        console.log(e)
    }
    if(!json || !json.sta) {
        json = defaultConfig()
    }
    json.ap.ssid = status.ssid
    json.ap.password = status.password
    fs.writeFileSync(configPath, JSON.stringify(json,null,4))
}

function autoConnect() {
    try{
        var json = fs.readFileSync(configPath)
        json = JSON.parse(json)
    }catch(e){
        console.log(e)
        console.log("bad json file:", configPath)
        json = defaultConfig()
    }

    let status = WiFi.getStatus()

    // sta
    if(status.status=='disconnected') {
        setTimeout(()=>{
            if(json.sta.ssid) {
                console.log("connect to ssid:", json.sta.ssid, json.sta.password)
                WiFi.connect(json.sta.ssid, json.sta.password||"", null, function(err){
                    if(err) {
                        console.log("connect wifi error:", err)
                    }
                    else {
                        "has connected to wifi "
                    }
                })
            }
        },1000)
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