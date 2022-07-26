class BeSensorBase {

    constructor(){
        this._handles = {}
        this._handleId = 0
    }

    watch(callback) {
        let hid = this._handleId ++
        this._handles[hid] = callback
        return hid
    }
    unwatch(hid) {
        delete this._handles[hid]
    }
    clearWatch() {
        this._handles.splice(0, this._handles.length)
    }
}


module.exports = BeSensorBase