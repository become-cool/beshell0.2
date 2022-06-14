
beapi.lvgl.Obj.prototype.holdKeys = function() {
    let screen = this.screen()
    if(!screen._keysHolders) {
        screen._keysHolders = []
    }
    if(screen._keysHolders[screen._keysHolders.length-1] == this) {
        return
    }
    screen._keysHolders.push(this)
}
beapi.lvgl.Obj.prototype.releaseKeys = function() {
    let screen = this.screen()
    if(!screen._keysHolders) {
        return
    }
    for(let i=screen._keysHolders.length-1;i>=0;i--) {
        if(screen._keysHolders[i] == this) {
            screen._keysHolders.splice(i,1)
            if(screen._keysHolders.length==0) {
                delete screen._keysHolders
            }
            return true
        }
    }
    return false
}

class KeysRouter {
    constructor(disp) {
        this.disp = disp
        disp.on("ipt.btn.press", (key)=>{
            let obj = this._findReceiver()
            obj && obj.emit("ipt.btn.press",key)
        })
        disp.on("ipt.btn.release", (key)=>{
            let obj = this._findReceiver()
            obj && obj.emit("ipt.btn.release",key)
        })
    }
    _findReceiver() {
        let screen = this.disp.activeScreen()
        if(!screen || !screen._keysHolders) {
            return null
        }
        return screen._keysHolders[screen._keysHolders.length-1]
    }
}
beapi.lvgl.KeysRouter = KeysRouter