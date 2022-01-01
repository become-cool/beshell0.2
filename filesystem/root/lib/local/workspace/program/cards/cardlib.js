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

module.exports = [
    {
        name: 'control' ,
        icon: 'IF' ,
        cards: [
            Print ,
            Hello ,
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