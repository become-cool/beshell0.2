const lv = require("lv")

let styleBg = new lv.Style({
    "border-color": lv.palette("grey") ,
    "border-width": 1 ,
    "radius": 6 ,
    "pad": 3
})

let styleIndic = new lv.Style({
    "radius": 3 ,
    "bg-opa": 255 ,
    "bg-color": lv.palette("grey") ,
})


class StatusBar extends lv.Row {
    constructor(parent, title) {
        super(parent)

        this.refs = this.fromJson({
            width: -1 ,
            height: -1 ,
            style: {
                "pad-bottom": 5 ,
                "pad-left": 2 ,
                "pad-right": 2 ,
            } ,
            children: [
                {
                    class: 'Label' ,
                    ref: 'title' ,
                    text: title ,
                    font: 'm10'
                } ,

                {
                    class: 'Bar' ,
                    ref: 'bar' ,
                    clear: true ,
                    width: 45 ,
                    value: 50 ,
                }
            ]
        })

        this.refs.bar.addStyle(styleBg)
        this.refs.bar.addStyle(styleIndic, lv.part.indicator)
    }

    setValue(v) {
        this.refs.bar.setValue(v)
    }
}

class SysStatus extends lv.Row {
    constructor(parent) {
        super(parent)

        this.fromJson({
            width: "100%" ,
            height: -1 ,
            style: {
                "flex-main-place": "space-between"
            }
        })

        this.cpu0= new StatusBar(this, "CPU0 ")
        this.cpu1= new StatusBar(this, "CPU1 ")
        if(this.screen().width()>=320) {
            this.flash= new StatusBar(this, "FLASH ")
        }
        this.mem= new StatusBar(this, "MEM ")

        if(!process.simulate) {
            this.refreshStatus()
            setInterval(()=>this.refreshStatus(), 1000)
        }
    }

    refreshStatus() {
        try{
            this.cpu0.setValue( process.cpuUsage(0) )
            this.cpu1.setValue( process.cpuUsage(1) )

            let mem = process.memoryUsage()
            let total = mem.heap_total + mem.psram_total
            let used = mem.heap_used + mem.psram_used
            if(total) {
                this.mem.setValue(Math.round(used*100 / total)) ;
            }

            let {total:fstotal, used:fsused} = beapi.fs.info("/")
            let {total:fshometotal, used:fshomeused} = beapi.fs.info("/home")
            this.flash?.setValue(Math.round((fsused+fshomeused)*100 / (fshometotal+fstotal))) ;

        }catch(e) {
            console.log(e)
        }
    }
}
module.exports = SysStatus