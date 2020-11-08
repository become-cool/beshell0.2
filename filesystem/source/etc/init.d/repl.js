let _lastPWD = process.env.HOME
let _echoInput = false
let _echoOutput = true

_repl_set_input_func(function(code){
    code = code.trim()

    if(_echoInput) {
        console.log(code)
    }

    let p = code.indexOf(' ')
    let cmd = p<0? code: code.substr(0, p)

    if(commands[cmd]) {
        commands[cmd] (p<0? '': code.substr(p+1))
        return
    }

    try{
        let res = eval(code)
        if(_echoOutput) {
            if(res==global) {
                console.log(JSON.stringify(Object.keys(res)))
            }
            else {
                switch(typeof res) {
                    case 'function':
                        console.log(res.toString())
                        break
                    case 'object':
                        console.log(JSON.stringify(res))
                    default:
                        console.log(res)
                }
            }
        }
    } catch(e) {
        console.log(e)
        console.log(e.stack)
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
        for(let filename of fs.readdirSync(path)) {
            if(fs.isDirSync(path+'/'+filename))
                filename+= '/'
            console.log(filename)
        }
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
    }
}

global.repl = {
    commands,
    
    setEcho(ouput, input) {
        _echoInput = !!input
        _echoOutput = !!ouput
    }
}