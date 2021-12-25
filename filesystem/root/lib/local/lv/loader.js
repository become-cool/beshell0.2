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
}
const ArrEvents = [
    'clicked'
]

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
            let clzname = json["class"] || json.clazz || "Obj"
            if(!clzname) {
                throw new Error("missing class name")
            }

            if( typeof clzname=="function" ){
                var obj = new clzname(parent)
            } else {
                if(!beapi.lvgl[clzname]){
                    throw new Error("unknow class name: "+clzname)
                }
                var obj = new beapi.lvgl[clzname](parent)
            }

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
                    }
                }
                if(ArrEvents.includes(propName) && typeof json[propName]=="function") {
                    this.on(propName, json[propName])
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

            if(json.children) {
                beapi.lvgl.fromJson(json.children, this, refs)
            }

            if(json.ref) {
                refs[json.ref] = this
            }
        }
    }catch(e){
        console.error(e)
        console.error(e.stack)
    }
    return refs
}
