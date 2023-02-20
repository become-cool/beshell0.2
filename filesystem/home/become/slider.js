console.log("==========================")
console.log("Camera Slideway")
console.log("==========================")


be.dev.joypad1.indev.enableEvent()

let target = {
    a : {
        x: 0 ,
        z: 0 ,
        x_with_z: 0 ,   // z轴联动位置：x轴途径此处时，z开始或到达
    } ,
    b : {
        x: 0 ,
        z: 0 ,
        x_with_z: 0 ,   // z轴联动位置：x轴途径此处时，z开始或到达
    }
}

// true: run to target.b ;  false: run to target.a
let autoTravalDir = true  

let keyStat = {
    'left' : false ,
    'right' : false ,
    'up' : false ,
    'down' : false ,
}


let travalXFreq = 100
let travalXMax = 500

let travalZFreq = 50
let travalZMax = 500


be.dev.stepperX.setFreq(travalXFreq)
be.dev.stepperZ.setFreq(travalZFreq)

be.dev.joypad1.indev.on("*",(act, key)=>{    

    if(act=='press') {
        switch(key) {
            case 'left':
                be.dev.stepperX.run(false)
                break
            case 'right':
                be.dev.stepperX.run(true)
                break

            case 'up':
                be.dev.stepperZ.run(true)
                break
            case 'down':
                be.dev.stepperZ.run(false)
                break

            case 'a' :
                if( !keyStat.select ) {
                    target.a.x = be.dev.stepperX.pos()
                }
                target.a.z = be.dev.stepperZ.pos()
                target.a.x_with_z = be.dev.stepperX.pos() // z轴联动位置
                
                autoTravalDir = true
                console.log(target.a)
                break
            case 'b' :
                if( !keyStat.select ) {
                    target.b.x = be.dev.stepperX.pos()
                }
                target.b.z = be.dev.stepperZ.pos()
                target.b.x_with_z = be.dev.stepperX.pos() // z轴联动位置

                autoTravalDir = false
                console.log(target.b)
                break

            case 'start':

                // select + start 重启
                if(keyStat.select) {
                    process.reboot()
                }

                // 中途停止
                if(be.dev.stepperX.isRunning()) {
                    be.dev.stepperX.stop()
                    be.dev.stepperZ.stop()
                    autoTravalDir = !autoTravalDir // 切换回本来的方向
                }

                else {
                    if(autoTravalDir) {
                        audoTravel(target.b, target.a)
                    } else {
                        audoTravel(target.a, target.b)
                    }
                    autoTravalDir = !autoTravalDir
                }

                break

            case 'select':
                
                // z轴加速
                if(keyStat.up) {
                    travalZFreq = Math.min(travalZFreq+50, travalZMax)
                    be.dev.stepperZ.setFreq(travalZFreq)
                }
                // z轴减速
                else if(keyStat.down) {
                    travalZFreq = Math.max(travalZFreq-50, 0)
                    be.dev.stepperZ.setFreq(travalZFreq)
                }

                // x轴加速
                else if(keyStat.right) {
                    travalXFreq = Math.min(travalXFreq+50, travalZMax)
                    be.dev.stepperX.setFreq(travalXFreq)
                }
                // x轴减速
                else if(keyStat.left) {
                    travalXFreq = Math.max(travalXFreq-50, 0)
                    be.dev.stepperX.setFreq(travalXFreq)
                }

                break
        }

        keyStat[key] = true
    }

    else if(act=='release') {
        if( key=='left' || key=='right' ) {
            be.dev.stepperX.stop()
        }
        else if( key=='up' || key=='down' ) {
            be.dev.stepperZ.stop()
        }
        
        keyStat[key] = false
    }
})


function audoTravel(to,_from) {

    let from = {
        x: be.dev.stepperX.pos() ,
        z: be.dev.stepperZ.pos() ,
    }
    let distX = Math.abs( to.x - from.x )
    let distZ = Math.abs( to.z - from.z )

    console.log("-------------")
    console.log("[x]", from.x, '->', to.x, ' = ', distX)
    console.log("[z]", from.z, '->', to.z, ' = ', distZ)

    console.log("x freq =", travalXFreq)

    // z轴不参与运动 (仅x轴运动)
    if( distZ == 0 ) {
        if(distX!=0) {
            be.dev.stepperX.runTo(to.x, travalXFreq)
        }
    }

    // z轴参与运动
    else {

        // x轴不参与运动 (仅z轴原地转动)
        if( distX == 0) {
            console.log("z freq =", travalZFreq)
            be.dev.stepperZ.runTo(to.z, travalZFreq)
        }

        // z轴 和 x轴 联动
        else {
            console.log("z async")

            // 计算 z 轴速度
            let distX_with_z = to.x_with_z - _from.x_with_z
            let freqZ = Math.floor( distZ/distX_with_z * travalXFreq )
    
            console.log("z freq =", freqZ)
            console.log("[x with z]", _from.x_with_z, '->', to.x_with_z, ' = ', distX_with_z)

            // z轴 和 x轴同步开始，不需要 x轴的 passing-by 事件
            if( from.x == _from.x_with_z ) {
                be.dev.stepperZ.runTo(to.z, travalXFreq)
            }

            // 到达 from.x_with_z 后开始 z 轴
            else {
                console.log("z later")
                be.dev.stepperX.setPassingBy(_from.x_with_z)
                be.dev.stepperX.race(['stop','passing-by'],(evt,pos)=>{
                    console.log(evt,pos)
                    if(evt=='passing-by') {
                        console.log("stepperZ.runTo()",to.z, ", from", be.dev.stepperZ.pos())
                        be.dev.stepperZ.runTo(to.z, travalXFreq)
                    }
                })
            }

            be.dev.stepperX.runTo(to.x, travalXFreq)
        }
    }
}

global.target = target