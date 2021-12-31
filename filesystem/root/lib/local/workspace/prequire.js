module.exports = function(){
    return prequire([
        "./NewAppDialog" ,
        "./ToolBar" ,
        "./AppModel" ,
        "./Zoomer" ,
        "./graph/parts/Port" ,
        "./graph/parts/Part" ,
        "./graph/parts/BePad" ,
        "./graph/parts/BeMotor" ,
        "./graph/parts/BeScreen" ,
        "./graph/GraphCanvas" ,
        "./graph/GraphTools" ,
        "./ui/WidgetHelper" ,
        "./ui/UITools" ,
        "./Workspace" ,
    ], require)
}