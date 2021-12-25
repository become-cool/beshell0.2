class Color{
    r=0; g=0; b=0;
    constructor(r,g,b) {
        this.r = (r||0) & 255
        this.g = (g||0) & 255
        this.b = (b||0) & 255
    }

    toRGB565() {
        let v = Math.round((this.b&255)*31/255)
        v|= Math.round((this.g&255)*63/255) << 5
        v|= Math.round((this.r&255)*31/255) << 11
        return  v
    }

    toHex() {
        let r = (this.r&255).toString(16).toUpperCase()
        if(r.length<2)
            r = '0'+r
        let g = (this.g&255).toString(16).toUpperCase()
        if(g.length<2)
            g = '0'+g
        let b = (this.b&255).toString(16).toUpperCase()
        if(b.length<2)
            b = '0'+b
        return '#'+r+g+b
    }
}
Color.fromHex = function(val){
    val = val.trim().replace(/^#/,'')
    let color = new Color
    if(val<3) {
        color.r = parseInt(val,16)
        color.g = color.r
        color.b = color.r
    }
    else{
        color.r = parseInt(val.substr(0,2),16)
        color.g = parseInt(val.substr(2,2),16)
        color.b = parseInt(val.substr(4,2),16)
    }
    return color
}
Color.fromRGB565 = function(val){
    let lb = val & 255
    let hb = (val >> 8) & 255
    return new Color(
        Math.round(((hb>>3) & 255) * 255 / 31) ,
        Math.round((((hb & 0x07) << 3) | ((lb >> 5) & 255))*255/63) ,
        Math.round((lb & 0x1f)* 255 / 31) 
    )
}

Number.prototype.toRGB = function(format) {
    if(!format) {
        format = 'RGB565'
    }
    if(!Color['from'+format]) {
        throw new Error('unknow color format: '+format)
    }
    return Color['from'+format](this)
}

module.exports = Color