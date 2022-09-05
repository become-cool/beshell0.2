const lv = require("lv")

function createCtrlBtn(name, clicked, group) {
    return {
        class: "Btn" ,
        text: lv.symbol[name] ,
        ref: 'btn'+name[0].toUpperCase()+name.slice(1) ,
        clicked ,
        group
    }
}

class Controller extends lv.Column {
    
    constructor(parent, group) {
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
                            createCtrlBtn("prev", ()=>{
                                this.emit("prev")
                            }, group) ,
                            createCtrlBtn("play",()=>{                                
                                this.setPlaying(!this.playing)
                                this.emit(this.playing?"play":"pause")
                            }, group) ,
                            createCtrlBtn("next", ()=>{
                                this.emit("next")
                            }, group) ,
        
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
                        group ,
                        clicked: () => {
                            this.player.menu.show()
                        }
                    } ,
                ]
            }]
        },this)

        beapi.lvgl.Group.focusObj(this.btnPlay)
    }

    setPlaying(playing) {
        this.playing = playing
        this.btnPlay.setText(playing?lv.symbol.pause:lv.symbol.play)
    }
}

module.exports = Controller