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
    }

    load() {
        console.log("load")
        if(this.loading) {
            return
        }

        this.loading = true

        this.uilst.clean()
        this.lst.length = 0

        this.loadDir("/mnt/sd", this.lst)
        
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

    loadDir(path, lst) {
        for(let filename of beapi.fs.readdirSync(path)||[]) {
            if(filename=='.' || filename=='..') {
                continue
            }
            let filepath = path+'/'+filename
            if (beapi.fs.isDirSync(filepath)) {
                this.loadDir(filepath,lst)
                continue
            }

            let res = filename.match(/^(.+)\.mp3$/i)
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