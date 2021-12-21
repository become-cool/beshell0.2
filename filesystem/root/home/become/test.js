console.log(__filename)

const lv = require("lv")

conn8 = beapi.mg.httpListen("0.0.0.0:8888", (event,req,rspn)=>{
    console.log("8888", event)
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
        else {
            rspn.reply("hi@ 8888")
        }
    }
    else if(event=="ws.msg" || event=="ws.ctl"){
        console.log(JSON.stringify(req))
        rspn.wsSend(req.data) ;
    }
})
conn6 = beapi.mg.httpListen("0.0.0.0:7777", (event,req,rspn)=>{
    console.log("7777", event)
    rspn.reply("hi@ 7777")
})
