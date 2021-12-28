class Draggable {
    target = null
    ox = 0
    oy = 0
    enabled = true
    dragging = false

    onstart = null
    ondragging = null
    onstop = null

    constructor(target) {
        this.target = target

        target.addFlag("clickable")

        target.on("pressed", ()=>{
            this.startDrag()
        })
        target.on("pressing", ()=>{
            if(!this.enabled || !this.dragging) {
                return
            }
            let pos = lv.inputPoint()
            pos.x += this.ox
            pos.y += this.oy
            if(this.ondragging) {
                if(this.ondragging(pos)==false) {
                    if(this.onstop) {
                        this.onstop(true)
                    }
                    this.dragging = false
                    return
                }
            }
            if(pos.x!=false) 
                this.target.setCoordX(pos.x)
            if(pos.y!=false) 
                this.target.setCoordY(pos.y)
        })
        target.on("released", ()=>{
            if(!this.dragging){
                return
            }
            if(this.onstop) {
                this.onstop(false)
            }
            this.dragging = false
        })
    }

    startDrag() {
        if(!this.enabled)
            return
        let {x,y} = lv.inputPoint()
        let [_x, _y] = this.target.coords()
        this.ox = _x - x
        this.oy = _y - y
        if(this.onstart) {
            if(this.onstart()==false) {
                return
            }
        }
        this.dragging = true
    }
}


beapi.lvgl.Obj.prototype.draggable = function(onstart, ondragging, onstop) {
    if(!this._draggable) {
        this._draggable = new Draggable(this)
    }
    this._draggable.onstart = onstart
    this._draggable.ondragging = ondragging
    this._draggable.onstop = onstop
    this._draggable.enabled = true
}
