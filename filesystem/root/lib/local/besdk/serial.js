
// Serial1.on("data", ConsoleSerialReceiver('uart', 0))
// Serial2.on("data", ConsoleSerialReceiver('uart', 1))

function ConsoleSerialReceiver(name, idx) {
    return function (data) {
        let output = `\nrun>>EmitSerialReceived('${name}', ${idx}, [`
        for(var i=0;i<data.length; i++) {
            output+= data.charCodeAt(i) + ","
        }
        output+= "])"
        console.log(output)
    }
}