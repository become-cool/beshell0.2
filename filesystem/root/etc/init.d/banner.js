module.exports = function() { console.log(`
 ____      ____                                     ____                       
|    ~.   |                       ..''''|         ||           |       |       
|____.'_  |______              .''      |_________||______     |       |       
|       ~.|                 ..'         |         ||           |       |       
|_______.'|___________....''            |         ||___________|_______|_______
 (c) 2020-2021 https://become.cool

version BeShell: ${process.versions.beshell}; quickjs: ${process.versions.quickjs}; esp-idf: ${process.versions["esp-idf"]}; [${process.platform}]
build: ${process.build}`) 
    if(process.platform=='linux') {
        console.log("pid:",process.pid)
    }
}
