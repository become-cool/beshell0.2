beapi.lvgl.__lv_obj_init = function () {
    this._handles={}
    this.on("#EVENT.ADD#",(eventName)=>{
        this.enableEvent(eventName)
    })
    this.on("#EVENT.CLEAR#",(eventName)=>{
        this.disableEvent(eventName)
    })
}