
// beapi.lvgl.Obj.prototype.fromJson = function fromJson(json, refs)
beapi.lvgl.List.prototype.fromJson = function(json, refs) {
    beapi.lvgl.Obj.prototype.fromJson.call(this, json, refs)
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
}