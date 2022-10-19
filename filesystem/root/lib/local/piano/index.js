const PianoKeyboard = require("/lib/local/besdk/driver/piano-keyboard.js")

function main () {

    let keyboard = null 
    for(let name in be.dev) {
        let dev = be.dev[name]
        if(dev instanceof PianoKeyboard) {
            keyboard = dev
            break
        }
    }
    
    if(!keyboard) {
        throw new Error("not found keyboard dev")
    }

    let piano = new beapi.audio.MIDIPlayer(keyboard.devId,keyboard.pl,keyboard.i2s,keyboard.keys,"/var/piano.sf2")
    global.piano = piano
}

main()
