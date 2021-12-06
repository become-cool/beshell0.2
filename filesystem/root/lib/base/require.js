function normalize(path) {
    path = path.replace(/[\\\/]+/gm, '/')                       // 合并连续的 / 或 \
                .replace(/\/$/, '')                             // 删除目录末尾的 /

    // 合并当前目录表示: /./ 
    while(path.match(/\/\.(\/|$)/gm))
        path = path.replace(/\/\.(\/|$)/gm, '/')
        
    // 合并上级目录表示: /../
    while(path.match(/\/[^\/]+\/\.\.(\/|$)/gm))
        path = path.replace(/\/[^\/]+\/\.\.(\/|$)/gm, '/')

    return path
}
function dirname(path) {
    let idx = path.lastIndexOf('/')
    if(idx<0) {
        return null
    }
    else {
        return path.substr(0, idx) || null
    }
}
beapi.fs.normalize = normalize
beapi.fs.dirname = dirname

function resolveExtName(path) {
    if( beapi.fs.isFileSync(path) )
        return path
    let pathExt = path + '.js'
    if( beapi.fs.isFileSync(pathExt) )
        return pathExt

    // npm package 
    if( beapi.fs.isDirSync(path) ) {

        let pkgpath = path + "/package.json"

        if(beapi.fs.isFileSync(pkgpath)) {
            try{
                let json = JSON.parse(beapi.fs.readFileSync(pkgpath))
                let mainpath = path + '/' + (json.main || 'index.js')
                if(beapi.fs.isFileSync(mainpath))
                    return mainpath
            }catch(e){
                console.log(e)
            }
        }
        
        if(beapi.fs.isFileSync(path+'/index.js')) {
            return path+'/index.js'
        }
    }
    
    return null
}

function __mkrequire(__dirname) {
    return function(id) {
        if(!id) {
            throw new Error()
        }
        let path 
        // home 目录
        if(id[0]=='~') {
            path = resolveExtName( process.env.HOME + id.substr(1) )
        }
        else if(id[0]=='.' && (id[1]=='/' || (id[1]=='.' && id[2]=='/'))) {
            if(__dirname)
                path = resolveExtName( __dirname + '/' + id )
            else 
                path = resolveExtName( process.env.PWD + '/' + id.substr(1) )
        }

        // node_modules
        else if(id[0]!='/') {
            // local modules
            for( let parent=__dirname; parent; parent=dirname(parent) ) {
                path = resolveExtName(parent + '/node_modules/' + id)
                if(path)
                    break
            }
            
            // system modules
            if(!path) {
                for(let dir of Module.globalPaths) {
                    path = resolveExtName(dir + '/' + id)
                    if(path)
                        break
                }
            }
        }
        else {
            path = resolveExtName(id)
        }

        if(!path) {
            throw new Error("require() cound not found: "+id)
        }

        path = normalize(path)
        if(Module.caches[path]) {
            return Module.caches[path]
        }

        // JSON 文件
        if(path.match(/\.json$/i)) {
            Module.caches[path] = JSON.load(path)
        }
        // JS 文件
        else {
            let scriptDir = dirname(path)
            // let script = "(function(require,__filename,__dirname) {var exports={};var module = {exports};" + beapi.fs.readFileSync(path) + ';return module.exports})(Module.__mkrequire("'+scriptDir+'"),"'+path+'","'+scriptDir+'");'
            let script = `(function(require,__filename,__dirname) {var exports={};var module = {exports}; ${beapi.fs.readFileSync(path)} ; return module.exports})(Module.__mkrequire('${scriptDir}'),'${path}','${scriptDir}');`

            Module.caches[path] = evalAsFile(script, path)
        }
        return Module.caches[path]
    }
}

const Module = {
    caches: {} ,
    globalPaths: [
        '/lib/node_modules'
    ] ,
    __mkrequire
}

JSON.load = function(path) {
    return JSON.parse(beapi.fs.readFileSync(path))
}

global.Module  = Module
global.require = __mkrequire()