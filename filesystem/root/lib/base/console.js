console.stringify = function(value,format) {
    let pool = []
    let str = JSON.stringify(value, (key,value)=>{
        if( key=="_handles" ) {
            return undefined
        }
        if(value===null) {
            return value
        }
        if( typeof value=="object" || typeof value=="function" ) {
            let idx = pool.indexOf(value)
            if( idx>=0 ) {
                return "<circular reference>#"+idx
            }
            pool.push(value)
            if(typeof value=="function") {
                return (value.name||'')+'<function>'
            }
        }
        return value
    },format? 2: 0)
    pool.length = 0
    return str
}

console.log = function() {
    let vals = []
    for(let v of arguments){
        switch(typeof v) {
            case 'number':
            case 'string':
            case 'boolean':
            case 'function':
                vals.push(v)
                break
            default:
                if(v instanceof Error) {
                    vals.push(v.message+"\r\n"+v.stack)
                }
                else {
                    vals.push(console.stringify(v))
                }
                break
        }
    }
    console.print(vals.join(" ")+"\n")
}
console.error = console.log