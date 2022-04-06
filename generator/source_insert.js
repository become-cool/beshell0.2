const fs = require('fs')

function start(name, commentStart) {
    return (commentStart||'//')+` AUTO GENERATE CODE START [${name}] --------`
}
function end(name, commentStart) {
    return (commentStart||'//')+` AUTO GENERATE CODE END [${name}] --------`
}
function srcInsert(src, code, startMart, endMark) {
    let markStartAt = src.indexOf(startMart)
    if(markStartAt<0) {
        // console.log("not found mark:"+startMart)
        return false
        // throw new Error ("not found mark "+startMart)
    }
    let markEndAt = src.indexOf(endMark)
    if(markEndAt<0) {
        console.log ("not found mark:"+endMark)
        return false
    }
    return src.substr(0, markStartAt) + startMart + "\r\n" + code + src.substr(markEndAt) 
}


function fileInsert(path, source, mark, commentStart) {
    let fileSrc = fs.readFileSync(path).toString()
    fileSrc = srcInsert(fileSrc, source, start(mark,commentStart),end(mark,commentStart))
    if(fileSrc!=false) {
        fs.writeFileSync(path, fileSrc)
        return fileSrc
    }
    return false
}

exports.start = start
exports.end = end
exports.srcInsert = srcInsert
exports.fileInsert = fileInsert