const lv = require('lv')

module.exports = class Disk extends lv.CleanObj {
    constructor(parent) {
        super(parent)
        this.fromJson({
            grow: true,
            children: [
                {
                    class: "Img" ,
                    ref:"photo" ,
                    align: "center" ,
                    src: __dirname + "/img/disk.png"
                }
            ]
        })
    }
}