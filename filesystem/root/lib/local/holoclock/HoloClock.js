const wifi = require("besdk/wifi")
const lv = require("lv")

const api = "http://api.service.become.cool/weather"

function fill2(num) {
    if(num<10) {
        return '0'+num
    }
    else {
        return num.toString()
    }
}

const days = { 1: '一',2: '二',3: '三',4: '四',5: '五',6: '六',7: '日'}

class HoloClock extends lv.Obj {
    constructor() {
        super()

        this.cityName = ''
        this.weatherCode = ''

        this.fromJson({
            style: {
                "bg-color": lv.rgb(0,0,0) ,
            } ,
            children: [{
                class: 'Column',
                width:"100%",
                height:"100%",
                children: [{
                    class: 'Label' ,
                    text: '00:00:00' ,
                    ref: 'labTime' ,
                    font: "m40" ,
                    style: {
                        'text-color': lv.rgb(255,255,255) ,
                        "pad-left": 20 ,
                        "pad-top": 30 ,
                    }
                } ,
                {
                    class: 'Label' ,
                    text: 'NA月NA日 /星期NA' ,
                    ref: 'labDate' ,
                    font: "msyh" ,
                    style: {
                        "pad-left": 20 ,
                        'text-color': lv.rgb(255,255,255) 
                    }
                } ,
                {
                    class:"Row" ,
                    grow: true ,
                    style: {
                        "pad-top": 10
                    } ,
                    children: [
                        {
                            class: "Column" ,
                            width: 140 ,
                            height: "100%" ,
                            trackAlign: "center" ,
                            children: [
                                { clear: true, grow:true} ,
                                {
                                    class: "Img" ,
                                    width: 64 ,
                                    height: 64 ,
                                    src: "/lib/icon/weather/999.png" ,
                                    ref: "imgWeather" ,
                                } ,
                            ]
                        } ,
                        {
                            class: 'Column' ,
                            height:"100%",
                            width: 100 ,
                            style: {
                                // "pad-row": 10 ,
                            } ,
                            children: [
                                { clear: true, grow:true} ,
                                {
                                    class: 'Label' ,
                                    text: this.cityName ,
                                    font: 'msyh' ,
                                    ref: 'labCity' ,
                                    width: '100%' ,
                                    style: {
                                        'text-color': lv.rgb(255,255,255) ,
                                        "text-align": "right" ,
                                        "pad-right": 20
                                    } ,
                                } ,
                                {
                                    class: 'Label' ,
                                    text: '--°C' ,
                                    font: 'msyh' ,
                                    ref: 'txtTemp' ,
                                    width: '100%' ,
                                    style: {
                                        'text-color': lv.rgb(255,255,255) ,
                                        "text-align": "right",
                                        "pad-right": 20
                                    } ,
                                } ,
                                {
                                    class: 'Label' ,
                                    text: '湿度：' ,
                                    ref: 'txtHumidity' ,
                                    font: 'msyh' ,
                                    width: '100%' ,
                                    style: {
                                        'text-color': lv.rgb(255,255,255) ,
                                        "text-align": "right",
                                        "pad-right": 20
                                    } ,
                                } ,
                            ]
                        }
                    ]
                },
                {
                    class: "Label" ,
                    text: "联网后刷新天气信息" ,
                    font: 'msyh' ,
                    ref:'txtDetail' ,
                    style: {
                        'text-color': lv.rgb(255,255,255) ,
                        "pad-top": 10,
                        "pad-left": 20 ,
                    }
                } ,
                { clear: true, height: 20}]
            }]
        }, this)

        setInterval(()=>{
            this.freshTime()
        }, 1000)

        this.freshTime()
        
        wifi.on("ip.got", async ()=>{
            this.queryWeather()
        })

        setInterval(()=>{
            this.queryWeather()
        }, 30* 60* 1000)

        let json = JSON.load("/home/become/.data/weather.json")
        if(json) {
            this.update(json)
        }

        // 居中显示
        this.setStyle('pad-left', (this.width() - 240)/2)
    }

    freshTime() {
        let tm = new Date(Date.now())
        this.labTime.setText( fill2(tm.getHours()) + ':' + fill2(tm.getMinutes()) + ":" + fill2(tm.getSeconds()) )
        this.labDate.setText( (tm.getMonth()+1) + '月' + tm.getDate() + '日  周' + days[tm.getDay()] )
    }
    
    async queryWeather() {
        for(let retry = 5 ; retry; retry--) {
            console.log(api)
            try{
                let body = await beapi.mg.get(api)
                console.log(body)
                body = JSON.parse(body)
                if(body){
                    this.update(body)
                    beapi.fs.mkdirSync("/home/become/.data/")
                    beapi.fs.writeFileSync("/home/become/.data/weather.json", JSON.stringify(body))
                }
                return

            }catch(e) {
                console.log("error:", e)
                await sleep(1000)
            }
        }
    }

    update(body) {

        this.labCity.setText( body.localText )
        this.txtTemp.setText( body.payload.text + ' ' + parseInt(body.payload.temp)+'°C' )
        this.txtHumidity.setText(`湿度 ${body.payload.humidity}%`)
        this.txtDetail.setText(`${body.payload.windDir} ${body.payload.windScale}级 ${body.payload.windSpeed}km/h`)

        this.imgWeather.setSrc(`/lib/icon/weather/${body.payload.icon}.png`)
    }

    setWeatherImg(imgName) {
        let imgSrc = weatherIconSrc(imgName)
        if(!imgSrc) imgSrc = '999.png'
        this.imgWeather.setSrc("/lib/icon/weather/"+imgSrc)
    }
}

function weatherIconSrc (src) {
    src = src.replace(/\.gif$/i,'')
    console.log(src)

    let d = src[0]
    if(d!='d'&&d!='n') {
        return null
    }
    d = d=='d'
    let w = parseInt(src.slice(1))
    if(isNaN(w)) {
        return null
    }
    if(w>=21&&w<=25) 
        w-= 13
    else if(w>=26&&w<=28) 
        w-= 11

    if(w==0)        return d? '100.png': '150.png'
    if(w==1)        return d? '104.png': '154.png'
    if(w==2)        return '101.png'
    if(w==3)        return d? '300.png': '350.png'
    if(w>=4&&w<=5)  return '303.png'
    if(w==6)        return '405.png'
    if(w>=7&&w<=8)  return '314.png'
    if(w>=9&&w<=10) return '315.png'
    if(w>=11&&w<=12)return '316.png'
    if(w>=13&&w<=14)return '400.png'
    if(w>=15)       return '401.png'
    if(w>=16)       return '402.png'
    if(w<=17)       return '402.png'
    if(w<=18)       return null
    if(w==19)        return '405.png'
    if(w==20)        return null
    
    return null
}

module.exports = HoloClock