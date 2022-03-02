// wifi mode
const MODE_NONE = 0
const MODE_STA = 1
const MODE_AP = 2
const MODE_APSTA = 3

// event type
EVENT_WIFI = 1
EVENT_IP = 2 

// WiFi events
const WIFI_READY = 0
const WIFI_SCAN_DONE = 1
const WIFI_STA_START = 2
const WIFI_STA_STOP = 3
const WIFI_STA_CONNECTED = 4
const WIFI_STA_DISCONNECTED = 5
const WIFI_STA_AUTHMODE_CHANGE = 6
const WIFI_STA_WPS_ER_SUCCESS = 7
const WIFI_STA_WPS_ER_FAILED = 8
const WIFI_STA_WPS_ER_TIMEOUT = 9
const WIFI_STA_WPS_ER_PIN = 10
const WIFI_STA_WPS_ER_PBC_OVERLAP = 11
const WIFI_AP_START = 12
const WIFI_AP_STOP = 13
const WIFI_AP_STACONNECTED = 14
const WIFI_AP_STADISCONNECTED = 15
const WIFI_AP_PROBEREQRECVED = 16
const WIFI_FTM_REPORT = 17
const WIFI_STA_BSS_RSSI_LOW = 18
const WIFI_ACTION_TX_STATUS = 19
const WIFI_ROC_DONE = 21
const WIFI_STA_BEACON_TIMEOUT = 21

let evtNames = []
evtNames[WIFI_SCAN_DONE] = "scan.done"
evtNames[WIFI_STA_START] = "sta.start"
evtNames[WIFI_STA_STOP] = "sta.stop"
evtNames[WIFI_STA_CONNECTED] = "sta.connected"
evtNames[WIFI_STA_DISCONNECTED] = "sta.disconnected"
evtNames[WIFI_AP_START] = "ap.start"
evtNames[WIFI_AP_STOP] = "ap.stop"
evtNames[WIFI_AP_STACONNECTED] = "ap.sta.connected"
evtNames[WIFI_AP_STADISCONNECTED] = "ap.sta.disconnected"

// IP events
const STA_GOT_IP = 0
const STA_LOST_IP = 1
const AP_STAIPASSIGNED = 2
const GOT_IP6 = 3
const ETH_GOT_IP = 4
const PPP_GOT_IP = 5
const PPP_LOST_IP = 6

let ipEvtNames = []
ipEvtNames[STA_GOT_IP] = "ip.got"
ipEvtNames[STA_LOST_IP] = "ip.lost"


// power safe
const PS_NONE = 0
const PS_MIN_MODEM = 1
const PS_MAX_MODEM = 2 


const wifi = module.exports = new beapi.EventEmitter()
wifi.start = beapi.wifi.start
wifi.stop = beapi.wifi.stop
wifi.mode = beapi.wifi.getMode
wifi.setMode = beapi.wifi.setMode


function contrastStatus(b) {
    let mode = wifi.mode()
    let status = 0
    if(b==beapi.wifi.staStarted()) {
        status|= MODE_STA
    }
    if(b==beapi.wifi.apStarted()) {
        status|= MODE_AP
    }
    return mode==status
}
wifi.isReady = function(){
    return contrastStatus(true)
}
wifi.start = function() {
    return new Promise(resolve=>{
        beapi.wifi.start()
        if(wifi.isReady()) {
            resolve()
        }
        else {
            wifi.once("start", resolve)
        }
    })
}
wifi.stop = function() {
    return new Promise(resolve=>{
        beapi.wifi.stop()
        if(contrastStatus(false)) {
            resolve()
        }
        else {
            wifi.once("stop", resolve)
        }
    })
}

wifi.connect = function(ssid,password,callback) {

    if(beapi.wifi.staConnected()) {
        wifi.stop()
    }
    
    let mode = wifi.mode()
    wifi.setMode(mode|MODE_STA)
    
    beapi.wifi.setStaConfig({ssid, password})

    beapi.wifi.start()

    if(callback) {
        wifi.once("sta.connected",callback)
    }
    return
    
    if(beapi.wifi.staConnected()) {
        callback && callback()
        return
    }

    if(_wifi_connect_callback) {
        callback && callback(new Error("wifi sta is connecting"))
        return
    }

    if(!beapi.wifi.staStarted()) {
        beapi.wifi.setMode( beapi.wifi.getMode() | MODE_STA )
        var ret = beapi.wifi.start()
        if(ret!=0) {
            callback && callback(new Error("beapi.wifi.start() failed: "+ret))
            return
        }
    }

    setTimeout(()=>{

        if(!beapi.wifi.staStarted()) {
            callback && callback(new Error("wifi sta has not readied"))
            return
        }
    
        var ret = beapi.wifi.connect(ssid,password)
        if(ret!=0){
            callback(new Error("beapi.wifi.connect() failed: "+ret))
            return
        }
    
        if(typeof callback=="function")
            _wifi_connect_callback = callback
    }, 0)
}

