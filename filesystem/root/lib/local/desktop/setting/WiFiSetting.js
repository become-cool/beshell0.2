const lv = require("lv")
const wifi = require("besdk/wifi")

function createSSIDPWD(refs, refSSID, refPWD, refStatus, ...btns) {
    let json = [
        {
            class: "Row" ,
            gap: 5 ,
            crossAlign: "center" ,
            children: [
                {
                    class: "Label" ,
                    text: "SSID" ,
                    width: 45 ,
                } ,
                {
                    class: "TextArea" ,
                    oneLine: true ,
                    grow: true ,
                    ref: refSSID ,
                    useKeyboard: true ,
                }
            ]
        } ,
        {
            class: "Row" ,
            gap: 5 ,
            crossAlign: "center" ,
            children: [
                {
                    class: "Label" ,
                    text: "密码" ,
                    width: 45 ,
                    font: "msyh"
                } ,
                {
                    class: "TextArea" ,
                    oneLine: true ,
                    passwordMode: true ,
                    grow: true ,
                    ref: refPWD ,
                    useKeyboard: true ,
                    passwordShowTime: 0 ,
                }
            ]
        } ,

        {
            class: "Row" ,
            mainAlign: "end" ,
            gap: 5 ,
            crossAlign: "center" ,
            children: [
                ... btns ,
                {
                    class: 'Btn' ,
                    text: beapi.lvgl.symbol.eye_open ,
                    clicked() {
                        if(refs[refPWD].passwordMode()) {
                            refs[refPWD].setPasswordMode(false)
                            this.setText(beapi.lvgl.symbol.eye_close)
                        }
                        else {
                            refs[refPWD].setPasswordMode(true)
                            this.setText(beapi.lvgl.symbol.eye_open)
                        }
                    }
                }
            ]
        } ,
    ]
    if(refStatus) {
        json.push({
            class: "Label" ,
            font: 'msyh' ,
            ref: refStatus ,
            text: ''
        })
    }
    return json
}

function createIPRow(title, refName) {
    return {
        class: 'Row', gap: 10,
        children: [
            { class: "Label", text: title, font: 'msyh', width: 45} ,
            { class: "Label", text: "0.0.0.0", ref: refName, grow: true, textAlign: 'right'} ,
        ]
    }
}

class WiFiSetting extends lv.TabView {

    constructor(parent) {
        super(parent, "top", 30)
        
        this.fromJson({
            anim: false ,
            items: {
                "STA": {
                    class: "Column" ,
                    gap: 5 ,
                    children: [

                        ... createSSIDPWD(this,'staSSID','staPassword', 'staStatus'
                            , {
                                class: 'Label' ,
                                text: 'connect...' ,
                                font: "m10" ,
                                ref: 'labConnecting' ,
                                visible: false ,
                                grow: true
                            }
                            , {
                                class: 'Btn' ,
                                text: "连接" ,
                                font: 'msyh' ,
                                ref: 'btnConnect' ,
                                clicked: () => {
                                    let ssid = this.staSSID.text()
                                    let password = this.staPassword.text()
                                    wifi.connect(ssid, password)
                                } , 
                                children: [{
                                    class: 'Spinner',
                                    width: 10 ,
                                    height: 10 ,
                                    ref: 'staSpinner' ,
                                    visible: false ,
                                    align: 'center'
                                }]
                            }
                            , {
                                class: 'Btn' ,
                                text: "断开" ,
                                font: 'msyh' ,
                                ref: 'btnDisconnect' ,
                                clicked: () => {                                    
                                    wifi.disconnect()
                                }
                            }
                            , {
                                class: 'Btn' ,
                                text: "扫描" ,
                                font: 'msyh'
                            }
                        ) ,
                        createIPRow('IP','staIP') ,
                        createIPRow('网关','staGateway') ,
                        createIPRow('掩码','staNetmask') ,
                    ]
                }, 
                "AP": {
                    class: "Column" ,
                    gap: 5 ,
                    children: [
                        ... createSSIDPWD(this,'apSSID','apPassword', null
                            , {
                                class: 'Btn' ,
                                text: "应用" ,
                                font: 'msyh' ,
                                clicked: () => {
                                    let ssid = this.apSSID.text()
                                    let password = this.apPassword.text()
                                    wifi.startAP(ssid, password)
                                    beapi.lvgl.msg.info("AP设置已经保存并应用")
                                }
                            }
                        ) ,
                        // createIPRow('IP','apIP') ,
                        // createIPRow('网关','apGateway') ,
                        // createIPRow('掩码','apNetmask') ,
                    ]
                }
            }
        }, this)

        this.btnConnect.setWidth(this.btnConnect.width())
        this.btnConnect.setHeight(this.btnConnect.height())

        wifi.on("sta.connecting",()=>{
            this.staStatus.setText("正在连接到热点 ...")

            this.btnConnect.label.hide()
            this.staSpinner?.show()

            this.btnConnect.show()
            this.btnDisconnect.hide()
        })
        wifi.on("sta.connected",()=>{
            this.staStatus.setText("已连接")

            this.staSpinner?.hide()
            this.btnConnect.label.show()

            this.btnConnect.hide()
            this.btnDisconnect.show()
        })
        wifi.on("sta.disconnected",()=>{
            this.staStatus.setText("未连接")

            this.staSpinner?.hide()
            this.btnConnect.label.show()

            this.btnConnect.show()
            this.btnDisconnect.hide()

            this.staIP.setText('0.0.0.0')
            this.staGateway.setText('0.0.0.0')
            this.staNetmask.setText('0.0.0.0')
        })

        wifi.on("ip.got",()=>{
            let ip = beapi.wifi.getIpInfo(1)
            this.staIP.setText(ip.ip)
            this.staGateway.setText(ip.gw)
            this.staNetmask.setText(ip.netmask)
        })
        wifi.on("ip.lost",()=>{
            this.staIP.setText('0.0.0.0')
            this.staGateway.setText('0.0.0.0')
            this.staNetmask.setText('0.0.0.0')
        })

        this.onActive()
    }


    onActive() {
        this.updateSta()
        this.updateAp()
    }

    updateSta() {
        let sta = wifi.status("sta")
        console.log(sta)
        this.staSSID.setText(sta.ssid)
        this.staPassword.setText(sta.password)

        this.btnConnect.setVisible(!sta.connected)
        this.btnDisconnect.setVisible(sta.connected)
        
        if(wifi.isConnecting()) {
            this.staStatus.setText('正在连接到热点...')
        }
        else {
            this.staStatus.setText(sta.connected?'已连接':'未连接')
        }
        
        this.staIP.setText(sta.ip)
        this.staGateway.setText(sta.gw)
        this.staNetmask.setText(sta.netmask)
    }
    
    updateAp() {
        let ap = wifi.status("ap")
        this.apSSID.setText(ap.ssid)
        this.apPassword.setText(ap.password)
    }
}
module.exports = WiFiSetting