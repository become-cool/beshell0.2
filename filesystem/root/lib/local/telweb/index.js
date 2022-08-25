function handler(event,req,rspn) { 
    if(event=='http.msg') {
        if(req.matchURI("/display/list")) {
            let json = []
            for(let disp of be.disp) {
                let jsonDisp = {width:disp.width(),height:disp.height()}
                if(disp==be.desktop) {
                    jsonDisp.desktop = true
                }
                json.push(jsonDisp)
            }
            rspn.reply(JSON.stringify(json))
        }
        else {
            // console.log("unhandle http request:",req.header("Host"),req.uri())
            rspn.close()
        }
        return ;
    }
    else {
        // console.log("unhandle telweb request:", event, req, rspn)
    }
}

function start(addr) {
    let server = beapi.mg.httpListen(addr, handler)
    server.startTelweb()
    return server
}

exports.start = start