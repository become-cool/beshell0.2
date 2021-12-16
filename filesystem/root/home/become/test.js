console.log(__filename)

conn = beapi.mg.httpListen("0.0.0.0:8888", (event,req,rspn)=>{
    if(event=='http.msg') {
        // console.log(JSON.stringify(req.allHeaders()))
        // console.log(event, req.uri())
        if( req.uri()=="/ws" ) {
            console.log("upgrade")
            rspn.upgrade(req)
        }
        else if( req.uri()=="/dir" ){
            rspn.serveDir(req, __dirname)
        }
    }
    else if(event=="ws.msg" || event=="ws.ctl"){
        console.log(JSON.stringify(req))
        rspn.wsSend(req.data) ;
    }
})