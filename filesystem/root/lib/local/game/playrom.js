const Button = require("besdk/driver/button.js")
const datapaht = "/home/become/.data/game.json"

if(!be.disp.length) {
    console.log("no display device")
    process.reboot()
}

let json = JSON.load(datapaht)
if(!json || !json.rom) {
    console.log("JSON.load() failed:", datapaht)
    process.reboot()
}

console.log("load rom", json.rom)

beapi.gameplayer.setDisplay(be.disp[0])

const keys = {
    "up": 0x01,
    "down": 0x02,
    "left": 0x04,
    "right": 0x08,
    "a": 0x10,
    "b": 0x20,
    "start": 0x40,
    "tab": 0x80,
}
for(let dev of Object.values(be.dev)) {
    if(dev instanceof Button && keys[dev.k]) {
        beapi.gameplayer.setBtns(dev.i,dev.k)
    }
}

beapi.gameplayer.play(json.rom)

