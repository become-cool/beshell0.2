function normalize(path) {
    path = path.replace(/[\\\/]+/gm, '/')                   // 合并连续的 / 或 \
                .replace(/\/\.(\/|$)/gm, '/')                   // 合并当前目录表示: /./ 
                .replace(/\/[^\/]+\/\.\.(\/|$)/gm, '/')         // 合并上级目录表示: /../
                .replace(/\/$/, '')                         // 删除目录末尾的 /
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
fs.normalize = normalize
fs.dirname = dirname

function resolveExtName(path) {
    if( fs.isFileSync(path) )
        return path
    let pathExt = path + '.js'
    if( fs.isFileSync(pathExt) )
        return pathExt

    // npm package 
    if( fs.isDirSync(path) ) {

        let pkgpath = path + "/package.json"

        if(fs.isFileSync(pkgpath)) {
            try{
                let json = JSON.parse(fs.readFileSync(pkgpath))
                let mainpath = path + '/' + (json.main || 'index.js')
                if(fs.isFileSync(mainpath))
                    return mainpath
            }catch(e){
                console.log(e)
            }
        }
        
        if(fs.isFileSync(path+'/index.js')) {
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
            throw new Error("Cound not found: "+id)
        }

        path = normalize(path)
        if(Module.caches[path]) {
            return Module.caches[path]
        }

        let scriptDir = dirname(path)
        let script = ` let module = {exports:{}} ; (function(exports, require, module, __filename, __dirname) { ${fs.readFileSync(path)}
})(module.exports, Module.__mkrequire("${scriptDir}"), module, "${path}", "${scriptDir}");
module.exports`
        Module.caches[path] = eval(script)
        return Module.caches[path]
    }
}

let Module = {
    caches: {} ,
    globalPaths: [
        '/lib/node_modules'
    ] ,
    __mkrequire
}

global.Module  = Module
global.require = __mkrequire()