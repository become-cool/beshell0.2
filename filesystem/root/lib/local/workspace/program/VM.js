class VM extends beapi.EventEmitter {

    cards = []

    constructor() {
        super()
    }

    addCard(card) {
        this.cards.push(card)
        // if(card instanceof CardEvent) {
        //     let type = card.eventType()
        //     if(type) {
        //         if(!this.eventCards[type]) {
        //             this.eventCards[type] = []
        //         }
        //         this.eventCards[type].push(card)
        //     }
        // }
    }

    // emit(eventType, eventName) {
    //     if(!this.eventCards[eventType]) {
    //         return
    //     }
    // }
}

module.exports = VM