beapi.lvgl.Obj.prototype.draggable = function(start, dragging, stop) {
    if(this._isDraggable) {
        return
    }
    this.addFlag("clickable")
    this._isDraggable = true

    let lx = 0, ly = 0
    this.on("pressed", ()=>{
        let {x,y} = beapi.lvgl.inputPoint()
        let [_x, _y] = this.coords()
        lx = x - _x
        ly = y - _y
        if(start) {
            start(lx,ly)
        }
    })
    this.on("released", ()=>{
        if(stop) {
            stop()
        }
    })
    this.on("pressing", ()=>{
        let {x,y} = lv.inputPoint()
        this.setCoords(x-lx, y-ly)
        if(dragging) {
            dragging(x,y)
        }
    })

}