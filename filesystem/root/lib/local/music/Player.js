const lv = require("lv")
const Controller = require("./Controller.js")

class Player extends lv.Column {
    constructor(parent) {
        super(parent)
        this.fromJson({
            children: [
                {
                    class: "Img" ,
                    ref:"photo" ,
                    src: __dirname + "/img/disk.png"
                } ,
                {
                    class: Controller ,
                    ref: "controller"
                }
            ]
        })
    }
}

module.exports = Player