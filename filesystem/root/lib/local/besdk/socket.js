exports.udp = {
    _handlers: {} ,
    onmessage(port, callback) {
        if(!this._handlers[port]) {
            beapi.udp.listen(port)
            this._handlers[port] = []
        }
        this._handlers[port].push(callback)
    } ,
}

beapi.udp.setRecvCallback((message, port, fromAddr, fromPort)=>{
    if(!exports.udp._handlers[port]) {
        return
    }
    for(let func of exports.udp._handlers[port]) {
        try{
            func(message)
        }catch(e){
            console.log(e)
        }
    }
})