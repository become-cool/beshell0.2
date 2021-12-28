module.exports = function(){
    return prequire([
        "./DlgNewApp" ,
        "./ToolBar" ,
        "./AppModel" ,
        "./graph/parts/Port" ,
        "./graph/parts/Part" ,
        "./graph/parts/BePad" ,
        "./graph/GraphCanvas" ,
        "./graph/GraphTools" ,
        "./graph/PartLib" ,
        "./Workspace" ,
    ], require)
}