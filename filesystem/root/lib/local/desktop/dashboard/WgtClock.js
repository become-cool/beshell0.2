const lv = require("lv")

function fill2(num) {
    if(num<10) {
        return '0'+num
    }
    else {
        return num.toString()
    }
}


class WgtClock extends lv.Obj {

    constructor(parent) {
        super(parent)
        this.removeStyleAll()
        
        this.refs = this.fromJson([
            {
                class: "Column" ,
                width: 180 ,
                height: -1 ,
                center: true ,
                children: [
                    {
                        class: "Label" ,
                        ref: "labTime" ,
                        font: "m40" ,
                        text: "00:00:00" ,
                    } ,
                    {
                        class: "Label" ,
                        ref: "labDate" ,
                        text: "0000/00/00"
                    }

                ] ,
            } ,
        ])

        setInterval(()=>{
            this.freshTime()
        }, 1000)

        this.freshTime()

    }

    freshTime() {
        let tm = new Date(Date.now() + 8*3600000)
        this.refs.labTime.setText( fill2(tm.getHours()) + ':' + fill2(tm.getMinutes()) + ":" + fill2(tm.getSeconds()) )
        this.refs.labDate.setText( fill2(tm.getFullYear()) + '/' + fill2(tm.getMonth()+1) + '/' + fill2(tm.getDate()) )
    }
}
module.exports = WgtClock