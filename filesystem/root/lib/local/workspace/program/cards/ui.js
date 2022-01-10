const lv = require("lv")
const base = require('./CardBase')

const pkgname = 'ui'
const bgColor = lv.palette("green")

function lstWidgets (graph){
    return graph?.model?.widgetNames()
}

class UIEvent extends base.CardEvent {
    static pkgname = pkgname
    constructor(parent, graph) {
        super(parent, graph)
        this.expr.addLabel("UI")
        graph?.model?.vm?.on("ui", (widgetName, eventName)=>{
            // console.log(widgetName, eventName, this.expr.slots.widget.value, this.expr.slots.event.value)
            if(widgetName!=this.expr.slots.widget.value || eventName!=this.expr.slots.event.value ) {
                return
            }
            this.run()
        })

        this.expr.addMenu(base.shareMenu("widgets"), "widget", lstWidgets)
        this.expr.addLabel("emit")
        this.expr.addMenu(base.shareMenu("events", ["clicked","pressed","pressing","released"]), "event")
    }
    bgColor() {
        return bgColor
    }
}


class UIGetText extends base.CardExpression {
    static pkgname = pkgname
    constructor(parent, graph) {
        super(parent, graph)
        this.addLabel("text of")
        this.addMenu(base.shareMenu("widgets"), "widget", lstWidgets)
    }
    evaluate() {
        let value = this.slots.widget.value
        let widget = this.graph.model.widgets[value]
        if(!widget) {
            return null
        }
        if(!widget.text) {
            return null
        }
        return widget.text()
    }
    bgColor() {
        return bgColor
    }
    generate() {
        let strname = JSON.stringify(this.expr.slots.widget.value)
        return `be.ui[${strname}].text()`
    }
}
class UISetText extends base.CardStatement {
    static pkgname = pkgname
    constructor(parent, graph) {
        super(parent, graph)
        this.expr.addLabel("set")
        this.expr.addMenu(base.shareMenu("widgets"), "widget", lstWidgets)
        this.expr.addLabel("text to")
        this.expr.addSlot("what")
    }
    run() {
        let wname = this.expr.slots.widget.value
        let widget = this.graph.model.widgets[wname]
        if(!widget) {
            this.runNext()
            return
        }
        let value = this.expr.slots.what.evaluate()
        console.log(value)
        widget.setText(value)

        this.runNext()
    }
    bgColor() {
        return bgColor
    }
    generate(indent) {
        let strname = JSON.stringify(this.expr.slots.widget.value)
        let what = this.expr.slots.what.generate()
        indent = " ".repeat(indent*4)
        return `${indent}be.ui[${strname}].setText(${what})`
    }
}

module.exports = {
    UIEvent ,
    UIGetText ,
    UISetText ,
}