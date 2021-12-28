const lv = require('lv')

const HandleSize = 20
const ZoomSnap = 0.15

class Zoomer extends lv.CleanObj{

    value = 2

    constructor(parent) {
        super(parent)

        this.refs = this.fromJson({
            width: 20 ,
            children: [
                {
                    class: lv.CleanObj ,
                    ref: "track" ,
                    width: 6 ,
                    height: "100%" ,
                    center: true ,
                    style: {
                        radius: 3 ,
                        "border-width": 1 ,
                        "border-color": lv.palette("grey") ,
                        "border-opa": 32 ,
                    }
                } ,
                {
                    class: lv.CleanObj ,
                    ref: "handle" ,
                    width:HandleSize ,
                    height:HandleSize ,
                    style: {
                        radius: HandleSize/2 ,
                        "border-width": 1 ,
                        "border-color": lv.palette("grey") ,
                    } ,
                    // text: lv.symbol.search ,
                    // font: "s16" ,
                } ,
            ]
        })

        this.refs.handle.setY( this.height() - this.refs.handle.height() )

        this.refs.handle.draggable(
            ()=>{

            }
            , pos=>{
                pos.x = false
                let fullvalue = this.height()-HandleSize
                let [,py] = this.coords()
                if( pos.y<py ) {
                    pos.y = py
                }
                else {
                    let maxY = py+fullvalue
                    if(pos.y>maxY) {
                        pos.y = maxY
                    }
                }

                let y = pos.y - py
                this.value = ((fullvalue-y)/fullvalue) * 3 + 1
                // console.log(this.value)

                let snap = false
                if(this.value>(2-ZoomSnap) && this.value<(2+ZoomSnap)) {
                    this.value = 2
                    snap = true
                }
                else if(this.value>(3-ZoomSnap) && this.value<(3+ZoomSnap)) {
                    this.value = 3
                    snap = true
                }
                if(snap) {
                    pos.y = fullvalue - Math.round(((this.value-1)/3) * fullvalue) + py
                }

                this.emit("value-changed", this.value)
            }
        )
    }
    
    setZoom(value) {
        this.value = value
        let fullvalue = this.height()-this.refs.handle.height()
        let y = fullvalue - Math.round(((this.value-1)/3) * fullvalue)
        this.refs.handle.setY(y)
        this.emit("value-changed", this.value)
    }
}

module.exports = Zoomer