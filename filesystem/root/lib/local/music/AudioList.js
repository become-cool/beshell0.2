const lv = require("lv")

module.exports = class AudioList extends lv.Column {
    
    constructor(parent) {
        super(parent)
        this.fromJson({
            style: {
                'bg-opa': 255 ,
                'bg-color': 0xFFFF ,
            } ,
            children: [{
                class: "Row" ,
                gap: 5 ,
                crossAlign: "center" ,
                children: [{
                    class: 'Btn', 
                    text: '返回', 
                    textColor: 0 ,
                    font: 'msyh', 
                    ref: 'btnBack' ,
                    style: {
                        'bg-opa': 0 ,
                        "shadow-width": 0 ,
                    } ,
                    clicked: ()=>{ this.hide() }
                },{
                    class: 'Label' ,
                    font: 'msyh',
                    textAlign: 'right' ,
                    text: '' ,
                    grow: true ,
                    style: {
                        'pad-right':10 
                    } ,
                    ref: 'msg'
                }]
            },{
                class: 'List' ,
                width: "100%" ,
                ref: 'uilst' ,
                grow: true ,
                items: [
                    { class: 'Btn', text: ''}
                ]
            }]
        }, this)

        this.loading = false

        this.lst = []

        let group = this.uilst.group()

        group.addObj(this.btnBack)

        this.on("ipt.btn.press",key=>{
            if(key=='down') {
                group.focusNext()
            }
            else if(key=='up'){
                group.focusPrev()
            }
            else if(key=='enter') {
                let btn = group.focused()
                if(btn) {
                    btn.emit("clicked")
                }
            }
        })
    }

    load() {
        if(this.loading) {
            return
        }

        this.loading = true

        this.uilst.clean()
        this.lst.length = 0

        // this.lst.push({title:"demo-8000", path:"/mnt/sd/music4/music-16b-2c-8000hz.mp3"})
        // this.lst.push({title:"demo-22050", path:"/mnt/sd/music4/music-16b-2c-22050hz.mp3"})
        // this.lst.push({title:"demo-44100", path:"/mnt/sd/music4/music-16b-2c-44100hz.mp3"})

        try{
            this.loadDir("/mnt/sd", this.lst, 3)
        }catch(e){
            console.error(e)
        }
        console.log(this.lst.length)
        
        for(let i=0;i<Math.min(this.lst.length,10);i++){
            let item = this.lst[i]
            let btn = this.uilst.addBtn(null, '')

            let title = item.title
            if(title.length>30) {
                title = title.slice(0,30) + '...'
            }

            btn.setText(title)
            btn.setFont("msyh")
            btn.on('clicked', ((i)=>{return ()=>{
                this.player.play(i)
                this.hide()
            }})(i))
        }

        this.loading = false
    }

    loadDir(path, lst, dep) {
        console.log("read", path)
        for(let item of beapi.fs.readdirSync(path,true)||[]) {
            if(item.name=='.' || item.name=='..' || item.name=='System Volume Information' || item.name=='$RECYCLE.BIN') {
                continue
            }
            let filepath = path+'/'+item.name
            if (item.type=='dir') {
                if(dep) {
                    this.loadDir(filepath,lst,dep-1)
                }
                continue
            }

            let res = item.name.match(/^(.+)\.mp3$/i)
            if( res ){
                lst.push({
                    title: res[1] ,
                    path: filepath ,
                })
                continue
            }
        }
    }
}