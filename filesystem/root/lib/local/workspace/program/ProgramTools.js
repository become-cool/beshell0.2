const lv = require('lv')

const cardlib = require("./cards/cardlib")
const CardStack = require("../comm/CardStack")

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
            category.stack = new CardStack(workspace, workspace)
            for(let cardconf of catconf.cards) {
                let card = new cardconf(category.stack)
                category.stack.addCard(card, (cardconf)=>{
                    let card = new cardconf(workspace.program, workspace.model.vm)
                    return card
                }, cardconf)
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