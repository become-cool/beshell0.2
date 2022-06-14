const lv = require('lv')
const WgtClock = require('./WgtClock')
const WiFiStatus = require('./WiFiStatus')
const SysStatus = require('./SysStatus')

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
                        class: WgtClock,
                        grow: 1 ,
                        ref: "clock"
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
                        style: {
                        } ,
                    } ,
                ]
            } ,
            {
                class: SysStatus ,
                ref: "row2" ,
            }]
        },this)
        
        for(let name in this.refs) {
            global[name] = this.refs[name]
        }
    }
    wakeup() {
        this.active = true
    }
    sleep() {
        this.active = false
    }
}
module.exports = Dashboard
