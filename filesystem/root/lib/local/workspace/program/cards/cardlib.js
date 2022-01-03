const base = require('./CardBase')
const lv = require("lv")

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

    static _menu = null
    static menu(parent) {
        if(!OperatorTwo._menu) {
            OperatorTwo._menu = new lv.Menu(parent||lv.active(), {
                items: ["+","-","*","/","^","|","&"]
            })
        }
        return OperatorTwo._menu
    }

    constructor(parent, vm) {
        super(parent, vm)
        this.addExprSlot("left", true)
        this.addMenu(OperatorTwo.menu(parent))
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