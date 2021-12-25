const lv = require('lv')

module.exports = {
    title: "BePad" ,
    width: 14 * 8 ,
    height: 8 * 8 ,
    "bg-color": lv.rgb(80,80,80) ,
    ports: [
        { title: "0", x:4, y:4 } ,
        { title: "2", x:12, y:4 } ,
        
        { title: "VCC", x:4, y:12 } ,
        { title: "GND", x:12, y:12 } ,
        
        { title: "34", x:4, y:20 } ,
        { title: "35", x:12, y:20 } ,
        
        { title: "4", x:4, y:28 } ,
        { title: "5", x:12, y:28 } ,
        
        { title: "1", x:4, y:36 } ,
        { title: "3", x:12, y:36 } ,
        
        { title: "VCC", x:4, y:44 } ,
        { title: "GND", x:12, y:44 } ,
        
        { title: "12", x:4, y:52 } ,
        { title: "13", x:12, y:52 } ,
        
        { title: "14", x:4, y:60 } ,
        { title: "15", x:12, y:60 } ,
    ]
}
