console.log(__filename)

conn = beapi.mg.httpListen("0.0.0.0:8888", (event,req,rspn)=>{
    console.log(event)
    if(event=='http.msg') {
        console.log(">>>",req.uri())
        req.upgrade(rspn)
    }
    else if(event=="ws.msg" || event=="ws.ctl"){
        console.log(JSON.stringify(req))
    }
})