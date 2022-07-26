// wifi mode
// const MODE_NONE = 0
const MODE_STA = 1
const MODE_AP = 2
// const MODE_APSTA = 3

// event type
EVENT_WIFI = 1
EVENT_IP = 2 

// WiFi events
// const WIFI_READY = 0
const WIFI_SCAN_DONE = 1
const WIFI_STA_START = 2
const WIFI_STA_STOP = 3
const WIFI_STA_CONNECTED = 4
const WIFI_STA_DISCONNECTED = 5
// const WIFI_STA_AUTHMODE_CHANGE = 6
// const WIFI_STA_WPS_ER_SUCCESS = 7
// const WIFI_STA_WPS_ER_FAILED = 8
// const WIFI_STA_WPS_ER_TIMEOUT = 9
// const WIFI_STA_WPS_ER_PIN = 10
// const WIFI_STA_WPS_ER_PBC_OVERLAP = 11
const WIFI_AP_START = 12
const WIFI_AP_STOP = 13
const WIFI_AP_STACONNECTED = 14
const WIFI_AP_STADISCONNECTED = 15
// const WIFI_AP_PROBEREQRECVED = 16
// const WIFI_FTM_REPORT = 17
// const WIFI_STA_BSS_RSSI_LOW = 18
// const WIFI_ACTION_TX_STATUS = 19
// const WIFI_ROC_DONE = 21
// const WIFI_STA_BEACON_TIMEOUT = 21

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
// const AP_STAIPASSIGNED = 2
// const GOT_IP6 = 3
// const ETH_GOT_IP = 4
// const PPP_GOT_IP = 5
// const PPP_LOST_IP = 6

let ipEvtNames = []
ipEvtNames[STA_GOT_IP] = "ip.got"
ipEvtNames[STA_LOST_IP] = "ip.lost"


// power safe
const PS_NONE = 0
// const PS_MIN_MODEM = 1
// const PS_MAX_MODEM = 2 


const wifi = module.exports = new beapi.EventEmitter()
wifi.start = beapi.wifi.start
wifi.stop = beapi.wifi.stop
wifi.mode = beapi.wifi.getMode
wifi.setMode = beapi.wifi.setMode

wifi.staStarted = beapi.wifi.staStarted
wifi.apStarted = beapi.wifi.apStarted

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
wifi.start = function(callback) {
    beapi.wifi.start()
    if(callback) {
        if(wifi.isReady()) {
            callback()
        }
        else {
            wifi.once("start", callback)
        }
    }
}
wifi.stop = function(callback) {
    beapi.wifi.stop()
    if(callback) {
        if(contrastStatus(false)) {
            callback()
        }
        else {
            wifi.once("stop", callback)
        }
    }
}

let _connecting = false
wifi.isConnecting = function() {
    return _connecting
}
wifi.connect = function(ssid,password,callback) {
    if(_connecting) {
        throw new Error("wifi STA is connecting")
    }
    _connecting = true
    let mode = wifi.mode()
    wifi.setMode(mode|MODE_STA)

    wifi.start(()=>{
        wifi.disconnect(()=>{
            wifi.race(["sta.disconnected","sta.connected"], (evt, ...args)=>{
                _connecting = false
                callback && callback(evt=="sta.connected", ...args)
            })
            beapi.wifi.setStaConfig({ssid, password})
            beapi.wifi.connect()
        })
    })
}

wifi.disconnect = function(callback) {
    if(!beapi.wifi.staConnected()) {
        callback && callback(true)
        return
    }
    _connecting = true
    wifi.race(["sta.disconnected","sta.connected"], evt=>{
        _connecting = false
        callback && callback(evt=="sta.disconnected")
    })
    beapi.wifi.disconnect() ;
}

wifi.startAP = function(ssid, password, callback) {
    wifi.stopAP(ret=>{
        if(!ret) {
            callback(false)
            return
        }
        if(typeof callback=="function") {        
            wifi.race(["ap.start","ap.stop"], evt=>callback(evt=="ap.start"))
        }
        wifi.setMode( wifi.mode() | MODE_AP )
        beapi.wifi.setAPConfig({ssid, password})
        wifi.start()
    })
}
wifi.stopAP = function(callback) {
    if(!wifi.apStarted()) {
        callback && callback(true)
        return
    }
    if(typeof callback=="function") {        
        wifi.race(["ap.start","ap.stop"], evt=>callback(evt=="ap.stop"))
    }
    wifi.setMode( wifi.mode() & (~MODE_AP) )
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

wifi.autostart = function() {
    wifi.start(()=>{
        console.log("wifi auto start")
        if(wifi.mode()&MODE_STA) {
            beapi.wifi.connect()
        }
    })
}


beapi.wifi.registerEventHandle(function(eventType, eventId, data){
    let eventName = null
    let eventArgv = []

    if(eventType==EVENT_WIFI) {
        eventName = evtNames[eventId]
        if(eventId==WIFI_STA_DISCONNECTED) {
            eventArgv.push( data )  // disconnect reason
        }
    }
    else if(eventType==EVENT_IP) {
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
beapi.wifi.setPS(PS_NONE)