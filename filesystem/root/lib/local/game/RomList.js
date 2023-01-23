module.exports = class RomList extends beapi.lvgl.Column {
    constructor(){
        super()

        this.fromJson({
            children:[
                {
                    class: "Row" ,
                    mainAlign: "end" ,
                    ref: "barTop" ,
                    children: [
                        {
                            class: "Btn" ,
                            text:"退出" , font:"msyh",
                            ref: 'btnExit' ,
                            clicked:()=>{
                                process.reboot()
                                // grp.releaseKeys(this.screen())
                                // beapi.lvgl.loadScreen(be.desktop)
                            }
                        }
                    ]
                } ,
                {
                    class: "List" ,
                    grow: true ,
                    width: "100%",
                    ref: 'lstRom' ,
                    visible: false ,
                } ,
                
                {
                    clear: true ,
                    width: "100%",
                    grow: true ,
                    children: [
                        { 
                            class:"Label" ,
                            font: 'msyh' ,
                            text: "正在加载ROM清单 ..." ,
                            align: "center" ,
                            ref: "txtMsg" ,
                        }
                    ]
                }
            ]
        },this)

        let grp = this.lstRom.group()
        grp.addObj( this.btnExit )

        setTimeout(()=>{
            grp.holdKeys(this.screen())

            this.load()
        }, 0)
    }

    load() {
        this.lstRom.hide()
        this.lstRom.clean()

        this.txtMsg.setText("正在加载ROM清单 ...")
        this.txtMsg.parent().show()

        setTimeout(()=>{

            let romcnt = 0

            walkRomDir("/home/become/game", (path, detail, emulator)=>{
                this.addRom(detail.name.slice(0, -4), path, emulator)
                romcnt ++
            },3)
            walkRomDir("/mnt/sd", (path, detail, emulator)=>{
                this.addRom(detail.name.slice(0, -4), path, emulator)
                romcnt ++
            },4)

            if(romcnt) {
                this.lstRom.show()
                this.txtMsg.parent().hide()
            }
            else {
                this.lstRom.hide()

                this.txtMsg.setText("请将游戏 ROM 存入主机 FLASH 或 SD存储卡")
                this.txtMsg.parent().show()
            }

        }, 0)
    }

    addRom(name, path, emulator) {

        let btn = this.lstRom.addBtn(null, '')
        btn.setText(name)
        btn.setFont("msyh")
        btn.on('clicked', ()=>{
            this.play(path, emulator)
        })
        btn.emulator = emulator
        return btn
    }

    play(romPath, emulator) {
        console.log(romPath)

        let data = JSON.stringify({rom: romPath, emulator})
        if(!beapi.fs.mkdirSync("/home/become/.data", true)){
            console.log("mkdirSync() failed:", "/home/become/.data")
            return
        }
        if(!beapi.fs.writeFileSync("/home/become/.data/game.json", data)){
            console.log("write json file failed:", "/home/become/.data/game.json")
            return
        }

        let filename = romPath.replace(/.+\/([^\/]+)\.[^\.]+/, "$1")

        this.barTop.hide()
        this.lstRom.hide()
        this.txtMsg.parent().show()
        this.txtMsg.setText("正在加载ROM: "+filename)
        
        setTimeout(()=>{
            require("/etc/init.d/boot.js").rebootToApp(__dirname + "/playrom.js", true)
        }, 0)
        
    }
}

function walkRomDir(dir,callback) {
    beapi.fs.walkFile(dir, (path, detail)=>{
        if(path.slice(-4).toLowerCase()==".gbc" || path.slice(-3).toLowerCase()==".gb"){
            callback(path, detail, 2)
        }
        else if(path.slice(-4).toLowerCase()==".nes"){
            callback(path, detail, 1)
        }
    }, 3)
}
