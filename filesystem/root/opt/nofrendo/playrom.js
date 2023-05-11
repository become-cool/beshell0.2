const Button = require("besdk/driver/button.js")
const datapath = "/home/become/.data/game.json"

function main() {
    if(!be.disp.length) {
        console.log("no display device")
        process.reboot()
    }

    let romPath
    let showcase = process.argv == 'showcase'
    console.log("showcase: ",showcase)

    if(showcase) {
        const games = ['超级玛丽.nes','松鼠大作战.nes','魂斗罗.nes','双截龙2.nes'].reduce((arr,name)=>{
            if(beapi.fs.existsSync('/home/become/game/'+name)) {
                arr.push(name)
            }
            return arr
        }, [])
    
        let randGame = games[Math.floor(Math.random() * games.length)]    
        romPath = '/home/become/game/' + randGame
    }

    else {
        let json = JSON.load(datapath)
        if(!json || !json.rom) {
            console.log("JSON.load() failed:", datapath)
            process.reboot()
        }
        romPath = json.rom
    }
    
    console.log(romPath)

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

    try{
        beapi.gameplayer.play(romPath, 1, showcase)
    }catch(e) {
        console.log(e)
    }

}

main()