beapi.EventEmitter = class EventEmitter {
    _handles = {}
    /**
     * 
     * @param {string|string[]} event 
     * @param {function} handle 
     * @param {bool} norepeat 
     */
    on(event, handle, norepeat) {
        if(typeof(event)=="string") {
            if(!this._handles[event]){
                this._handles[event] = []
                if(event!="#EVENT.ADD#"&&event!="#EVENT.CLEAR#")
                    this.emit("#EVENT.ADD#",event)
            }
            if(!norepeat || !this.isListening(event, handle)) {
                this._handles[event].push(handle)
            }
        }
        else if(event instanceof Array) {
            for(var eventName of event) {
                this.on(eventName, handle, repeat)
            }
        }
        return
    }

    /**
     * 
     * @param {string} eventName 
     * @param {function} handle 
     */
    once(eventName, handle, norepeat) {
        var wrapper =  (...args) => {
            this.off(eventName, wrapper.__origin)
            handle.apply(this, args)
        }
        wrapper.__origin = this.originHanlde(handle)
        this.on(eventName, wrapper, norepeat)
        return
    }

    off(eventName, handle, all) {
        if(!this._handles[eventName]) {
            return
        }
        for(let h=this._handles[eventName].length-1; h>=0; h--) {
            let func = this.originHanlde(this._handles[eventName][h])
            if(!handle || handle==func) {
                this._handles[eventName][h].__origin = null
                this._handles[eventName].splice(h,1)
                if(handle && !all) {
                    break
                }
            }
        }
        if(!this._handles[eventName].length) {
            delete this._handles[eventName]
            if(eventName!="#EVENT.ADD#"&&eventName!="#EVENT.CLEAR#")
                this.emit("#EVENT.CLEAR#",eventName)
        }
        return
    }

    originHanlde(handle) {
        for(var h=handle; h.__origin; h=h.__origin){}
        return h
    }

    isListening(event, handle) {
        if(!this._handles[event])
            return false
        for(let cb of this._handles[event]) {
            if( this.originHanlde(cb)==handle )
                return true
        }
        return false
    }

    emit(eventName, ...args) {
        if(eventName!='*'&&this._handles[eventName]) {
            for(let handle of this._handles[eventName]) {
                handle.apply(this, args)
            }
        }
        if(this._handles["*"]) {
            for(let handle of this._handles["*"]) {
                handle.apply(this, [eventName, ...args])
            }
        }
        return
    }

    destroy() {
        for(let eventName in this._handles) {
            this._handles[eventName].forEach(cb=>{
                if(cb.__origin) {
                    delete cb.__origin
                }
            })
            this._handles[eventName].splice(0)
            delete this._handles[eventName]
        }
    }
}
