class DeviceDriver extends beapi.EventEmitter {
    constructor(name) {
        super()
        this.name = name
    }
    setup() { return true }
    begin() { return true }
    end() {}
    unsetup() {}
    register(insName) {
        if(!be.dev) {
            be.dev = {}
        }
        if(!insName) {
            insName = DeviceDriver.genInsName(this.name,be.dev)
        }
        be.dev[insName] = this
    }

    static genInsName(prefix,pool) {
        let i = 1 ;
        for(; undefined!=pool[prefix+i]; i++) {}
        return prefix+i
    }
}
module.exports = DeviceDriver