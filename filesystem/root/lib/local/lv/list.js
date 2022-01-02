class Menu extends beapi.lvgl.Column {

    autoHide = true

    constructor(parent) {
        super(parent)
        this.setWidth(-1)
        this.setHeight(-1)
        this.setStyle("min-width", 80)
        this.setStyle("max-height", 200)
        this.hide()

        // lv.on("pressed", ()=>{
        //     if(autoHide) {
        //         this.hide()
        //     }
        // }) 
    }

    addItem(title, callback, font) {
        let item = new beapi.lvgl.Label(this, {
            width: -1 ,
            clicked: ()=>{
                if(callback) {
                    callback()
                }
                if(autoHide) {
                    this.hide()
                }
                this.emit("clicked", title, item)
            }
        })
        item.setText(title)
        if(font) {
            item.setFont(font)
        }
        return item
    }

    popup(x , y) {
        let screen = this.parent()
        if(screen) {
            let mx = screen.width() - this.width()
            let my = screen.height() - this.height()
            console.log(mx, my)
            if(x>mx) x = mx
            if(y>my) y = my
        }
        
        this.setCoords(x, y)
    
        this.moveForeground()
        this.updateLayout()
        this.show()
    }

    fromJson(json, refs) {
        this.autoHide = !!json.autoHide
        if(json.items) {
            for(let item of json.items) {
                if(typeof item=="string") {
                    item = {text: item}
                }
                this.addItem(item.text, item.callback, item.font)
            }
        }
        return beapi.lvgl.Obj.prototype.fromJson.call(this, json, refs)
    }
}

beapi.lvgl.Menu = Menu