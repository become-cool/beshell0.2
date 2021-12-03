beapi.lvgl.__lv_obj_init = function () {
    this._handles={}
    this.on("#EVENT.ADD#",function(eventName){
        console.log("add event:", eventName)
    })
    this.on("#EVENT.CLEAR#",function(eventName){
        console.log("clear event:", eventName)
    })
}