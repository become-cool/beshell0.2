const lv = require("lv")
const base = require('./CardBase')

function lstWidgets (graph){
    return graph.model.widgetNames()
}

class UIEvent extends base.CardEvent {

    constructor(parent, graph) {
        super(parent, graph)
        this.bgColor = lv.palette("green")
        this.expr.addLabel("UI")

        graph?.model?.vm?.on("ui", (widgetName, eventName)=>{
            // console.log(widgetName, eventName, this.expr.slots.widget.value, this.expr.slots.event.value)
            if(widgetName!=this.expr.slots.widget.value || eventName!=this.expr.slots.event.value ) {
                return
            }
            this.run()
        })

        this.expr.addMenu(base.shareMenu(graph, "widgets"), "widget", lstWidgets)
        this.expr.addLabel("emit")
        this.expr.addMenu(base.shareMenu(graph, "events", ["clicked","pressed","pressing","released"]), "event")
    }
}


class UIGetText extends base.CardExpression {
    constructor(parent, graph) {
        super(parent, graph)
        this.addLabel("Text of")
        this.addMenu(base.shareMenu(graph, "widgets"), "widget", lstWidgets)
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
}
class UISetText extends base.CardStatement {
    constructor(parent, graph) {
        super(parent, graph)
    }
}

module.exports = {
    UIEvent ,
    UIGetText ,
    UISetText ,
}