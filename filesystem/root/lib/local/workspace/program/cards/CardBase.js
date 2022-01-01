const lv = require('lv')

const PortSnapDis = 10*10
const BaseWidth = 100
const BaseHeight = 22

const styleCardBody = new lv.Style({
    "border-width": 1 ,
    "border-color": lv.palette("orange") ,
    "border-opa": 180 ,
    "bg-color": lv.palette("orange") ,
    "bg-opa": 255 ,
    "radius": 2 ,
})

const bodyDsc = new lv.DrawRectDsc()
bodyDsc.setBgColor(lv.palette("purple"))
bodyDsc.setBgOpa(255)
bodyDsc.setRadius(2)


class Input extends lv.TextArea {
    constructor(parent) {
        super(parent)
        this.fromJson({
            width: -1 ,
            height: -1 ,
            center: true ,
            oneLine: true ,
            style: {
                radius: lv.RADIUS_CIRCLE ,
                "pad-top": 2 ,
                "pad-bottom": 2 ,
                "pad-left": 8 ,
                "pad-right": 8 ,
            } ,
            clicked() {
                lv.keyboard().popup(this)
            } ,
        })
        let txt = this.child(0).as(lv.Label)
        txt.setFont("m10")
        txt.setWidth(-1)
        txt.setStyle("min-width",5)
    }
}

class CardSlotCompare extends lv.CleanObj{
    constructor(card) {
        super(card)
        this.fromJson({
            width: 30 ,
            height: 16 ,
            style: {
                radius: lv.RADIUS_CIRCLE ,
                "border-width": 1 ,
                "border-color": lv.palette("grey") ,
                "border-opa": 180 ,
                "bg-color": lv.palette("grey") ,
                "bg-opa": 60 ,
            }
        })
    }
}


class CardSlotExpression extends lv.CleanObj{

    expr = null
    input = null

    constructor(card, noInput) {
        super(card)
        this.fromJson({
            width: -1 ,
            height: -1 ,
            style: {
                radius: lv.RADIUS_CIRCLE ,
                "border-width": 1 ,
                "border-color": lv.palette("grey") ,
                "border-opa": 180 ,
                "bg-color": lv.palette("grey") ,
                "bg-opa": 60 ,
                "min-width": 24 ,
                "min-height": 16 ,
            }
        })

        if(!noInput) {
            this.input = new Input(this)
        }
    }

    in(expr) {
        this.expr = expr
        this.input.hide()
    }
    out() {
        this.expr = null
        this.input.show()
    }
}

class CardBase extends lv.Row{

    slots = {}

    constructor(parent, vm) {
        super(parent)
        this.fromJson({
            width: -1 ,
            height: -1 ,
            style: {
                "pad-left":10 ,
                "pad-right":10 ,
                "flex-cross-place": "center" ,
                "pad-column": 5 ,
                "min-width": 60 ,
                "min-height": 16 ,
            } ,
        })

        this.draggable()

        if(vm) {
            vm.addCard(this)
        }
    }

    addLabel(text) {
        let label = new lv.Label(this, {
            text ,
            width: -1 ,
            height: -1 ,
            font: "m10" ,
        })
        return label
    }

    addExprSlot(name, noInput) {
        let slot = new CardSlotExpression(this, noInput)
        this.slots[name] = slot
        return slot
    }
    addCompSlot(name) {
        let slot = new CardSlotCompare(this)
        this.slots[name] = slot
        return slot
    }

    hittest() {
        return false
    }
}

const SPortSlopeW = 3
const SPortX1 = 6
const SPortX2 = SPortX1 + SPortSlopeW
const SPortX2_5 = SPortX2 + 5
const SPortX3 = SPortX2 + 10
const SPortX4 = SPortX3 + SPortSlopeW
const SPortSlopeH = 4

