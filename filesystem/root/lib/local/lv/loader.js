const MapPropFuncs = {
    // clear: 'removeStyleAll' ,
    align: 'setAlign' ,
    flex: 'setFlexFlow' ,
    flexAlign: 'setFlexAlign' ,
    text: 'setText' ,
    grow: 'setFlexGrow' ,
    center: 'center' ,
    width: 'setWidth' ,
    height: 'setHeight' ,
    font: 'setFont' ,
    value: 'setValue' ,
    src: 'setSrc' ,
    longMode: 'setLongMode' ,
    oneLine: 'setOneLine' ,
    x: 'setX' ,
    y: 'setY' ,
    value: 'setValue' ,
    leftValue: 'setLeftValue' ,
}
const MapPressEvents = {
    pressed:'pressed',
    pressing:'pressing',
    released:'released',
    clicked:'clicked',
}
const MapEvents = {
    valueChanged: 'value-changed',
    __proto__: MapPressEvents ,
}

beapi.lvgl.fromJson = function fromJson(json, parent, refs){
    if(!refs) {
        refs={}
    }
    try{
        if(json instanceof Array) {
            for(let childJson of json) {
                beapi.lvgl.fromJson(childJson, parent, refs)
            }
        }
        else {
            let clz = json["class"] || json.clazz || "Obj"
            if(!clz) {
                throw new Error("missing class name")
            }

            if(typeof clz=='string'){
                if(typeof beapi.lvgl[clz]!='function'){
                    throw new Error("unknow class name: "+clz)
                }
                clz = beapi.lvgl[clz]
            }
            if( typeof clz!="function" ){
                throw new Error("invalid lv obj class: "+(typeof clz))
            }

            var obj = new clz(parent, ...(json.args || []))
            obj.fromJson(json,refs)
        }
    }catch(e){
        console.error(e)
        console.error(e.stack)
    }
    return refs
}

beapi.lvgl.Obj.prototype.fromJson = function fromJson(json, refs){
    if(!refs)
        refs={}
    try{
        if(json instanceof Array) {
            beapi.lvgl.fromJson(json, this, refs)
        }
        else {

            if(json.clear) {
                this.removeStyleAll()
            }

            for(let propName in json) {
                if(MapPropFuncs[propName]) {
                    let methodName = MapPropFuncs[propName]
                    if(typeof this[methodName]=='function') {
                        this[methodName]( json[propName] )
                        continue
                    }
                }
                if(MapEvents[propName] && typeof json[propName]=="function") {
                    this.on(MapEvents[propName], json[propName])
                    if(MapPressEvents[propName]) {
                        this.addFlag("clickable")
                    }
                    continue
                }
                let setter = 'set' + propName[0] + propName.substr(1)
                if(typeof this[setter]=='function') {
                    this[setter](json[propName])
                    continue
                }
            }

            if(json.style) {
                for(let propName in json.style) {
                    this.setStyle(propName, json.style[propName])
                }
            }

            if(json.flag) {
                for(let flag of json.flag) {
                    this.addFlag(flag)
                }
            }

            if(json.bubble) {
                this.addFlag("event-bubble")
            }
            
            if(json.props && typeof json.props=="object") {
                for(let name in json.props) {
                    this[name] = json.props[name]
                }
            }

            if(json.visible!=undefined){
                if(json.visible) {
                    this.show()
                } else {
                    this.hide()
                }
            }

            if(json.children) {
                beapi.lvgl.fromJson(json.children, this, refs)
            }

            if(json.ref) {
                refs[json.ref] = this
            }
        }
        
        this.updateLayout()

    }catch(e){
        console.error(e)
        console.error(e.stack)
    }



    return refs
}
