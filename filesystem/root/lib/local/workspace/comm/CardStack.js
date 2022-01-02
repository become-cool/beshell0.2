const lv = require('lv')

const CardHeight = 30
const CardPad =5

class Card extends lv.Label {
    constructor(parent){
        super(parent)
        this.fromJson({
            width: 120 ,
            height: CardHeight ,
            font: "msyh" ,
            style: {
                "bg-opa": 180 ,
                "pad-left": 5 ,
                "bg-color": lv.rgb(220,220,220) ,
                "text-color": lv.rgb(120,120,120) ,
            } ,
            flag: ["clickable"] ,
        })
    }
}

class CardStack extends lv.CleanObj{
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
            flag: ["scrollable"] ,
        })
        this.clearFlag("clickable")
        this.setScrollDir("ver")
        this.hide()
    }

    cards = []

    contentHeight = 0

    createCard(title, cb, cbData) {
        let card = new Card(this)
        card.setText(title)
        this.addCard(card, cb, cbData)
    }

    addCard(card, cb, cbData) {

        card.setY(this.contentHeight)
        this.contentHeight+= card.height() + CardPad

        let draggable = card.draggable()
        draggable.setDragging((pos)=>{
            pos.y = false
            if(pos.x>70) {
                return false
            }
        })
        draggable.setStop((cusStop)=>{
            card.setX(0)
            if(cusStop) {
                card.emit("new-part")
            }
        })

        card.on("new-part", ()=>{
            this.hide()
            let obj = cb && cb(cbData)
            if(!obj) {
                return
            }

            setTimeout(()=>{

                let pos = lv.inputPoint()
                obj.setCoords(pos.x-obj.width()/2, pos.y-obj.height()/2)
                obj.updateLayout()
    
                try{
                    lv.fakeIndev(pos.x, pos.y, false)
                    lv.tickIndev()
                    lv.fakeIndev(pos.x, pos.y, true)
                    lv.tickIndev()
                }catch(e){
                    console.log(e)
                    console.log(e.stack)
                }

            }, 0)
        })

        this.cards.push(card)
    }
}

module.exports = CardStack