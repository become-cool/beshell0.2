const lv = require('lv')
const UITools = require('./UITools')
const {WidgetHelper} = require('./WidgetHelper')

class UI extends lv.CleanObj{

    tools = null
    activeWidget = null
    workspace = null

    constructor(parent, workspace) {
        super(parent)
        this.workspace = workspace

        this.fromJson({
            width: "100%" ,
            height: "100%" ,
            visible: false ,
            children: [
                {
                    class: 'Label' ,
                    center: true ,
                    text: '应用还没有UI元素' ,
                    ref: 'holdspace' ,
                    font: 'msyh' ,
                    style: {
                        "text-color": lv.rgb(200,200,200)
                    }
                } ,
                {
                    class: lv.CleanObj ,
                    width: "100%" ,
                    height: "100%" ,
                    ref: "root" ,
                    clicked: () => {
                        this.setActiveWidget(null)
                    }
                } ,
                {
                    class: lv.CleanObj ,
                    ref: "shadow",
                    width: "100%" ,
                    height: "100%" ,
                    pressed : ()=>{
                        this.setActiveWidget(null)
                    }
                } ,
                {
                    class: WidgetHelper ,
                    ref: "helper" ,
                } ,
                 
            ] ,
        }, this)

        workspace.model.on("model.widget.new", ()=>{
            this.holdspace.hide()
        })
        workspace.model.on("model.widget.remove", ()=>{
            if(!workspace.model.widgets.length){
                this.holdspace.show()
            }
        })
    }

    createWidget(clz, json) {
        let widget = new clz(this.root)
        if(json) {
            widget.fromJson(json)
        }
        this.workspace.model.addWidget(widget)

        this.setActiveWidget(widget)

        return widget
    }

    setActiveWidget(widget){
        if(widget == this.activeWidget)
            return
        this.emit("ui.active.changed", widget, this.activeWidget)
        this.activeWidget = widget
        if(widget) {
            this.helper.attach(widget)
        }
        else {
            this.helper.disattach()
        }
    }
    
    viewTools(toolbar) {
        if(!this.tools) {
            this.tools = new UITools(toolbar, this.workspace)
        }
        return this.tools
    }
}

module.exports = UI