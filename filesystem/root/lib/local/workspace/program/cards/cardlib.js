const base = require('./CardBase')

class Print extends base.CardStatement {
    constructor(parent, vm) {
        super(parent, vm)
        this.addLabel("print")
        this.addExprSlot("what")
    }
}

class Hello extends base.CardStatement {
    constructor(parent, vm) {
        super(parent, vm)
        this.addLabel("say")
        this.addExprSlot("what", true)
    }
}

class OperatorTwo extends base.CardExpression {
    constructor(parent, vm) {
        super(parent, vm)
        this.addExprSlot("left", true)
        this.addLabel("+")
        this.addExprSlot("right")
    }
}

module.exports = [
    {
        name: 'control' ,
        icon: 'IF' ,
        cards: [
            Print ,
            Hello ,
            OperatorTwo ,
        ] ,
    } ,
    {
        name: 'math' ,
        icon: 'M' ,
        cards: [] ,
    } ,
    {
        name: 'control' ,
        icon: 'IF' ,
        cards: [] ,
    } ,
    {
        name: 'control' ,
        icon: 'IF' ,
        cards: [] ,
    } ,
]