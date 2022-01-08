const lv = require("lv")
const base = require('./CardBase')

class Print extends base.CardStatement {
    constructor(parent, graph) {
        super(parent, graph)
        this.expr.addLabel("print")
        this.expr.addSlot("what")
    }
    run() {
        let waht = this.expr.slots.what.evaluate()
        console.log(waht)
        this.runNext()
    }
}

class Hello extends base.CardStatement {
    constructor(parent, graph) {
        super(parent, graph)
        this.expr.addLabel("say")
        this.expr.addSlot("what")
    }
}

class OperatorTwo extends base.CardExpression {
    constructor(parent, graph) {
        super(parent, graph)
        this.addSlot("left").input.numeric = true
        this.addMenu(base.shareMenu(graph, "operators", ["+","-","*","/","^","|","&"]), "op")
        this.addSlot("right").input.numeric = true
    }
}

class If extends base.CardControl {
    constructor(parent, graph) {
        super(parent, graph)
        this.expr.addLabel("If")
        this.expr.addSlot("what")
    }
}
class IfElse extends base.CardControl {
    constructor(parent, graph) {
        super(parent, graph)
        this.expr.addLabel("If-Else")
        this.expr.addSlot("what")

        this.proc2 = this.createProcess()
    }
}

class IsNumber extends base.CardCompare {
    constructor(parent, graph) {
        super(parent, graph)
        this.addLabel("Is Number")
        this.addCompareSlot("what")
    }
}

module.exports = {
    Print ,
    Hello ,
    OperatorTwo ,
    If ,
    IfElse ,
    IsNumber ,
}