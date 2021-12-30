module.exports = function(){
    return prequire([
        "./NewAppDialog" ,
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