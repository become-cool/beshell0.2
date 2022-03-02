require("besdk")
require("besdk/pin.js")

beapi.telnet.callback = function(funcName, ...arglst){
    for(let i=0; i<arglst.length; i++) {
        arglst[i] = JSON.stringify(arglst[i])
    }
    beapi.telnet.rspn(0, 6, `${funcName}(${arglst.join(',')})`)
}

let __debounce_timers = {}
watchPins((gpio)=>{
    if(__debounce_timers[gpio]!=undefined) {
        return
    }
    __debounce_timers[gpio] = setTimeout(()=>{
        delete __debounce_timers[gpio]
        beapi.telnet.callback('EmitPinChanged', gpio, beapi.gpio.digitalRead(gpio))
    }, 20)
})
