// Weathers.co
//
var http = require("http.js");

function http_test1() {
	var host = "weathers.co"; // httpbin.org
	log("Host: " + host);
	if (host === null) {
		return;
	}
	function logHTTP(obj) {
		log("HTTP Status Code: " + obj.httpStatus);
		log("Headers: " + JSON.stringify(obj.headers));
	}

	http.request({
		host : host,
		port : 80,
		path : "/api.php?city=Dallas"
	}, function(response) {
		log("HTTP Response ready ...");
		response.on("data", function(data) {
			log("**************")
			log("Response data:");
			logHTTP(response);
			log(data);
			log("**************")

		});
		response.on("end", function() {
			log("**********************")
			log("*** Response complete");
			logHTTP(response);
			log("**********************")
		})
	});
}

http_test1();
