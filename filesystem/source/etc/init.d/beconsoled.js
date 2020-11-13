require("besdk")

function module_list(){
    let lst = []
    for(let filename of fs.readdirSync(process.env.HOME)){
        let path = process.env.HOME + '/' + filename
        let stat = fs.statSync(path)
        if(!stat.isDir) {
            continue
        }

        let packagePath = path + '/package.json'
        if(!fs.isFileSync(packagePath)) {
            continue
        }
        try{
            var pkg = JSON.load(packagePath)
        }catch(e){
            continue
        }
        if(!pkg.become) {
            continue
        }

        if(!pkg.become.version){
            pkg.become.version = pkg.version
        }
        pkg.become.folder = path
        lst.push(pkg.become)
    }
    return lst
}

global.beconsoled  = {
    module_list
} 
