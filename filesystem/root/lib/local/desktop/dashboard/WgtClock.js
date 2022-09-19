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

        let screenWidth = this.screen().width()
        
        this.fromJson([
            {
                class: "Column" ,
                width: -1 ,
                height: -1 ,
                style: {
                    "pad-left": 8
                } ,
                children: [
                    {
                        class: "Label" ,
                        ref: "labTime" ,
                        font: screenWidth<320? "m32": "m36" ,
                        text: "00:00:00" ,
                    } ,
                    {
                        class: "Label" ,
                        ref: "labDate" ,
                        text: "0000/00/00"
                    }

                ] ,
            } ,
        ],this)

        setInterval(()=>{
            this.freshTime()
        }, 1000)

        this.freshTime()
    }

    freshTime() {
        let tm = new Date(Date.now())
        this.labTime.setText( fill2(tm.getHours()) + ':' + fill2(tm.getMinutes()) + ":" + fill2(tm.getSeconds()) )
        this.labDate.setText( fill2(tm.getFullYear()) + '/' + fill2(tm.getMonth()+1) + '/' + fill2(tm.getDate()) )
    }
}
module.exports = WgtClock