class CardStatement extends CardBase{
    prev = null
    next = null
    inner = null
    constructor(parent, vm) {
        super(parent, vm)
        
        this.fromJson({
            style: {
                // "border-width": 1 ,
                // "border-color": lv.palette("orange") ,
                // "border-opa": 180 ,
                // "bg-color": lv.palette("orange") ,
                // "bg-opa": 255 ,
                // "radius": 2 ,
                "pad-top":SPortSlopeH + 2 ,
                "pad-bottom":SPortSlopeH + 3 ,
            }
        })
        
        this.on("draw-main",(event,clip)=>{
            let [x,y] = this.coords()
            let h = this.height()
            let w = this.width()
            // x+=1; y+=1; h-=2;
            lv.drawPolygon([
                [x,y] ,
                [x+SPortX1,y] ,
                [x+SPortX2,y+SPortSlopeH] ,
                [x+SPortX2,y+h-SPortSlopeH] ,
                [x,y+h-SPortSlopeH] ,
            ], clip, bodyDsc)

            lv.drawPolygon([
                [x+SPortX3,y+SPortSlopeH] ,
                [x+SPortX4,y] ,
                [x+w,y] ,
                [x+w,y+h-SPortSlopeH] ,
                [x+SPortX3,y+h-SPortSlopeH] ,
            ], clip, bodyDsc)
            
            lv.drawPolygon([
                [x+SPortX2,y+SPortSlopeH] ,
                [x+SPortX3,y+SPortSlopeH] ,
                [x+SPortX4,y+h-SPortSlopeH] ,
                [x+SPortX3,y+h] ,
                [x+SPortX2,y+h] ,
                [x+SPortX1,y+h-SPortSlopeH] ,
            ], clip, bodyDsc)

            // console.log(event, x,y,h)
        })

        
        let _insertTo = null
        let draggable = this.draggable()
        draggable.setStart(()=>{
            let widget = this.next
            let lst = []
            while(widget) {
                lst.push(widget)
                widget = widget.next
            }
            draggable.setFollowers(lst)
        })
        draggable.setDragging((pos)=>{
            for(let other of vm.cards) {
                if(other==this || !(other instanceof CardStatement)) {
                    continue
                }
                if(other._insertTest(this, pos)) {
                    _insertTo = other
                    // pos.x = pos.y = false
                    return
                }
        
            }
            if(_insertTo) {
                if(_insertTo.next) {
                    _insertTo._placeNext(_insertTo.next)
                }
                _insertTo = null
            }
        })
        // draggable.onafter = (pos)=>{
        //     this._followMe(pos)
        // }
        draggable.setStop(()=>{
            // draggable.setFollowers(null)
            if(_insertTo) {
                _insertTo.insert(this)
                _insertTo = null
            }
        })
    }

    coordsPortPrev(x, y) {
        if(x==undefined || y==undefined){
            [x, y] = this.coords()
        }
        return [x+SPortX2_5, y+SPortSlopeH]
    }
    coordsPortNext(x, y) {
        if(x==undefined || y==undefined){
            [x, y] = this.coords()
        }
        return [x+SPortX2_5, y+this.height()]
    }

    last() {
        if(this.next) {
            return this.next.last()
        }
        return this
    }

    insert(statement) {
        if(statement==this) {
            throw new Error("insert() self ??")
        }
        if(this.next) {
            statement.last().insert(this.next)
        }
        statement.prev = this
        this.next = statement
    }
    
    _insertTest(statement, pos) {

        let [ox, oy] = statement.coordsPortPrev(pos.x, pos.y)
        let [x,y] = this.coordsPortNext()
        if((x-ox)*(x-ox) + (y-oy)*(y-oy) < PortSnapDis) {
            this._placeNext(null, pos)
            if(this.next) {
                statement._placeNext(this.next)
            }
            return true
        }

        if(!this.prev) {
            ; ([ox, oy] = statement.coordsPortNext(pos.x, pos.y))
            ; ([x, y] = this.coordsPortPrev())
            if((x-ox)*(x-ox) + (y-oy)*(y-oy) < PortSnapDis) {
                this._placePrev(null, pos)
                return true
            }
        }

        return false
    }

    
    _placePrev(statement, out, x, y) {
        if(x==undefined) {
            ([x,y] = this.coords())
        }
        if(statement) {
            statement.setCoords(x, y-this.height()+SPortSlopeH)
        }
        if(out) {
            out.x = x
            out.y = y-this.height()+SPortSlopeH
        }
    }
    _placeNext(statement, out, x, y) {
        if(x==undefined) {
            ([x,y] = this.coords())
        }
        if(statement) {
            statement.setCoords(x, y+this.height()-SPortSlopeH)
        }
        if(out) {
            out.x = x
            out.y = y+this.height()-SPortSlopeH
        }
    }

    _followMe(pos) {
        if(!this.next){
            return
        }
        this._placeNext(this.next, pos, pos.x, pos.y)
        this.next._followMe(pos)
    }
}


class CardExpression extends CardBase{
    constructor(parent, vm) {
        super(parent, vm)
        
        this.fromJson({
            style: {
                "border-width": 1 ,
                "border-color": lv.palette("orange") ,
                "border-opa": 180 ,
                "bg-color": lv.palette("orange") ,
                "bg-opa": 255 ,
                "radius": lv.RADIUS_CIRCLE ,
            }
        })
    }
}

class CardCompare extends CardBase{
    constructor(parent, vm) {
        super(parent, vm)
        
        this.fromJson({
            // style: {
            //     "border-width": 1 ,
            //     "border-color": lv.palette("orange") ,
            //     "border-opa": 180 ,
            //     "bg-color": lv.palette("orange") ,
            //     "bg-opa": 255 ,
            //     "radius": lv.RADIUS_CIRCLE ,
            // } ,
            // children: [

            // ]
        })

        this.on("draw-main", (e, clip)=>{
            let h = this.height()
            let half = h/2
            let w = this.width()
            let [x,y] = this.coords()
            
            lv.drawPolygon([
                [x,y+half] ,
                [x+half,y] ,
                [x+w-half,y] ,
                [x+w,y+half] ,
                [x+w-half,y+h] ,
                [x+half,y+h] ,
            ], clip, bodyDsc)
        })
    }
}

exports.CardStatement = CardStatement
exports.CardExpression = CardExpression
exports.CardCompare = CardCompare