console.valueToString = function(val){
    if(val==global) {
        return "[global]"+JSON.stringify(Object.keys(sth))
    }
    else if(val instanceof Error) {
        return JSON.stringify({message:val.message, stack:val.stack})
    }
    else if(val===null) {
        return 'null'
    }
    else if(val===undefined) {
        return 'undefined'
    }
    else {
        switch(typeof val) {
            case 'object':
                return JSON.stringify(val)
            case 'function':
            default:
                return  val.toString()
        }
    }
}

console.log2 = function(){
    let output = []
    for(let argv of argvs) {
        output.push( console.valueToString(argv) )
    }
    console.print(output.join(" ")+"\n")
}