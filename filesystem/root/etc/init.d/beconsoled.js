beapi.telnet.callback = function(funcName, ...arglst){
    for(let i=0; i<arglst.length; i++) {
        arglst[i] = JSON.stringify(arglst[i])
    }
    beapi.telnet.rspn(0, 6, `${funcName}(${arglst.join(',')})`)
}

if(!process.simulate) {
    require("besdk/pin.js")
    
    let __debounce_timers = {}
    watchPins((gpio)=>{
        if(__debounce_timers[gpio]!=undefined) {
            return
        }
        __debounce_timers[gpio] = setTimeout(()=>{
            delete __debounce_timers[gpio]
            beapi.telnet.callback('EmitPinChanged', gpio, beapi.gpio.digitalRead(gpio))
        }, 20)
    })
}

global.beconsoled = {}


function loadApp(folder) {
    try{ return JSON.load(folder+'/package.json') }
    catch(e) { return null }
}
beconsoled.loadApp = loadApp
beconsoled.loadAppLib = function(libpath) {
    if(!libpath) {
        libpath = "/home/become"
    }
    let applst=[]
    for(let item of beapi.fs.readdirSync(libpath) ){
        if(item=='.'||item=='..') {
            continue
        }
        let folder = libpath + "/" +item
        let app = loadApp(folder)
        if(!app) {
            continue
        }

        applst.push({ local: folder, name: app.name, title: app.title, icon: app.icon })
    }
    return applst
}
function genAppPath(name, uuid) {
    let path = '/home/become/'+name
    if(!beapi.fs.existsSync(path)) {
        return path
    }
    uuid = uuid.replace(/\-/g,'')
    for(let len=4; len<uuid.length; len++) {
        let p = path + '-' + uuid.substr(0,len)
        if(!beapi.fs.existsSync(p)) {
            return p
        }
    }
    return null
}
beconsoled.createAppFolder = function(name, uuid) {
    if(!name) name = "UnnameApp"
    if(!uuid) uuid = beapi.utils.genUUID()
    let path = genAppPath(name, uuid)
    if(!path) {
        throw new Error("Can not use app folder path, uuid repeat ?")
    }
    if(beapi.fs.existsSync(path)) {
        throw new Error("app folder has exists:"+path)
    }
    if(!beapi.fs.mkdirSync(path)) {
        throw new Error("mkdir app path failed:"+path)
    }
    return path
}
beconsoled.usage = function(fs) {
    let mem = process.memoryUsage()
    let usage = {
        cpu0: process.cpuUsage(0) ,
        cpu1: process.cpuUsage(1) ,
        mem: Math.round(100*(mem.heap.used+mem.psram.used)/(mem.heap.total+mem.psram.total))
    }
    if(fs) {
        usage.root = beapi.fs.info("/")
        usage.home = beapi.fs.info("/home")
    }
    return usage
}
function findFakeIndev() {
    for(let indev of be.indev) {
        if(indev.driver=='fake') {
            return indev
        }
    }
}
beconsoled.prepareVirtualDesktop = function(width,height) {
    if(!be.disp.length) {
        let VirtualDisplay = require("besdk/driver/virtual-display")
        let dev = new VirtualDisplay()
        dev.setup({width: width||320, height: height||240 })
        dev.register()
        
        const Desktop = require("desktop/Desktop")
        be.desktop = new Desktop()
    }

    let fakeIndev = findFakeIndev()
    if(!fakeIndev){
        let FakeIndev = require("besdk/driver/fake-indev")
        let dev = new FakeIndev()
        dev.setup()
        dev.register()
        fakeIndev = dev.indev
    }

    return [be.disp[0].id(), fakeIndev.id(), be.disp[0].width(), be.disp[0].height()]
}
beconsoled.resetModel = function(){
    beapi.fs.unlinkSync("/home/become/config/setup.json")
    beapi.fs.unlinkSync("/home/become/config/boot.json")
}
beconsoled.setupedPresetName = function() {
    let setup = JSON.load("/home/become/config/setup.json")
    if(!setup) {
        return null
    }
    return setup.preset || null
}
beconsoled.sdMountPoints = function () {
    let points = []
    let SDSPI = require("/lib/local/besdk/driver/sdspi.js")
    for(let id in be.dev){
        let dev = be.dev[id]
        if(dev instanceof SDSPI && dev.mounted) {
            points.push(dev.opts.mount)
        }
    }
    return points
}