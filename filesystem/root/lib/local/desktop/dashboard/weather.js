const api = "http://api.service.become.cool/weather"

async function query() {
    for(let retry = 5 ; retry; retry--) {
        console.log(api)
        try{
            let body = await beapi.mg.get(api)
            body = JSON.parse(body)
            if(body){
                beapi.fs.mkdirSync("/home/become/.data/")
                beapi.fs.writeFileSync("/home/become/.data/weather.json", JSON.stringify(body))
                return body
            }
            return null

        }catch(e) {
            console.log("error:", e)
            await sleep(1000)
        }
    }
    return null
}

function loadCache() {
    return JSON.load("/home/become/.data/weather.json")
}

exports.query = query
exports.loadCache = loadCache