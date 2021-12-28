
beapi.lvgl.List.prototype.popup = function popup(x , y) {
    this.autoHide = true
}

beapi.lvgl.List.prototype.fromJson = function popup(json, refs) {

    this.autoHide = !!json.autoHide

    if(json.items) {
        for(let item of json.items) {
            if(typeof item=="string") {
                item = {text: item}
            }
            this.addItem(item.icon||null, item.text, item.callback||(!!item.btn))
        }
    }

    return beapi.lvgl.Obj.prototype.fromJson.call(this, json, refs)
}

beapi.lvgl.List.prototype.addItem = function(icon, text, callback) {
    if(!!callback) {
        var item = this.addBtn(icon||null, text||"")
        let label = item.child(item.childCnt).as(lv.Label)
        label.setFont("msyh")
    }
    else {
        var item = this.addText(text||"")
        item.as(lv.Label).setFont("msyh")
    }

    if(callback) {
        item.on("clicked", ()=>{
            if(this.autoHide)
                this.hide()
            if(typeof callback=="function") {
                callback()
            }
            else {
                this.emit("clicked", "clicked", item, text)
            }
        })
    }

    return item
}