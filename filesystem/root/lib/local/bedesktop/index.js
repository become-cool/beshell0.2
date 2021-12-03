const lv = beapi.lvgl

console.log("bedesktop index.js")



// beapi.lvgl.__lv_obj_init = function () {
//     this._handles={}
//     this.on("#EVENT.ADD#",(eventName)=>{
//         this.enableEvent(eventName)
//     })
//     this.on("#EVENT.CLEAR#",(eventName)=>{
//         this.disableEvent(eventName)
//     })
// }

function main(disp) {

    let screen = disp.activeScreen()

    let btn = new lv.Btn(screen)
    btn.center()

    let txt = new lv.Label(btn)
    txt.setText("hahahaha") 

    global.btn = btn
}


exports.main = main