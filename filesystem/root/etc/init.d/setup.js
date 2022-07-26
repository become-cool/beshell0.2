module.exports = function() {
    if(!be.dev) {
        be.dev = {disp:[], button:[]}
    }
    if(!be.part) {
        be.part = []
    }

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


    try{
        var setupConf = JSON.load("/home/become/config/setup.json")
    } catch(e) {
        console.log("~/config/setup.json not exists, use default setup config.") ;
        var setupConf = defaultSetupConf()
    }
    if(!setupConf) {
        console.log("setup failed: unknown part")
        return
    }
    // serial bus
    if(!process.simulate) {
        for(let num in setupConf.spi||{}){
            let spi = setupConf.spi[num]
            beapi.spi.setup(num, spi.miso, spi.mosi, spi.sck)
        }
        for(let num in setupConf.i2c||{}){
            let i2c = setupConf.i2c[num]
            beapi.i2c.setup(num, i2c.sda, i2c.scl)
        }
    }
    // part
    for(let partConf of setupConf.part||[]){
        try {
            if(partConf.disable) {
                continue
            }
            let part = new createPartFromDriver(partConf)
            if(!part){
                continue
            }
            if(partConf.varname) {
                be.part[partConf.varname] = part
            }
        }catch(e){
            console.log(e)
        }
    }
    return setupConf
}

const LibDefaultConf = {
    1:{0:{
            part:[ { driver: "ledrgb" ,"r": 5 , "g": 9 , "b": 16 , "com": 1 } ]
    }} ,
    3: {0:{
        spi: { 1: {miso:12,mosi:13,sck:14} } ,
        i2c: { 0: {sda:4,scl:5} } ,
        part: [
            {driver:'st7789v', varname:'disp1', setup:{dc:17, cs:15, spi:1, width:320, height:240, freq:80000000, MADCTL: 0x20|0x80, touch:{driver:'xpt2046', spi: 1, cs: 21}}}
            // , {driver:"BeJoypad", setup:{addr:0x33}}
        ]
    }} ,
    19: {0:{
        spi: { 1: {miso:12,mosi:13,sck:14} } ,
        part: [
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

function createDisplayTouch(disp, touchConf) {
    if(!disp || !touchConf) {
        return
    }
    disp.touch = new beapi.lvgl.InDevPointer( touchConf.driver, touchConf.spi, touchConf.cs )
}

function createDisplay(partConf) {
    let part = beapi.lvgl.createDisplay(partConf.driver, partConf.setup||{}) 
    createDisplayTouch(part, partConf.touch)
    if(!partConf.type) { 
        partConf.type = 'display'
    }
    if(partConf.desktop==false) {
        part.desktop = false
    }
    be.dev.disp.push(part)
    return part
}
function createPartFromDriver(partConf) {
    if(process.simulate && !(['virtual-display']).includes(partConf.driver)) {
        return
    }
    if((['st7789v','st7789','virtual-display']).includes(partConf.driver)) {
        return createDisplay(partConf)
    }
    else if(partConf.driver=='sdspi') {
        if(!partConf.setup.cs || !partConf.setup.spi || !partConf.setup.mount) {
            return
        }
        try{
            beapi.driver.mountSD(partConf.setup.spi, partConf.setup.cs, partConf.setup.mount)
            console.log("mount sd to", partConf.setup.mount)
        }catch(e){
            console.log("failed to mount sd")
            console.log(e)
        }
    }
    else if(partConf.driver=="camera") {
        beapi.driver.camera.setup(partConf.setup)
    } else {
        console.log("unknow part driver: ",partConf.driver)

    }
}