beapi.lvgl.Obj.prototype.draggable = function(onstart, ondragging, onstop) {
    if(!this._draggable) {
        this._draggable = new beapi.lvgl.Draggable(this)
        if(onstart) { this._draggable.setStart(onstart) }
        if(ondragging) { this._draggable.setDragging(ondragging) }
        if(onstop) { this._draggable.setStop(onstop) }
    }
    this._draggable.enable()
    return this._draggable
}

beapi.lvgl.Obj.prototype.asModal = function asModal() {
    this.on("visible", visible=>{
        if(visible) {
            this.holdKeys()
        }
        else {
            this.releaseKeys()
        }
    })
    this.on("ipt.btn.press",key=>{
        if(key=='esc') {
            this.hide()
        }
    })
}

beapi.lvgl.styleHighlight = new beapi.lvgl.Style({
    "border-width": 1 ,
    "border-color": beapi.lvgl.palette("red") ,
    "border-opa": 180
})
beapi.lvgl.Obj.prototype.highlight = function (on) {
    if( on==undefined || (!!on) ) {
        this.addStyle(beapi.lvgl.styleHighlight)
    }
    else {
        this.removeStyle(beapi.lvgl.styleHighlight)
    }
}

beapi.lvgl.Obj.prototype.hitTest = function hitTest(x,y) {
    if(x==undefined || x==null || y==undefined || y==null) {
        ; ({x,y} = lv.inputPoint())
    }
    let [x1,y1] = this.coords()
    return x>x1 && y>y1 && x<x1+this.width() && y<y1+this.height() 
}

beapi.lvgl.Obj.prototype._clearFlag = beapi.lvgl.Obj.prototype.clearFlag
beapi.lvgl.Obj.prototype.clearFlag = function (flag) {
    this._clearFlag(flag)
    if(flag=='hidden') {
        this.emit("visible", true, this)
    }
}
beapi.lvgl.Obj.prototype._addFlag = beapi.lvgl.Obj.prototype.addFlag
beapi.lvgl.Obj.prototype.addFlag = function (flag) {
    this._addFlag(flag)
    if(flag=='hidden') {
        this.emit("visible", false, this)
    }
}

beapi.lvgl.Obj.prototype.show = function show() {
    this.clearFlag("hidden")
}
beapi.lvgl.Obj.prototype.hide = function hide() {
    this.addFlag("hidden")
}
beapi.lvgl.Obj.prototype.toggle = function hide() {
    if(this.isVisible()) {
        this.addFlag("hidden")
        return false
    }
    else {
        this.clearFlag("hidden")
        return true
    }
}
beapi.lvgl.Obj.prototype.setVisible = function setVisible(visible) {
    if(visible) {
        this.clearFlag("hidden")
    } else {
        this.addFlag("hidden")
    }
}

beapi.lvgl.Obj.prototype.asRow = function asRow() {
    this.removeStyleAll()
    this.setFlexFlow("row")
}
beapi.lvgl.Obj.prototype.asColumn = function asColumn() {
    this.removeStyleAll()
    this.setFlexFlow("column")
}

beapi.lvgl.Btn.prototype.text = function text() {
    if(!this.label) {
        return null
    }
    return this.label.text()
}
beapi.lvgl.Btn.prototype.setText = function setText(text) {
    if(!this.label) {
        this.label = this.child(0)
        if(!this.label || this.label.constructor!=beapi.lvgl.Label) {
            this.label = new beapi.lvgl.Label(this)
        }
    }
    this.label.setText(text)
}
beapi.lvgl.Btn.prototype.setFont = function setFont(font) {
    if(!this.label) {
        this.label = new beapi.lvgl.Label(this)
    }
    this.label.setFont(font)
}
beapi.lvgl.Btn.prototype.setTextColor = function setTextColor(color) {
    if(!this.label) {
        this.label = new beapi.lvgl.Label(this)
    }
    this.label.setStyle("text-color",color)
}

