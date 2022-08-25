const lv = require("lv")
const wifi = require("besdk/wifi")

class WiFiStatus extends lv.Column {

    constructor(parent) {
        super(parent)
        
        this.fromJson([
            {
                class: "Label" ,
                font: "m12" ,
                text: lv.symbol.battery_3 + "  " ,
                width: "100%",
                style: {
                    "text-align": "right"
                }

            } ,
            {
                class: "Label" ,
                font: "m12" ,
                text: lv.symbol.wifi + " STA" ,
                style: {
                    "pad-top": 20
                }
            } ,
            {
                class: "Label" ,
                font: "m10" ,
                text: "not connected" ,
                ref: "staSSID"
            } ,
            {
                class: "Label" ,
                font: "m10" ,
                text: "0.0.0.0" ,
                ref: "staIP"
            } ,

            {
                class: "Label" ,
                font: "m12" ,
                text: lv.symbol.wifi + " AP" ,
                style: {
                    "pad-top": 10
                }
            } ,
            {
                class: "Label" ,
                font: "m10" ,
                text: "disable" ,
                ref: "apSSID"
            } ,
            {
                class: "Label" ,
                font: "m10" ,
                text: "0.0.0.0" ,
                ref: "apIP"
            } ,
        ], this)

        let staConf = beapi.wifi.getConfig(1)
        this.staSSID.setText(staConf.ssid||"not connected")
        let staIp = beapi.wifi.getIpInfo(1)
        this.staIP.setText(staIp.ip)

        wifi.on("sta.connecting",(e)=>{
            let staConf = beapi.wifi.getConfig(1)
            this.staSSID.setText("connecting to "+(staConf.ssid||"unknow ssid")+" ...")
        })
        wifi.on("sta.connected",(e)=>{
            let staConf = beapi.wifi.getConfig(1)
            this.staSSID.setText(staConf.ssid||"not connected")
        })
        wifi.on("sta.disconnected",(e)=>{
            this.staSSID.setText("not connected")
            this.staIP.setText("0.0.0.0")
        })
        wifi.on("ip.got",(e)=>{
            let staIp = beapi.wifi.getIpInfo(1)
            this.staIP.setText(staIp.ip)
        })
        wifi.on("ip.lost",(e)=>{
            this.staIP.setText("0.0.0.0")
        })
        
        let updateAP = () => {
            let apConf = beapi.wifi.getConfig(2)
            this.apSSID.setText(apConf.ssid||"disable")
            let apIp = beapi.wifi.getIpInfo(2)
            this.apIP.setText(apIp.ip)
        }
        updateAP()
        wifi.on("ap.start",updateAP)
        wifi.on("ap.stop",()=>{
            this.apSSID.setText("disable")
            this.apIP.setText("0.0.0.0")
        })
    }
}
module.exports = WiFiStatus