const lv = require("lv")
function fill2(num) {
    if(num<10) {
        return '0'+num
    }
    else {
        return num.toString()
    }
}

class HoloClock extends lv.Obj {
    constructor() {
        super()
        this.fromJson({
            style: {
                "bg-color": lv.rgb(0,0,0) ,
            } ,
            children: [{
                class: 'Column',
                width:"100%",
                height:"100%",
                children: [{
                    class:"Row" ,
                    width: "100%" ,
                    height: 100 ,
                    children: [
                        {
                            class: 'Label' ,
                            text: '00:00:00' ,
                            ref: 'labTime' ,
                            font: "m40" ,
                            style: {
                                'text-color': lv.rgb(255,255,255) ,
                                "pad-left": 20 ,
                                "pad-top": 30 ,
                            }
                        }
                    ]
                } ,
                {
                    class:"Row" ,
                    width: "100%" ,
                    grow: true ,
                    children: [
                        {
                            class: "Img" ,
                            src: "/lib/local/holoclock/cloudy.png",
                            width: 160 ,
                        } , 
                        {
                            class: 'Column' ,
                            height:"100%",
                            style: {
                                "pad-row": 10 ,
                            } ,
                            children: [
                                { clear: true, grow:true} ,
                                {
                                    class: 'Label' ,
                                    text: '苏州' ,
                                    font: 'msyh' ,
                                    ref: 'labCity' ,
                                    width: 80 ,
                                    style: {
                                        'text-color': lv.rgb(255,255,255) ,
                                        "text-align": "right" ,
                                        "pad-right": 20
                                    } ,
                                } ,
                                {
                                    class: 'Label' ,
                                    text: '34°C' ,
                                    ref: 'labTemp' ,
                                    width: 80 ,
                                    style: {
                                        'text-color': lv.rgb(255,255,255) ,
                                        "text-align": "right",
                                        "pad-right": 20
                                    } ,
                                } ,
                                { clear: true, height: 50}
                            ]
                        }
                    ]
                }]
            }]
        }, this)

        setInterval(()=>{
            this.freshTime()
        }, 1000)

        this.freshTime()

        global.clock = this
    }

    
    freshTime() {
        let tm = new Date(Date.now())
        this.labTime.setText( fill2(tm.getHours()) + ':' + fill2(tm.getMinutes()) + ":" + fill2(tm.getSeconds()) )
    }
}

function main() {
    const disp = be.dev.disp[0]
    if(!disp) {
        console.log("no disp device, exit HoloClock")
        return
    }
    let desktop = new HoloClock()
    global.desktop = desktop

    lv.loadScreen(desktop)
}
main()