beapi.lvgl.CleanObj = class CleanObj extends beapi.lvgl.Obj {
    constructor(parent) {
        super(parent)
        // this.removeStyleAll()
        this.setStyle("pad-top", 0)
        this.setStyle("pad-bottom", 0)
        this.setStyle("pad-left", 0)
        this.setStyle("pad-right", 0)
        this.setStyle("border-width", 0)
        this.setStyle("radius", 0)
        this.setStyle("bg-opa", 0)
        this.clearFlag("scrollable")
    }
}

function size(val) {
    if(typeof val=='string' && val[val.length-1]=='%') {
        let _val = Math.round(val.substr(0,val.length-1))
        if(isNaN(_val)) {
            throw new Error ("invalid percent number: "+val)
        }
        val = beapi.lvgl.pct(_val)
    }
    if(val==-1) {
        val = beapi.lvgl.SizeContent
    }
    return val
}

beapi.lvgl.Obj.prototype._setWidth = beapi.lvgl.Obj.prototype.setWidth
beapi.lvgl.Obj.prototype.setWidth = function(val) {
    return this._setWidth(size(val))
}

beapi.lvgl.Obj.prototype._setHeight = beapi.lvgl.Obj.prototype.setHeight
beapi.lvgl.Obj.prototype.setHeight = function(val) {
    return this._setHeight(size(val))
}
beapi.lvgl.Obj.prototype.children = function() {
    let children = []
    for(let i=0;i<this.childCnt();i++){
        children.push(this.child(i))
    }
    return children
}

beapi.lvgl.Label.prototype._setText = beapi.lvgl.Label.prototype.setText
beapi.lvgl.Label.prototype.setText = function(text) {
    if(this.text() == text) {
        return
    }
    this._setText(text)
}


beapi.lvgl.Keyboard.prototype.popup = function(textarea, cb) {
    if( !textarea instanceof beapi.lvgl.TextArea) {
        throw new Error("arg textarea of Keyboard.popup() must be an instance of beapi.lvgl.TextArea")
    }
    if( !this._doneCb ){
        this._doneCb = (event)=>{
            if(!this._popupCb){
                this.hide()
                return
            }
            if(this._popupCb(this, event)!=false) {
                this.hide()
            }
            this._popupCb = null
        }
        this.on("ready", this._doneCb)
        this.on("cancel", this._doneCb)
    }
    if(textarea) {
        this.setTextarea(textarea)
    }
    this._popupCb = cb || null
    this.show()
    return this
}


beapi.lvgl.Path.prototype.begin = function(x,y){
    this.bx = x || 0
    this.by = y || 0
    this.wx = x || 0
    this.wy = y || 0
}
beapi.lvgl.Path.prototype.end = function(){
    if(this.wx!=undefined && this.bx!=undefined && this.wy!=undefined && this.by!=undefined) {
        if(this.wx!=this.bx || this.wy!=this.by) {
            this.addLine(this.wx, this.wy, this.bx, this.by)
        }
    }
    delete this.bx, this.by, this.wx, this.wy
}
beapi.lvgl.Path.prototype.moveTo = function(x,y){
    this.wx = x
    this.wy = y
}
beapi.lvgl.Path.prototype.lineTo = function(x,y){
    if(!this.wx) this.wx = 0
    if(!this.wy) this.wy = 0
    this.addLine(this.wx, this.wy, x, y)
    this.wx = x
    this.wy = y
}
beapi.lvgl.Path.prototype.arcTo = function(x,y,cx,cy,clockwish){
    if(!this.wx) this.wx = 0
    if(!this.wy) this.wy = 0
    if(clockwish)
        this.addArc(this.wx, this.wy, x, y, cx,cy)
    else
        this.addArc(x, y, this.wx, this.wy, cx,cy)
    this.wx = x
    this.wy = y
}


