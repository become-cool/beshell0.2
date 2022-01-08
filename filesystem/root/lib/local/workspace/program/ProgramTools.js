const lv = require('lv')
const CardStack = require("../comm/CardStack")

const cardlib = [
    {
        name: 'basic' ,
        icon: 'If' ,
        cards: require("./cards/basic.js")
    } ,
    {
        name: 'parts' ,
        icon: 'H' ,
        cards: [] ,
    } ,
    {
        name: 'UI' ,
        icon: 'ui' ,
        cards: require("./cards/ui.js") ,
    } ,
]

class ProgramTools extends lv.Column{
    
    categories = {}
    activeCategory = null

    constructor(parent, workspace) {
        super(parent)

        this.refs = this.fromJson({
            width: "100%" ,
            height: -1 ,
            align: "top-mid" ,
            style: {
                "pad-row": 5 ,
                "flex-cross-place": "center" ,
            }, 
            children: []
        })

        let self = this
        for(let catconf of cardlib) {
            let category = new lv.Label(this, {
                text: catconf.icon ,
                font: 'm12' ,
                clicked() {
                    if(this.stack.toggle()) {
                        if(self.activeCategory) {
                            self.activeCategory.stack.hide()
                        }
                        self.activeCategory = this
                    }
                    else {
                        self.activeCategory = null
                    }
                }
            })
            category.stack = new CardStack(workspace)
            for(let name in catconf.cards) {
                let clz = catconf.cards[name]
                let card = new clz(category.stack, workspace.program)
                category.stack.addCard(card, (clz)=>{
                    self.activeCategory = null
                    let card = new clz(workspace.program, workspace.program)
                    return card
                }, clz)
            }
            // category.stack.createCard(catconf.name)s
            category.stack.setStyle("pad-left",10)

            this.categories[catconf.name] = category
        }
    }

    close() {
        if(this.activeCategory) {
            this.activeCategory.stack.hide()
        }
    }
}

module.exports = ProgramTools