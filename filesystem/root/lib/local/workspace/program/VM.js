const {CardStatement} = require("./cards/CardBase")

class VM {

    cards = []
    topStatments = []

    constructor() {
    }

    addCard(card) {
        this.cards.push(card)

        if(card instanceof CardStatement) {
            this.topStatments.push(card)
        }
    }
}

module.exports = VM