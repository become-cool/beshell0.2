const fs = require("fs")
const version = require("./version")


function mkMetaJson() {
    // 计算 beshell.bin 文件的大小，按 8 取整做为分区的尺寸
    const [appsize, rootsize] = require('./app-partition-size.js')

    const meta = {
        "name": "BeShell" ,
        "version": version ,
        "home": "https://become.cool" ,
        "description": "" ,
        "chips": ["esp32","esp32s3"] ,
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
                "address": "0x1000",
                "path": "bootloader.bin",
                "chiptype": "esp32"
            },
            {
                "address": "0x0",
                "path": "bootloader-s3.bin",
                "chiptype": "esp32s3"
            },
    
            {
                "address": "0x8000",
                "path": "partitions-4MB.bin",
                "flashsize": "4"
            },
            {
                "address": "0x8000",
                "path": "partitions-16MB.bin",
                "flashsize": "16"
            },
    
            {
                "address": "0x10000",
                "path": "beshell.bin",
                "chiptype": "esp32"
            },
            {
                "address": "0x10000",
                "path": "beshell-s3.bin",
                "chiptype": "esp32s3"
            },
    
            {
                "address": "0x"+(0x10000 + appsize).toString(16),
                "path": "fs-root.img"
            },
            {
                "address": "0x"+(0x10000 + appsize + rootsize).toString(16) ,
                "path": "fs-home.img"
            }
        ]
    }

    return JSON.stringify(meta,null,4)
}

let jsonstr = mkMetaJson()
fs.writeFileSync(__dirname+'/img/firmware.json', jsonstr)