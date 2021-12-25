module.exports = function(){
    return prequire([
        "./AppIcon",
        "./ScrApps",
        "./ScrSysApps",
        "./dashboard/Dashboard",
    ], require)
}