wifi.disconnect = function(callback) {
    if(!beapi.wifi.staConnected()) {
        callback && callback()
        return
    }
    if(typeof callback=="function") {
        wifi.once("sta.disconnected", callback)
    }
    beapi.wifi.disconnect() ;
}

wifi.status = function(netif) {
    if(!netif || netif=="apsta") {
        return {
            sta: wifi.status("sta"), 
            ap: wifi.status("ap")
        }
    }
    else if(netif=='sta') {
        let sta = beapi.wifi.getIpInfo(MODE_STA)
        sta.started = beapi.wifi.staStarted()
        sta.connected = beapi.wifi.staConnected()

        let config = beapi.wifi.getConfig(MODE_STA)
        sta.ssid = config.ssid
        sta.password = config.password
        
        return sta
    }
    else if(netif=='ap') {

        let ap = beapi.wifi.getIpInfo(MODE_AP)
        ap.started = beapi.wifi.apStarted()
    
        config = beapi.wifi.getConfig(MODE_AP)
        ap.ssid = config.ssid
        ap.password = config.password

        return ap
    }
    else {
        throw new Error("unknow netif: "+netif)
    }
}


const ConfigPath = "/etc/config/wifi.json"
function defaultConfig() {
    return {
        "sta": {
            "ssid": "" ,
            "password": "" ,
            "useDHCP": true ,
            "watch_dur": 0
        } ,

        "ap": {
            "ssid": "BECOME-${UUID}" ,
            "password": "" ,
        }
    }
}
function readWiFiConfig() {
    try{
        var json = JSON.load(ConfigPath)
    }catch(e){
        console.log(e)
        console.log("bad json file:", ConfigPath)
        json = defaultConfig()
    }
    return json
}


wifi.save = function(netif) {
    try{
        var json = beapi.fs.readFileSync(ConfigPath).asString()
        json = JSON.parse(json)
    }catch(e) {
        console.log(e)
    }
    if(!json || !json.sta) {
        json = defaultConfig()
    }

    let changed = false

    // for sta
    if(!netif || netif=="apsta" || netif=="sta") {
        let config = beapi.wifi.getConfig(MODE_STA)
        if(config.ssid) {
            json.sta.ssid = config.ssid
            json.sta.password = config.password
            changed = true
        }
    }

    // for ap
    if(!netif || netif=="apsta" || netif=="ap") {
        let config = beapi.wifi.getConfig(MODE_AP)
        if(config.ssid) {
            json.ap.ssid = config.ssid
            json.ap.password = config.password
            changed = true
        }
    }

    if(changed) {
        return !! beapi.fs.writeFileSync(ConfigPath, JSON.stringify(json,null,4))
    }
    else {
        return false
    }
}
function connectToAP() {
    let status = wifi.status("sta")
    if(status.connected) {
        return
    }
    let json = readWiFiConfig()
    if(!json.sta.ssid) {
        return
    }

    console.log("connect to ssid:", json.sta.ssid, json.sta.password)
    wifi.connect(json.sta.ssid, json.sta.password||"", function(err){
        if(err) {
            console.error("connect wifi error:", err)
        }
    })
}

wifi.restore = async function() {

    return
    await sleep(1000)
    await wifi.stop()
    await sleep(1000)
    await wifi.start()

    console.log("restore WiFi AP/STA ...")

    if(!beapi.wifi.staStarted()) {
        console.log("wifi.stdStarted() == false")
        return
    }

    let json = readWiFiConfig()
    let status = wifi.status()

    // sta
    if(!status.sta.connected) {
        setTimeout(connectToAP,1000)
        if(json.sta.watch_dur>0) {
            setInterval(connectToAP, json.sta.watch_dur)
        }
    }
    else {
        console.log("wifi status", status)
    }

    // ap
    let ssid = json.ap.ssid.replace("${UUID}", beapi.utils.partUUID().substr(-4))
    if( !status.ap.started || status.ap.ssid!=ssid ){
        beapi.wifi.startAP(ssid, json.ap.password)
    }
}


beapi.wifi.registerEventHandle(function(eventType, eventId, data){
    let eventName = null
    let eventArgv = []

    if(eventType==EVENT_WIFI) {
        if(evtNames[eventId])
            eventName = evtNames[eventId]
        if(eventId==WIFI_STA_DISCONNECTED) {
            eventArgv.push( data )  // disconnect reason
        }
    }
    else if(eventType==EVENT_IP) {
        if(ipEvtNames[eventId])
            eventName = ipEvtNames[eventId]
    }

    if(eventName) {
        wifi.emit(eventName, ...eventArgv)
    }

    if(eventType==EVENT_WIFI) {
        if(eventId==WIFI_STA_START || eventId==WIFI_AP_START) {
            if(contrastStatus(true)) {
                wifi.emit("start")
            }
        }
        else if(eventId==WIFI_STA_STOP || eventId==WIFI_AP_STOP) {
            if(contrastStatus(false)) {
                wifi.emit("stop")
            }
        }
    }
})


// 禁止 wifi 睡眠 (增加耗电)
// beapi.wifi.setPS(PS_NONE)
