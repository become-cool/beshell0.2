
const Wifi = require("Wifi")

const pinR = 5
const pinG = 34
const pinB = 35

// 关闭内置led
digitalWrite(2, 1)
digitalWrite(16, 1)


// 根据连接状态显示内置led
let timer = setInterval(()=>{

    if(!global.be.wifi.blink)
        return

    switch(Wifi.getStatus().station) {
        case 'connected' :
            digitalWrite(2, 0)                         // 打开led
            clearInterval(timer)
            return
        case 'connecting' :
            digitalWrite(2, digitalRead(2)? 0: 1)     // 闪烁
            return
        case 'no_ap_found' :
        case 'bad_password' :
        case 'connect_failed' :
            digitalWrite(2, 1)              
            clearInterval(timer)
            return
    }
},300)