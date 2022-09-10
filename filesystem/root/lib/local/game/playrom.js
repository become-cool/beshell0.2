
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
beapi.gameplayer.play(json.rom)

