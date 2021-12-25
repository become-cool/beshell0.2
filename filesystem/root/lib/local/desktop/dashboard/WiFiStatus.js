const lv = require("lv")

function fill2(num) {
    if(num<10) {
        return '0'+num
    }
    else {
        return num.toString()
    }
}


class WiFiStatus extends lv.Column {

    constructor(parent) {
        super(parent)
        
        this.refs = this.fromJson([
            
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
                ref: "labwifi" ,
                style: {
                    "pad-top": 20
                }
            } ,
            {
                class: "Label" ,
                font: "m10" ,
                text: "alee's home" ,
            } ,
            {
                class: "Label" ,
                font: "m10" ,
                text: "192.168.0.107" ,
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
                text: "Become3h40" ,
            } ,
            {
                class: "Label" ,
                font: "m10" ,
                text: "192.168.4.1" ,
            } ,
        ])
    }
}
module.exports = WiFiStatus