
beapi.fs.walkFile = function(dir, callback, depth) {
    if(depth==undefined) 
        depth = 3
    try{
        var children = beapi.fs.readdirSync(dir,true)
    }catch(e){
        return
    }
    for(let item of children||[]) {
        if(item.name=='.' || item.name=='..' || item.name=='System Volume Information' || item.name=='$RECYCLE.BIN') {
            continue
        }
        let path = dir+'/'+item.name
        if (item.type=='dir') {
            if(depth) {
                this.loadDir(path, callback, depth-1)
            }
            continue
        }
        callback(path, item)
    }
}
beapi.fs.walkFileWithExt = function(dir, extName, callback, depth) {
    if(extName[0] != '.')
        extName = '.' + extName
    beapi.fs.walkFile(dir, (path, detail)=>{
        if(path.slice(-4).toLowerCase() == extName) {
            callback(path, detail)
        }
    }, depth)
}