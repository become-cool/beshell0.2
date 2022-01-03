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
    numeric = false
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
            clicked: ()=>{
                lv.keyboard().popup(this).setMode(this.numeric? "number": "text-lower")
            } ,
        })
        let txt = this.child(0).as(lv.Label)
        txt.setFont("m10")
        txt.setWidth(-1)
        txt.setStyle("min-width",5)
    }
}

const SlotMinWidth = 24
const SlotMinHeight = 16

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
                "min-width": SlotMinWidth ,
                "min-height": SlotMinHeight ,
            }
        })
        if(!noInput) {
            this.input = new Input(this)
        }
    }
    desktop() {
        const Program = require("../Program")
        for(let parent = this.parent(); parent; parent=parent.parent()){
            if(parent instanceof Program) {
                return parent
            }
        }
    }

    insert(expr) {
        expr.setParent(this)
        expr.center()
        this.expr = expr
        if(this.input)
            this.input.hide()
    }
    remove() {
        if(!this.expr) {
            return
        }
        this.expr.setParent( this.desktop() )
        this.expr = null
        if(this.input)
            this.input.show()
    }

    // extend 用于拖出时扩大范围，别面在边界上抖动
    _testInsert(ex,ey, extend) {
        let [x,y] = this.coords()
        if(ey<y-extend)
            return false
        // 按空 slot 计算
        if(ey>y+SlotMinHeight+extend)
            return false
        if(ex<x+SlotMinHeight/2-extend || ex>x+SlotMinWidth-SlotMinHeight/2+extend) 
            return false
        return true
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

class CardMenu extends lv.Row{
    menu = null
    value = null
    constructor(card, menu) {
        super(card) 
        this.menu = menu

        this.fromJson({
            width: -1 ,
            height: -1 ,
            bubble: true ,
            style: {
                "flex-cross-place": "center" ,
            } ,
            children: [
                {
                    class: lv.Label ,
                    ref: "title" ,
                    width: -1 ,
                    height: -1 ,
                    font: "m12" ,
                    text: '' ,
                    style: {
                        "min-width": 20 ,
                        "text-align": "center" ,
                    }
                } , 
                {
                    class: lv.Label ,
                    width: -1 ,
                    height: 17 ,
                    font: "m10" ,
                    text: lv.symbol.down ,
                    style: {
                        "text-color": lv.rgb(220) ,
                    } ,
                    clicked: ()=>{
                        if(this.menu){
                            // release out of label
                            if(!this.hitTest()) {
                                return
                            }
                            this.menu.setActive(this.value)
                            this.menu.popup(null, null, (value)=>{
                                this.setValue(value)
                            })
                        }
                    }
                }
            ] ,
        }, this)

        this.setValue(null)
    }
    setValue(value) {
        if(this.value==value) {
            return
        }
        this.emit("value-change", value, this.value)
        this.value = value
        let title = this.menu?.findItem(value)?.text()
        this.title.setText(title||value)
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
                "pad-left":2 ,
                "pad-right":2 ,
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

    addLabel(text, name) {
        let label = new lv.Label(this, {
            text ,
            width: -1 ,
            height: -1 ,
            font: "m10" ,
            style: {
                "text-color": lv.rgb(220,220,220)
            }
        })
        if(name) {
            this[name] = label
        }
        return label
    }
    addMenu(menu, name) {
        let obj = new CardMenu(this, menu)
        if(name) {
            this[name] = obj
        }
        return obj
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
/*
      x1       x4
    .____     _______________________
    |    \___/                       |
    |    x2  x3                      |
    |____     _______________________|
         \___/
*/

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
                "pad-left":8 ,
                "pad-right":8 ,
                "pad-top":SPortSlopeH + 2 ,
                "pad-bottom":SPortSlopeH + 3 ,
            } ,
            // flag: ["top"]
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
            if(this.prev) {
                this.prev.next = null
                this.prev = null
            }
            this.moveForeground()
        })
        draggable.setDragging((pos)=>{
            let [px, py] = this.coordsPortPrev(pos.x, pos.y)
            for(let statement of vm.cards) {
                if(statement==this || !(statement instanceof CardStatement || statement.next) ) {
                    continue
                }
                if(statement._testInsert(px, py)) {
                    statement._placeNext(null, pos)                    
                    _insertTo = statement
                    return
                }
            }
            if(_insertTo) {
                _insertTo = null
            }
        })
        draggable.setStop(()=>{
            draggable.setFollowers(null)
            if(this.prev) {

            }
            if(_insertTo) {
                _insertTo.insert(this)
                _insertTo = null
            }
        })

        let requireMoveNext = false 
        this.on("size-changed", ()=>{
            if(!this.next || requireMoveNext)
                return
            requireMoveNext = true
            setTimeout(()=>{
                for(let next=this.next, prev=this; next; prev=next, next=next.next) {
                    prev._placeNext(next)
                    next.updateLayout()
                }
                requireMoveNext = false
            },0)
        })
    }

    // px, py 为 statement up port 的坐标(横线中点)
    _testInsert(px,py) {
        let [x,y] = this.coordsPortNext()
        return ((x-px)*(x-px) + (y-py)*(y-py)) < PortSnapDis
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
    _matchSlotClz = CardSlotExpression
    constructor(parent, vm) {
        super(parent, vm)
        this.appearance()

        let draggable = this.draggable()
        draggable.setStart(()=>{
            this.moveForeground()
        })
        draggable.setDragging(pos=>{
            let x = pos.x 
            let y = pos.y + this.height()/2
            let parent = this.parent()
            if( parent instanceof this._matchSlotClz ){
                if(!parent._testInsert(x, y, 10)) {
                    parent.remove()
                    this.setCoords(pos.x, pos.y)
                    this.updateLayout()
                }
                else {
                    pos.x = pos.y = false
                }
                return
            }
            for(let statement of vm.cards) {
                if(statement==this) {
                    continue
                }
                for(let name in statement.slots) {
                    let slot = statement.slots[name]
                    if( !(slot instanceof this._matchSlotClz) || slot.expr )
                        continue
                    if(!slot._testInsert(x, y, 0)) {
                        continue
                    }
                    slot.insert(this)
                    pos.x = pos.y = false
                    return
                }
            }
        })
    }

    appearance() {
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

class CardCompare extends CardExpression{
    constructor(parent, vm) {
        super(parent, vm)
        this._matchSlotClz = CardSlotCompare
    }
    appearance() {
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