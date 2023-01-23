const lv = require("lv")
const Disk = require('./wave_anim/Disk.js')
const Controller = require("./Controller.js")
const AudioList = require("./AudioList.js")

class PlayerUI extends lv.CleanObj {
    constructor(parent) {
        super(parent)

        console.log(process.memoryUsage().dma)
        // console.log("holdDMA",beapi.driver.holdDMA("audio",20*1024))
        
        this.group = new lv.Group()
        this.loopMethod = "不循环"
        this.player = new beapi.audio.Player

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
                        args: [this.group, this.player] ,
                        props: {
                            top: this ,
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
                        ref: 'btnLst' ,
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
                props: {top:this}
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
        
        this.group.holdKeys(this.screen())

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

        this.player.on("finish",()=>{
            if(this.loopMethod == "循环") {
                setTimeout(()=>this.play(this.currentIndex + 1), 1000)
            }
            else if(this.loopMethod == "单曲循环") {
                setTimeout(()=>this.play(this.currentIndex), 1000)
            }
            else if(this.loopMethod == "不循环") {
                this.waveAnim.stop()
            }
        })

        this.loadList()

        this.currentPath = null
        this.currentIndex = -1

        global.player = this
    }

    async play(idx) {
        
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

        this.currentPath = item.path
        this.currentIndex = idx
        console.log(this.currentPath)

        beapi.driver.releaseDMA("audio")

        await this.stop()
        this.player.playMP3(this.currentPath, true)
        
        console.log(process.memoryUsage())
    }

    stop() {
        return new Promise((resolve)=>{
            if(!this.player.isRunning()) {
                resolve()
                return
            }    
            this.player.once("stop", ()=>{
                resolve()
            })
            this.player.stop()
        })
    }

    pause() {
        this.player.pause()
        this.waveAnim.stop()
    }

    resume() {
        if(this.currentPath) {
            this.player.resume()
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
        process.reboot()

        // this.currentPath = null
        // this.currentIndex = -1
        // this.waveAnim.stop()

        // this.player.stop()
        // this.player.detach()
        // // delete this.player
        // // delete this.ctrl.player

        // lv.loadScreen(be.desktop)
    }
}

module.exports = PlayerUI

_singleton = null
module.exports.singleton = function() {
    if(!_singleton) {
        _singleton = new PlayerUI
    }
    
    lv.loadScreen(_singleton)
    global.app = _singleton
}