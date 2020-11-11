require("besdk")

const chunklen = 10

function resolve(ret, reqid, remain) {
    console.log('run<<EmitCmdRspn('+(reqid||-1)+','+JSON.stringify(ret)+', '+(remain||0)+')')
}
function throwexcpt(excpt, reqid) {
    if(typeof excpt == "string")
        excpt = {error: excpt}
    else if( excpt instanceof Error )
        excpt = { error: excpt.toString() /*, stack: excpt.stack*/ }
    console.log('run<<EmitCmdException('+(reqid||-1)+','+JSON.stringify(excpt)+')')
}

function ls(dirpath, detail, reqid) {
    try{
        let lst = fs.readdirSync(dirpath)
        for(let i=0;i<lst.length;i++){
            let info = fs.statSync(dirpath+'/'+lst[i])
            lst[i] = {
                name: lst[i], 
                type: info.isDir?'dir': 'file',
            }
            if(!info.isDir)
                lst[i].size = info.size
        }
        resolve(lst,reqid)
    }catch(e){
        throwexcpt(e, reqid)
        return
    }
}
function readFile(path, encode, reqid) {
    try{
        let stat = fs.statSync(path)
        if(!stat) {
            throwexcpt("path not exists:"+path, reqid)
            return
        }
        if(stat.isDir) {
            throwexcpt("path is a directory:"+path, reqid)
            return
        }

        let cnt = Math.ceil(stat.size / chunklen)
        for(let i=0;i<cnt; i++) {
            let data = fs.readFileSync(path, chunklen, i*chunklen)
            if(encode=='base64')
                data = utils.base64Encode(data)
            resolve(data, reqid, cnt-i-1)
        }
    }
    catch(e) {
        throwexcpt(e)
        return
    }
}
function writeFile(path, data, append, encode, reqid) {
    if(encode=='base64')
        data = utils.base64Decode(data)
    if( fs.writeFileSync(path, data, append) <0 ) {
        throwexcpt("Failded to write file", reqid)
        return
    }
    else {
        resolve(true, reqid)
    }
}
function unlink(path, reqid) {

}
function mkdir(path, reqid) {

}
function pwd(path, reqid) {
    resolve(process.env.PWD, reqid)
}


global.beconsoled  = {
    ls, readFile, writeFile, unlink, mkdir, pwd
} 
