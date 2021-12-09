
/**
 * 
 *     lfttop        top          rgttop
 *        O-----------O-------------O
 *        |                         |
 *        |                         |
 *    lft O        center           O rgt
 *        |                         |
 *        |                         |
 *        O-----------O-------------O
 *     lftbtm        btm          rgtbtm
 * 
 */

const Start = 0
const Center = 1
const End = 2

const AnchorWays = {
    lfttop: [ Start,   Start ],
    top:    [ Center,  Start ],
    rgttop: [ End,     Start ],
    lft:    [ Start,  Center ],
    center: [ Center, Center ],
    rgt:    [ End,    Center ],
    lftbtm: [ Start,     End ],
    btm:    [ Center,    End ],
    rgtbtm: [ End,       End ],
}

const DirH = 0
const DirV = 1

const Ways = []
Ways[DirH] = [
    ["lfttop","lft","lftbtm"] ,
    ["top","center","btm"] ,
    ["rgttop","rgt","rgtbtm"] ,
]
Ways[DirV] = [
    ["lfttop","top","rgttop"] ,
    ["lft","center","rgt"] ,
    ["lftbtm","btm","rgtbtm"] ,
]

const Frozen = 0
const ResizableForward = 1
const ResizableBackward = 2
const Resizable = 3
const Movable = 4
const Free = 7

class Anchor {

    constructor(rect, name){
        this.rect = rect
        this.name = name
    }
    
    way = null
    rect = null

    _linkTo = null
    _offsetX = 0
    _offsety = 0
    _beLinkeds = []

    // 递归检查锚点是否已经被固定
    isFixed(dir, olnyLinks, ignore) {
        // 根rect 
        let parent = this.rect.getParent()
        if(!parent) {
            return true
        }
        if(this._linkTo!=ignore && this._linkTo && this._linkTo.isFixed(dir, false, this)) {
            return true
        }
        for(let another of this._beLinkeds) {
            if(ignore!=another && another.isFixed(dir, false, this)) {
                return true
            } 
        }
        if(!olnyLinks) {
            return false ;
        }
    
        // 检查通路节点
    
        return false
    }

    another(link) {
        if(link.from==this) {
            return link.to
        }
        else if(link.to==this) {
            return link.from
        }
        return null
    }

    linkTo(target, anchorName, offsetX, offsetY) {
        if(!target){
            target = this.rect.getParent()
        }
        if(!anchorName) {
            anchorName = this.name
        }
        let to = target.anchor(anchorName)
        if(this._linkTo==to) {
            return
        }
        for(let anchor of this._beLinkeds) {
            if( anchor._linkTo==this ){
                return
            }
        }
        
        this._linkTo = to
        this._offsetX = offsetX || 0
        this._offsetY = offsetY || 0
        this._beLinkeds.push(to)

        this.update()
    }

    coords() {
        let [x,y] = this.rect.getCoords()
        if(AnchorWays[this.name][0]) {
            x+= this.rect.getWidth() * AnchorWays[this.name][0] / 2
        }
        if(AnchorWays[this.name][1]) {
            y+= this.rect.getHeight() * AnchorWays[this.name][1] / 2
        }
        return [x, y]
    }

    update() {
        let [x,y] = this._linkTo.coords()
        x+= this._offsetX
        y+= this._offsetY

        let [_x, _y] = this.coords()
        console.log("from", _x, _y)
        if(x!=_x) {
            updateAnchorToward(this, DirH, x-_x, "moveX")
        }
        if(y!=_y) {
            updateAnchorToward(this, DirV, y-_y, "moveY")
        }
    }
}

function updateAnchorToward(anchor, dir, delta, moveFunc) {
    let state = dirState(anchor.rect, dir, AnchorWays[anchor.name][dir])
    if(state==StateFrozen) {
        throw new Error("rect could not both move and resize on dir horizonal")
    }
    else if(state==StateNoResize||state==StateFree) {
        anchor.rect[moveFunc](delta)
    }
    else if(state==StateNoMove) {
    }

}

function wayState(rect, way) {
    for(let a of way) {
        if(!rect._a9[a]) {
            continue
        }
        if(rect._a9[a].isFixed()) {
            return true
        }
    }
    return false
}

function dirState(rect, dir, ignoreWay) {
    let fixedWays = 0
    let ways = Ways[dir]
    for(let w=0;w<ways.length;w++) {
        if(ignoreWay==w){
            continue
        }
    }
    let fixedSize = !! rect._a9[dir==0? 'width': 'height'] 
    if(fixedWays==2){
        return Frozen
    }
    else if(fixedWays==1) {
        return fixedSize? Frozen: Resizable
    }
    else {
        return fixedSize? Movable: Free
    }
}


beapi.lvgl.Obj.prototype.anchor = function(name, acceptNull) {
    if(!AnchorWays[name]){
        throw new Error("unknow anchor name: "+name)
    }
    if(!this._a9) {
        if(acceptNull) {
            return null
        }
        this._a9 = {}
    }
    if(!this._a9[name]) {
        if(acceptNull) {
            return null
        }
        this._a9[name] = new Anchor(this, name)
    }
    return this._a9[name]
}

beapi.lvgl.Obj.prototype.setFixedWidth = function(val) {
    if(!this._a9) {
        this._a9 = {}
    }
    this._a9.width = val
}
beapi.lvgl.Obj.prototype.getFixedWidth = function() {
    return this._a9? this._a9.width: -1
}
beapi.lvgl.Obj.prototype.setFixedHeight = function(val) {
    if(!this._a9) {
        this._a9 = {}
    }
    this._a9.height = val
}
beapi.lvgl.Obj.prototype.getFixedHeight = function() {
    return this._a9? this._a9.height: -1
}
