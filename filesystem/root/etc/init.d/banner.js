module.exports = function() { console.log(`
 ____      ____                                     ____                       
|    ~.   |                       ..''''|         ||           |       |       
|____.'_  |______              .''      |_________||______     |       |       
|       ~.|                 ..'         |         ||           |       |       
|_______.'|___________....''            |         ||___________|_______|_______
 (c) 2020 https://become.cool

version BeShell: ${process.versions.beshell}; quickjs: ${process.versions.quickjs}; esp-idf: ${process.versions["esp-idf"]}; ${process.simulate?'[simulation]':''}
build: ${process.build}`) 
    if(process.simulate) {
        console.log("pid:",process.pid)
    }
}
