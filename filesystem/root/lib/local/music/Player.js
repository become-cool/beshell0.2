const lv = require("lv")
const Disk = require('./wave_anim/Disk.js')
const Controller = require("./Controller.js")
const AudioList = require("./AudioList.js")

module.exports = class Player extends lv.CleanObj {
    constructor(parent) {
        super(parent)
        
        this.group = new lv.Group()
        this.loopMethod = "循环"

        this.fromJson({
            children:[{
                class: "Column" ,
                full: true ,
                crossAlign: "center" ,
                gap: 10 ,
                children: [
                    {
                        class: "CleanObj" ,
                        width: "100%" ,
                        grow: true,
                        children: [
                            {
                                class: Disk ,
                                full: true ,
                                align: "center",
                                ref: 'waveAnim'
                            } ,
                            {
                                class: "Row" ,
                                gap:8 ,
                                align: "top-mid" ,
                                full: true ,
                                style: {
                                    "pad-top":10 ,
                                    "pad-left":10 ,
                                    "pad-right":10 ,
                                } ,
                                children: [
                                    
                                    {
                                        class: "Btn" ,
                                        style: {
                                            "bg-opa": 0 ,
                                            "shadow-width": 0 ,
                                        } ,
                                        children: [{
                                            class: "Img" ,
                                            align: "center" ,
                                            src: "/lib/icon/16/list1.png" ,
                                        }] ,
                                        group: this.group ,
                                        clicked: () => {
                                            this.list.show()
                                        }
                                    } ,

                                    {
                                        class: "Label" ,
                                        text: this.loopMethod ,
                                        font: 'msyh' ,
                                        ref: "labMode" ,
                                        group: this.group ,
                                        clicked: ()=>{
                                            if(this.loopMethod=='不循环') {
                                                this.loopMethod = '循环'
                                            }
                                            else if(this.loopMethod=='循环') {
                                                this.loopMethod = '单曲循环'
                                            }
                                            else if(this.loopMethod=='单曲循环') {
                                                this.loopMethod = '不循环'
                                            }
                                            this.labMode.setText(this.loopMethod)
                                        }
                                    } ,
                                    {
                                        class: "CleanObj" ,
                                        grow: true
                                    } ,
                                    // {
                                    //     class: "Label" ,
                                    //     text: "0:0/0:0" ,
                                    //     ref: 'labProgress' ,
                                    // } ,
                                ] ,
                            } ,
                        ]
                    } ,
                    {
                        class: "Row" ,
                        gap:8 ,
                        children: [
                            {
                                class: "Label" ,
                                font: 'msyh' ,
                                text: "" ,
                                ref: 'labMsg' ,
                                textAlign: "center" ,
                                // longMode: "scroll" ,
                                grow: true
                            } ,
                        ] ,
                    } , 
                    {
                        class: Controller ,
                        ref: "ctrl" ,
                        args: [this.group] ,
                        props: {
                            player: this ,
                        }
                    }
                ]
            },{
                class: "List" ,
                width: 140 ,
                height: "100%" ,
                align: "right-mid" ,
                ref: "menu" ,
                visible: false ,
                modal: true ,
                items: [
                    {
                        class: "Btn" ,
                        text: "关闭菜单" ,
                        font: "msyh" ,
                        clicked: ()=>{
                            this.menu.hide()
                        }
                    } ,
                    {
                        class: "Btn" ,
                        text: "歌曲列表" ,
                        font: "msyh" ,
                        clicked: ()=>{
                            this.list.show()
                            this.menu.hide()
                        }
                    } ,
                    {
                        class: "Btn" ,
                        text: "重新加载歌曲" ,
                        font: "msyh" ,
                        clicked: ()=>{
                            this.loadList()
                            this.menu.hide()
                        }
                    } ,
                    {
                        class: "Btn" ,
                        text: "退出" ,
                        font: "msyh" ,
                        clicked: ()=>{
                            this.menu.hide()
                            this.exit()
                        }
                    } ,
                ]
            },{
                class: AudioList ,
                full: true ,
                visible: false ,
                ref: "list" ,
                modal: true ,
                props: {player:this}
            }]
        },this)

        
        this.styleFocused = new lv.Style({
            "border-width": 1,
            "border-color": lv.palette("green"),
            "radius": 3,
        })
        for(let obj of this.group.allObjs()){
            obj.addStyle(this.styleFocused, 2) // LV_STATE_FOCUSED: 2
        }
        
        this.holdKeys()
        this.on("ipt.btn.press",key=>{
            console.log(key)
            if(key=='left'||key=='up') {
                this.group.focusPrev()
            }
            else if(key=='right'||key=='down'||key=='tab') {
                this.group.focusNext()
            }
            else if(key=='enter') {
                let obj = this.group.focused()
                if(obj) {
                    obj.emit("clicked")
                }
            }
        })

        // 
        this.menu.on("ipt.btn.press", key=>{
            console.log(key)
            let grp = this.menu.group()
            if(key=='left'||key=='up') {
                grp.focusPrev()
            }
            else if(key=='right'||key=='down'||key=='tab') {
                grp.focusNext()
            }
            else if(key=='enter') {
                let obj = grp.focused()
                if(obj) {
                    obj.emit("clicked")
                }
            }
        })

        this.ctrl.on('play',()=>{
            console.log("ctrl resume")
            this.resume()
        })
        this.ctrl.on('pause',()=>{
            console.log("ctrl pause")
            this.pause()
        })
        this.ctrl.on('next',()=>{
            this.play(this.currentIndex+1)
        })
        this.ctrl.on('prev',()=>{
            this.play(this.currentIndex-1)
        })

        beapi.audio.on("finish",()=>{
            if(this.loopMethod == "循环") {
                setTimeout(()=>this.play(this.currentIndex + 1), 1000)
            }
            else if(this.loopMethod == "单曲循环") {
                setTimeout(()=>this.play(this.currentIndex), 1000)
            }
        })

        this.loadList()

        this.currentPath = null
        this.currentIndex = -1

        global.player = this
    }

    play(idx) {
        
        if(this.list.lst.length<=0) {
            console.log("audio list is empty")
            return
        }
        if(idx<0) {
            idx = this.list.lst.length - 1
        }
        else if(idx>=this.list.lst.length) {
            idx = 0
        }


        let item = this.list.lst[idx]

        // console.log("play", item.path)
        this.labMsg.setText("正在播放: "+item.title)

        this.waveAnim.play()
        this.ctrl.setPlaying(true)

        // beapi.audio.stop()

        this.currentPath = item.path
        this.currentIndex = idx
        console.log(this.currentPath)

        beapi.driver.releaseDMA("audio")
        beapi.audio.playMP3(this.currentPath)
    }

    pause() {
        beapi.audio.pause()
        this.waveAnim.stop()
    }

    resume() {
        if(this.currentPath) {
            beapi.audio.resume()
            this.waveAnim.play()
        }
        else {
            this.play(0)
        }
    }

    loadList(){
        this.labMsg.setText("正在加载音乐...")
        setTimeout(()=>{
            this.list.load()
            this.labMsg.setText("音乐单加载完成")
        },500)
    }

    exit() {
        this.currentPath = null
        this.currentIndex = -1
        this.waveAnim.stop()
        this.ctrl.setPlaying(false)
        beapi.audio.deinit()
        lv.loadScreen(be.desktop)
    }
}

