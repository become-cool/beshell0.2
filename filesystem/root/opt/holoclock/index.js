const HoloClock = require("./HoloClock.js")

function main() {
    const disp = be.disp[0]
    if(!disp) {
        console.log("no disp device, exit HoloClock")
        return
    }
    let desktop = new HoloClock()
    global.desktop = desktop

    lv.loadScreen(desktop)
}
main()