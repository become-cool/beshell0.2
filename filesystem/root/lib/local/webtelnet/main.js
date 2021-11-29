const static = require(__dirname+"/http-static.js")

static(beapi.http.telnet, "/webtelnet/*", __dirname+"/public")

beapi.http.telnet.route("/", req=>{
    req.end("this is root") ;
})