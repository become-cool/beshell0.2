require("./obj.js")
require("./widgets.js")
require("./menu.js")
require("./loader.js")
beapi.lvgl.Color = require("./color.js")
require("./animate.js")
require("./input.js")


beapi.lvgl.dir = {
    NONE: 0 ,
    LEFT: 1 ,
    RIGHT: 2 ,
    TOP: 4 ,
    BOTTOM: 8 ,
}
beapi.lvgl.dir.HOR = beapi.lvgl.dir.LEFT | beapi.lvgl.dir.RIGHT
beapi.lvgl.dir.VER = beapi.lvgl.dir.TOP | beapi.lvgl.dir.BOTTOM
beapi.lvgl.dir.ALL = beapi.lvgl.dir.HOR | beapi.lvgl.dir.VER


beapi.lvgl.side = {
    none     : 0x00,
    bottom   : 0x01,
    top      : 0x02,
    left     : 0x04,
    right    : 0x08,
    full     : 0x0F,
}

beapi.lvgl.RADIUS_CIRCLE = 0x7FFF


beapi.lvgl.part = {
    main : 0x000000 ,
    scrollbar : 0x010000 ,
    indicator : 0x020000 ,
    knob : 0x030000 ,
    selected : 0x040000 ,
    items : 0x050000 ,
    ticks : 0x060000 ,
    cursor : 0x070000 ,
    custom_first : 0x080000 ,
    any : 0x0F0000 ,
}
beapi.lvgl.state = {
    default : 0x0000 ,
    checked : 0x0001 ,
    focused : 0x0002 ,
    focus_key : 0x0004 ,
    edited : 0x0008 ,
    hovered : 0x0010 ,
    pressed : 0x0020 ,
    scrolled : 0x0040 ,
    disabled : 0x0080 ,
    user_1 : 0x1000 ,
    user_2 : 0x2000 ,
    user_3 : 0x4000 ,
    user_4 : 0x8000 ,
    any : 0xFFFF ,
}

beapi.lvgl.active = function () {
    return be.disp[0].activeScreen()
}
beapi.lvgl.sysLayer = function () {
    return be.disp[0].sysLayer()
}
beapi.lvgl.topLayer = function () {
    return be.disp[0].topLayer()
}

let sharedKeyboard = null
beapi.lvgl.keyboard = function () {
    if(!sharedKeyboard){
        sharedKeyboard = new lv.Keyboard(beapi.lvgl.active())
    }
    else {
        sharedKeyboard.setParent(beapi.lvgl.active())
    }
    return sharedKeyboard
}


let _msgbox = null
function msgbox() {
    if(!_msgbox) {
        _msgbox = 
        _msgbox.center()
    }
    return _msgbox
}
beapi.lvgl.msg = {
    info(title, desc) {
        let box = new lv.MsgBox(beapi.lvgl.active(), title||"", desc||"", [], true)
        box.title().setFont("msyh")
        let text = box.text()
        if(text) text.setFont("msyh")
        box.center()
    }
}
beapi.lvgl.msg.sucess = beapi.lvgl.msg.info
beapi.lvgl.msg.error = beapi.lvgl.msg.info
beapi.lvgl.msg.warning = beapi.lvgl.msg.info


const mapIndevCallbacks = {
    pressed (target) {
        beapi.lvgl.emit("pressed", "pressed", target)
    } ,
    pressing (target) {
        beapi.lvgl.emit("pressing", "pressing", target)

    } ,
    released (target) {
        beapi.lvgl.emit("released", "released", target)
    } ,
}
beapi.lvgl.on("#EVENT.ADD#", (name)=>{
    if(name=="pressed" || name=="pressing" || name=="released") {
        beapi.lvgl.setIndevCallback(name, mapIndevCallbacks[name])
    }
})
beapi.lvgl.on("#EVENT.CLEAR#", (name)=>{
    if(name=="pressed" || name=="pressing" || name=="released") {
        beapi.lvgl.clearIndevCallback(name)
    }
})


module.exports = beapi.lvgl
global.lv = beapi.lvgl