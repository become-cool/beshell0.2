const lv = require("lv")

class SelectMode extends lv.Obj {
    
    constructor(disp) {
        super()
        
        lv.loadScreen(this)
        this.setStyle("bg-color", lv.rgb(242))

        this.refs = this.fromJson({
            width: "100%" ,
            height: "100%" ,
            clear: true ,
            flex: "column" ,
            style: {
                "pad-row": 16 ,
                "pad-column": 16 ,
                "pad-top": 8 ,
                "pad-left": 8 ,
            } ,
            children: [
                {
                    class: 'Label' ,
                    text: "选择游戏模式" ,
                    font: 'msyh' ,
                    height:-1 ,
                } ,
                {
                    class: 'Row' ,
                    width: "100%" ,
                    height: -1 ,
                    children: [
                        {
                            class: 'Column' ,
                            children: [
                                {
                                    class: 'Img' ,
                                    width: "64" ,
                                    height: "64" ,
                                    "src": "/lib/icon/64/tv.png"
                                } ,
                                {
                                    class: 'Label' ,
                                    text: '主机模式' ,
                                    font: 'msyh' ,
                                    height:-1 ,
                                }
                            ]
                        } ,

                        
                        {
                            class: 'Column' ,
                            children: [
                                {
                                    class: 'Img' ,
                                    width: "64" ,
                                    height: "64" ,
                                    "src": "/lib/icon/64/psp.png"
                                } ,
                                {
                                    class: 'Label' ,
                                    text: '掌机模式' ,
                                    font: 'msyh' ,
                                    height:-1 ,
                                }
                            ]
                        } ,
                        
                        {
                            class: 'Column' ,
                            children: [
                                {
                                    class: 'Img' ,
                                    width: "64" ,
                                    height: "64" ,
                                    "src": "/lib/icon/64/joystick.png"
                                } ,
                                {
                                    class: 'Label' ,
                                    text: '自定义' ,
                                    font: 'msyh' ,
                                    height:-1 ,
                                }
                            ]
                        }
                    ]

                }
            ]
        })
        
    }
}

module.exports = SelectMode