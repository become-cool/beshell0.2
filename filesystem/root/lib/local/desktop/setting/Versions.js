const lv = require("lv")
const wifi = require("besdk/wifi")

const PartNames = {
    1: "BeCore16" ,
    2: "BeCore32" ,
    3: "BePad" ,
    19: "BeCamera" ,
}

class Versions extends lv.Column {

    constructor(parent) {
        super(parent)
        
        let json = {
            width: "100%" ,
            style: {
                "pad-row": 10,
                "pad-top": 20
            } ,
            children: []
        }
        function addVersionItem(title, version) {
            json.children.push({
                class: "Row" , width: "100%" , height: "-1" ,
                children: [{
                    class: "Label" ,
                    text: title ,
                    font: "m14" ,
                    grow: true ,
                } ,
                {
                    class: "Label" ,
                    font: "m12" ,
                    text: version
                }]
            })
        }
        addVersionItem(PartNames[beapi.utils.partId()] || "Unknow Part", 'VER ' + beapi.utils.partVersion())
        addVersionItem("BeShell", process.versions.beshell)
        addVersionItem("QuickJS", process.versions.quickjs)
        addVersionItem("LVGL", process.versions.lvgl)
        addVersionItem("ESP-IDF", process.versions["esp-idf"]||"not supported")
        addVersionItem("Build", process.build)
        this.fromJson(json, this)

    }
}
module.exports = Versions