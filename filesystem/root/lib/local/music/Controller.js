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
    
    constructor(parent, group, player) {
        super(parent)

        this.player = player

        this.fromJson({
            height: 80 ,
            crossAlign: "center" ,
            children: [
            // {
            //     class: "CleanObj" ,
            //     height:30 ,
            //     width: "100%" ,
            //     children:[{
            //         class: "Slider" ,
            //         align: "center" ,
            //         width: "80%" ,
            //         height: 5 ,
            //         value: 10 ,
            //         style: {
            //             "pad-top": 20 ,
            //             "pad-bottom": 20 ,
            //         } ,
            //         ref: "progress" ,
            //     }]
            // } ,
            {
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
                                this.emit(this.isPlaying()?"pause":"play")
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
                            this.top.menu.show()
                        }
                    } ,
                ]
            }]
        },this)

        beapi.lvgl.Group.focusObj(this.btnPlay)

        this.updateStatus()
        player.on("*", (ev)=>{
            console.log(ev)
            this.updateStatus()
        })
    }
    
    isPlaying() {
        return this.player.isRunning() && !this.player.isPaused()
    }

    updateStatus() {
        if(this.isPlaying()) {
            console.log("lv.symbol.pause")
            this.btnPlay.setText(lv.symbol.pause)
        }
        else {
            console.log("lv.symbol.play")
            this.btnPlay.setText(lv.symbol.play)
        }
    }

}

module.exports = Controller