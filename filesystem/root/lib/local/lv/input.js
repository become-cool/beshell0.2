
function holdKeys(screen, holder, onkeypress) {
    screen.disp().keysRouter()
    if(!screen._keysHolders) {
        screen._keysHolders = []
    }
    if(screen._keysHolders[screen._keysHolders.length-1] == holder) {
        return
    }
    screen._keysHolders.push(holder)
    if(onkeypress) {
        holder.__keysHandler = onkeypress
        holder.on("ipt.btn.press", onkeypress, false)
    }
}
function releaseKeys(screen,holder) {
    if(holder.__keysHandler) {
        holder.off("ipt.btn.press", holder.__keysHandler)
    }
    if(!screen._keysHolders) {
        return
    }
    for(let i=screen._keysHolders.length-1;i>=0;i--) {
        if(screen._keysHolders[i] == holder) {
            screen._keysHolders.splice(i,1)
            if(screen._keysHolders.length==0) {
                delete screen._keysHolders
            }
            return true
        }
    }
    return false
}

beapi.lvgl.Obj.prototype.holdKeys = function(onkeypress) {
    let screen = this.screen()
    if(!screen) {
        throw new Error("no screen to hold keys")
    }
    holdKeys(screen,this,onkeypress)
}
beapi.lvgl.Obj.prototype.releaseKeys = function() {
    let screen = this.screen()
    if(!screen) {
        return true
    }
    return releaseKeys(screen, this)
}

beapi.lvgl.Group.prototype.holdKeys = function(screen, onkeypress) {
    holdKeys(screen, this, onkeypress || (key => {
        if(key=='up'||key=='left') {
            this.focusPrev()
        }
        else if(key=='down'||key=='right'||key=='tab') {
            this.focusNext()
        }
        else if(key=='enter') {
            this.focused()?.emit("clicked")
        }
    }))
}
beapi.lvgl.Group.prototype.releaseKeys = function(screen) {
    releaseKeys(screen, this)
}

const mapkeys = {
    'a': 'enter' ,
    'start': 'enter' ,
    'b': 'esc' ,
    'select': 'tab' ,
}
class KeysRouter {
    constructor(disp) {
        this.disp = disp
        disp.on("ipt.btn.press", (key)=>{
            let obj = this._findReceiver()
            obj && obj.emit("ipt.btn.press",mapkeys[key]||key)
        })
        disp.on("ipt.btn.release", (key)=>{
            let obj = this._findReceiver()
            obj && obj.emit("ipt.btn.release",mapkeys[key]||key)
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
beapi.lvgl.Display.prototype.keysRouter = function(){
    if(!this._keysRouter) {
        this._keysRouter = new KeysRouter(this)
    }
    return this._keysRouter
}