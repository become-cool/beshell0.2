
let hb = 0x75 
let lb = 0x69

let pi = 0
let pixels = []

// RRRRR GGG | GGG BBBBB
pixels[pi] = Math.round(((hb>>3) & 255) * 255 / 31 )
pixels[pi+1] = Math.round((((hb & 0x07) << 3) | ((lb >> 5) & 255))*255/63)
pixels[pi+2] = Math.round((lb & 0x1f)* 255 / 31 )
pixels[pi+3] = 255


console.log("BE RGB:   rgb(", pixels[0], pixels[1], pixels[2],")")
console.log("BE BGR:   rgb(", pixels[2], pixels[1], pixels[0],")")

hb = 0x69
lb = 0x75 
pi = 0
pixels.length = 0

// RRRRR GGG | GGG BBBBB
pixels[pi] = Math.round(((hb>>3) & 255) * 255 / 31 )
pixels[pi+1] = Math.round((((hb & 0x07) << 3) | ((lb >> 5) & 255))*255/63)
pixels[pi+2] = Math.round((lb & 0x1f)* 255 / 31 )
pixels[pi+3] = 255

console.log("LE RGB:   rgb(", pixels[0], pixels[1], pixels[2],")")
console.log("LE BGR:   rgb(", pixels[2], pixels[1], pixels[0],")")