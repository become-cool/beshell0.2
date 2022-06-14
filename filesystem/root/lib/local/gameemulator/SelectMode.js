const lv = require("lv")

class SelectMode extends lv.Obj {
    
    constructor(disp) {
        super()
        
        this.group = 

        this.setStyle("bg-color", lv.rgb(242))
        this.fromJson({
            width: "100%" ,
            height: "100%" ,
            clear: true ,
            flex: "column" ,
            style: {
                "pad-row": 30 ,
                "pad-top": 20 ,
                "pad-left": 20 ,
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
                    height: 100 ,
                    ref: "row" ,
                    style: {
                        "pad-column": 30 ,
                        "flex-main-place": "center" ,
                    } ,
                    children: [
                        {
                            class: ModeItem ,
                            text: '主机模式' ,
                            iconSelected: '/lib/icon/48/tv.png' ,
                            iconUnselected: '/lib/icon/48/tv-gray.png' ,
                            ref: 'mode-tv' ,
                        } ,
                        {
                            class: ModeItem ,
                            text: '掌机模式' ,
                            iconSelected: '/lib/icon/48/psp.png' ,
                            iconUnselected: '/lib/icon/48/psp-gray.png' ,
                            ref: 'mode-psp' ,
                        } ,
                        {
                            class: ModeItem ,
                            text: '自定义' ,
                            iconSelected: '/lib/icon/48/joystick.png' ,
                            iconUnselected: '/lib/icon/48/joystick-gray.png' ,
                            ref: 'mode-custom' ,
                        }
                    ]

                }
            ]
        }, this)
        
        lv.loadScreen(this)

        let allModes = ['tv', 'psp', 'custom']
        this.setMode("tv")

        this.holdKeys()
        this.on("ipt.btn.press", key=>{
            if(key=='left') {
                let i = allModes.indexOf(this.mode) - 1
                if(i<0) {
                    i = allModes.length-1
                }
                this.setMode(allModes[i])
            } else if(key='right') {
                let i = allModes.indexOf(this.mode) + 1
                if(i>=allModes.length) {
                    i=0
                }
                this.setMode(allModes[i])
            }
            else if(key=='a' || key=='enter') {
                this.run(this.mode)
            }
        })

        global.game = this
    }

    setMode(name) {
        this["mode-tv"].setSelected(name=='tv')
        this["mode-psp"].setSelected(name=='psp')
        this["mode-custom"].setSelected(name=='custom')
        this.mode = name
    }

    run(mode) {

    }
}

class ModeItem extends lv.Column {
    fromJson(json, refs) {
        if(json.ref && refs) {
            refs[json.ref] = this
            delete json.ref
        }
        super.fromJson({...json,
            width: 70 ,
            style: {
                "pad-row": 10 ,
                "flex-cross-place": "center" ,
            } ,
            children: [
                {
                    clear: true ,
                    width: "48" ,
                    height: "48" ,
                    children: [
                        {
                            class: 'Img' ,
                            "src": json.iconSelected ,
                            y:0 ,
                            ref: 'iconSelected' ,
                        } ,
                        {
                            class: 'Img' ,
                            y:0 ,
                            "src": json.iconUnselected ,
                            ref: 'iconUnselected' ,
                        } ,
                    ]
                },
                {
                    class: 'Label' ,
                    text: json.text ,
                    "text-align": "center" ,
                    font: 'msyh' ,
                    height:-1 ,
                }
            ]
        }, this)
    }
    setSelected(isSelected) {
        this.iconSelected.setVisible(isSelected)
        this.iconUnselected.setVisible(!isSelected)
    }
}

module.exports = SelectMode