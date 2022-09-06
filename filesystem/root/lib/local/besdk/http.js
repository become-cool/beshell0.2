beapi.mg.get = function(url) {
    return new Promise(function(resolve, reject) {
        let conn = beapi.mg.connect(url, (event,data)=>{
            switch(event) {
                case 'connect':
                    let info = beapi.mg.parseUrl(url)
                    try{
                        if(url.match(/^https:\/\//i)) {
                            conn.initTLS(info.host)
                        }
                    }catch(e) {
                        console.log(e)
                    }
                    
                    conn.send(`GET ${info.uri} HTTP/1.0\r\n`)
                    conn.send(`Host: ${info.host}\r\n`)
                    conn.send(`User-Agent: BeShell\r\n`)
                    conn.send(`Content-Type: application/json\r\n\r\n`)
                    
                    break
                case 'http.msg':
                    resolve(data.body())
                    break
                case 'http.chunk':
                    resolve(data.chunk())
                    break
                case 'timeout':
                case 'error':
                    reject(event)
                    break
            }
        })
    })
}