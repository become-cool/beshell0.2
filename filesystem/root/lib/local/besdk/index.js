global.be = {
    dev: {} ,
    disp: [] ,
    indev: [] ,
    part: [] ,
    var: {} ,
    server: [] ,
    desktop: null ,
    bus: new beapi.EventEmitter,
    ui: {
        set(widgetName, methodName, ...argv) {
            beapi.telnet.rspn(0, 6, `EmitUISetter("${widgetName}", "${methodName}", ${JSON.stringify(argv)})`)
        }
    }
}