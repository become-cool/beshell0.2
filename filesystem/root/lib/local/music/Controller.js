const lv = require("lv")

function createCtrlBtn(name, clicked) {
    return {
        class: "Btn" ,
        text: lv.symbol[name] ,
        ref: 'btn'+name[0].toUpperCase()+name.slice(1) ,
        clicked
    }
}

class Controller extends lv.Column {
    
    constructor(parent) {
        super(parent)

        this.playing = false

        this.fromJson({

            height: 80 ,
            crossAlign: "center" ,
            children: [{
                class: "CleanObj" ,
                height:30 ,
                width: "100%" ,
                children:[{
                    class: "Slider" ,
                    align: "center" ,
                    width: "80%" ,
                    height: 5 ,
                    value: 10 ,
                    style: {
                        "pad-top": 20 ,
                        "pad-bottom": 20 ,
                    } ,
                    ref: "progress" ,
                }]
            }
            , {
                class: "CleanObj" ,
                width: "100%",
                grow: true ,
                children: [
                    {
                        class: "Row" ,
                        align: "center" ,
                        mainAlign: "center" ,
                        gap: 20 ,
                        children: [
                            createCtrlBtn("prev") ,
                            createCtrlBtn("play",()=>{
                                if(this.playing) {
                                    this.playing = false
                                    this.btnPlay.setText(lv.symbol.play)
                                }
                                else {
                                    this.playing = true
                                    this.btnPlay.setText(lv.symbol.pause)
                                }
                            }) ,
                            createCtrlBtn("next") ,
        
                        ]
        
        
                    } ,

                    {
                        class: "Btn" ,
                        align: "right-mid" ,
                        ref: 'btnMore' ,
                        style: {
                            "bg-opa": 0 ,
                            "shadow-width": 0 ,
                        } ,
                        children: [{
                            class: "Img" ,
                            align: "center" ,
                            src: "/lib/icon/16/more.png" ,
                        }] ,
                        clicked: () => {
                            this.player.menu.show()
                        }
                    } ,
                ]
            }]
        },this)
    }
}

module.exports = Controller