const fs = require("fs")
const exec = require("child_process").execSync
const beconsolePath = __dirname + "/../../beconsole.next/platform/nw.js"

// 计算 beshell.bin 文件的大小，按 8 取整做为分区的尺寸
const [appsize, rootsize] = require('./app-partition-size.js')

const version = require("./version")

let target = "all"
for(let t of ['fs', 'fs-home', 'fs-root']) {
    if( process.argv.includes(t) ) {
        target = t
    }
}

const meta = {
    "name": "BeShell" ,
    "version": version ,
    "home": "https://become.cool" ,
    "description": "" ,
    "chips": ["esp32"] ,
    "author": "BECOME" ,
    "priority": 5 ,
    "write-options": {
        "argvs": ["-z"] ,
        "baud": [921600] ,
        "flash_mode": "dio" ,
        "flash_freq": "80m"
    } ,
    "bin": [
        {
            "address": "0x1000" ,
            "path": "bootloader.bin"
        } ,
        {
            "address": "0x8000" ,
            "path": "partitions-4MB.bin" ,
            "flashsize": "4"
        },
        {
            "address": "0x8000" ,
            "path": "partitions-16MB.bin",
            "flashsize": "16"
        },
        {
            "address": "0x10000" ,
            "path": "beshell.bin" 
        } ,

        {
            "address": '0x'+(0x10000+appsize).toString(16) ,
            "path": "fs-root.img" ,
        } ,

        {
            "address": '0x'+(0x10000+appsize+rootsize).toString(16) ,
            "path": "fs-home.img" ,
        }
    ]
}
let metastring = JSON.stringify(meta,null,4)


function mkdir(path) {
    try{ 
        fs.mkdirSync(path)
    }catch(e){}
}
function cp(from, to) {
    let cmd = `cp ${__dirname}/${from} ${to}`
    console.log(cmd)
    exec(cmd)
}
function dist(targetDir) {

    let path = targetDir + "/beshell-"+version

    console.log("\r\ndispense to", path)

    mkdir(path)

    if(target=='all') {
        fs.writeFileSync(path+'/firmware.json', metastring)
        cp("img/bootloader.bin", path+"/")
        cp("img/partitions-4MB.bin", path+"/")
        cp("img/partitions-16MB.bin", path+"/")
        cp("../build/beshell.bin", path+"/")
    }
    if(target=='all' || target=='fs' || target=='fs-root') {
        cp("img/fs-root.img", path+"/")
    }
    if(target=='all' || target=='fs' || target=='fs-home') {
        cp("img/fs-home.img", path+"/")
    }
}

dist(__dirname+"/firmware/")
dist(beconsolePath+'/tmp/assets/firmware')
dist(beconsolePath+'/assets/firmware')