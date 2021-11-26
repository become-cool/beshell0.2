// nvs,      data, nvs,      0x9000,      0x6000
// phy_init, data, phy,      0xf000,      0x1000
// factory,  app,  factory,  0x10000,   0x170000
// fsroot,       data, fat,      0x180000,  0x280000
// fshome,       data, fat,      0x180000,  0x280000

let parttables = [
    {
        name: "nvs",
        type: "data",
        subtype: "nvs",
        offset: 0x9000,
        size: 24*1024,
    } ,
    {
        name: "phy_init",
        type: "data",
        subtype: "phy",
        size: 4*1024,
    } ,
    {
        name: "factory",
        type: "app",
        subtype: "factory",
        size: 2*1024*1024,
    } ,
    {
        name: "fsroot",
        type: "data",
        subtype: "fat",
        size: 256*1024,
    } ,
    {
        name: "fshome",
        type: "data",
        subtype: "fat",
    } ,
]


function generatePartitionsTable(parts, flashSize) {

    process.stdout.write(`# FLASH Size: ${(flashSize/1024/1024).toFixed(2)}MB\n`)
    process.stdout.write(`# Name,   Type, SubType, Offset,  Size, Flags\n`)
    process.stdout.write(`# Note: if you change the phy_init or app partition offset, make sure to change the offset in Kconfig.projbuild\n`)

    let offset = 0
    for(let partinfo of parts) {
        if(partinfo.offset!=undefined) {
            offset = partinfo.offset
        }

        let size = partinfo.size
        if(size==undefined) {
            size = flashSize - offset
        }
        
        process.stdout.write(`${partinfo.name}, ${partinfo.type}, ${partinfo.subtype}, 0x${offset.toString(16).toUpperCase()}, 0x${size.toString(16).toUpperCase()} \n`)

        offset+= size
    }

    process.stdout.write(`\n\n\n\n`)
}



generatePartitionsTable(parttables, 4*1024*1024)
generatePartitionsTable(parttables, 16*1024*1024)