const lv = require("lv")
const Disk = require('./wave_anim/Disk.js')
const Controller = require("./Controller.js")
const AudioList = require("./AudioList.js")

module.exports = class Player extends lv.CleanObj {
    constructor(parent) {
        super(parent)
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
                                        clicked: () => {
                                            this.list.show()
                                        }
                                    } ,

                                    {
                                        class: "Label" ,
                                        text: "循环" ,
                                        font: 'msyh' ,
                                        ref: "labMode"
                                    } ,
                                    {
                                        class: "CleanObj" ,
                                        grow: true
                                    } ,
                                    {
                                        class: "Label" ,
                                        text: "0:0/0:0" ,
                                        ref: 'labProgress' ,
                                    } ,
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
                                text: "正在加载音频列表 ..." ,
                                ref: 'labMsg' ,
                                textAlign: "center" ,
                                grow: true
                            } ,
                        ] ,
                    } , 
                    {
                        class: Controller ,
                        ref: "ctrl" ,
                        props: {
                            player: this
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
                    } ,
                    {
                        class: "Btn" ,
                        text: "重新加载歌曲" ,
                        font: "msyh" ,
                    } ,
                    {
                        class: "Btn" ,
                        text: "退出" ,
                        font: "msyh" ,
                        clicked: ()=>{
                            this.menu.hide()
                            lv.loadScreen(be.desktop)
                        }
                    } ,
                ]
            },{
                class: AudioList ,
                full: true ,
                visible: false ,
                ref: "list" ,
                props: {player:this}
            }]
        },this)

        setTimeout(()=>{
            this.list.load()
        },0)
    }

    play(idx) {
        console.log(idx, this.list.lst.length)
        if(this.list.lst.length<=idx || idx<0) {
            return
        }
        let item = this.list.lst[idx]

        console.log("play", item.path)
        this.labMsg.setText("正在播放："+item.title)
    }
}

