let _lastPWD = process.env.HOME

const CMD_RUN = 1 ;
const CMD_CALL = 2 ;
const CMD_CALL_ASYNC = 3 ;
const CMD_RSPN = 4 ;
const CMD_EXCEPTION = 5 ;


let _pending_pkg_id = -1
let _pending_code = ''



function thousands(num) {
    num=num.toString().split(".");  // 分隔小数点
    var arr=num[0].split("").reverse();  // 转换成字符数组并且倒序排列
    var res=[];
    for(var i=0,len=arr.length;i<len;i++){
      if(i%3===0&&i!==0){
         res.push(",");   // 添加分隔符
      }
      res.push(arr[i]);
    }
    res.reverse(); // 再次倒序成为正确的顺序
    if(num[1]){  // 如果有小数的话添加小数部分
      res=res.join("").concat("."+num[1]);
    }else{
      res=res.join("");
    }
    return res;
}
function formatByteNum(num, useUnit) {
    if(useUnit) {
        if(num>1073741824){
            return thousands(Math.round(num*10/1073741824)/10) + 'G'
        }else if(num>1048576){
            return thousands(Math.round(num*10/1048576)/10) + 'M'
        }else if(num>1024){
            return thousands(Math.round(num*10/1024)/10) + 'K'
        }else {
            return thousands(num)
        }
    }
    else {
        return thousands(num)
    }
}
function widthByChar(str) {
    return str.length
}
function printTable(table) {
    let clmsW = []
    let c, r
    let sep = '    '
    for(r=0;r<table.length;r++) {
        for(c=0;c<table[r].length;c++) {
            if(clmsW[c]==undefined) {
                clmsW[c] = 0
            }
            let width = widthByChar(table[r][c])
            if(width>clmsW[c]) {
                clmsW[c] = width
            }
        }
    }
    for(r=0;r<table.length;r++) {
        for(c=0;c<table[r].length;c++) {
            let w = widthByChar(table[r][c])
            table[r][c]+= " ".repeat(clmsW[c] - w)
        }
        console.log(table[r].join(sep))
    }
}



beapi.telnet.registerHandler(function(pkgid, remain, pkgcmd, code){

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
            beapi.telnet.rspn(pkgid, CMD_RSPN, "<~ASSHELL>")
            return
        }

        if(pkgcmd == CMD_CALL_ASYNC) {
            evalAsFile(`(async ()=>{let resolve = beapi.repl._mkresolve(${pkgid});let reject = beapi.repl._mkreject(${pkgid}); try{${code}}catch(e){reject(e)}})()`, "REPL")
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

function resolvepath(path,onlyFile) {
    let _path
    if(path=='~' ) {
        _path = process.env.HOME
    } 
    else if(path[0]=='~' && path[1]=='/') {
        _path = process.env.HOME + path.substr(1)
    }
    else if(path[0]!='/') {
        _path = process.env.PWD + '/' + path
    }
    else {
        _path = path ;
    }
    if(!!onlyFile) {
        if(!beapi.fs.isFileSync(_path)) {
            throw new Error ("path not a exists file")
        }
    }
    return _path
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
    path = resolvepath(path)
    let stat = beapi.fs.statSync(resolvepath(path))
    if(!stat) {
        console.log("path not exists:", path)
        return
    }
    if(!stat.isDir) {
        console.log(stat)
        return
    }
    for(let item of beapi.fs.readdirSync(path,true)) {
        if(item.name=='.' || item.name=='..')
            continue
        if( item.type=='dir' ) {
            console.log(item.name+'/')
        }
        else {
            console.log(item.name)
        }
    }
}

function stat(path) {
    console.log(beapi.fs.statSync(resolvepath(path)))
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
function rm(path) {
    if(!path) {
        console.log("rm <file>")
        return
    }
    path = resolvepath(path)
    beapi.fs.unlinkSync(path)
}
function touch() {
    beapi.fs.writeFileSync(resolvepath(path), '')
}
function cat(path) {
    console.log(beapi.fs.readFileSync(resolvepath(path)).asString())
}
function pwd() {
    console.log(process.env.PWD)
}
function mkdir(path) {
    path = resolvepath(path)
    beapi.fs.mkdirSync(path)
}
function compile(path) {
    if(!path) {
        console.log("compile <script>|<dir>")
        return false
    }
    path = resolvepath(path)
    path = beapi.fs.normalize(path)
    if(beapi.fs.isFileSync(path)) {
        beapi.fs.unlinkSync(path+".bin")
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
            return false
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
        return false
    }
}


function free(...args) {
    let info = process.memoryUsage()
    if(args.includes('-j')) {
        console.log(info)
        return
    }
    let h = args.includes('-h')
    let used, free, total
    let table = [['', '[used]', '[%]', '[free]', '[total]']]
    for(let key in info) {
        ;({used, free, total} = info[key])
        table.push([
            key.toUpperCase()
            , formatByteNum(used,h)
            , Math.floor(used*1000/total)/10+'%'
            , formatByteNum(free,h)
            , formatByteNum(total,h)
        ])
    }
    printTable(table)
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
function reboot() {
    process.reboot()
}
function onboot(path) {
    if(!path) {
        console.log("onboot <script path>")
        return false
    }
    path = resolvepath(path,true)
    require(__dirname+"/boot.js").setAutoScript(path)
}
function help() {
    console.log("command:", Object.keys(ShellCmds).join(", "))
}

const ShellCmds = {
    cd, pwd, ls, cp, rm, mv, touch, cat, stat, mkdir
    , require:cmd_require
    , free, reset, reboot, onboot
    , compile, help, "?": help
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

beapi.repl = {
    _mkresolve(pkgid) {
        return function(ret) {
            beapi.telnet.rspn(pkgid, CMD_RSPN, console.stringify(ret))
        }
    } ,
    _mkreject(pkgid) {
        return function(ret) {
            beapi.telnet.rspn(pkgid, CMD_EXCEPTION, console.stringify(ret))
        }
    } ,
    cmds: ShellCmds
}