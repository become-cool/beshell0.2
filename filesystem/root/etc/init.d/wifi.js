const configPath = "/etc/config/wifi.json"
function defaultConfig() {
    return {
            "sta": {
            "ssid": "" ,
            "password": "" ,
            "useDHCP": true ,
            "retry_dur": 0
        } ,

        "ap": {
            "ssid": "BECOME-${UUID}" ,
            "password": "" ,
        }
    }
}
function readWiFiConfig() {
    try{
        var json = fs.readFileSync(configPath)
        json = JSON.parse(json)
    }catch(e){
        console.log(e)
        console.log("bad json file:", configPath)
        json = defaultConfig()
    }
    return json
}

global.WiFi = {
    connect: beshellapi.wifi.connect ,
    disconnect: beshellapi.wifi.disconnect ,
    getMode: beshellapi.wifi.getMode ,
    setMode: beshellapi.wifi.setMode ,
    getStatus: beshellapi.wifi.getStatus ,
    getAPStatus: beshellapi.wifi.getAPStatus ,
    setHostname: beshellapi.wifi.setHostname ,
    startAP: beshellapi.wifi.startAP ,
    stopAP: beshellapi.wifi.stopAP ,
}

WiFi.save = function() {
    let status = beshellapi.wifi.getStatus()
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
    let status = beshellapi.wifi.getAPStatus()
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

function connectToAP() {
    let status = beshellapi.wifi.getStatus()
    if(status.status!='disconnected') {
        return
    }
    let json = readWiFiConfig()
    if(!json.sta.ssid) {
        return
    }

    console.log("connect to ssid:", json.sta.ssid, json.sta.password)
    beshellapi.wifi.connect(json.sta.ssid, json.sta.password||"", null, function(err){
        if(err) {
            console.log("connect wifi error:", err)
        }
        else {
            "has connected to wifi "
        }
    })
}

function autoConnect() {
    let json = readWiFiConfig()
    let status = beshellapi.wifi.getStatus()

    // sta
    if(status.status=='disconnected') {
        setTimeout(connectToAP,1000)
        if(json.sta.retry_dur>0) {
            setInterval(connectToAP, json.sta.retry_dur)
        }
    }
    else {
        console.log("wifi status", status.status)
    }

    // ap
    let apstatus = beshellapi.wifi.getAPStatus()
    let ssid = json.ap.ssid.replace("${UUID}", beshellapi.utils.uuid().substr(-4))
    if( !apstatus.started || apstatus.ssid!=ssid ){
        beshellapi.wifi.startAP(ssid, json.ap.password)
    }

}

exports.autoConnect = autoConnect

autoConnect()