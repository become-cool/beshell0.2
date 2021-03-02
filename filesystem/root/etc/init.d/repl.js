let _lastPWD = process.env.HOME

const CMD_RUN = 1 ;
const CMD_CALL = 2 ;
const CMD_CALL_ASYNC = 3 ;
const CMD_RSPN = 4 ;
const CMD_EXCEPTION = 5 ;

function _mkresolve(pkgid) {
    return function(ret) {
        telnet.send(pkgid, CMD_RSPN, JSON.stringify(ret))
    }
}
function _mkreject(pkgid) {
    return function(ret) {
        telnet.send(pkgid, CMD_EXCEPTION, JSON.stringify(ret))
    }
}

let _pending_pkg_id = -1
let _pending_code = ''

beapi._repl_set_input_func(function(pkgid, remain, pkgcmd, code){

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
        if(pkgcmd == CMD_RUN) {
            evalAsFile(code, "REPL")
        }
        else if(pkgcmd == CMD_CALL) {
            let res = evalAsFile(code, "REPL")
            telnet.send(pkgid, CMD_RSPN, JSON.stringify(res))
        }
        else if(pkgcmd == CMD_CALL_ASYNC) {
            evalAsFile(`(async ()=>{let resolve = repl._mkresolve(${pkgid});let reject = repl._mkreject(${pkgid}); try{${code}}catch(e){reject(e)}})()`, "REPL")
        }

    } catch(e) {
        telnet.send(pkgid, CMD_EXCEPTION, console.valueToString(e))
        return
    }
})

function resolvepath(path) {
    if(path=='~' ) {
        path = process.env.HOME
    } 
    else if(path[0]=='~' && path[1]=='/') {
        path = process.env.HOME + path.substr(1)
    } 
    else if(path[0]!='/') {
        path = process.env.PWD + '/' + path
    }
    return path
}

function cd(path) {
    if(path=='-') {
        path = _lastPWD
    }
    path = path? resolvepath(path): process.env.HOME
    if(!fs.isDirSync(path)) {
        throw new Error("Is not a directory."+path)
    }
    path = fs.normalize(path) || '/'
    if(process.env.PWD != path) {
        _lastPWD = process.env.PWD
        process.env.PWD = path
    }
    return process.env.PWD
}


global.repl = {
    _mkresolve, _mkreject, cd, resolvepath
}

