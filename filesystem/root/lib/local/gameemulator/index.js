const lv = require("lv")
const SelectMode = require("./SelectMode")

let selector = new SelectMode(lv.defauleDisplay())


// beapi.i2c.setup(0, 4, 5)
// beapi.gameplayer.setJoypad(0,51,-1,0)
// beapi.gameplayer.setDisplay(beapi.lvgl.defaultDisplay())
// beapi.gameplayer.play("/home/become/mario.nes")