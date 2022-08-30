module.exports = function() {
    try{
        let partJson = JSON.load("/home/become/config/part.json")
        if(partJson.id!=undefined) {
            beapi.utils.primitivePartId = beapi.utils.partId
            beapi.utils.partId = function() { return partJson.id }
        }
        if(partJson.version!=undefined) {
            beapi.utils.primitivePartVersion = beapi.utils.partVersion
            beapi.utils.partVersion = function() { return partJson.version }
        }
    }catch(e) {}


    if(!process.simulate) {
        try{
            var setupConf = JSON.load("/home/become/config/setup.json")
        } catch(e) {
            console.log("~/config/setup.json not exists or invalid, use default setup config.") ;
            var setupConf = defaultSetupConf()
        }
        if(!setupConf) {
            console.error("setup failed: unknown part")
            return
        }
        // serial bus
        for(let num in setupConf.spi||{}){
            let spi = setupConf.spi[num]
            beapi.spi.setup(num, spi.miso, spi.mosi, spi.sck)
        }
        for(let num in setupConf.i2c||{}){
            let i2c = setupConf.i2c[num]
            beapi.i2c.setup(parseInt(num), i2c.sda, i2c.scl)
        }
        for(let num in setupConf.i2s||{}){
            let opts = setupConf.i2s[num]
            beapi.i2s.setup(parseInt(num), opts)
        }

        // dev
        for(let devConf of setupConf.dev||[]){
            try {
                if(devConf.disable) {
                    continue
                }
                createDevFromDriver(devConf)
            }catch(e){
                console.log(e)
            }
        }
        return setupConf
    }
}

const LibDefaultConf = {
    1:{0:{
            dev:[ { driver: "led-rgb" ,"r": 5 , "g": 9 , "b": 16 , "com": 1 } ]
    }} ,
    3: {0:{
        spi: { 1: {miso:12,mosi:13,sck:14} } ,
        dev: [
            {"driver":"ST7789V", "setup":{"dc":18, "cs":19, "spi":1, "width":320, "height":240, "freq":60000000, "MADCTL": 0x40|0x20}}
            , {"driver":"XPT2046", "setup":{spi:1, cs:21, invX:true, invY:true, maxX:320, maxY:240, offsetX:11}}
            
        ]
    }} ,
    19: {0:{
        spi: { 1: {miso:12,mosi:13,sck:14} } ,
        dev: [
            {driver:'sdspi',setup:{cs:25,spi:1,mount:'/mnt/sd'}} ,
            {driver:'camera',setup:{d0:39,d1:37,d2:36,d3:38,d4:34,d5:19,d6:9,d7:22,xclk:5,pclk:35,vsync:23,href:27,sda:21,scl:18,pwdn:-1,reset:-1,ledc_channel:1,ledc_timer:1,jpeg_quality:10,fb_count:2,xclk_freq:20000000,size:"QVGA",format:"jpeg"}}
        ]
    }}
}

function defaultSetupConf() {
    let partId = beapi.utils.partId()
    let partVersion = beapi.utils.partVersion()
    return LibDefaultConf[partId] && LibDefaultConf[partId][partVersion]
}

function createDevFromDriver(devConf) {
    try{
        console.log("/lib/local/besdk/driver/"+devConf.driver+'.js')
        var driver = require("/lib/local/besdk/driver/"+devConf.driver+'.js')
    }catch(e){
        console.error(e)
        console.error("unknow driver", devConf.driver)
    }
    try {
    let dev = new driver
        if(dev.setup(devConf.setup)==false) {
            console.error(devConf.driver,'device setup failed.')
            return
        }
        if(dev.begin(devConf.begin)==false) {
            console.error(devConf.driver,'device begin failed.')
            return
        }
        dev.register(devConf.varname)
        return dev
    }catch(e) {
        console.error(e)
    }
}