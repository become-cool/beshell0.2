const lv = require("lv")
const WiFiSetting = require("./WiFiSetting.js")
const Versions = require("./Versions.js")

class Setting extends lv.CleanObj {

    constructor(parent) {
        super(parent)

        const itemStyle = {
            "pad-left": 5 ,
            "pad-right": 5 ,
        }
        this.fromJson({
            children: [
                {
                    class: "Row" ,
                    height: "100%" ,
                    style: {
                        "pad-column": 10
                    } ,
                    children: [
                        {
                            class:"List" ,
                            width: 80 ,
                            height: "100%" ,
                            ref: 'menu' ,
                            style: {
                                "pad-left": 0 ,
                                "pad-right": 0 ,
                                'border-width': 0
                            } ,
                            items: [
                                {
                                    text: "版本信息" ,
                                    font: "msyh" ,
                                    clicked: () => {
                                        this.switchSetting('versions')
                                    } ,
                                    style: itemStyle
                                } ,
                                {
                                    text: "WiFi" ,
                                    clicked: () => {
                                        this.switchSetting('wifi')
                                    } ,
                                    style: itemStyle
                                } ,
                                {
                                    text: "返回" ,
                                    font: "msyh" ,
                                    clicked: () => {
                                        be.bus.emit("return-desktop")
                                    } ,
                                    style: itemStyle
                                }
                            ]
                        } ,
                        {
                            height: "100%" ,
                            grow: true ,
                            ref: "pageOutter"
                        }
                    ]
                }
            ]

        }, this)

        this.activePage = null
        this.activePageName = 'wifi'
        this.pages = {
            'wifi': WiFiSetting ,
            'versions': Versions ,
        }

        this.switchSetting(this.activePageName)
    }

    switchSetting(page) {
        if(page==this.activePage) {
            return
        }
        if(this.activePage) {
            this.activePage.hide()
        }
        if( !(this.pages[page] instanceof beapi.lvgl.Obj) ) {
            this.pages[page] = new this.pages[page](this.pageOutter)
        }
        this.activePageName = page
        this.activePage = this.pages[page]
        this.activePage.show()

        if(typeof this.activePage.onActive=='function') {
            this.activePage.onActive()
        }
    }
}
module.exports = Setting