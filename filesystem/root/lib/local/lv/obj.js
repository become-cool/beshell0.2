

beapi.lvgl.Obj.prototype.show = function show() {
    this.clearFlag("hidden")
}
beapi.lvgl.Obj.prototype.hide = function hide() {
    this.addFlag("hidden")
}


beapi.lvgl.Obj.prototype.asRow = function asRow() {
    this.removeStyleAll()
    this.setFlexFlow("row")
}
beapi.lvgl.Obj.prototype.asColumn = function asColumn() {
    this.removeStyleAll()
    this.setFlexFlow("column")
}

beapi.lvgl.Btn.prototype.setText = function setText(text) {
    if(!this.label) {
        this.label = new beapi.lvgl.Label(this)
    }
    this.label.setText(text)
}
beapi.lvgl.Btn.prototype.setFont = function setFont(font) {
    if(!this.label) {
        this.label = new beapi.lvgl.Label(this)
    }
    this.label.setFont(font)
}

class Row extends beapi.lvgl.Obj {
    constructor(parent) {
        super(parent)
        this.asRow()
        this.setWidth("100%")
        this.setHeight(-1)
    }
}
beapi.lvgl.Row = Row
class Column extends beapi.lvgl.Obj {
    constructor(parent) {
        super(parent)
        this.asColumn()
        this.setWidth(-1)
        this.setHeight("100%")
    }
}
beapi.lvgl.Column = Column

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

beapi.lvgl.Obj.prototype.primitiveSetWidth = beapi.lvgl.Obj.prototype.setWidth
beapi.lvgl.Obj.prototype.setWidth = function(val) {
    return this.primitiveSetWidth(size(val))
}

beapi.lvgl.Obj.prototype.primitiveSetHeight = beapi.lvgl.Obj.prototype.setHeight
beapi.lvgl.Obj.prototype.setHeight = function(val) {
    return this.primitiveSetHeight(size(val))
}


beapi.lvgl.Keyboard.prototype.popup = function(textarea, cb) {
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
}