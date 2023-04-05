const lv = require('lv')
const WgtClock = require('./WgtClock')
const WiFiStatus = require('./WiFiStatus')
const SysStatus = require('./SysStatus')
const wifi = require("besdk/wifi")
const weather = require("./weather")

class Dashboard extends lv.Column {
    active = false
    constructor(parent,desktop) {
        super(parent)

        this.fromJson({
            width: "100%" ,
            height: "100%" ,
            style: {
                "bg-opa": 255 ,
                "bg-color": lv.rgb(235)
            } ,
            children: [ {
                class: 'Row' ,
                grow: 1 ,
                ref: "row1" ,
                width: "100%" ,
                children: [
                    {
                        class: 'Column' ,
                        grow: 1 ,
                        height: "100%" ,
                        children: [
                            {
                                class: WgtClock,
                                width: "100%" ,
                                ref: "clock" ,
                                style: {
                                    "pad-top": 40
                                }
                            } ,
                            {
                                class: 'CleanObj',
                                grow: 1 ,
                            } ,
                            {
                                class: "Label" ,
                                width: "100%" ,
                                height: -1 ,
                                text: "" ,
                                // text: "苏州 13C" ,
                                font: "msyh" ,
                                ref: "row1" ,
                                style: {
                                    "pad-left": 8 ,
                                } ,
                                ref: "weather1"
                            } ,
                            {
                                class: "Label" ,
                                width: "100%" ,
                                height: -1 ,
                                text: "" ,
                                // text: "多云 湿度:77%" ,
                                font: "msyh" ,
                                ref: "row1" ,
                                style: {
                                    "pad-left": 8 ,
                                } ,
                                ref: "weather2"
                            } ,
                            {
                                class: "Label" ,
                                width: "100%" ,
                                height: -1 ,
                                text: "" ,
                                // text: "东北风 / 风速10 / 风力2" ,
                                font: "msyh" ,
                                ref: "row1" ,
                                style: {
                                    "pad-left": 8 ,
                                    "pad-bottom": 20 ,
                                } ,
                                ref: "weather3"
                            } ,
                        ]
                    } ,

                    {
                        class: "Obj" , 
                        height: "100%" ,
                        width: 20 ,
                        clear: true ,
                        children: [
                            {
                                class: "Line" , 
                                ref: "line" ,
                                height: "80%" ,
                                center: true ,
                                style: {
                                    "border-width": 1 ,
                                    "border-color": lv.palette("grey") ,
                                }
                            } ,
                        ]
                    } ,
                    {
                        class: WiFiStatus,
                        ref: "wifistatus" ,
                        height: "100%" ,
                        width: 80 ,
                    } ,
                ]
            } ,
            {
                class: SysStatus ,
                ref: "row2" ,
            }]
        },this)

        // this.updateWeather( weather.loadCache() )
        // wifi.on("ip.got", async ()=>{
        //     console.log("ip.got")
        //     this.updateWeather( await weather.query() )
        // })
        // // 每 3 小时更新一次
        // setInterval(async ()=>{
        //     this.updateWeather( await weather.query() )
        // }, 3*60*60*1000) 
    }
    wakeup() {
        this.active = true
    }
    sleep() {
        this.active = false
    }

    // updateWeather(info) {
    //     if(!info) {
    //         return
    //     }
    //     this.weather1.setText(`${info.localText} ${info.payload.temp}°C ${info.payload.text}`)
    //     this.weather2.setText(`湿度:${info.payload.humidity}%`)
    //     this.weather3.setText(`${info.payload.windDir} / 风力${info.payload.windScale} / 风速${info.payload.windSpeed}`)
    // }
}
module.exports = Dashboard
