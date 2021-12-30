const lv = require('lv')


class WidgetHelper extends lv.CleanObj{

    target = null

    constructor(parent) {
        super(parent)
        global.helper = this

        this.clearFlag("clickable")

        this.fromJson({
            visible : false ,
            style: {
                // "border-width": 1 ,
                // "border-color": lv.palette("blue") ,
                // "bg-color": lv.palette("grey") ,
            } ,
            children: [
                // {
                //     class: 'Obj' ,
                // }
            ]
        })

        // this.draggable(null, (pos)=>{
        //     this.target.setCoords(pos.x, pos.y)
        // })
    }

    attach(widget) {
        if(this.target) {
            if(this.target.shadow) {
                this.target.shadow.removeStyle(WidgetShadow.styleActive)
            }
        }
        this.setWidth(widget.width())
        this.setHeight(widget.height())
        this.setCoords( ... widget.coords() )
        this.target = widget
        
        if(widget.shadow) {
            widget.shadow.addStyle(WidgetShadow.styleActive)
        }

        this.show()
    }

    disattach() {
        if(this.target) {
            if(this.target.shadow) {
                this.target.shadow.removeStyle(WidgetShadow.styleActive)
            }
        }
        this.hide()
    }
}


class WidgetShadow extends lv.CleanObj{
    target = null
    constructor(widget, ui) {
        super(ui.shadow)
        this.target = widget
        this.fromJson({
            style: {
                // "bg-color": lv.palette("blue") ,
                // "bg-opa": 100 ,
            }
        })

        this.setHeight(widget.height())
        this.setWidth(widget.width())
        this.draggable(null, pos=>{
            widget.setCoords(pos.x, pos.y)
            if(ui.activeWidget==widget){
                ui.helper.setCoords(pos.x, pos.y)
            }
        })
        this.on("clicked",()=>{
            ui.setActiveWidget(widget)
        })
    }
    setCoords(x, y) {
        super.setCoords(x,y)
        this.target.setCoords(x,y)
    }
    
    static styleActive = new lv.Style({
        "border-width": 1 ,
        "border-color": lv.palette("yellow") ,
        "border-opa": 100 ,
        "shadow-color": lv.palette("yellow") ,
        "shadow-spread": 1 ,
        "shadow-width": 1 ,
    })
}


exports.WidgetHelper = WidgetHelper
exports.WidgetShadow = WidgetShadow