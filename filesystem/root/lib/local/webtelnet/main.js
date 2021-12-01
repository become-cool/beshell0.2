const static = require(__dirname+"/http-static.js")

static(beapi.http.telnet, "/webtelnet", __dirname+"/public")

beapi.http.telnet.route("/", req=>{
    req.rspnStatus("302 Found") ;
    req.rspnHeader("Location", "/webtelnet/") ;
    req.end() ;
})