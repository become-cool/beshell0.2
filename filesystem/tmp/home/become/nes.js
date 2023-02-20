// beapi.i2c.setup(0, 4, 5)
// beapi.gameplayer.setJoypad(0,51,-1,0)
// beapi.gameplayer.setDisplay(be.disp[0])
// beapi.gameplayer.play("/home/become/game/超级玛丽.nes")

require("/etc/init.d/boot.js").rebootToApp("/lib/local/game/playrom.js", true)
