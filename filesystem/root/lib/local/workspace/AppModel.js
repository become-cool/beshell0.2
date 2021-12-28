const BePad = require("./graph/parts/BePad")

class AppModel extends beapi.EventEmitter{
    constructor(workspace) {
        super()
        this.workspace = workspace

        this.host = null
        this.parts =[]
    }

    createHostFromDevice(){
        this.host = new BePad(this.workspace.graph)
        this.parts.push(this.host)
    }

    createPart(partClass) {
        let part = new partClass(this.workspace.graph)
        this.parts.push(part)
        return part
    }
}

module.exports = AppModel