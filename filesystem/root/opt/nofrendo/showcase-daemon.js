const waitms = 60*1000

function main() {

    console.log("showcase-daemon:", waitms)
    
    let disp = beapi.lvgl.activeDisplay()
    if(!disp) {
        console.log("lvgl not start")
        return
    }

    let lastIpt = Date.now()
    // 
    disp.on("ipt.btn.press",key=>{
        lastIpt = Date.now()
        console.log("ipt.btn.press")
    })

    setInterval(()=>{

        console.log("showcase-daemon timer on")

        if(Date.now()-lastIpt <= waitms) {
            return
        }

        console.log("No input detected for more than",waitms,", Switching to Showcase mode")

        setTimeout(()=>{
            require("/etc/init.d/boot.js").rebootToApp(__dirname + "/playrom.js", true, "showcase")
        }, 0)

    },waitms/2)

}
main()
