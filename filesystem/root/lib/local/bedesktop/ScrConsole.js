const lv = require("lv")
class ScrConsole extends lv.Obj {
    constructor(parent) {
        super(parent)

        this.setFlexFlow("column")
        
        this.refs = lv.fromJson([
            {
                class: 'Row' ,
                grow: 1 ,
                ref: "row1" ,
                style: {
                    "border-width": 2
                } ,
                bubble: true ,
                width: "100%" ,
                children: [
                    {
                        class:"Obj",
                        grow: 1 ,
                    } ,
                    {
                        class:"Column",
                        height: "100%" ,
                        width: -1 ,
                        bubble: true ,
                        style: {
                            "border-width": 1
                        } ,
                        children: [
                            {class: 'Btn'}
                        ]
                    } ,
                ]
            } ,
            {
                class: 'Row' ,
                ref: "row2" ,
                height: -1 ,
                width: "100%" ,
                bubble: true ,
                style: {
                    "flex-main-place": "space-between"
                } ,
                children: [
                    {
                        class: 'Btn' ,
                        text: 'btn1' ,
                        ref: "btn1" ,
                        width: 100 ,
                        clicked() {
                            console.log("1111")
                        }
                    } ,
                    {
                        class: 'Btn' ,
                        text: 'btn2' ,
                        ref: "btn2" ,
                        clicked() {
                            console.log("2222")
                        }
                    }
                ]
            } ,
        ], this)

        

        this.on("pressing",()=>{
            console.log("pressing")
        })
        this.on("pressed",()=>{
            console.log("pressed")
        })
        this.on("clicked",()=>{
            console.log("clicked")
        })


        for(let name in this.refs) {
            global[name] = this.refs[name]
        }
        
    }
}
module.exports = ScrConsole