const lv = require('lv')
const {BeMotor, BeServo} = require('./parts/BeMotor')
const {BeScreen130, BeScreen096} = require('./parts/BeScreen')

const CardHeight = 30
const CardPad =5

class PartCard extends lv.Label {
    constructor(parent){
        super(parent)
        this.fromJson({
            width: 120 ,
            height: CardHeight ,
            font: "msyh" ,
            style: {
                "bg-opa": 255 ,
                "pad-left": 5 ,
                // "text-align": "center"
            } ,
            flag: ["clickable"] ,
        })
        this.draggable(
            null, pos=>{
                pos.y = false
                if(pos.x>70) {
                    return false
                }
            } ,
            (cusStop)=>{
                this.setX(0)
                if(cusStop) {
                    this.emit("new-part")
                }
            }
        )
    }
}

class PartLib extends lv.CleanObj{
    constructor(workspace) {
        super(workspace)
        this.setWidth(120)

        this.fromJson({
            width: 200 ,
            height: "100%" ,
            x: 20, y: 0 , 
            autoHide: true ,
            style: {
                // "border-width": 1 ,
                // "pad-row": 5,
            } ,
        })
        this.clearFlag("clickable")
        this.setScrollDir("ver")

        this.addCart(BeMotor)
        this.addCart(BeServo)
        this.addCart(BeScreen130)
        this.addCart(BeScreen096)
        // this.addCart("LED")
        // this.addCart("RGB LED")
        // this.addCart("姿态传感器")
        // this.addCart("颜色传感器")
        // this.addCart("气压计")
        // this.addCart("红外线收发器")
        // this.addCart("超声波")
        // this.addCart("扬声器")
        // this.addCart("麦克风")
        // this.addCart("温度传感器")
        // this.addCart("湿度传感器")
    }

    cards = []

    addCart(partClass) {
        let card = new PartCard(this)
        card.setText(partClass.config.title)

        let y = (CardHeight + CardPad) * this.cards.length
        card.setY(y)

        card.on("new-part", ()=>{
            this.hide()
            let part = this.parent().model.createPart(partClass)
            part.repaint(this.parent().zoomer.value)

            this.parent().graph.setActivePart(part)

            setTimeout(()=>{

                let pos = lv.inputPoint()
                part.setCoords(pos.x-part.width()/2, pos.y-part.height()/2)
                part.updateLayout()
    
                try{
                    lv.fakeIndev(pos.x, pos.y, false)
                    lv.fakeIndev(pos.x, pos.y, true)
                }catch(e){
                    console.log(e)
                    console.log(e.stack)
                }

            }, 0)
        })

        this.cards.push(card)
    }
}

module.exports = PartLib