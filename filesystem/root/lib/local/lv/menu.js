class Menu extends beapi.lvgl.Obj {
    static styleItem = new beapi.lvgl.Style({
        "pad-top": 2 ,
        "pad-bottom": 2 ,
    })
    autoHide = true
    indev_cb = null
    popup_cb = null
    items_cb = null
    value = null

    constructor(json) {
        let sys = lv.sysLayer()
        super(sys)
        this.fromJson({
            width: -1 ,
            height: -1 ,
            visible: false ,
            style:{
                "max-height": 200 ,
                "pad": 0 ,
            } ,
            children: [
                {
                    class: lv.CleanObj ,
                    ref: "pressed" ,
                    width: "100%" ,
                    visible: false ,
                    style: {
                        "bg-color": beapi.lvgl.rgb(180) ,
                        "bg-opa": 255 ,
                    }
                } ,
                {
                    class: lv.Column ,
                    ref: "column" ,
                    width: -1 ,
                    height: -1 ,
                    style: {
                        "bg-opa": 0 ,
                        "pad": 8 ,
                    }
                }
            ]
        }, this)

        this.column.on("pressed", ()=>{
            let item = this._itemHitTest()
            if(!item) {
                return
            }
            this.setActiveItem(item)
        })
        this.column.on("clicked", ()=>{
            let item = this._itemHitTest()
            if(!item) {
                return
            }
            this.pressed.hide()
            if(item.callback) {
                item.callback()
            }
            this.emit("clicked", item.value, item)
            if(this.popup_cb) {
                this.popup_cb(item.value)
                this.popup_cb = null
            }
            if(this.autoHide) {
                this.hide()
            }
        })
        
        if(json){
            this.fromJson(json)
        }

        this.indev_cb = ()=>{
            if(!this.hitTest()) {
                this.hide()
            }
        }
    }

    _itemHitTest() {
        let {y} = lv.inputPoint()
        for(let i=0;i<this.column.childCnt();i++)  {
            let item = this.column.child(i)
            let [,_y] = item.coords()
            let _h = item.height()
            if( y>_y && y<_y+_h) {
                return item
            }
        }
    }

    addItem(value, title, callback, font) {
        let item = new beapi.lvgl.Label(this.column, {
            width: -1 ,
            height: -1 ,
            bubble : true,
        })

        item.addStyle(Menu.styleItem)
        item.setText((title==null||title==undefined)? value: title)
        item.callback = callback
        item.value = value
        if(font) {
            item.setFont(font)
        }
        return item
    }

    setActive(value) {
        let item = this.findItem(value)
        if(item) {
            this.setActiveItem(item)
        }
    }
    setActiveByTitle(title) {
        let item = this.findItemByTitle(title)
        if(item) {
            this.setActiveItem(item)
        }
    }
    setActiveItem(item) {
        if(item.parent()!=this.column) {
            throw new Error("item not belongs this list")
        }
        let [x] = this.coords()
        let [,y] = item.coords()
        this.pressed.setHeight(item.height())
        this.pressed.setWidth(this.width()-this.style("border-width"))
        this.pressed.setCoords(x, y)
        this.pressed.show()
        this.value = item.value
    }
    findItem(value) {
        for(let i=0;i<this.column.childCnt();i++)  {
            let item = this.column.child(i)
            if(item.value == value) {
                return item
            }
        }
    }
    findItemByTitle(title) {
        for(let i=0;i<this.column.childCnt();i++)  {
            let item = this.column.child(i)
            if(item.text() == title) {
                return item
            }
        }
    }

    activeTitle() {
        let item = this.findItem(this.value)
        return item? item.text(): null
    }

    clean() {
        this.column.clean()
    }

    popup(x , y, callback) {
        if(this.items_cb) {
            this.clean()
            let items = this.items_cb()
            if(items){
                this.fromItemJson(items)
            }
        }
        if(x==undefined || x==null || y==undefined || y==null) {
            ({x,y} = lv.inputPoint())
        }
        let screen = this.parent()
        if(screen) {
            let mx = screen.width() - this.width()
            let my = screen.height() - this.height()
            if(x>mx) x = mx
            if(y>my) y = my
        }
        
        this.setCoords(x, y)
    
        this.moveForeground()
        this.updateLayout()
        this.show()

        this.popup_cb = callback || null
        lv.on("pressed", this.indev_cb, true)
    }

    fromJson(json, refs) {
        if(json.autoHide!=undefined) {
            this.autoHide = !!json.autoHide
        }
        if(json.items) {
            if(json.items instanceof Array) {
                this.fromItemJson(json.items)
            }
            else if (typeof json.items=="function"){
                this.items_cb = json.items
            }
        }
        return beapi.lvgl.Obj.prototype.fromJson.call(this, json, refs)
    }

    fromItemJson(array) {
        for(let item of array) {
            if(typeof item=="string") {
                item = {value: item}
            }
            this.addItem(item.value, item.title, item.callback, item.font)
        }
    }

    hide() {
        this.popup_cb = null
        lv.off("pressed", this.indev_cb, true)
        super.hide()
    }
}

beapi.lvgl.Menu = Menu