
beapi.lvgl.TextArea.prototype.fromJson = function(json, refs) {
    if(json.useKeyboard) {
        this.on("clicked", ()=>{
            lv.keyboard().popup(this)
        })
    }
    return beapi.lvgl.Obj.prototype.fromJson.call(this, json, refs)
}


beapi.lvgl.TabView.prototype.fromJson = function(json, refs) {
    if(typeof(json.items)=='object') {
        for(let title in json.items) {
            console.log(title)
            let item = this.addTab(title)
            if(json.items[title]) {
                beapi.lvgl.fromJson(json.items[title],item,refs)
            }
        }
    }
    return beapi.lvgl.Obj.prototype.fromJson.call(this, json, refs)
}



beapi.lvgl.List.prototype.fromJson = function(json, refs) {
    if(json.items instanceof Array) {
        for(let itemJson of json.items) {
            let item
            let clz = itemJson.class? itemJson.class.toLowerCase(): 'btn'
            if(clz=='text' || clz=='label') {
                item = this.addText(itemJson.icon||null, itemJson.text)
            }
            else if(clz=='btn') {
                item = this.addBtn(itemJson.icon||null, itemJson.text)
            }
            else {
                console.error("invalid list item class:", itemJson.class)
                continue
            }
            item.fromJson(itemJson, refs)
        }
    }
    return beapi.lvgl.Obj.prototype.fromJson.call(this, json, refs)
}


class Row extends beapi.lvgl.Obj {
    constructor(parent) {
        super(parent)
        this.asRow()
        this.setWidth("100%")
        this.setHeight(-1)
    }
    fromJson (json, refs) {
        if(json.gap) {
            this.setStyle("pad-column", parseInt(json.gap))
        }
        return super.fromJson(json, refs)
    }
}

class Column extends beapi.lvgl.Obj {
    constructor(parent) {
        super(parent)
        this.asColumn()
        this.setWidth("100%")
        this.setHeight(-1)
    }
    
    fromJson (json, refs) {
        if(json.gap) {
            this.setStyle("pad-row", json.gap)
        }
        return super.fromJson(json, refs)
    }
}

beapi.lvgl.Row = Row
beapi.lvgl.Column = Column