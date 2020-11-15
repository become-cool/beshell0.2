let _lastPWD = process.env.HOME

const CMD_RUN = 1 ;
const CMD_CALL = 2 ;
const CMD_CALL_ASYNC = 3 ;
const CMD_RSPN = 4 ;
const CMD_EXCEPTION = 5 ;
// const CMD_CALLBACK = 6 ;
// const CMD_OUTPUT = 7 ;

// const CMD_FILE_PUSH_REQ = 10 ;
// const CMD_FILE_APPEND_REQ = 11 ;
// const CMD_FILE_PULL_REQ = 12 ;

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

_repl_set_input_func(function(pkgid, remain, pkgcmd, code){

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

    let p = code.indexOf(' ')
    let cmd = p<0? code: code.substr(0, p)

    if(commands[cmd]) {
        let res = commands[cmd] (p<0? '': code.substr(p+1))
        telnet.send(pkgid, CMD_RSPN, "<~empty rspn~>")
        return
    }

    try{
        if(pkgcmd == CMD_RUN) {
            evalAsFile(code, "REPL")
        }
        else if(pkgcmd == CMD_CALL) {
            let res = evalAsFile(code, "REPL")
            telnet.send(pkgid, CMD_RSPN, JSON.stringify(res))
        }
        else if(pkgcmd == CMD_CALL_ASYNC) {
            evalAsFile(`{let resolve = _mkresolve(${pkgid});let reject = _mkreject(${pkgid}); code}`, "REPL")
        }

    } catch(e) {
        telnet.send(pkgid, CMD_EXCEPTION, console.valueToString(e))
        return
    }
})

function resolvepath(path) {
    if(path=='~') {
        path = process.env.HOME
    } 
    else if(path=='-') {
        path = _lastPWD
    }
    else if(path[0]!='/') {
        path = process.env.PWD + '/' + path
    }
    return path
}

let commands = {
    ls: function(path) {
        path = path? resolvepath(path): process.env.PWD
        let output = ''
        for(let filename of fs.readdirSync(path)) {
            let stat = fs.statSync(path+'/'+filename)
            if(stat.isDir)
                filename+= '/'
            else {
                let size = stat.size
                if(size>1048576)
                    size = (size/1048576).toFixed(1) + 'MB'
                else if(size>1024)
                    size = (size/1024).toFixed(1) + 'KB'
                filename = '[' + size + "]\t" + filename
            }
            output+= filename + "\n"
        }
        console.log(output)
    } ,

    cd: function(path) {

        path = path? resolvepath(path): process.env.HOME

        if(!fs.isDirSync(path)) {
            console.log(path, "is not a directory.")
            return
        }
        
        path = fs.normalize(path) || '/'

        if(process.env.PWD != path) {
            _lastPWD = process.env.PWD
            process.env.PWD = path
        }
        console.log(process.env.PWD)
    } ,

    pwd: function() {
        console.log(process.env.PWD)
    } ,

    ".": function(path) {
        
        path = path? resolvepath(path): process.env.PWD
        
        require(path)
    } ,

    free: function(unit){
        if(unit=='k')
            console.log(Math.round(utils.freeStacks()/1024)+'KB')
        else 
            console.log(utils.freeStacks())
    } ,

    reset: function(level) {
        level = parseInt(level.trim())
        isNaN(level)? process.reset(): process.reset(level)
    } ,

    reboot: function() {
        process.reboot()
    } ,

    cat: function(path) {
        if(!path)
            return
        path = resolvepath(path)
        console.log(fs.readFileSync(path))
    } ,

    rm: function(path) {
        if(!path)
            return
        path = resolvepath(path)
        if(!fs.unlinkSync(path)) {
            console.log('Failded to rm path', path)
        }
    }
}

global.repl = {
    commands, _mkresolve, _mkreject
}