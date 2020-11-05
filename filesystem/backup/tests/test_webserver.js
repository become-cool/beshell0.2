var HTTP = require("http");
var FS = require("fs");

var WEBSERVER_PORT = 80;

var server = HTTP.createServer(requestHandler);
server.listen(WEBSERVER_PORT);

log("WebServer listening on port " + WEBSERVER_PORT);

function requestHandler(request, response) {
	log("Heap: "+ ESP32.getState().heapSize);
   request.on("end", function() {
   	switch(request.path) {
   	default:
   		var data = FS.loadFile(request.path);
   		if (data === null) {
   			response.writeHead(404);
   		} else {
   			response.writeHead(200);
   			response.write(data);
   		}
   	}
      response.end();
   }); // on("end")
} // requestHandler