exports.udp = {
    _handles: {} ,
    onmessage(port, callback) {
        if(!this._handles[port]) {
            beapi.udp.listen(port)
            this._handles[port] = []
        }
        this._handles[port].push(callback)
    } ,
}

beapi.udp.setRecvCallback((message, port, fromAddr, fromPort)=>{
    if(!exports.udp._handles[port]) {
        return
    }
    for(let func of exports.udp._handles[port]) {
        try{
            func(message)
        }catch(e){
            console.log(e)
        }
    }
})