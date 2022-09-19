const lv = require("lv")

module.exports = class AudioList extends lv.Column {
    
    constructor(parent) {
        super(parent)
        this.page = 1 ;
        this.pageCnt = 1 ;
        this.itemsPerPage = 8 ;
        this.itemHeight = 24
        this.fromJson({
            style: {
                'bg-opa': 255 ,
                'bg-color': 0xFFFF ,
            } ,
            children: [{
                class: "Row" ,
                gap: 5 ,
                crossAlign: "center" ,
                style: {
                    'pad-column': 10
                }
                , children: [{
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
                }
                , {clear: true, grow: true, height:1}
                , {
                    class:"Btn", text: "<", ref: 'btnPrevPage',
                    clicked: () => {
                        this.loadPage(this.page-1)
                    }
                }
                , {
                    class: 'Label' ,
                    text: this.page + '/' + this.pageCnt ,
                    // width: -1 ,
                    grow: true ,
                    ref: 'txtPage'
                }
                , {
                    class:"Btn", text: ">", ref: 'btnNextPage',
                    clicked: () => {
                        this.loadPage(this.page+1)
                    }
                }]
                
            },{
                class: 'List' ,
                width: "100%" ,
                ref: 'uilst' ,
                grow: true ,
                items: []
            }]

            , on:{
                "ipt.btn.press": key=>{
                    console.log(key)
                    if(key=='up') {
                        group.focusPrev()
                    }
                    else if(key=='down' || key=='tab') {
                        group.focusNext()
                    }
                    else if(key=='left') {
                        this.loadPage(this.page-1)
                    }
                    else if(key=='right') {
                        this.loadPage(this.page+1)
                    }
                    else if(key=='enter') {
                        group.focused()?.emit("clicked")
                    }
                    else if(key=='esc') {
                        this.hide()
                    }
                }
            }
        }, this)

        this.loading = false

        this.lst = []

        let group = this.uilst.group()
        group.addObj(this.btnBack)
        group.addObj(this.btnPrevPage)
        group.addObj(this.btnNextPage)

        let self = this
        this.onItemClick = function () {
            self.top.play(this.idx)
            self.hide()
        }
    }

    load() {
        if(this.loading) {
            return
        }

        this.loading = true

        this.lst.length = 0

        try{
            this.loadDir("/mnt/sd", this.lst, 3)
        }catch(e){
            console.error(e)
        }

        this.page = 1
        this.pageCnt = Math.ceil(this.lst.length/this.itemsPerPage)

        // beapi.lvgl.setAllocSPIRAM(true)
        
        this.loadPage(1)

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

    loadPage(page) {
        if(page>this.pageCnt) {
            page = 1
        }
        else if(page<1) {
            page = this.pageCnt
        }

        this.page = page
        this.txtPage.setText(this.page + '/' + this.pageCnt)

        let dma = process.memoryUsage().dma.free

        let from = this.itemsPerPage*(page-1)
        let to = Math.min( this.itemsPerPage*page, this.lst.length )
        console.log(from, to)
        for(let i=from; i < to; i++ ){
            let item = this.lst[i]
            let btn = this.btn(i-from)

            btn.setText(item.title)
            btn.idx = i
            btn.show()
        }

        for(let i=to; i<10; i++) {
            this.btn(i,false)?.hide()
        }

        // beapi.lvgl.setAllocSPIRAM(false)

        console.log(dma-process.memoryUsage().dma.free)
    }

    btn(i) {
        if(i>=this.uilst.childCnt()) {
            let btn = this.uilst.addText('')
            btn.setWidth(this.uilst.width()-4)
            btn.setHeight(this.itemHeight)
            btn.setLongMode("dot")
            btn.setFont("msyh")
            btn.addFlag("clickable")
            btn.on('clicked', this.onItemClick)
            return btn
        }
        else {
            return this.uilst.child(i)
        }
    }

    random() {
        
    }
}