global.be = {
    part: {} ,
    var: {} ,
    bus: new beapi.EventEmitter,
    ui: {
        set(widgetName, methodName, ...argv) {
            telnet.send(0, 6, `EmitUISetter("${widgetName}", "${methodName}", ${JSON.stringify(argv)})`)
        }
    }
}