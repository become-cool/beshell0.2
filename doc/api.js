var http = require('http')
var querystring = require('querystring')

const api_token = '9f4099375217996be775a9d86a55c3d94d8d550c82873b215197fb6f'
const api_host = 'doc.become.cool'
const api_pid = 1

const api_uri_modify = '/api/modify_doc/?token=' + api_token
const api_uri_create = '/api/create_doc/?token=' + api_token

async function post(host, url, data) {

    let body = querystring.stringify(data)

    var options = {
        host: host,
        port: 80,
        method: 'POST',
        path: url,
        headers: {
            'Content-Type': 'application/x-www-form-urlencoded',
            'Content-Length': body.length
        }
    }

    return new Promise(function(resolve, reject) {
        let req = http.request(options, function (res) {
            var result = ''
            res.on('data', function (chunk) {
                result += chunk
            });
            res.on('end', function () {
                console.log(result)
                resolve( JSON.parse(result) )
            });
            res.on('error', function (err) {
                reject(err)
            })
        })

        // req error
        req.on('error', function (err) {
            console.log(err)
        })

        //send request witht the body form
        req.write(body)
        req.end()
    })

}

async function createDocCloud(parent_did, title, doc) {
    return post(api_host, api_uri_create, {
        pid: api_pid ,
        parent_doc:parent_did,
        title, doc
    })
}


function updateCloud(did, title, doc) {
    return post(api_host, api_uri_modify, {
        pid: api_pid ,
        did, title, doc
    })
}


exports.updateCloud = updateCloud
exports.createDocCloud = createDocCloud
exports.api_host = api_host