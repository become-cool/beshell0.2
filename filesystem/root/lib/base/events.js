beapi.EventEmitter = class EventEmitter {
    _handlers = {}
    /**
     * 
     * @param {string|string[]} event 
     * @param {function} handle 
     * @param {bool} norepeat 
     */
    on(event, handle, norepeat) {
        if(typeof(event)=="string") {
            if(!this._handlers[event]){
                this._handlers[event] = []
                if(event!="#EVENT.ADD#"&&event!="#EVENT.CLEAR#")
                    this.emit("#EVENT.ADD#",event)
            }
            if(!norepeat || !this.isListening(event, handle)) {
                this._handlers[event].push(handle)
            }
        }
        else if(event instanceof Array) {
            for(var eventName of event) {
                this.on(eventName, handle, repeat)
            }
        }
        return this
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
        return this
    }

    /**
     * 
     * @param {string[]} eventName array
     * @param {function} handle 
     */
    race(events, callback) {
        let h = (evt, ...args) => {
            if(!events.includes(evt)) {
                return
            }
            this.off("*", h)
            callback(evt, ...args)
        }
        this.on("*", h)
        return this
    }

    off(eventName, handle, all) {
        if(!this._handlers[eventName]) {
            return
        }
        for(let h=this._handlers[eventName].length-1; h>=0; h--) {
            let func = this.originHanlde(this._handlers[eventName][h])
            if(!handle || handle==func) {
                this._handlers[eventName][h].__origin = null
                this._handlers[eventName].splice(h,1)
                if(handle && !all) {
                    break
                }
            }
        }
        if(!this._handlers[eventName].length) {
            delete this._handlers[eventName]
            if(eventName!="#EVENT.ADD#"&&eventName!="#EVENT.CLEAR#")
                this.emit("#EVENT.CLEAR#",eventName)
        }
        return this
    }

    originHanlde(handle) {
        for(var h=handle; h.__origin; h=h.__origin){}
        return h
    }

    isListening(event, handle) {
        if(!this._handlers[event])
            return false
        for(let cb of this._handlers[event]) {
            if( this.originHanlde(cb)==handle )
                return true
        }
        return false
    }

    emit(eventName, ...args) {
        if(eventName!='*'&&this._handlers[eventName]) {
            for(let handle of this._handlers[eventName]) {
                handle.apply(this, args)
            }
        }
        if(this._handlers["*"]) {
            for(let handle of this._handlers["*"]) {
                handle.apply(this, [eventName, ...args])
            }
        }
        return this
    }

    destroy() {
        for(let eventName in this._handlers) {
            this._handlers[eventName].forEach(cb=>{
                if(cb.__origin) {
                    delete cb.__origin
                }
            })
            this._handlers[eventName].splice(0)
            delete this._handlers[eventName]
        }
    }
}
