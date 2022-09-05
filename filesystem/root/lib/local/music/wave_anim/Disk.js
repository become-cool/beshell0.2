const lv = require('lv')

module.exports = class Disk extends lv.CleanObj {
    constructor(parent) {
        super(parent)
        this.fromJson({
            grow: true,
            children: [
                {
                    class: "Img" ,
                    ref:"photo" ,
                    align: "center" ,
                    src: __dirname + "/img/disk.png"
                }
            ]
        }, this)
    }

    play() {
        this.stop()

        this.photo.setAngle(0)
        let angle = 0
        this._timer = setInterval(()=>{
            angle+=120
            if(angle>=3600) angle = 0
            this.photo.setAngle(angle)
        },100)
    }

    stop(){
        if(this._timer!=undefined) {
            clearTimeout(this._timer)
            this._timer = undefined
        }
    }
}