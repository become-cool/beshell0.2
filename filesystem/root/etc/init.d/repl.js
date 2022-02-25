let _lastPWD = process.env.HOME

const CMD_RUN = 1 ;
const CMD_CALL = 2 ;
const CMD_CALL_ASYNC = 3 ;
const CMD_RSPN = 4 ;
const CMD_EXCEPTION = 5 ;

function _mkresolve(pkgid) {
    return function(ret) {
        beapi.telnet.rspn(pkgid, CMD_RSPN, JSON.stringify(ret))
    }
}
function _mkreject(pkgid) {
    return function(ret) {
        beapi.telnet.rspn(pkgid, CMD_EXCEPTION, JSON.stringify(ret))
    }
}

let _pending_pkg_id = -1
let _pending_code = ''

beapi.telnet.registerHandle(function(pkgid, remain, pkgcmd, code){

    if(_pending_pkg_id>0 && _pending_pkg_id!=pkgid) {
        _pending_pkg_id = -1
        _pending_code = ''
    }

    _pending_code+= code

    if(remain>0) {
        return
    }
    
    code = _pending_code.trim()
    _pending_pkg_id = -1
    _pending_code = ''

    try{
        if( runShellCmd(code) ) {
            beapi.telnet.rspn(pkgid, CMD_RSPN, "")
            return
        }

        if(pkgcmd == CMD_CALL_ASYNC) {
            evalAsFile(`(async ()=>{let resolve = repl._mkresolve(${pkgid});let reject = repl._mkreject(${pkgid}); try{${code}}catch(e){reject(e)}})()`, "REPL")
        }
        else {
            let res = evalAsFile(code, "REPL")
            if(pkgcmd == CMD_CALL) {
                beapi.telnet.rspn(pkgid, CMD_RSPN, console.stringify(res))
            }
        }
    } catch(e) {
        if(e instanceof Error) {
            e = {message:e.message,stack:e.stack}
        }
        beapi.telnet.rspn(pkgid, CMD_EXCEPTION, console.stringify(e))
        return
    }
})

function resolvepath(path) {
    if(path=='~' ) {
        return process.env.HOME
    } 
    else if(path[0]=='~' && path[1]=='/') {
        return process.env.HOME + path.substr(1)
    }
    else if(path[0]!='/') {
        return process.env.PWD + '/' + path
    }
    return path
}

function cd(path) {
    if(path=='-') {
        path = _lastPWD
    }
    path = path? resolvepath(path): process.env.HOME
    if(!beapi.fs.isDirSync(path)) {
        throw new Error("Is not a directory: "+path)
    }
    path = beapi.fs.normalize(path) || '/'
    if(process.env.PWD != path) {
        _lastPWD = process.env.PWD
        process.env.PWD = path
    }
    return process.env.PWD
}

function ls(path) {
    if(!path) {
        path = process.env.PWD
    }
    for(let item of beapi.fs.readdirSync(path)) {
        if(item=='.')
            continue
        if( beapi.fs.isDirSync(process.env.PWD+'/'+item) ) {
            console.log(item+'/')
        }
        else {
            console.log(item)
        }
    }
}


function cmd_require(path) {
    if(!path) {
        console.log("require <script>|<dir>")
        return
    }
    path = resolvepath(path)
    return require(path, true)
}
function mv() {
    console.log("this repl cmd not implemented")
}
function cp(path) {
    console.log("this repl cmd not implemented")
}
function rm() {
    console.log("this repl cmd not implemented")
}
function touch() {
    console.log("this repl cmd not implemented")
}
function cat(path) {
    console.log(beapi.fs.readFileSync(resolvepath(path)).asString())
}
function pwd() {
    console.log(process.env.PWD)
}
function compile(path) {
    if(!path) {
        console.log("compile <script>|<dir>")
        return
    }
    path = resolvepath(path)
    path = beapi.fs.normalize(path)
    if(beapi.fs.isFileSync(path)) {
        let script = beapi.fs.readFileSync(path).asString()
        if(path.substr(0,10)!='/lib/base/') {
            script = Module.wrapExportMeta(script, beapi.fs.dirname(path), path)
        }
        try{
            script = compileScript(script, path, true)
        }catch(e){
            console.error("compile failed:", path)
            console.error(e)
            console.error(e.stack)
            return
        }
        beapi.fs.writeFileSync( path+".bin", script)
        console.log("compile script to:", path+".bin")
    }
    else if(beapi.fs.isDirSync(path)) {
        for(let item of beapi.fs.readdirSync(path)) {
            if(item=='.' || item=='..')
                continue
            let child = path+'/'+item
            if( beapi.fs.isDirSync(child) || child.substr(-3).toLowerCase()=='.js' ) {
                compile(child)
            }
        }
    }
    else {
        console.error("path not exists: " + path)
    }
}

function free() {
    console.log(process.memoryUsage())
}
function reset(level) {
    level = parseInt((level||'5').trim())
    if(isNaN(level)) {
        process.reset()
    }
    else {
        process.reset(level)
    }
}

const ShellCmds = {
    cd, pwd, ls, cp, rm, mv, touch, cat
    , require:cmd_require
    , free, reset
    , compile
}


function runShellCmd(code) {
    let argvs = code.trim().split(/ +/)
    let cmd = argvs.shift()
    if(!ShellCmds[cmd]) {
        return false
    }
    
    ShellCmds[cmd].apply(null, argvs)

    return true
}
