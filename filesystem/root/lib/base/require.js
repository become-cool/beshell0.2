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
    if( beapi.fs.isDirSync(path) ) {
        // npm package 
        let pkgpath = path + "/package.json"
        if(beapi.fs.isFileSync(pkgpath)) {
            try{
                let json = JSON.parse(beapi.fs.readFileSync(pkgpath).asString())
                if(json.main) {
                    path+= '/' + json.main
                }
                else {
                    path+= '/index.js'
                }
            }catch(e){
                console.log(e)
            }
        }
        else {
            path+= '/index.js'
        }
    }
    let srcExists = beapi.fs.isFileSync(path)
    if(!srcExists) {
        if(path.substr(-3).toLowerCase()!='.js') {
            path+= '.js'
            srcExists = beapi.fs.isFileSync(path)
        }
    }

    if(Module.autoBin==0) {
        return [path, srcExists, false]
    }
    else {
        return [path, srcExists, beapi.fs.isFileSync(path+".bin")]
    }
}

function __mkrequire(__dirname) {
    return function(id, discahce) {
        if(!id) {
            throw new Error()
        }
        discahce = !!discahce

        if(Module.autoBin==-1) {
            Module.autoBin = process.simulate? 1: 2
        }

        let path
        let srcExists = false
        let binExists = false
        // home 目录
        if(id[0]=='~') {
            ; [path,srcExists,binExists] = resolveExtName( process.env.HOME + id.substr(1) )
        }
        else if(id[0]=='.' && (id[1]=='/' || (id[1]=='.' && id[2]=='/'))) {
            if(__dirname){
                ; [path,srcExists,binExists] = resolveExtName( __dirname + '/' + id )
            } else {
                ; [path,srcExists,binExists] = resolveExtName( process.env.PWD + '/' + id.substr(1) )
            }
        }

        // node_modules
        else if(id[0]!='/') {
            // local modules
            for( let parent=__dirname; parent; parent=dirname(parent) ) {
                ; [path,srcExists,binExists] = resolveExtName(parent + '/node_modules/' + id)
                if(srcExists || binExists)
                    break
            }
            
            // system modules
            if(!srcExists && !binExists) {
                for(let dir of Module.globalPaths) {
                    ; [path,srcExists,binExists] = resolveExtName(dir + '/' + id)
                    if(srcExists || binExists){
                        break
                    }
                }
            }
        }
        else {
            ; [path,srcExists,binExists] = resolveExtName(id)
        }

        if(!srcExists && !binExists) {
            throw new Error("require() cound not found: "+id)
        }

        path = normalize(path)
        if( !discahce && Module.caches[path]) {
            return Module.caches[path]
        }

        let result ;

        // JSON 文件
        if(path.substr(-5).toLowerCase()==".json") {
            result = JSON.load(path)
        }
        // JS 文件
        else {
            let isBinFile = path.substr(-4).toLowerCase()==".bin"
            if( isBinFile
                || (Module.autoBin==2 && binExists) // 优先二进制文件
                || (Module.autoBin==1 && !srcExists && binExists) // 备选二进制文件
            ) {
                let bytecode = beapi.fs.readFileSync(isBinFile? path: (path+".bin"))
                result = evalBin(bytecode)
            }
            else {
                if(!srcExists) {
                    throw new Error("require() cound not found: "+id)
                }
                let scriptDir = dirname(path)
                let script = wrapExportMeta(beapi.fs.readFileSync(path).asString(), scriptDir, path)

                if(Module.autoBin==1) {
                    let bytecode = compileScript(script, path)
                    try{
                        beapi.fs.writeFileSync( path+".bin", bytecode)
                    }catch(e){
                        console.error(e)
                        console.error(e.stack)
                    }
                    result = evalBin(bytecode)
                }
                else {
                    result = evalAsFile(script, path)
                }
            }
        }
        if(!discahce){
            Module.caches[path] = result
        }
        return result
    }
}

function wrapExportMeta(script, __dirname, __filename) {
    return `(function(require,__filename,__dirname) {var exports={};var module = {exports}; ${script} ; return module.exports})(Module.__mkrequire('${__dirname}'),'${__filename}','${__dirname}');`
}

const Module = {
    // 0: 不自动加载字节码文件; 1: 优先源码，同时编译字节码(Dev模式); 2: 优先字节码 (Product模式)
    autoBin: -1 , 
    caches: {} ,
    globalPaths: [
        '/lib/local' ,
        '/lib/node_modules' ,
    ] ,
    __mkrequire ,
    wrapExportMeta ,
}

JSON.load = function(path) {
    try{
        return JSON.parse(beapi.fs.readFileSync(path).asString())
    }catch(e){
        return null
    }
}

global.Module  = Module
global.require = __mkrequire()
global.prequire = async function(scripts, require, cb) {
    if(!require)
        require = global.require
    await sleep(0)
    let loaded = 0
    for(let path of scripts) {
        try{
            cb && cb(loaded, scripts.length, path)
            require(path)
            loaded ++
        }catch(e){
            console.log(e)
        }
        await sleep(0)
    }
    cb && cb(loaded, scripts.length)
}