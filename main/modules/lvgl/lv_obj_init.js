beapi.lvgl.__lv_obj_init = function () {
    this._handles={}
    this.on("#EVENT.ADD#",(n)=>{
        this.enableEvent(n=="*"?"all":n)
    })
    this.on("#EVENT.CLEAR#",(n)=>{
        this.disableEvent( n=="*"?"all":n )
    })
}