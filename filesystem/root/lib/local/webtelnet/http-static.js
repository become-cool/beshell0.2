module.exports = function(server, uriPattern, rootdir){
    server.route(uriPattern, req=>{


        
        req.end("hi") ;
    })
}