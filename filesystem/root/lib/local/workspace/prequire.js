module.exports = function(){
    return prequire([
        "./DlgNewApp" ,
        "./ToolBar" ,
        "./graph/conf" ,
        "./graph/parts/Port" ,
        "./graph/parts/Part" ,
        "./graph/parts/BePad" ,
        "./graph/GraphCanvas" ,
        "./MainScreen" ,
    ], require)
}