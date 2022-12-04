const fs = require("fs")
const exec = require("child_process").execSync
const beconsolePath = __dirname + "/../../beconsole.next/platform/nw.js"

const version = require("./version")
const getEsp32Target = require("./target").getEsp32Target

function mkdir(path) {
    try{ 
        fs.mkdirSync(path)
    }catch(e){
        // console.log(e)
    }
}
function cp(from, to) {
    let cmd = `cp ${__dirname}/${from} ${to}`
    console.log(cmd)
    exec(cmd)
}

function dist(firmware, targetDir) {

    let path = targetDir + "/beshell-"+version
    console.log("\r\ndispense to", path, firmware)
    mkdir(path)

    let postfix = getEsp32Target()=='esp32s3'? '-s3': ''

    let dispenseJson = false
    let dispenseBeshell = false
    let dispenseFSHome = false
    let dispenseFSRoot = false
    let dispensePartition = false
    let dispenseBootloader = false

    if(firmware=='all') {
        dispenseJson = true
        dispenseBeshell = true
        dispenseFSHome = true
        dispenseFSRoot = true
        dispensePartition = true
        dispenseBootloader = true
    }
    else if(firmware=='beshell') {
        dispenseBeshell = true
    }
    else if(firmware=='fs') {
        dispenseJson = true
        dispenseFSHome = true
        dispenseFSRoot = true
    }
    else if(firmware=='fs-root') {
        dispenseFSRoot = true
    }
    else if(firmware=='fs-home') {
        dispenseFSHome = true
    }
    else {
        throw new Error("unknow task:", firmware)
    }

    if(dispenseJson) {
        cp('img/firmware.json', path+'/firmware.json')
    }

    if(dispenseBootloader) {
        cp("../build/bootloader/bootloader.bin", path+"/bootloader"+postfix+".bin")
    }
    
    if(dispensePartition) {
        cp("img/partitions-4MB.bin", path+"/")
        cp("img/partitions-16MB.bin", path+"/")
    }

    if(dispenseBeshell) {
        cp("../build/beshell.bin", path+"/beshell"+postfix+".bin")
    }

    if(dispenseFSRoot) {
        cp("img/fs-root.img", path+"/")
    }

    if(dispenseFSHome) {
        cp("img/fs-home.img", path+"/")
    }
}

let firmware = "all"
for(let t of ['beshell', 'fs', 'fs-home', 'fs-root']) {
    if( process.argv.includes(t) ) {
        firmware = t
    }
}

// 本地
dist(firmware, __dirname+"/firmware/")

// 远程
dist(firmware, beconsolePath+'/tmp/assets/firmware')
dist(firmware, beconsolePath+'/assets/firmware')