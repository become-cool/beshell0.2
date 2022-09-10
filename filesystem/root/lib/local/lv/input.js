
function holdKeys(screen, receiver) {
    screen.disp().keysRouter()
    if(!screen._keysHolders) {
        screen._keysHolders = []
    }
    if(screen._keysHolders[screen._keysHolders.length-1] == receiver) {
        return
    }
    screen._keysHolders.push(receiver)
}
function releaseKeys(screen,receiver) {
    if(!screen._keysHolders) {
        return
    }
    for(let i=screen._keysHolders.length-1;i>=0;i--) {
        if(screen._keysHolders[i] == receiver) {
            screen._keysHolders.splice(i,1)
            if(screen._keysHolders.length==0) {
                delete screen._keysHolders
            }
            return true
        }
    }
    return false
}

beapi.lvgl.Obj.prototype.holdKeys = function() {
    let screen = this.screen()
    if(!screen) {
        throw new Error("no screen to hold keys")
    }
    holdKeys(screen,this)
}
beapi.lvgl.Obj.prototype.releaseKeys = function() {
    let screen = this.screen()
    if(!screen) {
        return true
    }
    return releaseKeys(screen, this)
}

beapi.lvgl.Group.prototype.holdKeys = function(screen) {
    holdKeys(screen, this)
    this.__keysHandler = (key) => {
        if(key=='up') {
            this.focusPrev()
        }
        else if(key=='down'||key=='tab') {
            this.focusNext()
        }
        else if(key=='enter') {
            let obj = this.focused()
            console.log(obj)
            if(obj) {
                obj.emit("clicked")
            }
        }
    }
    this.on("ipt.btn.press", this.__keysHandler)
}
beapi.lvgl.Group.prototype.releaseKeys = function(screen) {
    releaseKeys(screen, this)
    if(this.__keysHandler) {
        this.off("ipt.btn.press", this.__keysHandler)
        this.__keysHandler = null
    }
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