class WAMSObject {
    ptr = 0
}
class EventEmitter extends WAMSObject {
    _handles = {}
    /**
     * 
     * @param {string|string[]} event 
     * @param {function} handle 
     * @param {bool} norepeat 
     */
    on(event, handle, norepeat) {
        if(typeof(event)=="string") {
            if(!this._handles[event]){
                this._handles[event] = []
                if(event!="#EVENT.ADD#"&&event!="#EVENT.CLEAR#")
                    this.emit("#EVENT.ADD#",event)
            }
            if(!norepeat || !this.isListening(event, handle)) {
                this._handles[event].push(handle)
            }
        }
        else if(event instanceof Array) {
            for(var eventName of event) {
                this.on(eventName, handle, repeat)
            }
        }
        return
    }
    /**
     * 
     * @param {string} eventName 
     * @param {function} handle 
     */
    once(eventName, handle, norepeat) {
        var wrapper =  (...args) => {
            this.off(eventName, wrapper)
            handle.apply(this, args)
        }
        wrapper.__origin = handle
        this.on(eventName, wrapper, norepeat)
        return
    }
    off(eventName, handle, all) {
        if(!this._handles[eventName]) {
            return
        }
        for(let h=this._handles[eventName].length-1; h>=0; h--) {
            let func = this.originHanlde(this._handles[eventName][h])
            if(!handle || handle==func) {
                this._handles[eventName][h].__origin = null
                this._handles[eventName].splice(h,1)
                if(handle && !all) {
                    break
                }
            }
        }
        if(!this._handles[eventName].length) {
            delete this._handles[eventName]
            if(eventName!="#EVENT.ADD#"&&eventName!="#EVENT.CLEAR#")
                this.emit("#EVENT.CLEAR#",eventName)
        }
        return
    }
    originHanlde(handle) {
        for(var h=handle; h.__origin; h=h.__origin){}
        return h
    }
    isListening(event, handle) {
        if(!this._handles[event])
            return false
        for(let cb of this._handles[event]) {
            if( this.originHanlde(cb)==handle )
                return true
        }
        return false
    }
    emit(eventName, ...args) {
        if(eventName!='*'&&this._handles[eventName]) {
            for(let handle of this._handles[eventName]) {
                handle.apply(this, args)
            }
        }
        if(this._handles["*"]) {
            for(let handle of this._handles["*"]) {
                handle.apply(this, [eventName, ...args])
            }
        }
        return
    }
    destroy() {
        for(let eventName in this._handles) {
            this._handles[eventName].forEach(cb=>{
                if(cb.__origin) {
                    delete cb.__origin
                }
            })
            this._handles[eventName].splice(0)
            delete this._handles[eventName]
        }
    }
}

class DispDrawBuf extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_disp_draw_buf_create()
    }
// AUTO GENERATE CODE START [DispDrawBuf METHODS] --------
    init(buf1, buf2, size_in_px_cnt){
        return Module._lv_disp_draw_buf_init(this.ptr, buf1, buf2, size_in_px_cnt)
    }
    buf1(){
        return Module._lv_disp_draw_buf_get_buf1(this.ptr)
    }
    setBuf1(buf1){
        return Module._lv_disp_draw_buf_set_buf1(this.ptr, buf1)
    }
    buf2(){
        return Module._lv_disp_draw_buf_get_buf2(this.ptr)
    }
    setBuf2(buf2){
        return Module._lv_disp_draw_buf_set_buf2(this.ptr, buf2)
    }
    bufAct(){
        return Module._lv_disp_draw_buf_get_buf_act(this.ptr)
    }
    setBufAct(buf_act){
        return Module._lv_disp_draw_buf_set_buf_act(this.ptr, buf_act)
    }
    size(){
        return Module._lv_disp_draw_buf_get_size(this.ptr)
    }
    setSize(size){
        return Module._lv_disp_draw_buf_set_size(this.ptr, size)
    }
    area(){
        return Module._lv_disp_draw_buf_get_area(this.ptr)
    }
    setArea(area){
        return Module._lv_disp_draw_buf_set_area(this.ptr, area)
    }
    flushing(){
        return Module._lv_disp_draw_buf_get_flushing(this.ptr)
    }
    setFlushing(flushing){
        return Module._lv_disp_draw_buf_set_flushing(this.ptr, flushing)
    }
    flushingLast(){
        return Module._lv_disp_draw_buf_get_flushing_last(this.ptr)
    }
    setFlushingLast(flushing_last){
        return Module._lv_disp_draw_buf_set_flushing_last(this.ptr, flushing_last)
    }
    lastArea(){
        return Module._lv_disp_draw_buf_get_last_area(this.ptr)
    }
    setLastArea(last_area){
        return Module._lv_disp_draw_buf_set_last_area(this.ptr, last_area)
    }
    lastPart(){
        return Module._lv_disp_draw_buf_get_last_part(this.ptr)
    }
    setLastPart(last_part){
        return Module._lv_disp_draw_buf_set_last_part(this.ptr, last_part)
    }
// AUTO GENERATE CODE END [DispDrawBuf METHODS] --------
}

class DispDrv extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_disp_drv_create()
        Module._lv_disp_drv_init(this.ptr)
    }
// AUTO GENERATE CODE START [DispDrv METHODS] --------
    init(){
        return Module._lv_disp_drv_init(this.ptr)
    }
    register(){
        return Module._lv_disp_drv_register(this.ptr)
    }
    useGenericSetPxCb(cf){
        return Module._lv_disp_drv_use_generic_set_px_cb(this.ptr, cf)
    }
    horRes(){
        return Module._lv_disp_drv_get_hor_res(this.ptr)
    }
    setHorRes(hor_res){
        return Module._lv_disp_drv_set_hor_res(this.ptr, hor_res)
    }
    verRes(){
        return Module._lv_disp_drv_get_ver_res(this.ptr)
    }
    setVerRes(ver_res){
        return Module._lv_disp_drv_set_ver_res(this.ptr, ver_res)
    }
    physicalHorRes(){
        return Module._lv_disp_drv_get_physical_hor_res(this.ptr)
    }
    setPhysicalHorRes(physical_hor_res){
        return Module._lv_disp_drv_set_physical_hor_res(this.ptr, physical_hor_res)
    }
    physicalVerRes(){
        return Module._lv_disp_drv_get_physical_ver_res(this.ptr)
    }
    setPhysicalVerRes(physical_ver_res){
        return Module._lv_disp_drv_set_physical_ver_res(this.ptr, physical_ver_res)
    }
    offsetX(){
        return Module._lv_disp_drv_get_offset_x(this.ptr)
    }
    setOffsetX(offset_x){
        return Module._lv_disp_drv_set_offset_x(this.ptr, offset_x)
    }
    offsetY(){
        return Module._lv_disp_drv_get_offset_y(this.ptr)
    }
    setOffsetY(offset_y){
        return Module._lv_disp_drv_set_offset_y(this.ptr, offset_y)
    }
    drawBuf(){
        return Module._lv_disp_drv_get_draw_buf(this.ptr)
    }
    setDrawBuf(draw_buf){
        return Module._lv_disp_drv_set_draw_buf(this.ptr, draw_buf)
    }
    directMode(){
        return Module._lv_disp_drv_get_direct_mode(this.ptr)
    }
    setDirectMode(direct_mode){
        return Module._lv_disp_drv_set_direct_mode(this.ptr, direct_mode)
    }
    fullRefresh(){
        return Module._lv_disp_drv_get_full_refresh(this.ptr)
    }
    setFullRefresh(full_refresh){
        return Module._lv_disp_drv_set_full_refresh(this.ptr, full_refresh)
    }
    swRotate(){
        return Module._lv_disp_drv_get_sw_rotate(this.ptr)
    }
    setSwRotate(sw_rotate){
        return Module._lv_disp_drv_set_sw_rotate(this.ptr, sw_rotate)
    }
    antialiasing(){
        return Module._lv_disp_drv_get_antialiasing(this.ptr)
    }
    setAntialiasing(antialiasing){
        return Module._lv_disp_drv_set_antialiasing(this.ptr, antialiasing)
    }
    rotated(){
        return Module._lv_disp_drv_get_rotated(this.ptr)
    }
    setRotated(rotated){
        return Module._lv_disp_drv_set_rotated(this.ptr, rotated)
    }
    screenTransp(){
        return Module._lv_disp_drv_get_screen_transp(this.ptr)
    }
    setScreenTransp(screen_transp){
        return Module._lv_disp_drv_set_screen_transp(this.ptr, screen_transp)
    }
    dpi(){
        return Module._lv_disp_drv_get_dpi(this.ptr)
    }
    setDpi(dpi){
        return Module._lv_disp_drv_set_dpi(this.ptr, dpi)
    }
    flushCb(){
        return Module._lv_disp_drv_get_flush_cb(this.ptr)
    }
    setFlushCb(flush_cb){
        return Module._lv_disp_drv_set_flush_cb(this.ptr, flush_cb)
    }
    rounderCb(){
        return Module._lv_disp_drv_get_rounder_cb(this.ptr)
    }
    setRounderCb(rounder_cb){
        return Module._lv_disp_drv_set_rounder_cb(this.ptr, rounder_cb)
    }
    setPxCb(){
        return Module._lv_disp_drv_get_set_px_cb(this.ptr)
    }
    setSetPxCb(set_px_cb){
        return Module._lv_disp_drv_set_set_px_cb(this.ptr, set_px_cb)
    }
    monitorCb(){
        return Module._lv_disp_drv_get_monitor_cb(this.ptr)
    }
    setMonitorCb(monitor_cb){
        return Module._lv_disp_drv_set_monitor_cb(this.ptr, monitor_cb)
    }
    waitCb(){
        return Module._lv_disp_drv_get_wait_cb(this.ptr)
    }
    setWaitCb(wait_cb){
        return Module._lv_disp_drv_set_wait_cb(this.ptr, wait_cb)
    }
    cleanDcacheCb(){
        return Module._lv_disp_drv_get_clean_dcache_cb(this.ptr)
    }
    setCleanDcacheCb(clean_dcache_cb){
        return Module._lv_disp_drv_set_clean_dcache_cb(this.ptr, clean_dcache_cb)
    }
    gpuWaitCb(){
        return Module._lv_disp_drv_get_gpu_wait_cb(this.ptr)
    }
    setGpuWaitCb(gpu_wait_cb){
        return Module._lv_disp_drv_set_gpu_wait_cb(this.ptr, gpu_wait_cb)
    }
    drvUpdateCb(){
        return Module._lv_disp_drv_get_drv_update_cb(this.ptr)
    }
    setDrvUpdateCb(drv_update_cb){
        return Module._lv_disp_drv_set_drv_update_cb(this.ptr, drv_update_cb)
    }
    gpuFillCb(){
        return Module._lv_disp_drv_get_gpu_fill_cb(this.ptr)
    }
    setGpuFillCb(gpu_fill_cb){
        return Module._lv_disp_drv_set_gpu_fill_cb(this.ptr, gpu_fill_cb)
    }
    colorChromaKey(){
        return Module._lv_disp_drv_get_color_chroma_key(this.ptr)
    }
    setColorChromaKey(color_chroma_key){
        return Module._lv_disp_drv_set_color_chroma_key(this.ptr, color_chroma_key)
    }
    userData(){
        return Module._lv_disp_drv_get_user_data(this.ptr)
    }
    setUserData(user_data){
        return Module._lv_disp_drv_set_user_data(this.ptr, user_data)
    }
// AUTO GENERATE CODE END [DispDrv METHODS] --------
}

class Disp extends WAMSObject {
    constructor(drv) {
        super()
        this.ptr = Module._lv_disp_drv_register(drv.ptr)
    }
// AUTO GENERATE CODE START [Disp METHODS] --------
    remove(){
        return Module._lv_disp_remove(this.ptr)
    }
    setDefault(){
        return Module._lv_disp_set_default(this.ptr)
    }
    horRes(){
        return Module._lv_disp_get_hor_res(this.ptr)
    }
    verRes(){
        return Module._lv_disp_get_ver_res(this.ptr)
    }
    physicalHorRes(){
        return Module._lv_disp_get_physical_hor_res(this.ptr)
    }
    physicalVerRes(){
        return Module._lv_disp_get_physical_ver_res(this.ptr)
    }
    offsetX(){
        return Module._lv_disp_get_offset_x(this.ptr)
    }
    offsetY(){
        return Module._lv_disp_get_offset_y(this.ptr)
    }
    antialiasing(){
        return Module._lv_disp_get_antialiasing(this.ptr)
    }
    setRotation(rotation){
        return Module._lv_disp_set_rotation(this.ptr, rotation)
    }
    rotation(){
        return Module._lv_disp_get_rotation(this.ptr)
    }
    next(){
        return Module._lv_disp_get_next(this.ptr)
    }
    drawBuf(){
        return Module._lv_disp_get_draw_buf(this.ptr)
    }
    scrAct(){
        return Module._lv_disp_get_scr_act(this.ptr)
    }
    scrPrev(){
        return Module._lv_disp_get_scr_prev(this.ptr)
    }
    layerTop(){
        return Module._lv_disp_get_layer_top(this.ptr)
    }
    layerSys(){
        return Module._lv_disp_get_layer_sys(this.ptr)
    }
    setTheme(th){
        return Module._lv_disp_set_theme(this.ptr, th)
    }
    theme(){
        return Module._lv_disp_get_theme(this.ptr)
    }
    setBgColor(color){
        return Module._lv_disp_set_bg_color(this.ptr, color)
    }
    setBgImage(img_src){
        return Module._lv_disp_set_bg_image(this.ptr, img_src)
    }
    setBgOpa(opa){
        return Module._lv_disp_set_bg_opa(this.ptr, opa)
    }
    trigActivity(){
        return Module._lv_disp_trig_activity(this.ptr)
    }
    cleanDcache(){
        return Module._lv_disp_clean_dcache(this.ptr)
    }
    driver(){
        return Module._lv_disp_get_driver(this.ptr)
    }
    setDriver(driver){
        return Module._lv_disp_set_driver(this.ptr, driver)
    }
    refrTimer(){
        return Module._lv_disp_get_refr_timer(this.ptr)
    }
    setRefrTimer(refr_timer){
        return Module._lv_disp_set_refr_timer(this.ptr, refr_timer)
    }
    screens(){
        return Module._lv_disp_get_screens(this.ptr)
    }
    setScreens(screens){
        return Module._lv_disp_set_screens(this.ptr, screens)
    }
    actScr(){
        return Module._lv_disp_get_act_scr(this.ptr)
    }
    setActScr(act_scr){
        return Module._lv_disp_set_act_scr(this.ptr, act_scr)
    }
    prevScr(){
        return Module._lv_disp_get_prev_scr(this.ptr)
    }
    setPrevScr(prev_scr){
        return Module._lv_disp_set_prev_scr(this.ptr, prev_scr)
    }
    scrToLoad(){
        return Module._lv_disp_get_scr_to_load(this.ptr)
    }
    setScrToLoad(scr_to_load){
        return Module._lv_disp_set_scr_to_load(this.ptr, scr_to_load)
    }
    topLayer(){
        return Module._lv_disp_get_top_layer(this.ptr)
    }
    setTopLayer(top_layer){
        return Module._lv_disp_set_top_layer(this.ptr, top_layer)
    }
    sysLayer(){
        return Module._lv_disp_get_sys_layer(this.ptr)
    }
    setSysLayer(sys_layer){
        return Module._lv_disp_set_sys_layer(this.ptr, sys_layer)
    }
    screenCnt(){
        return Module._lv_disp_get_screen_cnt(this.ptr)
    }
    setScreenCnt(screen_cnt){
        return Module._lv_disp_set_screen_cnt(this.ptr, screen_cnt)
    }
    delPrev(){
        return Module._lv_disp_get_del_prev(this.ptr)
    }
    setDelPrev(del_prev){
        return Module._lv_disp_set_del_prev(this.ptr, del_prev)
    }
    bgOpa(){
        return Module._lv_disp_get_bg_opa(this.ptr)
    }
    bgColor(){
        return Module._lv_disp_get_bg_color(this.ptr)
    }
    bgImg(){
        return Module._lv_disp_get_bg_img(this.ptr)
    }
    setBgImg(bg_img){
        return Module._lv_disp_set_bg_img(this.ptr, bg_img)
    }
    bgFn(){
        return Module._lv_disp_get_bg_fn(this.ptr)
    }
    setBgFn(bg_fn){
        return Module._lv_disp_set_bg_fn(this.ptr, bg_fn)
    }
    invAreas(){
        return Module._lv_disp_get_inv_areas(this.ptr)
    }
    setInvAreas(inv_areas){
        return Module._lv_disp_set_inv_areas(this.ptr, inv_areas)
    }
    invAreaJoined(){
        return Module._lv_disp_get_inv_area_joined(this.ptr)
    }
    setInvAreaJoined(inv_area_joined){
        return Module._lv_disp_set_inv_area_joined(this.ptr, inv_area_joined)
    }
    invP(){
        return Module._lv_disp_get_inv_p(this.ptr)
    }
    setInvP(inv_p){
        return Module._lv_disp_set_inv_p(this.ptr, inv_p)
    }
    lastActivityTime(){
        return Module._lv_disp_get_last_activity_time(this.ptr)
    }
    setLastActivityTime(last_activity_time){
        return Module._lv_disp_set_last_activity_time(this.ptr, last_activity_time)
    }
// AUTO GENERATE CODE END [Disp METHODS] --------
}
class Obj extends WAMSObject {
    constructor(parent) {
        super()
        this.ptr = Module._lv_obj_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [Obj METHODS] --------
    del(){
        return Module._lv_obj_del(this.ptr)
    }
    clean(){
        return Module._lv_obj_clean(this.ptr)
    }
    delDelayed(delay_ms){
        return Module._lv_obj_del_delayed(this.ptr, delay_ms)
    }
    delAsync(){
        return Module._lv_obj_del_async(this.ptr)
    }
    setParent(parent){
        return Module._lv_obj_set_parent(this.ptr, parent)
    }
    swap(obj2){
        return Module._lv_obj_swap(this.ptr, obj2)
    }
    moveToIndex(index){
        return Module._lv_obj_move_to_index(this.ptr, index)
    }
    treeWalk(cb, user_data){
        return Module._lv_obj_tree_walk(this.ptr, cb, user_data)
    }
    setPos(x, y){
        return Module._lv_obj_set_pos(this.ptr, x, y)
    }
    setX(x){
        return Module._lv_obj_set_x(this.ptr, x)
    }
    setY(y){
        return Module._lv_obj_set_y(this.ptr, y)
    }
    setSize(w, h){
        return Module._lv_obj_set_size(this.ptr, w, h)
    }
    refrSize(){
        return Module._lv_obj_refr_size(this.ptr)
    }
    setWidth(w){
        return Module._lv_obj_set_width(this.ptr, w)
    }
    setHeight(h){
        return Module._lv_obj_set_height(this.ptr, h)
    }
    setContentWidth(w){
        return Module._lv_obj_set_content_width(this.ptr, w)
    }
    setContentHeight(h){
        return Module._lv_obj_set_content_height(this.ptr, h)
    }
    setLayout(layout){
        return Module._lv_obj_set_layout(this.ptr, layout)
    }
    markLayoutAsDirty(){
        return Module._lv_obj_mark_layout_as_dirty(this.ptr)
    }
    setAlign(align){
        return Module._lv_obj_set_align(this.ptr, align)
    }
    align(align, x_ofs, y_ofs){
        return Module._lv_obj_align(this.ptr, align, x_ofs, y_ofs)
    }
    alignTo(base, align, x_ofs, y_ofs){
        return Module._lv_obj_align_to(this.ptr, base, align, x_ofs, y_ofs)
    }
    refreshSelfSize(){
        return Module._lv_obj_refresh_self_size(this.ptr)
    }
    refrPos(){
        return Module._lv_obj_refr_pos(this.ptr)
    }
    moveTo(x, y){
        return Module._lv_obj_move_to(this.ptr, x, y)
    }
    moveChildrenBy(x_diff, y_diff, ignore_floating){
        return Module._lv_obj_move_children_by(this.ptr, x_diff, y_diff, ignore_floating)
    }
    setExtClickArea(size){
        return Module._lv_obj_set_ext_click_area(this.ptr, size)
    }
    hitTest(point){
        return Module._lv_obj_hit_test(this.ptr, point)
    }
    setScrollbarMode(mode){
        return Module._lv_obj_set_scrollbar_mode(this.ptr, mode)
    }
    setScrollDir(dir){
        return Module._lv_obj_set_scroll_dir(this.ptr, dir)
    }
    setScrollSnapX(align){
        return Module._lv_obj_set_scroll_snap_x(this.ptr, align)
    }
    setScrollSnapY(align){
        return Module._lv_obj_set_scroll_snap_y(this.ptr, align)
    }
    scrollTop(){
        return Module._lv_obj_get_scroll_top(this.ptr)
    }
    scrollBottom(){
        return Module._lv_obj_get_scroll_bottom(this.ptr)
    }
    scrollLeft(){
        return Module._lv_obj_get_scroll_left(this.ptr)
    }
    scrollRight(){
        return Module._lv_obj_get_scroll_right(this.ptr)
    }
    scrollEnd(end){
        return Module._lv_obj_get_scroll_end(this.ptr, end)
    }
    scrollBy(x, y, anim_en){
        return Module._lv_obj_scroll_by(this.ptr, x, y, anim_en)
    }
    scrollTo(x, y, anim_en){
        return Module._lv_obj_scroll_to(this.ptr, x, y, anim_en)
    }
    scrollToX(x, anim_en){
        return Module._lv_obj_scroll_to_x(this.ptr, x, anim_en)
    }
    scrollToY(y, anim_en){
        return Module._lv_obj_scroll_to_y(this.ptr, y, anim_en)
    }
    scrollToView(anim_en){
        return Module._lv_obj_scroll_to_view(this.ptr, anim_en)
    }
    scrollToViewRecursive(anim_en){
        return Module._lv_obj_scroll_to_view_recursive(this.ptr, anim_en)
    }
    updateSnap(anim_en){
        return Module._lv_obj_update_snap(this.ptr, anim_en)
    }
    scrollbarArea(hor, ver){
        return Module._lv_obj_get_scrollbar_area(this.ptr, hor, ver)
    }
    scrollbarInvalidate(){
        return Module._lv_obj_scrollbar_invalidate(this.ptr)
    }
    readjustScroll(anim_en){
        return Module._lv_obj_readjust_scroll(this.ptr, anim_en)
    }
    addStyle(style, selector){
        return Module._lv_obj_add_style(this.ptr, style, selector)
    }
    removeStyle(style, selector){
        return Module._lv_obj_remove_style(this.ptr, style, selector)
    }
    refreshStyle(part, prop){
        return Module._lv_obj_refresh_style(this.ptr, part, prop)
    }
    setLocalStyleProp(prop, value, selector){
        return Module._lv_obj_set_local_style_prop(this.ptr, prop, value, selector)
    }
    localStyleProp(prop, value, selector){
        return Module._lv_obj_get_local_style_prop(this.ptr, prop, value, selector)
    }
    removeLocalStyleProp(prop, selector){
        return Module._lv_obj_remove_local_style_prop(this.ptr, prop, selector)
    }
    fadeIn(time, delay){
        return Module._lv_obj_fade_in(this.ptr, time, delay)
    }
    fadeOut(time, delay){
        return Module._lv_obj_fade_out(this.ptr, time, delay)
    }
    setStyleWidth(value, selector){
        return Module._lv_obj_set_style_width(this.ptr, value, selector)
    }
    setStyleMinWidth(value, selector){
        return Module._lv_obj_set_style_min_width(this.ptr, value, selector)
    }
    setStyleMaxWidth(value, selector){
        return Module._lv_obj_set_style_max_width(this.ptr, value, selector)
    }
    setStyleHeight(value, selector){
        return Module._lv_obj_set_style_height(this.ptr, value, selector)
    }
    setStyleMinHeight(value, selector){
        return Module._lv_obj_set_style_min_height(this.ptr, value, selector)
    }
    setStyleMaxHeight(value, selector){
        return Module._lv_obj_set_style_max_height(this.ptr, value, selector)
    }
    setStyleX(value, selector){
        return Module._lv_obj_set_style_x(this.ptr, value, selector)
    }
    setStyleY(value, selector){
        return Module._lv_obj_set_style_y(this.ptr, value, selector)
    }
    setStyleAlign(value, selector){
        return Module._lv_obj_set_style_align(this.ptr, value, selector)
    }
    setStyleTransformWidth(value, selector){
        return Module._lv_obj_set_style_transform_width(this.ptr, value, selector)
    }
    setStyleTransformHeight(value, selector){
        return Module._lv_obj_set_style_transform_height(this.ptr, value, selector)
    }
    setStyleTranslateX(value, selector){
        return Module._lv_obj_set_style_translate_x(this.ptr, value, selector)
    }
    setStyleTranslateY(value, selector){
        return Module._lv_obj_set_style_translate_y(this.ptr, value, selector)
    }
    setStyleTransformZoom(value, selector){
        return Module._lv_obj_set_style_transform_zoom(this.ptr, value, selector)
    }
    setStyleTransformAngle(value, selector){
        return Module._lv_obj_set_style_transform_angle(this.ptr, value, selector)
    }
    setStylePadTop(value, selector){
        return Module._lv_obj_set_style_pad_top(this.ptr, value, selector)
    }
    setStylePadBottom(value, selector){
        return Module._lv_obj_set_style_pad_bottom(this.ptr, value, selector)
    }
    setStylePadLeft(value, selector){
        return Module._lv_obj_set_style_pad_left(this.ptr, value, selector)
    }
    setStylePadRight(value, selector){
        return Module._lv_obj_set_style_pad_right(this.ptr, value, selector)
    }
    setStylePadRow(value, selector){
        return Module._lv_obj_set_style_pad_row(this.ptr, value, selector)
    }
    setStylePadColumn(value, selector){
        return Module._lv_obj_set_style_pad_column(this.ptr, value, selector)
    }
    setStyleRadius(value, selector){
        return Module._lv_obj_set_style_radius(this.ptr, value, selector)
    }
    setStyleClipCorner(value, selector){
        return Module._lv_obj_set_style_clip_corner(this.ptr, value, selector)
    }
    setStyleOpa(value, selector){
        return Module._lv_obj_set_style_opa(this.ptr, value, selector)
    }
    setStyleColorFilterDsc(value, selector){
        return Module._lv_obj_set_style_color_filter_dsc(this.ptr, value, selector)
    }
    setStyleColorFilterOpa(value, selector){
        return Module._lv_obj_set_style_color_filter_opa(this.ptr, value, selector)
    }
    setStyleAnimTime(value, selector){
        return Module._lv_obj_set_style_anim_time(this.ptr, value, selector)
    }
    setStyleAnimSpeed(value, selector){
        return Module._lv_obj_set_style_anim_speed(this.ptr, value, selector)
    }
    setStyleTransition(value, selector){
        return Module._lv_obj_set_style_transition(this.ptr, value, selector)
    }
    setStyleBlendMode(value, selector){
        return Module._lv_obj_set_style_blend_mode(this.ptr, value, selector)
    }
    setStyleLayout(value, selector){
        return Module._lv_obj_set_style_layout(this.ptr, value, selector)
    }
    setStyleBaseDir(value, selector){
        return Module._lv_obj_set_style_base_dir(this.ptr, value, selector)
    }
    setStyleBgColor(value, selector){
        return Module._lv_obj_set_style_bg_color(this.ptr, value, selector)
    }
    setStyleBgColorFiltered(value, selector){
        return Module._lv_obj_set_style_bg_color_filtered(this.ptr, value, selector)
    }
    setStyleBgOpa(value, selector){
        return Module._lv_obj_set_style_bg_opa(this.ptr, value, selector)
    }
    setStyleBgGradColor(value, selector){
        return Module._lv_obj_set_style_bg_grad_color(this.ptr, value, selector)
    }
    setStyleBgGradColorFiltered(value, selector){
        return Module._lv_obj_set_style_bg_grad_color_filtered(this.ptr, value, selector)
    }
    setStyleBgGradDir(value, selector){
        return Module._lv_obj_set_style_bg_grad_dir(this.ptr, value, selector)
    }
    setStyleBgMainStop(value, selector){
        return Module._lv_obj_set_style_bg_main_stop(this.ptr, value, selector)
    }
    setStyleBgGradStop(value, selector){
        return Module._lv_obj_set_style_bg_grad_stop(this.ptr, value, selector)
    }
    setStyleBgImgSrc(value, selector){
        return Module._lv_obj_set_style_bg_img_src(this.ptr, value, selector)
    }
    setStyleBgImgOpa(value, selector){
        return Module._lv_obj_set_style_bg_img_opa(this.ptr, value, selector)
    }
    setStyleBgImgRecolor(value, selector){
        return Module._lv_obj_set_style_bg_img_recolor(this.ptr, value, selector)
    }
    setStyleBgImgRecolorFiltered(value, selector){
        return Module._lv_obj_set_style_bg_img_recolor_filtered(this.ptr, value, selector)
    }
    setStyleBgImgRecolorOpa(value, selector){
        return Module._lv_obj_set_style_bg_img_recolor_opa(this.ptr, value, selector)
    }
    setStyleBgImgTiled(value, selector){
        return Module._lv_obj_set_style_bg_img_tiled(this.ptr, value, selector)
    }
    setStyleBorderColor(value, selector){
        return Module._lv_obj_set_style_border_color(this.ptr, value, selector)
    }
    setStyleBorderColorFiltered(value, selector){
        return Module._lv_obj_set_style_border_color_filtered(this.ptr, value, selector)
    }
    setStyleBorderOpa(value, selector){
        return Module._lv_obj_set_style_border_opa(this.ptr, value, selector)
    }
    setStyleBorderWidth(value, selector){
        return Module._lv_obj_set_style_border_width(this.ptr, value, selector)
    }
    setStyleBorderSide(value, selector){
        return Module._lv_obj_set_style_border_side(this.ptr, value, selector)
    }
    setStyleBorderPost(value, selector){
        return Module._lv_obj_set_style_border_post(this.ptr, value, selector)
    }
    setStyleTextColor(value, selector){
        return Module._lv_obj_set_style_text_color(this.ptr, value, selector)
    }
    setStyleTextColorFiltered(value, selector){
        return Module._lv_obj_set_style_text_color_filtered(this.ptr, value, selector)
    }
    setStyleTextOpa(value, selector){
        return Module._lv_obj_set_style_text_opa(this.ptr, value, selector)
    }
    setStyleTextFont(value, selector){
        return Module._lv_obj_set_style_text_font(this.ptr, value, selector)
    }
    setStyleTextLetterSpace(value, selector){
        return Module._lv_obj_set_style_text_letter_space(this.ptr, value, selector)
    }
    setStyleTextLineSpace(value, selector){
        return Module._lv_obj_set_style_text_line_space(this.ptr, value, selector)
    }
    setStyleTextDecor(value, selector){
        return Module._lv_obj_set_style_text_decor(this.ptr, value, selector)
    }
    setStyleTextAlign(value, selector){
        return Module._lv_obj_set_style_text_align(this.ptr, value, selector)
    }
    setStyleImgOpa(value, selector){
        return Module._lv_obj_set_style_img_opa(this.ptr, value, selector)
    }
    setStyleImgRecolor(value, selector){
        return Module._lv_obj_set_style_img_recolor(this.ptr, value, selector)
    }
    setStyleImgRecolorFiltered(value, selector){
        return Module._lv_obj_set_style_img_recolor_filtered(this.ptr, value, selector)
    }
    setStyleImgRecolorOpa(value, selector){
        return Module._lv_obj_set_style_img_recolor_opa(this.ptr, value, selector)
    }
    setStyleOutlineWidth(value, selector){
        return Module._lv_obj_set_style_outline_width(this.ptr, value, selector)
    }
    setStyleOutlineColor(value, selector){
        return Module._lv_obj_set_style_outline_color(this.ptr, value, selector)
    }
    setStyleOutlineColorFiltered(value, selector){
        return Module._lv_obj_set_style_outline_color_filtered(this.ptr, value, selector)
    }
    setStyleOutlineOpa(value, selector){
        return Module._lv_obj_set_style_outline_opa(this.ptr, value, selector)
    }
    setStyleOutlinePad(value, selector){
        return Module._lv_obj_set_style_outline_pad(this.ptr, value, selector)
    }
    setStyleShadowWidth(value, selector){
        return Module._lv_obj_set_style_shadow_width(this.ptr, value, selector)
    }
    setStyleShadowOfsX(value, selector){
        return Module._lv_obj_set_style_shadow_ofs_x(this.ptr, value, selector)
    }
    setStyleShadowOfsY(value, selector){
        return Module._lv_obj_set_style_shadow_ofs_y(this.ptr, value, selector)
    }
    setStyleShadowSpread(value, selector){
        return Module._lv_obj_set_style_shadow_spread(this.ptr, value, selector)
    }
    setStyleShadowColor(value, selector){
        return Module._lv_obj_set_style_shadow_color(this.ptr, value, selector)
    }
    setStyleShadowColorFiltered(value, selector){
        return Module._lv_obj_set_style_shadow_color_filtered(this.ptr, value, selector)
    }
    setStyleShadowOpa(value, selector){
        return Module._lv_obj_set_style_shadow_opa(this.ptr, value, selector)
    }
    setStyleLineWidth(value, selector){
        return Module._lv_obj_set_style_line_width(this.ptr, value, selector)
    }
    setStyleLineDashWidth(value, selector){
        return Module._lv_obj_set_style_line_dash_width(this.ptr, value, selector)
    }
    setStyleLineDashGap(value, selector){
        return Module._lv_obj_set_style_line_dash_gap(this.ptr, value, selector)
    }
    setStyleLineRounded(value, selector){
        return Module._lv_obj_set_style_line_rounded(this.ptr, value, selector)
    }
    setStyleLineColor(value, selector){
        return Module._lv_obj_set_style_line_color(this.ptr, value, selector)
    }
    setStyleLineColorFiltered(value, selector){
        return Module._lv_obj_set_style_line_color_filtered(this.ptr, value, selector)
    }
    setStyleLineOpa(value, selector){
        return Module._lv_obj_set_style_line_opa(this.ptr, value, selector)
    }
    setStyleArcWidth(value, selector){
        return Module._lv_obj_set_style_arc_width(this.ptr, value, selector)
    }
    setStyleArcRounded(value, selector){
        return Module._lv_obj_set_style_arc_rounded(this.ptr, value, selector)
    }
    setStyleArcColor(value, selector){
        return Module._lv_obj_set_style_arc_color(this.ptr, value, selector)
    }
    setStyleArcColorFiltered(value, selector){
        return Module._lv_obj_set_style_arc_color_filtered(this.ptr, value, selector)
    }
    setStyleArcOpa(value, selector){
        return Module._lv_obj_set_style_arc_opa(this.ptr, value, selector)
    }
    setStyleArcImgSrc(value, selector){
        return Module._lv_obj_set_style_arc_img_src(this.ptr, value, selector)
    }
    initDrawRectDsc(part, draw_dsc){
        return Module._lv_obj_init_draw_rect_dsc(this.ptr, part, draw_dsc)
    }
    initDrawLabelDsc(part, draw_dsc){
        return Module._lv_obj_init_draw_label_dsc(this.ptr, part, draw_dsc)
    }
    initDrawImgDsc(part, draw_dsc){
        return Module._lv_obj_init_draw_img_dsc(this.ptr, part, draw_dsc)
    }
    initDrawLineDsc(part, draw_dsc){
        return Module._lv_obj_init_draw_line_dsc(this.ptr, part, draw_dsc)
    }
    initDrawArcDsc(part, draw_dsc){
        return Module._lv_obj_init_draw_arc_dsc(this.ptr, part, draw_dsc)
    }
    calculateExtDrawSize(part){
        return Module._lv_obj_calculate_ext_draw_size(this.ptr, part)
    }
    refreshExtDrawSize(){
        return Module._lv_obj_refresh_ext_draw_size(this.ptr)
    }
    classInitObj(){
        return Module._lv_obj_class_init_obj(this.ptr)
    }
    isEditable(){
        return Module._lv_obj_is_editable(this.ptr)
    }
    isGroupDef(){
        return Module._lv_obj_is_group_def(this.ptr)
    }
    addEventCb(event_cb, filter, user_data){
        return Module._lv_obj_add_event_cb(this.ptr, event_cb, filter, user_data)
    }
    removeEventCb(event_cb){
        return Module._lv_obj_remove_event_cb(this.ptr, event_cb)
    }
    removeEventCbWithUserData(event_cb, event_user_data){
        return Module._lv_obj_remove_event_cb_with_user_data(this.ptr, event_cb, event_user_data)
    }
    removeEventDsc(event_dsc){
        return Module._lv_obj_remove_event_dsc(this.ptr, event_dsc)
    }
    create(){
        return Module._lv_obj_create(this.ptr)
    }
    addFlag(f){
        return Module._lv_obj_add_flag(this.ptr, f)
    }
    clearFlag(f){
        return Module._lv_obj_clear_flag(this.ptr, f)
    }
    addState(state){
        return Module._lv_obj_add_state(this.ptr, state)
    }
    clearState(state){
        return Module._lv_obj_clear_state(this.ptr, state)
    }
    allocateSpecAttr(){
        return Module._lv_obj_allocate_spec_attr(this.ptr)
    }
    setFlexFlow(flow){
        return Module._lv_obj_set_flex_flow(this.ptr, flow)
    }
    setFlexAlign(main_place, cross_place, track_cross_place){
        return Module._lv_obj_set_flex_align(this.ptr, main_place, cross_place, track_cross_place)
    }
    setFlexGrow(grow){
        return Module._lv_obj_set_flex_grow(this.ptr, grow)
    }
    setStyleFlexFlow(value, selector){
        return Module._lv_obj_set_style_flex_flow(this.ptr, value, selector)
    }
    setStyleFlexMainPlace(value, selector){
        return Module._lv_obj_set_style_flex_main_place(this.ptr, value, selector)
    }
    setStyleFlexCrossPlace(value, selector){
        return Module._lv_obj_set_style_flex_cross_place(this.ptr, value, selector)
    }
    setStyleFlexTrackPlace(value, selector){
        return Module._lv_obj_set_style_flex_track_place(this.ptr, value, selector)
    }
    setStyleFlexGrow(value, selector){
        return Module._lv_obj_set_style_flex_grow(this.ptr, value, selector)
    }
    setTile(tile_obj, anim_en){
        return Module._lv_obj_set_tile(this.ptr, tile_obj, anim_en)
    }
    setTileId(col_id, row_id, anim_en){
        return Module._lv_obj_set_tile_id(this.ptr, col_id, row_id, anim_en)
    }
    setGridDscArray(col_dsc, row_dsc){
        return Module._lv_obj_set_grid_dsc_array(this.ptr, col_dsc, row_dsc)
    }
    setGridAlign(column_align, row_align){
        return Module._lv_obj_set_grid_align(this.ptr, column_align, row_align)
    }
    setGridCell(column_align, col_pos, col_span, row_align, row_pos, row_span){
        return Module._lv_obj_set_grid_cell(this.ptr, column_align, col_pos, col_span, row_align, row_pos, row_span)
    }
    setStyleGridRowDscArray(value, selector){
        return Module._lv_obj_set_style_grid_row_dsc_array(this.ptr, value, selector)
    }
    setStyleGridColumnDscArray(value, selector){
        return Module._lv_obj_set_style_grid_column_dsc_array(this.ptr, value, selector)
    }
    setStyleGridRowAlign(value, selector){
        return Module._lv_obj_set_style_grid_row_align(this.ptr, value, selector)
    }
    setStyleGridColumnAlign(value, selector){
        return Module._lv_obj_set_style_grid_column_align(this.ptr, value, selector)
    }
    setStyleGridCellColumnPos(value, selector){
        return Module._lv_obj_set_style_grid_cell_column_pos(this.ptr, value, selector)
    }
    setStyleGridCellColumnSpan(value, selector){
        return Module._lv_obj_set_style_grid_cell_column_span(this.ptr, value, selector)
    }
    setStyleGridCellRowPos(value, selector){
        return Module._lv_obj_set_style_grid_cell_row_pos(this.ptr, value, selector)
    }
    setStyleGridCellRowSpan(value, selector){
        return Module._lv_obj_set_style_grid_cell_row_span(this.ptr, value, selector)
    }
    setStyleGridCellXAlign(value, selector){
        return Module._lv_obj_set_style_grid_cell_x_align(this.ptr, value, selector)
    }
    setStyleGridCellYAlign(value, selector){
        return Module._lv_obj_set_style_grid_cell_y_align(this.ptr, value, selector)
    }
    classP(){
        return Module._lv_obj_get_class_p(this.ptr)
    }
    setClassP(class_p){
        return Module._lv_obj_set_class_p(this.ptr, class_p)
    }
    specAttr(){
        return Module._lv_obj_get_spec_attr(this.ptr)
    }
    setSpecAttr(spec_attr){
        return Module._lv_obj_set_spec_attr(this.ptr, spec_attr)
    }
    styles(){
        return Module._lv_obj_get_styles(this.ptr)
    }
    setStyles(styles){
        return Module._lv_obj_set_styles(this.ptr, styles)
    }
    setCoords(coords){
        return Module._lv_obj_set_coords(this.ptr, coords)
    }
    flags(){
        return Module._lv_obj_get_flags(this.ptr)
    }
    setFlags(flags){
        return Module._lv_obj_set_flags(this.ptr, flags)
    }
    setState(state){
        return Module._lv_obj_set_state(this.ptr, state)
    }
    layoutInv(){
        return Module._lv_obj_get_layout_inv(this.ptr)
    }
    setLayoutInv(layout_inv){
        return Module._lv_obj_set_layout_inv(this.ptr, layout_inv)
    }
    scrLayoutInv(){
        return Module._lv_obj_get_scr_layout_inv(this.ptr)
    }
    setScrLayoutInv(scr_layout_inv){
        return Module._lv_obj_set_scr_layout_inv(this.ptr, scr_layout_inv)
    }
    skipTrans(){
        return Module._lv_obj_get_skip_trans(this.ptr)
    }
    setSkipTrans(skip_trans){
        return Module._lv_obj_set_skip_trans(this.ptr, skip_trans)
    }
    styleCnt(){
        return Module._lv_obj_get_style_cnt(this.ptr)
    }
    setStyleCnt(style_cnt){
        return Module._lv_obj_set_style_cnt(this.ptr, style_cnt)
    }
    hLayout(){
        return Module._lv_obj_get_h_layout(this.ptr)
    }
    setHLayout(h_layout){
        return Module._lv_obj_set_h_layout(this.ptr, h_layout)
    }
    wLayout(){
        return Module._lv_obj_get_w_layout(this.ptr)
    }
    setWLayout(w_layout){
        return Module._lv_obj_set_w_layout(this.ptr, w_layout)
    }
// AUTO GENERATE CODE END [Obj METHODS] --------
}
class Label extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_label_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [Label METHODS] --------
    create(){
        return Module._lv_label_create(this.ptr)
    }
    setText(text){
        return Module._lv_label_set_text(this.ptr, text)
    }
    setTextFmt(fmt){
        return Module._lv_label_set_text_fmt(this.ptr, fmt)
    }
    setTextStatic(text){
        return Module._lv_label_set_text_static(this.ptr, text)
    }
    setLongMode(long_mode){
        return Module._lv_label_set_long_mode(this.ptr, long_mode)
    }
    setRecolor(en){
        return Module._lv_label_set_recolor(this.ptr, en)
    }
    setTextSelStart(index){
        return Module._lv_label_set_text_sel_start(this.ptr, index)
    }
    setTextSelEnd(index){
        return Module._lv_label_set_text_sel_end(this.ptr, index)
    }
    insText(pos, txt){
        return Module._lv_label_ins_text(this.ptr, pos, txt)
    }
    cutText(pos, cnt){
        return Module._lv_label_cut_text(this.ptr, pos, cnt)
    }
// AUTO GENERATE CODE END [Label METHODS] --------
}
class Arc extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_arc_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [Arc METHODS] --------
    create(){
        return Module._lv_arc_create(this.ptr)
    }
    setStartAngle(start){
        return Module._lv_arc_set_start_angle(this.ptr, start)
    }
    setEndAngle(end){
        return Module._lv_arc_set_end_angle(this.ptr, end)
    }
    setAngles(start, end){
        return Module._lv_arc_set_angles(this.ptr, start, end)
    }
    setBgStartAngle(start){
        return Module._lv_arc_set_bg_start_angle(this.ptr, start)
    }
    setBgEndAngle(end){
        return Module._lv_arc_set_bg_end_angle(this.ptr, end)
    }
    setBgAngles(start, end){
        return Module._lv_arc_set_bg_angles(this.ptr, start, end)
    }
    setRotation(rotation){
        return Module._lv_arc_set_rotation(this.ptr, rotation)
    }
    setMode(type){
        return Module._lv_arc_set_mode(this.ptr, type)
    }
    setValue(value){
        return Module._lv_arc_set_value(this.ptr, value)
    }
    setRange(min, max){
        return Module._lv_arc_set_range(this.ptr, min, max)
    }
    setChangeRate(rate){
        return Module._lv_arc_set_change_rate(this.ptr, rate)
    }
    angleStart(){
        return Module._lv_arc_get_angle_start(this.ptr)
    }
    angleEnd(){
        return Module._lv_arc_get_angle_end(this.ptr)
    }
    bgAngleStart(){
        return Module._lv_arc_get_bg_angle_start(this.ptr)
    }
    bgAngleEnd(){
        return Module._lv_arc_get_bg_angle_end(this.ptr)
    }
// AUTO GENERATE CODE END [Arc METHODS] --------
}
class Bar extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_bar_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [Bar METHODS] --------
    create(){
        return Module._lv_bar_create(this.ptr)
    }
    setValue(value, anim){
        return Module._lv_bar_set_value(this.ptr, value, anim)
    }
    setStartValue(start_value, anim){
        return Module._lv_bar_set_start_value(this.ptr, start_value, anim)
    }
    setRange(min, max){
        return Module._lv_bar_set_range(this.ptr, min, max)
    }
    setMode(mode){
        return Module._lv_bar_set_mode(this.ptr, mode)
    }
    mode(){
        return Module._lv_bar_get_mode(this.ptr)
    }
    obj(){
        return Module._lv_bar_get_obj(this.ptr)
    }
    setObj(obj){
        return Module._lv_bar_set_obj(this.ptr, obj)
    }
    curValue(){
        return Module._lv_bar_get_cur_value(this.ptr)
    }
    setCurValue(cur_value){
        return Module._lv_bar_set_cur_value(this.ptr, cur_value)
    }
    setMinValue(min_value){
        return Module._lv_bar_set_min_value(this.ptr, min_value)
    }
    setMaxValue(max_value){
        return Module._lv_bar_set_max_value(this.ptr, max_value)
    }
    indicArea(){
        return Module._lv_bar_get_indic_area(this.ptr)
    }
    setIndicArea(indic_area){
        return Module._lv_bar_set_indic_area(this.ptr, indic_area)
    }
    curValueAnim(){
        return Module._lv_bar_get_cur_value_anim(this.ptr)
    }
    setCurValueAnim(cur_value_anim){
        return Module._lv_bar_set_cur_value_anim(this.ptr, cur_value_anim)
    }
    startValueAnim(){
        return Module._lv_bar_get_start_value_anim(this.ptr)
    }
    setStartValueAnim(start_value_anim){
        return Module._lv_bar_set_start_value_anim(this.ptr, start_value_anim)
    }
// AUTO GENERATE CODE END [Bar METHODS] --------
}
class Btn extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_btn_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [Btn METHODS] --------
    create(){
        return Module._lv_btn_create(this.ptr)
    }
// AUTO GENERATE CODE END [Btn METHODS] --------
}
class BtnMatrix extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_btnmatrix_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [BtnMatrix METHODS] --------
    create(){
        return Module._lv_btnmatrix_create(this.ptr)
    }
    setMap(map){
        return Module._lv_btnmatrix_set_map(this.ptr, map)
    }
    setCtrlMap(ctrl_map){
        return Module._lv_btnmatrix_set_ctrl_map(this.ptr, ctrl_map)
    }
    setSelectedBtn(btn_id){
        return Module._lv_btnmatrix_set_selected_btn(this.ptr, btn_id)
    }
    setBtnCtrl(btn_id, ctrl){
        return Module._lv_btnmatrix_set_btn_ctrl(this.ptr, btn_id, ctrl)
    }
    clearBtnCtrl(btn_id, ctrl){
        return Module._lv_btnmatrix_clear_btn_ctrl(this.ptr, btn_id, ctrl)
    }
    setBtnCtrlAll(ctrl){
        return Module._lv_btnmatrix_set_btn_ctrl_all(this.ptr, ctrl)
    }
    clearBtnCtrlAll(ctrl){
        return Module._lv_btnmatrix_clear_btn_ctrl_all(this.ptr, ctrl)
    }
    setBtnWidth(btn_id, width){
        return Module._lv_btnmatrix_set_btn_width(this.ptr, btn_id, width)
    }
    setOneChecked(en){
        return Module._lv_btnmatrix_set_one_checked(this.ptr, en)
    }
    hasBtnCtrl(btn_id, ctrl){
        return Module._lv_btnmatrix_has_btn_ctrl(this.ptr, btn_id, ctrl)
    }
    obj(){
        return Module._lv_btnmatrix_get_obj(this.ptr)
    }
    setObj(obj){
        return Module._lv_btnmatrix_set_obj(this.ptr, obj)
    }
    mapP(){
        return Module._lv_btnmatrix_get_map_p(this.ptr)
    }
    setMapP(map_p){
        return Module._lv_btnmatrix_set_map_p(this.ptr, map_p)
    }
    buttonAreas(){
        return Module._lv_btnmatrix_get_button_areas(this.ptr)
    }
    setButtonAreas(button_areas){
        return Module._lv_btnmatrix_set_button_areas(this.ptr, button_areas)
    }
    ctrlBits(){
        return Module._lv_btnmatrix_get_ctrl_bits(this.ptr)
    }
    setCtrlBits(ctrl_bits){
        return Module._lv_btnmatrix_set_ctrl_bits(this.ptr, ctrl_bits)
    }
    btnCnt(){
        return Module._lv_btnmatrix_get_btn_cnt(this.ptr)
    }
    setBtnCnt(btn_cnt){
        return Module._lv_btnmatrix_set_btn_cnt(this.ptr, btn_cnt)
    }
    rowCnt(){
        return Module._lv_btnmatrix_get_row_cnt(this.ptr)
    }
    setRowCnt(row_cnt){
        return Module._lv_btnmatrix_set_row_cnt(this.ptr, row_cnt)
    }
    btnIdSel(){
        return Module._lv_btnmatrix_get_btn_id_sel(this.ptr)
    }
    setBtnIdSel(btn_id_sel){
        return Module._lv_btnmatrix_set_btn_id_sel(this.ptr, btn_id_sel)
    }
    oneCheck(){
        return Module._lv_btnmatrix_get_one_check(this.ptr)
    }
    setOneCheck(one_check){
        return Module._lv_btnmatrix_set_one_check(this.ptr, one_check)
    }
// AUTO GENERATE CODE END [BtnMatrix METHODS] --------
}
class Canvas extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_canvas_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [Canvas METHODS] --------
    create(){
        return Module._lv_canvas_create(this.ptr)
    }
    setBuffer(buf, w, h, cf){
        return Module._lv_canvas_set_buffer(this.ptr, buf, w, h, cf)
    }
    setPxColor(x, y, c){
        return Module._lv_canvas_set_px_color(this.ptr, x, y, c)
    }
    setPxOpa(x, y, opa){
        return Module._lv_canvas_set_px_opa(this.ptr, x, y, opa)
    }
    setPalette(id, c){
        return Module._lv_canvas_set_palette(this.ptr, id, c)
    }
    px(x, y){
        return Module._lv_canvas_get_px(this.ptr, x, y)
    }
    img(){
        return Module._lv_canvas_get_img(this.ptr)
    }
    copyBuf(to_copy, x, y, w, h){
        return Module._lv_canvas_copy_buf(this.ptr, to_copy, x, y, w, h)
    }
    transform(img, angle, zoom, offset_x, offset_y, pivot_x, pivot_y, antialias){
        return Module._lv_canvas_transform(this.ptr, img, angle, zoom, offset_x, offset_y, pivot_x, pivot_y, antialias)
    }
    blurHor(area, r){
        return Module._lv_canvas_blur_hor(this.ptr, area, r)
    }
    blurVer(area, r){
        return Module._lv_canvas_blur_ver(this.ptr, area, r)
    }
    fillBg(color, opa){
        return Module._lv_canvas_fill_bg(this.ptr, color, opa)
    }
    drawRect(x, y, w, h, draw_dsc){
        return Module._lv_canvas_draw_rect(this.ptr, x, y, w, h, draw_dsc)
    }
    drawText(x, y, max_w, draw_dsc, txt){
        return Module._lv_canvas_draw_text(this.ptr, x, y, max_w, draw_dsc, txt)
    }
    drawImg(x, y, src, draw_dsc){
        return Module._lv_canvas_draw_img(this.ptr, x, y, src, draw_dsc)
    }
    drawLine(points, point_cnt, draw_dsc){
        return Module._lv_canvas_draw_line(this.ptr, points, point_cnt, draw_dsc)
    }
    drawPolygon(points, point_cnt, draw_dsc){
        return Module._lv_canvas_draw_polygon(this.ptr, points, point_cnt, draw_dsc)
    }
    drawArc(x, y, r, start_angle, end_angle, draw_dsc){
        return Module._lv_canvas_draw_arc(this.ptr, x, y, r, start_angle, end_angle, draw_dsc)
    }
// AUTO GENERATE CODE END [Canvas METHODS] --------
}
class Checkbox extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_checkbox_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [Checkbox METHODS] --------
    create(){
        return Module._lv_checkbox_create(this.ptr)
    }
    setText(txt){
        return Module._lv_checkbox_set_text(this.ptr, txt)
    }
    setTextStatic(txt){
        return Module._lv_checkbox_set_text_static(this.ptr, txt)
    }
// AUTO GENERATE CODE END [Checkbox METHODS] --------
}
class Dropdown extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_dropdown_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [Dropdown METHODS] --------
    create(){
        return Module._lv_dropdown_create(this.ptr)
    }
    setText(txt){
        return Module._lv_dropdown_set_text(this.ptr, txt)
    }
    setOptions(options){
        return Module._lv_dropdown_set_options(this.ptr, options)
    }
    setOptionsStatic(options){
        return Module._lv_dropdown_set_options_static(this.ptr, options)
    }
    addOption(option, pos){
        return Module._lv_dropdown_add_option(this.ptr, option, pos)
    }
    clearOptions(){
        return Module._lv_dropdown_clear_options(this.ptr)
    }
    setSelected(sel_opt){
        return Module._lv_dropdown_set_selected(this.ptr, sel_opt)
    }
    setDir(dir){
        return Module._lv_dropdown_set_dir(this.ptr, dir)
    }
    setSymbol(symbol){
        return Module._lv_dropdown_set_symbol(this.ptr, symbol)
    }
    setSelectedHighlight(en){
        return Module._lv_dropdown_set_selected_highlight(this.ptr, en)
    }
    list(){
        return Module._lv_dropdown_get_list(this.ptr)
    }
    text(){
        return Module._lv_dropdown_get_text(this.ptr)
    }
    symbol(){
        return Module._lv_dropdown_get_symbol(this.ptr)
    }
    selectedHighlight(){
        return Module._lv_dropdown_get_selected_highlight(this.ptr)
    }
    open(){
        return Module._lv_dropdown_open(this.ptr)
    }
    close(){
        return Module._lv_dropdown_close(this.ptr)
    }
// AUTO GENERATE CODE END [Dropdown METHODS] --------
}
class Img extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_img_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [Img METHODS] --------
    create(){
        return Module._lv_img_create(this.ptr)
    }
    setSrc(src){
        return Module._lv_img_set_src(this.ptr, src)
    }
    setOffsetX(x){
        return Module._lv_img_set_offset_x(this.ptr, x)
    }
    setOffsetY(y){
        return Module._lv_img_set_offset_y(this.ptr, y)
    }
    setAngle(angle){
        return Module._lv_img_set_angle(this.ptr, angle)
    }
    setPivot(x, y){
        return Module._lv_img_set_pivot(this.ptr, x, y)
    }
    setZoom(zoom){
        return Module._lv_img_set_zoom(this.ptr, zoom)
    }
    setAntialias(antialias){
        return Module._lv_img_set_antialias(this.ptr, antialias)
    }
    setSizeMode(mode){
        return Module._lv_img_set_size_mode(this.ptr, mode)
    }
    src(){
        return Module._lv_img_get_src(this.ptr)
    }
    offsetX(){
        return Module._lv_img_get_offset_x(this.ptr)
    }
    offsetY(){
        return Module._lv_img_get_offset_y(this.ptr)
    }
    angle(){
        return Module._lv_img_get_angle(this.ptr)
    }
    pivot(pivot){
        return Module._lv_img_get_pivot(this.ptr, pivot)
    }
    zoom(){
        return Module._lv_img_get_zoom(this.ptr)
    }
    antialias(){
        return Module._lv_img_get_antialias(this.ptr)
    }
    sizeMode(){
        return Module._lv_img_get_size_mode(this.ptr)
    }
    obj(){
        return Module._lv_img_get_obj(this.ptr)
    }
    setObj(obj){
        return Module._lv_img_set_obj(this.ptr, obj)
    }
    offset(){
        return Module._lv_img_get_offset(this.ptr)
    }
    setOffset(offset){
        return Module._lv_img_set_offset(this.ptr, offset)
    }
    w(){
        return Module._lv_img_get_w(this.ptr)
    }
    setW(w){
        return Module._lv_img_set_w(this.ptr, w)
    }
    h(){
        return Module._lv_img_get_h(this.ptr)
    }
    setH(h){
        return Module._lv_img_set_h(this.ptr, h)
    }
    srcType(){
        return Module._lv_img_get_src_type(this.ptr)
    }
    setSrcType(src_type){
        return Module._lv_img_set_src_type(this.ptr, src_type)
    }
    cf(){
        return Module._lv_img_get_cf(this.ptr)
    }
    setCf(cf){
        return Module._lv_img_set_cf(this.ptr, cf)
    }
    objSizeMode(){
        return Module._lv_img_get_obj_size_mode(this.ptr)
    }
    setObjSizeMode(obj_size_mode){
        return Module._lv_img_set_obj_size_mode(this.ptr, obj_size_mode)
    }
// AUTO GENERATE CODE END [Img METHODS] --------
}
class Line extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_line_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [Line METHODS] --------
    create(){
        return Module._lv_line_create(this.ptr)
    }
    setPoints(points, point_num){
        return Module._lv_line_set_points(this.ptr, points, point_num)
    }
    setYInvert(en){
        return Module._lv_line_set_y_invert(this.ptr, en)
    }
// AUTO GENERATE CODE END [Line METHODS] --------
}
class Roller extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_roller_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [Roller METHODS] --------
    create(){
        return Module._lv_roller_create(this.ptr)
    }
    setOptions(options, mode){
        return Module._lv_roller_set_options(this.ptr, options, mode)
    }
    setSelected(sel_opt, anim){
        return Module._lv_roller_set_selected(this.ptr, sel_opt, anim)
    }
    setVisibleRowCount(row_cnt){
        return Module._lv_roller_set_visible_row_count(this.ptr, row_cnt)
    }
// AUTO GENERATE CODE END [Roller METHODS] --------
}
class Slider extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_slider_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [Slider METHODS] --------
    create(){
        return Module._lv_slider_create(this.ptr)
    }
// AUTO GENERATE CODE END [Slider METHODS] --------
}
class Switch extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_switch_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [Switch METHODS] --------
    create(){
        return Module._lv_switch_create(this.ptr)
    }
// AUTO GENERATE CODE END [Switch METHODS] --------
}
class Table extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_table_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [Table METHODS] --------
    create(){
        return Module._lv_table_create(this.ptr)
    }
    setCellValue(row, col, txt){
        return Module._lv_table_set_cell_value(this.ptr, row, col, txt)
    }
    setCellValueFmt(row, col, fmt){
        return Module._lv_table_set_cell_value_fmt(this.ptr, row, col, fmt)
    }
    setRowCnt(row_cnt){
        return Module._lv_table_set_row_cnt(this.ptr, row_cnt)
    }
    setColCnt(col_cnt){
        return Module._lv_table_set_col_cnt(this.ptr, col_cnt)
    }
    setColWidth(col_id, w){
        return Module._lv_table_set_col_width(this.ptr, col_id, w)
    }
    addCellCtrl(row, col, ctrl){
        return Module._lv_table_add_cell_ctrl(this.ptr, row, col, ctrl)
    }
    clearCellCtrl(row, col, ctrl){
        return Module._lv_table_clear_cell_ctrl(this.ptr, row, col, ctrl)
    }
    cellValue(row, col){
        return Module._lv_table_get_cell_value(this.ptr, row, col)
    }
    rowCnt(){
        return Module._lv_table_get_row_cnt(this.ptr)
    }
    colCnt(){
        return Module._lv_table_get_col_cnt(this.ptr)
    }
    colWidth(col){
        return Module._lv_table_get_col_width(this.ptr, col)
    }
    hasCellCtrl(row, col, ctrl){
        return Module._lv_table_has_cell_ctrl(this.ptr, row, col, ctrl)
    }
    selectedCell(row, col){
        return Module._lv_table_get_selected_cell(this.ptr, row, col)
    }
// AUTO GENERATE CODE END [Table METHODS] --------
}
class TextArea extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_textarea_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [TextArea METHODS] --------
    create(){
        return Module._lv_textarea_create(this.ptr)
    }
    addChar(c){
        return Module._lv_textarea_add_char(this.ptr, c)
    }
    addText(txt){
        return Module._lv_textarea_add_text(this.ptr, txt)
    }
    delChar(){
        return Module._lv_textarea_del_char(this.ptr)
    }
    delCharForward(){
        return Module._lv_textarea_del_char_forward(this.ptr)
    }
    setText(txt){
        return Module._lv_textarea_set_text(this.ptr, txt)
    }
    setPlaceholderText(txt){
        return Module._lv_textarea_set_placeholder_text(this.ptr, txt)
    }
    setCursorPos(pos){
        return Module._lv_textarea_set_cursor_pos(this.ptr, pos)
    }
    setCursorClickPos(en){
        return Module._lv_textarea_set_cursor_click_pos(this.ptr, en)
    }
    setPasswordMode(en){
        return Module._lv_textarea_set_password_mode(this.ptr, en)
    }
    setOneLine(en){
        return Module._lv_textarea_set_one_line(this.ptr, en)
    }
    setAcceptedChars(list){
        return Module._lv_textarea_set_accepted_chars(this.ptr, list)
    }
    setMaxLength(num){
        return Module._lv_textarea_set_max_length(this.ptr, num)
    }
    setInsertReplace(txt){
        return Module._lv_textarea_set_insert_replace(this.ptr, txt)
    }
    setTextSelection(en){
        return Module._lv_textarea_set_text_selection(this.ptr, en)
    }
    setPasswordShowTime(time){
        return Module._lv_textarea_set_password_show_time(this.ptr, time)
    }
    setAlign(align){
        return Module._lv_textarea_set_align(this.ptr, align)
    }
    placeholderText(){
        return Module._lv_textarea_get_placeholder_text(this.ptr)
    }
    cursorClickPos(){
        return Module._lv_textarea_get_cursor_click_pos(this.ptr)
    }
    acceptedChars(){
        return Module._lv_textarea_get_accepted_chars(this.ptr)
    }
    maxLength(){
        return Module._lv_textarea_get_max_length(this.ptr)
    }
    textSelection(){
        return Module._lv_textarea_get_text_selection(this.ptr)
    }
    passwordShowTime(){
        return Module._lv_textarea_get_password_show_time(this.ptr)
    }
    clearSelection(){
        return Module._lv_textarea_clear_selection(this.ptr)
    }
    cursorRight(){
        return Module._lv_textarea_cursor_right(this.ptr)
    }
    cursorLeft(){
        return Module._lv_textarea_cursor_left(this.ptr)
    }
    cursorDown(){
        return Module._lv_textarea_cursor_down(this.ptr)
    }
    cursorUp(){
        return Module._lv_textarea_cursor_up(this.ptr)
    }
    obj(){
        return Module._lv_textarea_get_obj(this.ptr)
    }
    setObj(obj){
        return Module._lv_textarea_set_obj(this.ptr, obj)
    }
    setLabel(label){
        return Module._lv_textarea_set_label(this.ptr, label)
    }
    placeholderTxt(){
        return Module._lv_textarea_get_placeholder_txt(this.ptr)
    }
    setPlaceholderTxt(placeholder_txt){
        return Module._lv_textarea_set_placeholder_txt(this.ptr, placeholder_txt)
    }
    pwdTmp(){
        return Module._lv_textarea_get_pwd_tmp(this.ptr)
    }
    setPwdTmp(pwd_tmp){
        return Module._lv_textarea_set_pwd_tmp(this.ptr, pwd_tmp)
    }
    pwdShowTime(){
        return Module._lv_textarea_get_pwd_show_time(this.ptr)
    }
    setPwdShowTime(pwd_show_time){
        return Module._lv_textarea_set_pwd_show_time(this.ptr, pwd_show_time)
    }
    selStart(){
        return Module._lv_textarea_get_sel_start(this.ptr)
    }
    setSelStart(sel_start){
        return Module._lv_textarea_set_sel_start(this.ptr, sel_start)
    }
    selEnd(){
        return Module._lv_textarea_get_sel_end(this.ptr)
    }
    setSelEnd(sel_end){
        return Module._lv_textarea_set_sel_end(this.ptr, sel_end)
    }
    textSelInProg(){
        return Module._lv_textarea_get_text_sel_in_prog(this.ptr)
    }
    setTextSelInProg(text_sel_in_prog){
        return Module._lv_textarea_set_text_sel_in_prog(this.ptr, text_sel_in_prog)
    }
    textSelEn(){
        return Module._lv_textarea_get_text_sel_en(this.ptr)
    }
    setTextSelEn(text_sel_en){
        return Module._lv_textarea_set_text_sel_en(this.ptr, text_sel_en)
    }
    pwdMode(){
        return Module._lv_textarea_get_pwd_mode(this.ptr)
    }
    setPwdMode(pwd_mode){
        return Module._lv_textarea_set_pwd_mode(this.ptr, pwd_mode)
    }
// AUTO GENERATE CODE END [TextArea METHODS] --------
}
class MsgBox extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_msgbox_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [MsgBox METHODS] --------
    create(title, txt, btn_txts, add_close_btn){
        return Module._lv_msgbox_create(this.ptr, title, txt, btn_txts, add_close_btn)
    }
    title(){
        return Module._lv_msgbox_get_title(this.ptr)
    }
    closeBtn(){
        return Module._lv_msgbox_get_close_btn(this.ptr)
    }
    text(){
        return Module._lv_msgbox_get_text(this.ptr)
    }
    content(){
        return Module._lv_msgbox_get_content(this.ptr)
    }
    btns(){
        return Module._lv_msgbox_get_btns(this.ptr)
    }
    activeBtn(){
        return Module._lv_msgbox_get_active_btn(this.ptr)
    }
    activeBtnText(){
        return Module._lv_msgbox_get_active_btn_text(this.ptr)
    }
    close(){
        return Module._lv_msgbox_close(this.ptr)
    }
    closeAsync(){
        return Module._lv_msgbox_close_async(this.ptr)
    }
// AUTO GENERATE CODE END [MsgBox METHODS] --------
}
class Keyboard extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_keyboard_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [Keyboard METHODS] --------
    create(){
        return Module._lv_keyboard_create(this.ptr)
    }
    setTextarea(ta){
        return Module._lv_keyboard_set_textarea(this.ptr, ta)
    }
    setMode(mode){
        return Module._lv_keyboard_set_mode(this.ptr, mode)
    }
    setPopovers(en){
        return Module._lv_keyboard_set_popovers(this.ptr, en)
    }
    setMap(mode, map, ctrl_map){
        return Module._lv_keyboard_set_map(this.ptr, mode, map, ctrl_map)
    }
// AUTO GENERATE CODE END [Keyboard METHODS] --------
}
class TileView extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_tileview_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [TileView METHODS] --------
    create(){
        return Module._lv_tileview_create(this.ptr)
    }
    addTile(row_id, col_id, dir){
        return Module._lv_tileview_add_tile(this.ptr, row_id, col_id, dir)
    }
    tileAct(){
        return Module._lv_tileview_get_tile_act(this.ptr)
    }
// AUTO GENERATE CODE END [TileView METHODS] --------
}
class List extends Obj {
    constructor(parent) {
        super()
        this.ptr = Module._lv_list_create(parent?(parent.ptr||0):0)
    }
// AUTO GENERATE CODE START [List METHODS] --------
    create(){
        return Module._lv_list_create(this.ptr)
    }
    addText(txt){
        return Module._lv_list_add_text(this.ptr, txt)
    }
    addBtn(icon, txt){
        return Module._lv_list_add_btn(this.ptr, icon, txt)
    }
    btnText(btn){
        return Module._lv_list_get_btn_text(this.ptr, btn)
    }
// AUTO GENERATE CODE END [List METHODS] --------
}

class Animimg extends EventEmitter {
    constructor(parent) {
        super()
        this.ptr = Module._lv_animimg_create(parent.ptr)
    }
// AUTO GENERATE CODE START [Animimg METHODS] --------
    create(){
        return Module._lv_animimg_create(this.ptr)
    }
    setSrc(dsc, num){
        return Module._lv_animimg_set_src(this.ptr, dsc, num)
    }
    start(){
        return Module._lv_animimg_start(this.ptr)
    }
    setDuration(duration){
        return Module._lv_animimg_set_duration(this.ptr, duration)
    }
    setRepeatCount(count){
        return Module._lv_animimg_set_repeat_count(this.ptr, count)
    }
// AUTO GENERATE CODE END [Animimg METHODS] --------
}

class Calendar extends EventEmitter {
    constructor(parent) {
        super()
        this.ptr = Module._lv_calendar_create(parent.ptr)
    }
// AUTO GENERATE CODE START [Calendar METHODS] --------
    create(){
        return Module._lv_calendar_create(this.ptr)
    }
    setTodayDate(year, month, day){
        return Module._lv_calendar_set_today_date(this.ptr, year, month, day)
    }
    setShowedDate(year, month){
        return Module._lv_calendar_set_showed_date(this.ptr, year, month)
    }
    setHighlightedDates(highlighted, date_num){
        return Module._lv_calendar_set_highlighted_dates(this.ptr, highlighted, date_num)
    }
    setDayNames(day_names){
        return Module._lv_calendar_set_day_names(this.ptr, day_names)
    }
    headerArrowCreate(){
        return Module._lv_calendar_header_arrow_create(this.ptr)
    }
    headerDropdownCreate(){
        return Module._lv_calendar_header_dropdown_create(this.ptr)
    }
// AUTO GENERATE CODE END [Calendar METHODS] --------
}

class Chart extends EventEmitter {
    constructor(parent) {
        super()
        this.ptr = Module._lv_chart_create(parent.ptr)
    }
// AUTO GENERATE CODE START [Chart METHODS] --------
    create(){
        return Module._lv_chart_create(this.ptr)
    }
    setType(type){
        return Module._lv_chart_set_type(this.ptr, type)
    }
    setPointCount(cnt){
        return Module._lv_chart_set_point_count(this.ptr, cnt)
    }
    setRange(axis, min, max){
        return Module._lv_chart_set_range(this.ptr, axis, min, max)
    }
    setUpdateMode(update_mode){
        return Module._lv_chart_set_update_mode(this.ptr, update_mode)
    }
    setDivLineCount(hdiv, vdiv){
        return Module._lv_chart_set_div_line_count(this.ptr, hdiv, vdiv)
    }
    setZoomX(zoom_x){
        return Module._lv_chart_set_zoom_x(this.ptr, zoom_x)
    }
    setZoomY(zoom_y){
        return Module._lv_chart_set_zoom_y(this.ptr, zoom_y)
    }
    setAxisTick(axis, major_len, minor_len, major_cnt, minor_cnt, label_en, draw_size){
        return Module._lv_chart_set_axis_tick(this.ptr, axis, major_len, minor_len, major_cnt, minor_cnt, label_en, draw_size)
    }
    pointPosById(ser, id, p_out){
        return Module._lv_chart_get_point_pos_by_id(this.ptr, ser, id, p_out)
    }
    refresh(){
        return Module._lv_chart_refresh(this.ptr)
    }
    addSeries(color, axis){
        return Module._lv_chart_add_series(this.ptr, color, axis)
    }
    removeSeries(series){
        return Module._lv_chart_remove_series(this.ptr, series)
    }
    hideSeries(series, hide){
        return Module._lv_chart_hide_series(this.ptr, series, hide)
    }
    setSeriesColor(series, color){
        return Module._lv_chart_set_series_color(this.ptr, series, color)
    }
    setXStartPoint(ser, id){
        return Module._lv_chart_set_x_start_point(this.ptr, ser, id)
    }
    addCursor(color, dir){
        return Module._lv_chart_add_cursor(this.ptr, color, dir)
    }
    setCursorPos(cursor, pos){
        return Module._lv_chart_set_cursor_pos(this.ptr, cursor, pos)
    }
    setCursorPoint(cursor, ser, point_id){
        return Module._lv_chart_set_cursor_point(this.ptr, cursor, ser, point_id)
    }
    cursorPoint(cursor){
        return Module._lv_chart_get_cursor_point(this.ptr, cursor)
    }
    setAllValue(ser, value){
        return Module._lv_chart_set_all_value(this.ptr, ser, value)
    }
    setNextValue(ser, value){
        return Module._lv_chart_set_next_value(this.ptr, ser, value)
    }
    setNextValue2(ser, x_value, y_value){
        return Module._lv_chart_set_next_value2(this.ptr, ser, x_value, y_value)
    }
    setValueById(ser, id, value){
        return Module._lv_chart_set_value_by_id(this.ptr, ser, id, value)
    }
    setValueById2(ser, id, x_value, y_value){
        return Module._lv_chart_set_value_by_id2(this.ptr, ser, id, x_value, y_value)
    }
    setExtYArray(ser, array){
        return Module._lv_chart_set_ext_y_array(this.ptr, ser, array)
    }
    setExtXArray(ser, array){
        return Module._lv_chart_set_ext_x_array(this.ptr, ser, array)
    }
// AUTO GENERATE CODE END [Chart METHODS] --------
}

class Colorwheel extends EventEmitter {
    constructor(parent) {
        super()
        this.ptr = Module._lv_colorwheel_create(parent.ptr)
    }
// AUTO GENERATE CODE START [Colorwheel METHODS] --------
    create(knob_recolor){
        return Module._lv_colorwheel_create(this.ptr, knob_recolor)
    }
    setHsv(hsv){
        return Module._lv_colorwheel_set_hsv(this.ptr, hsv)
    }
    setRgb(color){
        return Module._lv_colorwheel_set_rgb(this.ptr, color)
    }
    setMode(mode){
        return Module._lv_colorwheel_set_mode(this.ptr, mode)
    }
    setModeFixed(fixed){
        return Module._lv_colorwheel_set_mode_fixed(this.ptr, fixed)
    }
    hsv(){
        return Module._lv_colorwheel_get_hsv(this.ptr)
    }
    rgb(){
        return Module._lv_colorwheel_get_rgb(this.ptr)
    }
    colorMode(){
        return Module._lv_colorwheel_get_color_mode(this.ptr)
    }
    colorModeFixed(){
        return Module._lv_colorwheel_get_color_mode_fixed(this.ptr)
    }
// AUTO GENERATE CODE END [Colorwheel METHODS] --------
}

class Imgbtn extends EventEmitter {
    constructor(parent) {
        super()
        this.ptr = Module._lv_imgbtn_create(parent.ptr)
    }
// AUTO GENERATE CODE START [Imgbtn METHODS] --------
    create(){
        return Module._lv_imgbtn_create(this.ptr)
    }
    setSrc(state, src_left, src_mid, src_right){
        return Module._lv_imgbtn_set_src(this.ptr, state, src_left, src_mid, src_right)
    }
    setState(state){
        return Module._lv_imgbtn_set_state(this.ptr, state)
    }
    srcLeft(state){
        return Module._lv_imgbtn_get_src_left(this.ptr, state)
    }
    srcMiddle(state){
        return Module._lv_imgbtn_get_src_middle(this.ptr, state)
    }
    srcRight(state){
        return Module._lv_imgbtn_get_src_right(this.ptr, state)
    }
// AUTO GENERATE CODE END [Imgbtn METHODS] --------
}

class Led extends EventEmitter {
    constructor(parent) {
        super()
        this.ptr = Module._lv_led_create(parent.ptr)
    }
// AUTO GENERATE CODE START [Led METHODS] --------
    create(){
        return Module._lv_led_create(this.ptr)
    }
    setColor(color){
        return Module._lv_led_set_color(this.ptr, color)
    }
    setBrightness(bright){
        return Module._lv_led_set_brightness(this.ptr, bright)
    }
    on(){
        return Module._lv_led_on(this.ptr)
    }
    off(){
        return Module._lv_led_off(this.ptr)
    }
    toggle(){
        return Module._lv_led_toggle(this.ptr)
    }
// AUTO GENERATE CODE END [Led METHODS] --------
}

class Menu extends EventEmitter {
    constructor(parent) {
        super()
        this.ptr = Module._lv_menu_create(parent.ptr)
    }
// AUTO GENERATE CODE START [Menu METHODS] --------
// AUTO GENERATE CODE END [Menu METHODS] --------
}

class Meter extends EventEmitter {
    constructor(parent) {
        super()
        this.ptr = Module._lv_meter_create(parent.ptr)
    }
// AUTO GENERATE CODE START [Meter METHODS] --------
    create(){
        return Module._lv_meter_create(this.ptr)
    }
    addScale(){
        return Module._lv_meter_add_scale(this.ptr)
    }
    setScaleTicks(scale, cnt, width, len, color){
        return Module._lv_meter_set_scale_ticks(this.ptr, scale, cnt, width, len, color)
    }
    setScaleMajorTicks(scale, nth, width, len, color, label_gap){
        return Module._lv_meter_set_scale_major_ticks(this.ptr, scale, nth, width, len, color, label_gap)
    }
    setScaleRange(scale, min, max, angle_range, rotation){
        return Module._lv_meter_set_scale_range(this.ptr, scale, min, max, angle_range, rotation)
    }
    addNeedleLine(scale, width, color, r_mod){
        return Module._lv_meter_add_needle_line(this.ptr, scale, width, color, r_mod)
    }
    addNeedleImg(scale, src, pivot_x, pivot_y){
        return Module._lv_meter_add_needle_img(this.ptr, scale, src, pivot_x, pivot_y)
    }
    addArc(scale, width, color, r_mod){
        return Module._lv_meter_add_arc(this.ptr, scale, width, color, r_mod)
    }
    addScaleLines(scale, color_start, color_end, local, width_mod){
        return Module._lv_meter_add_scale_lines(this.ptr, scale, color_start, color_end, local, width_mod)
    }
    setIndicatorValue(indic, value){
        return Module._lv_meter_set_indicator_value(this.ptr, indic, value)
    }
    setIndicatorStartValue(indic, value){
        return Module._lv_meter_set_indicator_start_value(this.ptr, indic, value)
    }
    setIndicatorEndValue(indic, value){
        return Module._lv_meter_set_indicator_end_value(this.ptr, indic, value)
    }
// AUTO GENERATE CODE END [Meter METHODS] --------
}

class Span extends EventEmitter {
    constructor(parent) {
        super()
        this.ptr = Module._lv_span_create(parent.ptr)
    }
// AUTO GENERATE CODE START [Span METHODS] --------
    txt(){
        return Module._lv_span_get_txt(this.ptr)
    }
    setTxt(txt){
        return Module._lv_span_set_txt(this.ptr, txt)
    }
    spangroup(){
        return Module._lv_span_get_spangroup(this.ptr)
    }
    setSpangroup(spangroup){
        return Module._lv_span_set_spangroup(this.ptr, spangroup)
    }
    style(){
        return Module._lv_span_get_style(this.ptr)
    }
    setStyle(style){
        return Module._lv_span_set_style(this.ptr, style)
    }
    staticFlag(){
        return Module._lv_span_get_static_flag(this.ptr)
    }
    setStaticFlag(static_flag){
        return Module._lv_span_set_static_flag(this.ptr, static_flag)
    }
// AUTO GENERATE CODE END [Span METHODS] --------
}

class Spinbox extends EventEmitter {
    constructor(parent) {
        super()
        this.ptr = Module._lv_spinbox_create(parent.ptr)
    }
// AUTO GENERATE CODE START [Spinbox METHODS] --------
    create(){
        return Module._lv_spinbox_create(this.ptr)
    }
    setValue(i){
        return Module._lv_spinbox_set_value(this.ptr, i)
    }
    setRollover(b){
        return Module._lv_spinbox_set_rollover(this.ptr, b)
    }
    setDigitFormat(digit_count, separator_position){
        return Module._lv_spinbox_set_digit_format(this.ptr, digit_count, separator_position)
    }
    setStep(step){
        return Module._lv_spinbox_set_step(this.ptr, step)
    }
    setRange(range_min, range_max){
        return Module._lv_spinbox_set_range(this.ptr, range_min, range_max)
    }
    setPos(pos){
        return Module._lv_spinbox_set_pos(this.ptr, pos)
    }
    setDigitStepDirection(direction){
        return Module._lv_spinbox_set_digit_step_direction(this.ptr, direction)
    }
    rollover(){
        return Module._lv_spinbox_get_rollover(this.ptr)
    }
    value(){
        return Module._lv_spinbox_get_value(this.ptr)
    }
    step(){
        return Module._lv_spinbox_get_step(this.ptr)
    }
    stepNext(){
        return Module._lv_spinbox_step_next(this.ptr)
    }
    stepPrev(){
        return Module._lv_spinbox_step_prev(this.ptr)
    }
    increment(){
        return Module._lv_spinbox_increment(this.ptr)
    }
    decrement(){
        return Module._lv_spinbox_decrement(this.ptr)
    }
// AUTO GENERATE CODE END [Spinbox METHODS] --------
}

class Spinner extends EventEmitter {
    constructor(parent) {
        super()
        this.ptr = Module._lv_spinner_create(parent.ptr)
    }
// AUTO GENERATE CODE START [Spinner METHODS] --------
    create(time, arc_length){
        return Module._lv_spinner_create(this.ptr, time, arc_length)
    }
// AUTO GENERATE CODE END [Spinner METHODS] --------
}

class Tabview extends EventEmitter {
    constructor(parent) {
        super()
        this.ptr = Module._lv_tabview_create(parent.ptr)
    }
// AUTO GENERATE CODE START [Tabview METHODS] --------
    create(tab_pos, tab_size){
        return Module._lv_tabview_create(this.ptr, tab_pos, tab_size)
    }
    addTab(name){
        return Module._lv_tabview_add_tab(this.ptr, name)
    }
    content(){
        return Module._lv_tabview_get_content(this.ptr)
    }
    tabBtns(){
        return Module._lv_tabview_get_tab_btns(this.ptr)
    }
    setAct(id, anim_en){
        return Module._lv_tabview_set_act(this.ptr, id, anim_en)
    }
    tabAct(){
        return Module._lv_tabview_get_tab_act(this.ptr)
    }
// AUTO GENERATE CODE END [Tabview METHODS] --------
}

class Win extends EventEmitter {
    constructor(parent) {
        super()
        this.ptr = Module._lv_win_create(parent.ptr)
    }
// AUTO GENERATE CODE START [Win METHODS] --------
    create(header_height){
        return Module._lv_win_create(this.ptr, header_height)
    }
    addTitle(txt){
        return Module._lv_win_add_title(this.ptr, txt)
    }
    addBtn(icon, btn_w){
        return Module._lv_win_add_btn(this.ptr, icon, btn_w)
    }
    header(){
        return Module._lv_win_get_header(this.ptr)
    }
    content(){
        return Module._lv_win_get_content(this.ptr)
    }
// AUTO GENERATE CODE END [Win METHODS] --------
}

class Timer extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_timer_create()
    }
// AUTO GENERATE CODE START [Timer METHODS] --------
    period(){
        return Module._lv_timer_get_period(this.ptr)
    }
    lastRun(){
        return Module._lv_timer_get_last_run(this.ptr)
    }
    setLastRun(last_run){
        return Module._lv_timer_set_last_run(this.ptr, last_run)
    }
    timerCb(){
        return Module._lv_timer_get_timer_cb(this.ptr)
    }
    setTimerCb(timer_cb){
        return Module._lv_timer_set_timer_cb(this.ptr, timer_cb)
    }
    userData(){
        return Module._lv_timer_get_user_data(this.ptr)
    }
    setUserData(user_data){
        return Module._lv_timer_set_user_data(this.ptr, user_data)
    }
    repeatCount(){
        return Module._lv_timer_get_repeat_count(this.ptr)
    }
    paused(){
        return Module._lv_timer_get_paused(this.ptr)
    }
    setPaused(paused){
        return Module._lv_timer_set_paused(this.ptr, paused)
    }
// AUTO GENERATE CODE END [Timer METHODS] --------
}

class SqrtRes extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_sqrt_res_create()
    }
// AUTO GENERATE CODE START [SqrtRes METHODS] --------
    i(){
        return Module._lv_sqrt_res_get_i(this.ptr)
    }
    setI(i){
        return Module._lv_sqrt_res_set_i(this.ptr, i)
    }
    f(){
        return Module._lv_sqrt_res_get_f(this.ptr)
    }
    setF(f){
        return Module._lv_sqrt_res_set_f(this.ptr, f)
    }
// AUTO GENERATE CODE END [SqrtRes METHODS] --------
}

class MemMonitor extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_mem_monitor_create()
    }
// AUTO GENERATE CODE START [MemMonitor METHODS] --------
    totalSize(){
        return Module._lv_mem_monitor_get_total_size(this.ptr)
    }
    setTotalSize(total_size){
        return Module._lv_mem_monitor_set_total_size(this.ptr, total_size)
    }
    freeCnt(){
        return Module._lv_mem_monitor_get_free_cnt(this.ptr)
    }
    setFreeCnt(free_cnt){
        return Module._lv_mem_monitor_set_free_cnt(this.ptr, free_cnt)
    }
    freeSize(){
        return Module._lv_mem_monitor_get_free_size(this.ptr)
    }
    setFreeSize(free_size){
        return Module._lv_mem_monitor_set_free_size(this.ptr, free_size)
    }
    freeBiggestSize(){
        return Module._lv_mem_monitor_get_free_biggest_size(this.ptr)
    }
    setFreeBiggestSize(free_biggest_size){
        return Module._lv_mem_monitor_set_free_biggest_size(this.ptr, free_biggest_size)
    }
    usedCnt(){
        return Module._lv_mem_monitor_get_used_cnt(this.ptr)
    }
    setUsedCnt(used_cnt){
        return Module._lv_mem_monitor_set_used_cnt(this.ptr, used_cnt)
    }
    maxUsed(){
        return Module._lv_mem_monitor_get_max_used(this.ptr)
    }
    setMaxUsed(max_used){
        return Module._lv_mem_monitor_set_max_used(this.ptr, max_used)
    }
    usedPct(){
        return Module._lv_mem_monitor_get_used_pct(this.ptr)
    }
    setUsedPct(used_pct){
        return Module._lv_mem_monitor_set_used_pct(this.ptr, used_pct)
    }
    fragPct(){
        return Module._lv_mem_monitor_get_frag_pct(this.ptr)
    }
    setFragPct(frag_pct){
        return Module._lv_mem_monitor_set_frag_pct(this.ptr, frag_pct)
    }
// AUTO GENERATE CODE END [MemMonitor METHODS] --------
}

class MemBuf extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_mem_buf_create()
    }
// AUTO GENERATE CODE START [MemBuf METHODS] --------
    p(){
        return Module._lv_mem_buf_get_p(this.ptr)
    }
    setP(p){
        return Module._lv_mem_buf_set_p(this.ptr, p)
    }
    size(){
        return Module._lv_mem_buf_get_size(this.ptr)
    }
    setSize(size){
        return Module._lv_mem_buf_set_size(this.ptr, size)
    }
    used(){
        return Module._lv_mem_buf_get_used(this.ptr)
    }
    setUsed(used){
        return Module._lv_mem_buf_set_used(this.ptr, used)
    }
// AUTO GENERATE CODE END [MemBuf METHODS] --------
}

class Anim extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_anim_create()
    }
// AUTO GENERATE CODE START [Anim METHODS] --------
    var(){
        return Module._lv_anim_get_var(this.ptr)
    }
    execCb(){
        return Module._lv_anim_get_exec_cb(this.ptr)
    }
    startCb(){
        return Module._lv_anim_get_start_cb(this.ptr)
    }
    readyCb(){
        return Module._lv_anim_get_ready_cb(this.ptr)
    }
    getValueCb(){
        return Module._lv_anim_get_get_value_cb(this.ptr)
    }
    pathCb(){
        return Module._lv_anim_get_path_cb(this.ptr)
    }
    startValue(){
        return Module._lv_anim_get_start_value(this.ptr)
    }
    setStartValue(start_value){
        return Module._lv_anim_set_start_value(this.ptr, start_value)
    }
    currentValue(){
        return Module._lv_anim_get_current_value(this.ptr)
    }
    setCurrentValue(current_value){
        return Module._lv_anim_set_current_value(this.ptr, current_value)
    }
    endValue(){
        return Module._lv_anim_get_end_value(this.ptr)
    }
    setEndValue(end_value){
        return Module._lv_anim_set_end_value(this.ptr, end_value)
    }
    time(){
        return Module._lv_anim_get_time(this.ptr)
    }
    actTime(){
        return Module._lv_anim_get_act_time(this.ptr)
    }
    setActTime(act_time){
        return Module._lv_anim_set_act_time(this.ptr, act_time)
    }
    playbackDelay(){
        return Module._lv_anim_get_playback_delay(this.ptr)
    }
    playbackTime(){
        return Module._lv_anim_get_playback_time(this.ptr)
    }
    repeatDelay(){
        return Module._lv_anim_get_repeat_delay(this.ptr)
    }
    repeatCnt(){
        return Module._lv_anim_get_repeat_cnt(this.ptr)
    }
    setRepeatCnt(repeat_cnt){
        return Module._lv_anim_set_repeat_cnt(this.ptr, repeat_cnt)
    }
    earlyApply(){
        return Module._lv_anim_get_early_apply(this.ptr)
    }
    playbackNow(){
        return Module._lv_anim_get_playback_now(this.ptr)
    }
    setPlaybackNow(playback_now){
        return Module._lv_anim_set_playback_now(this.ptr, playback_now)
    }
    runRound(){
        return Module._lv_anim_get_run_round(this.ptr)
    }
    setRunRound(run_round){
        return Module._lv_anim_set_run_round(this.ptr, run_round)
    }
    startCbCalled(){
        return Module._lv_anim_get_start_cb_called(this.ptr)
    }
    setStartCbCalled(start_cb_called){
        return Module._lv_anim_set_start_cb_called(this.ptr, start_cb_called)
    }
// AUTO GENERATE CODE END [Anim METHODS] --------
}

class Color8 extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_color8_create()
    }
// AUTO GENERATE CODE START [Color8 METHODS] --------
    full(){
        return Module._lv_color8_get_full(this.ptr)
    }
    setFull(full){
        return Module._lv_color8_set_full(this.ptr, full)
    }
// AUTO GENERATE CODE END [Color8 METHODS] --------
}

class Color16 extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_color16_create()
    }
// AUTO GENERATE CODE START [Color16 METHODS] --------
    full(){
        return Module._lv_color16_get_full(this.ptr)
    }
    setFull(full){
        return Module._lv_color16_set_full(this.ptr, full)
    }
// AUTO GENERATE CODE END [Color16 METHODS] --------
}

class Color32 extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_color32_create()
    }
// AUTO GENERATE CODE START [Color32 METHODS] --------
    full(){
        return Module._lv_color32_get_full(this.ptr)
    }
    setFull(full){
        return Module._lv_color32_set_full(this.ptr, full)
    }
// AUTO GENERATE CODE END [Color32 METHODS] --------
}

class ColorHsv extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_color_hsv_create()
    }
// AUTO GENERATE CODE START [ColorHsv METHODS] --------
    h(){
        return Module._lv_color_hsv_get_h(this.ptr)
    }
    setH(h){
        return Module._lv_color_hsv_set_h(this.ptr, h)
    }
    s(){
        return Module._lv_color_hsv_get_s(this.ptr)
    }
    setS(s){
        return Module._lv_color_hsv_set_s(this.ptr, s)
    }
    v(){
        return Module._lv_color_hsv_get_v(this.ptr)
    }
    setV(v){
        return Module._lv_color_hsv_set_v(this.ptr, v)
    }
// AUTO GENERATE CODE END [ColorHsv METHODS] --------
}

class ColorFilterDsc extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_color_filter_dsc_create()
    }
// AUTO GENERATE CODE START [ColorFilterDsc METHODS] --------
    filterCb(){
        return Module._lv_color_filter_dsc_get_filter_cb(this.ptr)
    }
    setFilterCb(filter_cb){
        return Module._lv_color_filter_dsc_set_filter_cb(this.ptr, filter_cb)
    }
    userData(){
        return Module._lv_color_filter_dsc_get_user_data(this.ptr)
    }
    setUserData(user_data){
        return Module._lv_color_filter_dsc_set_user_data(this.ptr, user_data)
    }
// AUTO GENERATE CODE END [ColorFilterDsc METHODS] --------
}

class Point extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_point_create()
    }
// AUTO GENERATE CODE START [Point METHODS] --------
    x(){
        return Module._lv_point_get_x(this.ptr)
    }
    setX(x){
        return Module._lv_point_set_x(this.ptr, x)
    }
    y(){
        return Module._lv_point_get_y(this.ptr)
    }
    setY(y){
        return Module._lv_point_set_y(this.ptr, y)
    }
// AUTO GENERATE CODE END [Point METHODS] --------
}

class Area extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_area_create()
    }
// AUTO GENERATE CODE START [Area METHODS] --------
    x1(){
        return Module._lv_area_get_x1(this.ptr)
    }
    setX1(x1){
        return Module._lv_area_set_x1(this.ptr, x1)
    }
    y1(){
        return Module._lv_area_get_y1(this.ptr)
    }
    setY1(y1){
        return Module._lv_area_set_y1(this.ptr, y1)
    }
    x2(){
        return Module._lv_area_get_x2(this.ptr)
    }
    setX2(x2){
        return Module._lv_area_set_x2(this.ptr, x2)
    }
    y2(){
        return Module._lv_area_get_y2(this.ptr)
    }
    setY2(y2){
        return Module._lv_area_set_y2(this.ptr, y2)
    }
// AUTO GENERATE CODE END [Area METHODS] --------
}

class ImgHeader extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_img_header_create()
    }
// AUTO GENERATE CODE START [ImgHeader METHODS] --------
    cf(){
        return Module._lv_img_header_get_cf(this.ptr)
    }
    setCf(cf){
        return Module._lv_img_header_set_cf(this.ptr, cf)
    }
    alwaysZero(){
        return Module._lv_img_header_get_always_zero(this.ptr)
    }
    setAlwaysZero(always_zero){
        return Module._lv_img_header_set_always_zero(this.ptr, always_zero)
    }
    reserved(){
        return Module._lv_img_header_get_reserved(this.ptr)
    }
    setReserved(reserved){
        return Module._lv_img_header_set_reserved(this.ptr, reserved)
    }
    w(){
        return Module._lv_img_header_get_w(this.ptr)
    }
    setW(w){
        return Module._lv_img_header_set_w(this.ptr, w)
    }
    h(){
        return Module._lv_img_header_get_h(this.ptr)
    }
    setH(h){
        return Module._lv_img_header_set_h(this.ptr, h)
    }
// AUTO GENERATE CODE END [ImgHeader METHODS] --------
}

class ImgDsc extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_img_dsc_create()
    }
// AUTO GENERATE CODE START [ImgDsc METHODS] --------
    header(){
        return Module._lv_img_dsc_get_header(this.ptr)
    }
    setHeader(header){
        return Module._lv_img_dsc_set_header(this.ptr, header)
    }
    dataSize(){
        return Module._lv_img_dsc_get_data_size(this.ptr)
    }
    setDataSize(data_size){
        return Module._lv_img_dsc_set_data_size(this.ptr, data_size)
    }
    data(){
        return Module._lv_img_dsc_get_data(this.ptr)
    }
    setData(data){
        return Module._lv_img_dsc_set_data(this.ptr, data)
    }
// AUTO GENERATE CODE END [ImgDsc METHODS] --------
}

class ImgTransformDsc extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_img_transform_dsc_create()
    }
// AUTO GENERATE CODE START [ImgTransformDsc METHODS] --------
// AUTO GENERATE CODE END [ImgTransformDsc METHODS] --------
}

class Ll extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_ll_create()
    }
// AUTO GENERATE CODE START [Ll METHODS] --------
    nSize(){
        return Module._lv_ll_get_n_size(this.ptr)
    }
    setNSize(n_size){
        return Module._lv_ll_set_n_size(this.ptr, n_size)
    }
    head(){
        return Module._lv_ll_get_head(this.ptr)
    }
    setHead(head){
        return Module._lv_ll_set_head(this.ptr, head)
    }
    tail(){
        return Module._lv_ll_get_tail(this.ptr)
    }
    setTail(tail){
        return Module._lv_ll_set_tail(this.ptr, tail)
    }
// AUTO GENERATE CODE END [Ll METHODS] --------
}

class IndevData extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_indev_data_create()
    }
// AUTO GENERATE CODE START [IndevData METHODS] --------
    point(){
        return Module._lv_indev_data_get_point(this.ptr)
    }
    setPoint(point){
        return Module._lv_indev_data_set_point(this.ptr, point)
    }
    key(){
        return Module._lv_indev_data_get_key(this.ptr)
    }
    setKey(key){
        return Module._lv_indev_data_set_key(this.ptr, key)
    }
    btnId(){
        return Module._lv_indev_data_get_btn_id(this.ptr)
    }
    setBtnId(btn_id){
        return Module._lv_indev_data_set_btn_id(this.ptr, btn_id)
    }
    encDiff(){
        return Module._lv_indev_data_get_enc_diff(this.ptr)
    }
    setEncDiff(enc_diff){
        return Module._lv_indev_data_set_enc_diff(this.ptr, enc_diff)
    }
    state(){
        return Module._lv_indev_data_get_state(this.ptr)
    }
    setState(state){
        return Module._lv_indev_data_set_state(this.ptr, state)
    }
    continueReading(){
        return Module._lv_indev_data_get_continue_reading(this.ptr)
    }
    setContinueReading(continue_reading){
        return Module._lv_indev_data_set_continue_reading(this.ptr, continue_reading)
    }
// AUTO GENERATE CODE END [IndevData METHODS] --------
}

class _IndevProc extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv__indev_proc_create()
    }
// AUTO GENERATE CODE START [_IndevProc METHODS] --------
    state(){
        return Module.__lv_indev_proc_get_state(this.ptr)
    }
    setState(state){
        return Module.__lv_indev_proc_set_state(this.ptr, state)
    }
    longPrSent(){
        return Module.__lv_indev_proc_get_long_pr_sent(this.ptr)
    }
    setLongPrSent(long_pr_sent){
        return Module.__lv_indev_proc_set_long_pr_sent(this.ptr, long_pr_sent)
    }
    resetQuery(){
        return Module.__lv_indev_proc_get_reset_query(this.ptr)
    }
    setResetQuery(reset_query){
        return Module.__lv_indev_proc_set_reset_query(this.ptr, reset_query)
    }
    disabled(){
        return Module.__lv_indev_proc_get_disabled(this.ptr)
    }
    setDisabled(disabled){
        return Module.__lv_indev_proc_set_disabled(this.ptr, disabled)
    }
    waitUntilRelease(){
        return Module.__lv_indev_proc_get_wait_until_release(this.ptr)
    }
    setWaitUntilRelease(wait_until_release){
        return Module.__lv_indev_proc_set_wait_until_release(this.ptr, wait_until_release)
    }
    prTimestamp(){
        return Module.__lv_indev_proc_get_pr_timestamp(this.ptr)
    }
    setPrTimestamp(pr_timestamp){
        return Module.__lv_indev_proc_set_pr_timestamp(this.ptr, pr_timestamp)
    }
    longprRepTimestamp(){
        return Module.__lv_indev_proc_get_longpr_rep_timestamp(this.ptr)
    }
    setLongprRepTimestamp(longpr_rep_timestamp){
        return Module.__lv_indev_proc_set_longpr_rep_timestamp(this.ptr, longpr_rep_timestamp)
    }
// AUTO GENERATE CODE END [_IndevProc METHODS] --------
}

class Indev extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_indev_create()
    }
// AUTO GENERATE CODE START [Indev METHODS] --------
    driver(){
        return Module._lv_indev_get_driver(this.ptr)
    }
    setDriver(driver){
        return Module._lv_indev_set_driver(this.ptr, driver)
    }
    proc(){
        return Module._lv_indev_get_proc(this.ptr)
    }
    setProc(proc){
        return Module._lv_indev_set_proc(this.ptr, proc)
    }
    cursor(){
        return Module._lv_indev_get_cursor(this.ptr)
    }
    group(){
        return Module._lv_indev_get_group(this.ptr)
    }
    btnPoints(){
        return Module._lv_indev_get_btn_points(this.ptr)
    }
    setBtnPoints(btn_points){
        return Module._lv_indev_set_btn_points(this.ptr, btn_points)
    }
// AUTO GENERATE CODE END [Indev METHODS] --------
}

class FontGlyphDsc extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_font_glyph_dsc_create()
    }
// AUTO GENERATE CODE START [FontGlyphDsc METHODS] --------
    advW(){
        return Module._lv_font_glyph_dsc_get_adv_w(this.ptr)
    }
    setAdvW(adv_w){
        return Module._lv_font_glyph_dsc_set_adv_w(this.ptr, adv_w)
    }
    boxW(){
        return Module._lv_font_glyph_dsc_get_box_w(this.ptr)
    }
    setBoxW(box_w){
        return Module._lv_font_glyph_dsc_set_box_w(this.ptr, box_w)
    }
    boxH(){
        return Module._lv_font_glyph_dsc_get_box_h(this.ptr)
    }
    setBoxH(box_h){
        return Module._lv_font_glyph_dsc_set_box_h(this.ptr, box_h)
    }
    ofsX(){
        return Module._lv_font_glyph_dsc_get_ofs_x(this.ptr)
    }
    setOfsX(ofs_x){
        return Module._lv_font_glyph_dsc_set_ofs_x(this.ptr, ofs_x)
    }
    ofsY(){
        return Module._lv_font_glyph_dsc_get_ofs_y(this.ptr)
    }
    setOfsY(ofs_y){
        return Module._lv_font_glyph_dsc_set_ofs_y(this.ptr, ofs_y)
    }
    bpp(){
        return Module._lv_font_glyph_dsc_get_bpp(this.ptr)
    }
    setBpp(bpp){
        return Module._lv_font_glyph_dsc_set_bpp(this.ptr, bpp)
    }
// AUTO GENERATE CODE END [FontGlyphDsc METHODS] --------
}

class Font extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_font_create()
    }
// AUTO GENERATE CODE START [Font METHODS] --------
    getGlyphDsc(){
        return Module._lv_font_get_get_glyph_dsc(this.ptr)
    }
    setGetGlyphDsc(get_glyph_dsc){
        return Module._lv_font_set_get_glyph_dsc(this.ptr, get_glyph_dsc)
    }
    getGlyphBitmap(){
        return Module._lv_font_get_get_glyph_bitmap(this.ptr)
    }
    setGetGlyphBitmap(get_glyph_bitmap){
        return Module._lv_font_set_get_glyph_bitmap(this.ptr, get_glyph_bitmap)
    }
    setLineHeight(line_height){
        return Module._lv_font_set_line_height(this.ptr, line_height)
    }
    baseLine(){
        return Module._lv_font_get_base_line(this.ptr)
    }
    setBaseLine(base_line){
        return Module._lv_font_set_base_line(this.ptr, base_line)
    }
    subpx(){
        return Module._lv_font_get_subpx(this.ptr)
    }
    setSubpx(subpx){
        return Module._lv_font_set_subpx(this.ptr, subpx)
    }
    underlinePosition(){
        return Module._lv_font_get_underline_position(this.ptr)
    }
    setUnderlinePosition(underline_position){
        return Module._lv_font_set_underline_position(this.ptr, underline_position)
    }
    underlineThickness(){
        return Module._lv_font_get_underline_thickness(this.ptr)
    }
    setUnderlineThickness(underline_thickness){
        return Module._lv_font_set_underline_thickness(this.ptr, underline_thickness)
    }
    dsc(){
        return Module._lv_font_get_dsc(this.ptr)
    }
    setDsc(dsc){
        return Module._lv_font_set_dsc(this.ptr, dsc)
    }
    userData(){
        return Module._lv_font_get_user_data(this.ptr)
    }
    setUserData(user_data){
        return Module._lv_font_set_user_data(this.ptr, user_data)
    }
// AUTO GENERATE CODE END [Font METHODS] --------
}

class StyleValue extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_style_value_create()
    }
// AUTO GENERATE CODE START [StyleValue METHODS] --------
    num(){
        return Module._lv_style_value_get_num(this.ptr)
    }
    setNum(num){
        return Module._lv_style_value_set_num(this.ptr, num)
    }
    ptr(){
        return Module._lv_style_value_get_ptr(this.ptr)
    }
    setPtr(ptr){
        return Module._lv_style_value_set_ptr(this.ptr, ptr)
    }
    color(){
        return Module._lv_style_value_get_color(this.ptr)
    }
    setColor(color){
        return Module._lv_style_value_set_color(this.ptr, color)
    }
// AUTO GENERATE CODE END [StyleValue METHODS] --------
}

class StyleTransitionDsc extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_style_transition_dsc_create()
    }
// AUTO GENERATE CODE START [StyleTransitionDsc METHODS] --------
    props(){
        return Module._lv_style_transition_dsc_get_props(this.ptr)
    }
    setProps(props){
        return Module._lv_style_transition_dsc_set_props(this.ptr, props)
    }
    userData(){
        return Module._lv_style_transition_dsc_get_user_data(this.ptr)
    }
    setUserData(user_data){
        return Module._lv_style_transition_dsc_set_user_data(this.ptr, user_data)
    }
    pathXcb(){
        return Module._lv_style_transition_dsc_get_path_xcb(this.ptr)
    }
    setPathXcb(path_xcb){
        return Module._lv_style_transition_dsc_set_path_xcb(this.ptr, path_xcb)
    }
    time(){
        return Module._lv_style_transition_dsc_get_time(this.ptr)
    }
    setTime(time){
        return Module._lv_style_transition_dsc_set_time(this.ptr, time)
    }
    delay(){
        return Module._lv_style_transition_dsc_get_delay(this.ptr)
    }
    setDelay(delay){
        return Module._lv_style_transition_dsc_set_delay(this.ptr, delay)
    }
// AUTO GENERATE CODE END [StyleTransitionDsc METHODS] --------
}

class StyleConstProp extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_style_const_prop_create()
    }
// AUTO GENERATE CODE START [StyleConstProp METHODS] --------
    prop(){
        return Module._lv_style_const_prop_get_prop(this.ptr)
    }
    setProp(prop){
        return Module._lv_style_const_prop_set_prop(this.ptr, prop)
    }
    value(){
        return Module._lv_style_const_prop_get_value(this.ptr)
    }
    setValue(value){
        return Module._lv_style_const_prop_set_value(this.ptr, value)
    }
// AUTO GENERATE CODE END [StyleConstProp METHODS] --------
}

class Style extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_style_create()
    }
// AUTO GENERATE CODE START [Style METHODS] --------
    prop1(){
        return Module._lv_style_get_prop1(this.ptr)
    }
    setProp1(prop1){
        return Module._lv_style_set_prop1(this.ptr, prop1)
    }
    isConst(){
        return Module._lv_style_get_is_const(this.ptr)
    }
    setIsConst(is_const){
        return Module._lv_style_set_is_const(this.ptr, is_const)
    }
    hasGroup(){
        return Module._lv_style_get_has_group(this.ptr)
    }
    setHasGroup(has_group){
        return Module._lv_style_set_has_group(this.ptr, has_group)
    }
    propCnt(){
        return Module._lv_style_get_prop_cnt(this.ptr)
    }
    setPropCnt(prop_cnt){
        return Module._lv_style_set_prop_cnt(this.ptr, prop_cnt)
    }
// AUTO GENERATE CODE END [Style METHODS] --------
}

class _ObjStyle extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv__obj_style_create()
    }
// AUTO GENERATE CODE START [_ObjStyle METHODS] --------
    style(){
        return Module.__lv_obj_style_get_style(this.ptr)
    }
    setStyle(style){
        return Module.__lv_obj_style_set_style(this.ptr, style)
    }
    selector(){
        return Module.__lv_obj_style_get_selector(this.ptr)
    }
    setSelector(selector){
        return Module.__lv_obj_style_set_selector(this.ptr, selector)
    }
    isLocal(){
        return Module.__lv_obj_style_get_is_local(this.ptr)
    }
    setIsLocal(is_local){
        return Module.__lv_obj_style_set_is_local(this.ptr, is_local)
    }
    isTrans(){
        return Module.__lv_obj_style_get_is_trans(this.ptr)
    }
    setIsTrans(is_trans){
        return Module.__lv_obj_style_set_is_trans(this.ptr, is_trans)
    }
// AUTO GENERATE CODE END [_ObjStyle METHODS] --------
}

class _ObjStyleTransitionDsc extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv__obj_style_transition_dsc_create()
    }
// AUTO GENERATE CODE START [_ObjStyleTransitionDsc METHODS] --------
    time(){
        return Module.__lv_obj_style_transition_dsc_get_time(this.ptr)
    }
    setTime(time){
        return Module.__lv_obj_style_transition_dsc_set_time(this.ptr, time)
    }
    delay(){
        return Module.__lv_obj_style_transition_dsc_get_delay(this.ptr)
    }
    setDelay(delay){
        return Module.__lv_obj_style_transition_dsc_set_delay(this.ptr, delay)
    }
    selector(){
        return Module.__lv_obj_style_transition_dsc_get_selector(this.ptr)
    }
    setSelector(selector){
        return Module.__lv_obj_style_transition_dsc_set_selector(this.ptr, selector)
    }
    prop(){
        return Module.__lv_obj_style_transition_dsc_get_prop(this.ptr)
    }
    setProp(prop){
        return Module.__lv_obj_style_transition_dsc_set_prop(this.ptr, prop)
    }
    pathCb(){
        return Module.__lv_obj_style_transition_dsc_get_path_cb(this.ptr)
    }
    setPathCb(path_cb){
        return Module.__lv_obj_style_transition_dsc_set_path_cb(this.ptr, path_cb)
    }
    userData(){
        return Module.__lv_obj_style_transition_dsc_get_user_data(this.ptr)
    }
    setUserData(user_data){
        return Module.__lv_obj_style_transition_dsc_set_user_data(this.ptr, user_data)
    }
// AUTO GENERATE CODE END [_ObjStyleTransitionDsc METHODS] --------
}

class FsDrv extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_fs_drv_create()
    }
// AUTO GENERATE CODE START [FsDrv METHODS] --------
    letter(){
        return Module._lv_fs_drv_get_letter(this.ptr)
    }
    setLetter(letter){
        return Module._lv_fs_drv_set_letter(this.ptr, letter)
    }
    readyCb(){
        return Module._lv_fs_drv_get_ready_cb(this.ptr)
    }
    setReadyCb(ready_cb){
        return Module._lv_fs_drv_set_ready_cb(this.ptr, ready_cb)
    }
    openCb(){
        return Module._lv_fs_drv_get_open_cb(this.ptr)
    }
    setOpenCb(open_cb){
        return Module._lv_fs_drv_set_open_cb(this.ptr, open_cb)
    }
    closeCb(){
        return Module._lv_fs_drv_get_close_cb(this.ptr)
    }
    setCloseCb(close_cb){
        return Module._lv_fs_drv_set_close_cb(this.ptr, close_cb)
    }
    readCb(){
        return Module._lv_fs_drv_get_read_cb(this.ptr)
    }
    setReadCb(read_cb){
        return Module._lv_fs_drv_set_read_cb(this.ptr, read_cb)
    }
    writeCb(){
        return Module._lv_fs_drv_get_write_cb(this.ptr)
    }
    setWriteCb(write_cb){
        return Module._lv_fs_drv_set_write_cb(this.ptr, write_cb)
    }
    seekCb(){
        return Module._lv_fs_drv_get_seek_cb(this.ptr)
    }
    setSeekCb(seek_cb){
        return Module._lv_fs_drv_set_seek_cb(this.ptr, seek_cb)
    }
    tellCb(){
        return Module._lv_fs_drv_get_tell_cb(this.ptr)
    }
    setTellCb(tell_cb){
        return Module._lv_fs_drv_set_tell_cb(this.ptr, tell_cb)
    }
    dirOpenCb(){
        return Module._lv_fs_drv_get_dir_open_cb(this.ptr)
    }
    setDirOpenCb(dir_open_cb){
        return Module._lv_fs_drv_set_dir_open_cb(this.ptr, dir_open_cb)
    }
    dirReadCb(){
        return Module._lv_fs_drv_get_dir_read_cb(this.ptr)
    }
    setDirReadCb(dir_read_cb){
        return Module._lv_fs_drv_set_dir_read_cb(this.ptr, dir_read_cb)
    }
    dirCloseCb(){
        return Module._lv_fs_drv_get_dir_close_cb(this.ptr)
    }
    setDirCloseCb(dir_close_cb){
        return Module._lv_fs_drv_set_dir_close_cb(this.ptr, dir_close_cb)
    }
    userData(){
        return Module._lv_fs_drv_get_user_data(this.ptr)
    }
    setUserData(user_data){
        return Module._lv_fs_drv_set_user_data(this.ptr, user_data)
    }
// AUTO GENERATE CODE END [FsDrv METHODS] --------
}

class FsFile extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_fs_file_create()
    }
// AUTO GENERATE CODE START [FsFile METHODS] --------
    fileD(){
        return Module._lv_fs_file_get_file_d(this.ptr)
    }
    setFileD(file_d){
        return Module._lv_fs_file_set_file_d(this.ptr, file_d)
    }
    drv(){
        return Module._lv_fs_file_get_drv(this.ptr)
    }
    setDrv(drv){
        return Module._lv_fs_file_set_drv(this.ptr, drv)
    }
// AUTO GENERATE CODE END [FsFile METHODS] --------
}

class FsDir extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_fs_dir_create()
    }
// AUTO GENERATE CODE START [FsDir METHODS] --------
    dirD(){
        return Module._lv_fs_dir_get_dir_d(this.ptr)
    }
    setDirD(dir_d){
        return Module._lv_fs_dir_set_dir_d(this.ptr, dir_d)
    }
    drv(){
        return Module._lv_fs_dir_get_drv(this.ptr)
    }
    setDrv(drv){
        return Module._lv_fs_dir_set_drv(this.ptr, drv)
    }
// AUTO GENERATE CODE END [FsDir METHODS] --------
}

class ImgDecoder extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_img_decoder_create()
    }
// AUTO GENERATE CODE START [ImgDecoder METHODS] --------
    infoCb(){
        return Module._lv_img_decoder_get_info_cb(this.ptr)
    }
    openCb(){
        return Module._lv_img_decoder_get_open_cb(this.ptr)
    }
    readLineCb(){
        return Module._lv_img_decoder_get_read_line_cb(this.ptr)
    }
    closeCb(){
        return Module._lv_img_decoder_get_close_cb(this.ptr)
    }
    userData(){
        return Module._lv_img_decoder_get_user_data(this.ptr)
    }
    setUserData(user_data){
        return Module._lv_img_decoder_set_user_data(this.ptr, user_data)
    }
// AUTO GENERATE CODE END [ImgDecoder METHODS] --------
}

class ImgDecoderDsc extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_img_decoder_dsc_create()
    }
// AUTO GENERATE CODE START [ImgDecoderDsc METHODS] --------
    decoder(){
        return Module._lv_img_decoder_dsc_get_decoder(this.ptr)
    }
    setDecoder(decoder){
        return Module._lv_img_decoder_dsc_set_decoder(this.ptr, decoder)
    }
    src(){
        return Module._lv_img_decoder_dsc_get_src(this.ptr)
    }
    setSrc(src){
        return Module._lv_img_decoder_dsc_set_src(this.ptr, src)
    }
    color(){
        return Module._lv_img_decoder_dsc_get_color(this.ptr)
    }
    setColor(color){
        return Module._lv_img_decoder_dsc_set_color(this.ptr, color)
    }
    frameId(){
        return Module._lv_img_decoder_dsc_get_frame_id(this.ptr)
    }
    setFrameId(frame_id){
        return Module._lv_img_decoder_dsc_set_frame_id(this.ptr, frame_id)
    }
    srcType(){
        return Module._lv_img_decoder_dsc_get_src_type(this.ptr)
    }
    setSrcType(src_type){
        return Module._lv_img_decoder_dsc_set_src_type(this.ptr, src_type)
    }
    header(){
        return Module._lv_img_decoder_dsc_get_header(this.ptr)
    }
    setHeader(header){
        return Module._lv_img_decoder_dsc_set_header(this.ptr, header)
    }
    imgData(){
        return Module._lv_img_decoder_dsc_get_img_data(this.ptr)
    }
    setImgData(img_data){
        return Module._lv_img_decoder_dsc_set_img_data(this.ptr, img_data)
    }
    timeToOpen(){
        return Module._lv_img_decoder_dsc_get_time_to_open(this.ptr)
    }
    setTimeToOpen(time_to_open){
        return Module._lv_img_decoder_dsc_set_time_to_open(this.ptr, time_to_open)
    }
    errorMsg(){
        return Module._lv_img_decoder_dsc_get_error_msg(this.ptr)
    }
    setErrorMsg(error_msg){
        return Module._lv_img_decoder_dsc_set_error_msg(this.ptr, error_msg)
    }
    userData(){
        return Module._lv_img_decoder_dsc_get_user_data(this.ptr)
    }
    setUserData(user_data){
        return Module._lv_img_decoder_dsc_set_user_data(this.ptr, user_data)
    }
// AUTO GENERATE CODE END [ImgDecoderDsc METHODS] --------
}

class _ImgCacheEntry extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv__img_cache_entry_create()
    }
// AUTO GENERATE CODE START [_ImgCacheEntry METHODS] --------
    decDsc(){
        return Module.__lv_img_cache_entry_get_dec_dsc(this.ptr)
    }
    setDecDsc(dec_dsc){
        return Module.__lv_img_cache_entry_set_dec_dsc(this.ptr, dec_dsc)
    }
    life(){
        return Module.__lv_img_cache_entry_get_life(this.ptr)
    }
    setLife(life){
        return Module.__lv_img_cache_entry_set_life(this.ptr, life)
    }
// AUTO GENERATE CODE END [_ImgCacheEntry METHODS] --------
}

class _DrawMaskSaved extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv__draw_mask_saved_create()
    }
// AUTO GENERATE CODE START [_DrawMaskSaved METHODS] --------
    param(){
        return Module.__lv_draw_mask_saved_get_param(this.ptr)
    }
    setParam(param){
        return Module.__lv_draw_mask_saved_set_param(this.ptr, param)
    }
    customId(){
        return Module.__lv_draw_mask_saved_get_custom_id(this.ptr)
    }
    setCustomId(custom_id){
        return Module.__lv_draw_mask_saved_set_custom_id(this.ptr, custom_id)
    }
// AUTO GENERATE CODE END [_DrawMaskSaved METHODS] --------
}

class _DrawMaskCommonDsc extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv__draw_mask_common_dsc_create()
    }
// AUTO GENERATE CODE START [_DrawMaskCommonDsc METHODS] --------
    cb(){
        return Module.__lv_draw_mask_common_dsc_get_cb(this.ptr)
    }
    setCb(cb){
        return Module.__lv_draw_mask_common_dsc_set_cb(this.ptr, cb)
    }
    type(){
        return Module.__lv_draw_mask_common_dsc_get_type(this.ptr)
    }
    setType(type){
        return Module.__lv_draw_mask_common_dsc_set_type(this.ptr, type)
    }
// AUTO GENERATE CODE END [_DrawMaskCommonDsc METHODS] --------
}

class DrawMaskLineParam extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_draw_mask_line_param_create()
    }
// AUTO GENERATE CODE START [DrawMaskLineParam METHODS] --------
    dsc(){
        return Module._lv_draw_mask_line_param_get_dsc(this.ptr)
    }
    setDsc(dsc){
        return Module._lv_draw_mask_line_param_set_dsc(this.ptr, dsc)
    }
    origo(){
        return Module._lv_draw_mask_line_param_get_origo(this.ptr)
    }
    setOrigo(origo){
        return Module._lv_draw_mask_line_param_set_origo(this.ptr, origo)
    }
    xySteep(){
        return Module._lv_draw_mask_line_param_get_xy_steep(this.ptr)
    }
    setXySteep(xy_steep){
        return Module._lv_draw_mask_line_param_set_xy_steep(this.ptr, xy_steep)
    }
    yxSteep(){
        return Module._lv_draw_mask_line_param_get_yx_steep(this.ptr)
    }
    setYxSteep(yx_steep){
        return Module._lv_draw_mask_line_param_set_yx_steep(this.ptr, yx_steep)
    }
    steep(){
        return Module._lv_draw_mask_line_param_get_steep(this.ptr)
    }
    setSteep(steep){
        return Module._lv_draw_mask_line_param_set_steep(this.ptr, steep)
    }
    spx(){
        return Module._lv_draw_mask_line_param_get_spx(this.ptr)
    }
    setSpx(spx){
        return Module._lv_draw_mask_line_param_set_spx(this.ptr, spx)
    }
    flat(){
        return Module._lv_draw_mask_line_param_get_flat(this.ptr)
    }
    setFlat(flat){
        return Module._lv_draw_mask_line_param_set_flat(this.ptr, flat)
    }
    inv(){
        return Module._lv_draw_mask_line_param_get_inv(this.ptr)
    }
    setInv(inv){
        return Module._lv_draw_mask_line_param_set_inv(this.ptr, inv)
    }
// AUTO GENERATE CODE END [DrawMaskLineParam METHODS] --------
}

class DrawMaskAngleParam extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_draw_mask_angle_param_create()
    }
// AUTO GENERATE CODE START [DrawMaskAngleParam METHODS] --------
    dsc(){
        return Module._lv_draw_mask_angle_param_get_dsc(this.ptr)
    }
    setDsc(dsc){
        return Module._lv_draw_mask_angle_param_set_dsc(this.ptr, dsc)
    }
    startLine(){
        return Module._lv_draw_mask_angle_param_get_start_line(this.ptr)
    }
    setStartLine(start_line){
        return Module._lv_draw_mask_angle_param_set_start_line(this.ptr, start_line)
    }
    endLine(){
        return Module._lv_draw_mask_angle_param_get_end_line(this.ptr)
    }
    setEndLine(end_line){
        return Module._lv_draw_mask_angle_param_set_end_line(this.ptr, end_line)
    }
    deltaDeg(){
        return Module._lv_draw_mask_angle_param_get_delta_deg(this.ptr)
    }
    setDeltaDeg(delta_deg){
        return Module._lv_draw_mask_angle_param_set_delta_deg(this.ptr, delta_deg)
    }
// AUTO GENERATE CODE END [DrawMaskAngleParam METHODS] --------
}

class _DrawMaskRadiusCircleDsc extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv__draw_mask_radius_circle_dsc_create()
    }
// AUTO GENERATE CODE START [_DrawMaskRadiusCircleDsc METHODS] --------
    buf(){
        return Module.__lv_draw_mask_radius_circle_dsc_get_buf(this.ptr)
    }
    setBuf(buf){
        return Module.__lv_draw_mask_radius_circle_dsc_set_buf(this.ptr, buf)
    }
    cirOpa(){
        return Module.__lv_draw_mask_radius_circle_dsc_get_cir_opa(this.ptr)
    }
    setCirOpa(cir_opa){
        return Module.__lv_draw_mask_radius_circle_dsc_set_cir_opa(this.ptr, cir_opa)
    }
    xStartOnY(){
        return Module.__lv_draw_mask_radius_circle_dsc_get_x_start_on_y(this.ptr)
    }
    setXStartOnY(x_start_on_y){
        return Module.__lv_draw_mask_radius_circle_dsc_set_x_start_on_y(this.ptr, x_start_on_y)
    }
    opaStartOnY(){
        return Module.__lv_draw_mask_radius_circle_dsc_get_opa_start_on_y(this.ptr)
    }
    setOpaStartOnY(opa_start_on_y){
        return Module.__lv_draw_mask_radius_circle_dsc_set_opa_start_on_y(this.ptr, opa_start_on_y)
    }
    life(){
        return Module.__lv_draw_mask_radius_circle_dsc_get_life(this.ptr)
    }
    setLife(life){
        return Module.__lv_draw_mask_radius_circle_dsc_set_life(this.ptr, life)
    }
    usedCnt(){
        return Module.__lv_draw_mask_radius_circle_dsc_get_used_cnt(this.ptr)
    }
    setUsedCnt(used_cnt){
        return Module.__lv_draw_mask_radius_circle_dsc_set_used_cnt(this.ptr, used_cnt)
    }
    radius(){
        return Module.__lv_draw_mask_radius_circle_dsc_get_radius(this.ptr)
    }
    setRadius(radius){
        return Module.__lv_draw_mask_radius_circle_dsc_set_radius(this.ptr, radius)
    }
// AUTO GENERATE CODE END [_DrawMaskRadiusCircleDsc METHODS] --------
}

class DrawMaskRadiusParam extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_draw_mask_radius_param_create()
    }
// AUTO GENERATE CODE START [DrawMaskRadiusParam METHODS] --------
    dsc(){
        return Module._lv_draw_mask_radius_param_get_dsc(this.ptr)
    }
    setDsc(dsc){
        return Module._lv_draw_mask_radius_param_set_dsc(this.ptr, dsc)
    }
    circle(){
        return Module._lv_draw_mask_radius_param_get_circle(this.ptr)
    }
    setCircle(circle){
        return Module._lv_draw_mask_radius_param_set_circle(this.ptr, circle)
    }
// AUTO GENERATE CODE END [DrawMaskRadiusParam METHODS] --------
}

class DrawMaskFadeParam extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_draw_mask_fade_param_create()
    }
// AUTO GENERATE CODE START [DrawMaskFadeParam METHODS] --------
    dsc(){
        return Module._lv_draw_mask_fade_param_get_dsc(this.ptr)
    }
    setDsc(dsc){
        return Module._lv_draw_mask_fade_param_set_dsc(this.ptr, dsc)
    }
// AUTO GENERATE CODE END [DrawMaskFadeParam METHODS] --------
}

class DrawMaskMapParam extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_draw_mask_map_param_create()
    }
// AUTO GENERATE CODE START [DrawMaskMapParam METHODS] --------
    dsc(){
        return Module._lv_draw_mask_map_param_get_dsc(this.ptr)
    }
    setDsc(dsc){
        return Module._lv_draw_mask_map_param_set_dsc(this.ptr, dsc)
    }
// AUTO GENERATE CODE END [DrawMaskMapParam METHODS] --------
}

class DrawRectDsc extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_draw_rect_dsc_create()
    }
// AUTO GENERATE CODE START [DrawRectDsc METHODS] --------
    radius(){
        return Module._lv_draw_rect_dsc_get_radius(this.ptr)
    }
    setRadius(radius){
        return Module._lv_draw_rect_dsc_set_radius(this.ptr, radius)
    }
    blendMode(){
        return Module._lv_draw_rect_dsc_get_blend_mode(this.ptr)
    }
    setBlendMode(blend_mode){
        return Module._lv_draw_rect_dsc_set_blend_mode(this.ptr, blend_mode)
    }
    bgColor(){
        return Module._lv_draw_rect_dsc_get_bg_color(this.ptr)
    }
    setBgColor(bg_color){
        return Module._lv_draw_rect_dsc_set_bg_color(this.ptr, bg_color)
    }
    bgGradColor(){
        return Module._lv_draw_rect_dsc_get_bg_grad_color(this.ptr)
    }
    setBgGradColor(bg_grad_color){
        return Module._lv_draw_rect_dsc_set_bg_grad_color(this.ptr, bg_grad_color)
    }
    bgMainColorStop(){
        return Module._lv_draw_rect_dsc_get_bg_main_color_stop(this.ptr)
    }
    setBgMainColorStop(bg_main_color_stop){
        return Module._lv_draw_rect_dsc_set_bg_main_color_stop(this.ptr, bg_main_color_stop)
    }
    bgGradColorStop(){
        return Module._lv_draw_rect_dsc_get_bg_grad_color_stop(this.ptr)
    }
    setBgGradColorStop(bg_grad_color_stop){
        return Module._lv_draw_rect_dsc_set_bg_grad_color_stop(this.ptr, bg_grad_color_stop)
    }
    bgOpa(){
        return Module._lv_draw_rect_dsc_get_bg_opa(this.ptr)
    }
    setBgOpa(bg_opa){
        return Module._lv_draw_rect_dsc_set_bg_opa(this.ptr, bg_opa)
    }
    bgGradDir(){
        return Module._lv_draw_rect_dsc_get_bg_grad_dir(this.ptr)
    }
    setBgGradDir(bg_grad_dir){
        return Module._lv_draw_rect_dsc_set_bg_grad_dir(this.ptr, bg_grad_dir)
    }
    bgImgSrc(){
        return Module._lv_draw_rect_dsc_get_bg_img_src(this.ptr)
    }
    setBgImgSrc(bg_img_src){
        return Module._lv_draw_rect_dsc_set_bg_img_src(this.ptr, bg_img_src)
    }
    bgImgSymbolFont(){
        return Module._lv_draw_rect_dsc_get_bg_img_symbol_font(this.ptr)
    }
    setBgImgSymbolFont(bg_img_symbol_font){
        return Module._lv_draw_rect_dsc_set_bg_img_symbol_font(this.ptr, bg_img_symbol_font)
    }
    bgImgRecolor(){
        return Module._lv_draw_rect_dsc_get_bg_img_recolor(this.ptr)
    }
    setBgImgRecolor(bg_img_recolor){
        return Module._lv_draw_rect_dsc_set_bg_img_recolor(this.ptr, bg_img_recolor)
    }
    bgImgOpa(){
        return Module._lv_draw_rect_dsc_get_bg_img_opa(this.ptr)
    }
    setBgImgOpa(bg_img_opa){
        return Module._lv_draw_rect_dsc_set_bg_img_opa(this.ptr, bg_img_opa)
    }
    bgImgRecolorOpa(){
        return Module._lv_draw_rect_dsc_get_bg_img_recolor_opa(this.ptr)
    }
    setBgImgRecolorOpa(bg_img_recolor_opa){
        return Module._lv_draw_rect_dsc_set_bg_img_recolor_opa(this.ptr, bg_img_recolor_opa)
    }
    bgImgTiled(){
        return Module._lv_draw_rect_dsc_get_bg_img_tiled(this.ptr)
    }
    setBgImgTiled(bg_img_tiled){
        return Module._lv_draw_rect_dsc_set_bg_img_tiled(this.ptr, bg_img_tiled)
    }
    borderColor(){
        return Module._lv_draw_rect_dsc_get_border_color(this.ptr)
    }
    setBorderColor(border_color){
        return Module._lv_draw_rect_dsc_set_border_color(this.ptr, border_color)
    }
    borderWidth(){
        return Module._lv_draw_rect_dsc_get_border_width(this.ptr)
    }
    setBorderWidth(border_width){
        return Module._lv_draw_rect_dsc_set_border_width(this.ptr, border_width)
    }
    borderOpa(){
        return Module._lv_draw_rect_dsc_get_border_opa(this.ptr)
    }
    setBorderOpa(border_opa){
        return Module._lv_draw_rect_dsc_set_border_opa(this.ptr, border_opa)
    }
    borderPost(){
        return Module._lv_draw_rect_dsc_get_border_post(this.ptr)
    }
    setBorderPost(border_post){
        return Module._lv_draw_rect_dsc_set_border_post(this.ptr, border_post)
    }
    borderSide(){
        return Module._lv_draw_rect_dsc_get_border_side(this.ptr)
    }
    setBorderSide(border_side){
        return Module._lv_draw_rect_dsc_set_border_side(this.ptr, border_side)
    }
    outlineColor(){
        return Module._lv_draw_rect_dsc_get_outline_color(this.ptr)
    }
    setOutlineColor(outline_color){
        return Module._lv_draw_rect_dsc_set_outline_color(this.ptr, outline_color)
    }
    outlineWidth(){
        return Module._lv_draw_rect_dsc_get_outline_width(this.ptr)
    }
    setOutlineWidth(outline_width){
        return Module._lv_draw_rect_dsc_set_outline_width(this.ptr, outline_width)
    }
    outlinePad(){
        return Module._lv_draw_rect_dsc_get_outline_pad(this.ptr)
    }
    setOutlinePad(outline_pad){
        return Module._lv_draw_rect_dsc_set_outline_pad(this.ptr, outline_pad)
    }
    outlineOpa(){
        return Module._lv_draw_rect_dsc_get_outline_opa(this.ptr)
    }
    setOutlineOpa(outline_opa){
        return Module._lv_draw_rect_dsc_set_outline_opa(this.ptr, outline_opa)
    }
    shadowColor(){
        return Module._lv_draw_rect_dsc_get_shadow_color(this.ptr)
    }
    setShadowColor(shadow_color){
        return Module._lv_draw_rect_dsc_set_shadow_color(this.ptr, shadow_color)
    }
    shadowWidth(){
        return Module._lv_draw_rect_dsc_get_shadow_width(this.ptr)
    }
    setShadowWidth(shadow_width){
        return Module._lv_draw_rect_dsc_set_shadow_width(this.ptr, shadow_width)
    }
    shadowOfsX(){
        return Module._lv_draw_rect_dsc_get_shadow_ofs_x(this.ptr)
    }
    setShadowOfsX(shadow_ofs_x){
        return Module._lv_draw_rect_dsc_set_shadow_ofs_x(this.ptr, shadow_ofs_x)
    }
    shadowOfsY(){
        return Module._lv_draw_rect_dsc_get_shadow_ofs_y(this.ptr)
    }
    setShadowOfsY(shadow_ofs_y){
        return Module._lv_draw_rect_dsc_set_shadow_ofs_y(this.ptr, shadow_ofs_y)
    }
    shadowSpread(){
        return Module._lv_draw_rect_dsc_get_shadow_spread(this.ptr)
    }
    setShadowSpread(shadow_spread){
        return Module._lv_draw_rect_dsc_set_shadow_spread(this.ptr, shadow_spread)
    }
    shadowOpa(){
        return Module._lv_draw_rect_dsc_get_shadow_opa(this.ptr)
    }
    setShadowOpa(shadow_opa){
        return Module._lv_draw_rect_dsc_set_shadow_opa(this.ptr, shadow_opa)
    }
// AUTO GENERATE CODE END [DrawRectDsc METHODS] --------
}

class DrawLabelDsc extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_draw_label_dsc_create()
    }
// AUTO GENERATE CODE START [DrawLabelDsc METHODS] --------
    font(){
        return Module._lv_draw_label_dsc_get_font(this.ptr)
    }
    setFont(font){
        return Module._lv_draw_label_dsc_set_font(this.ptr, font)
    }
    selStart(){
        return Module._lv_draw_label_dsc_get_sel_start(this.ptr)
    }
    setSelStart(sel_start){
        return Module._lv_draw_label_dsc_set_sel_start(this.ptr, sel_start)
    }
    selEnd(){
        return Module._lv_draw_label_dsc_get_sel_end(this.ptr)
    }
    setSelEnd(sel_end){
        return Module._lv_draw_label_dsc_set_sel_end(this.ptr, sel_end)
    }
    color(){
        return Module._lv_draw_label_dsc_get_color(this.ptr)
    }
    setColor(color){
        return Module._lv_draw_label_dsc_set_color(this.ptr, color)
    }
    selColor(){
        return Module._lv_draw_label_dsc_get_sel_color(this.ptr)
    }
    setSelColor(sel_color){
        return Module._lv_draw_label_dsc_set_sel_color(this.ptr, sel_color)
    }
    selBgColor(){
        return Module._lv_draw_label_dsc_get_sel_bg_color(this.ptr)
    }
    setSelBgColor(sel_bg_color){
        return Module._lv_draw_label_dsc_set_sel_bg_color(this.ptr, sel_bg_color)
    }
    lineSpace(){
        return Module._lv_draw_label_dsc_get_line_space(this.ptr)
    }
    setLineSpace(line_space){
        return Module._lv_draw_label_dsc_set_line_space(this.ptr, line_space)
    }
    letterSpace(){
        return Module._lv_draw_label_dsc_get_letter_space(this.ptr)
    }
    setLetterSpace(letter_space){
        return Module._lv_draw_label_dsc_set_letter_space(this.ptr, letter_space)
    }
    ofsX(){
        return Module._lv_draw_label_dsc_get_ofs_x(this.ptr)
    }
    setOfsX(ofs_x){
        return Module._lv_draw_label_dsc_set_ofs_x(this.ptr, ofs_x)
    }
    ofsY(){
        return Module._lv_draw_label_dsc_get_ofs_y(this.ptr)
    }
    setOfsY(ofs_y){
        return Module._lv_draw_label_dsc_set_ofs_y(this.ptr, ofs_y)
    }
    opa(){
        return Module._lv_draw_label_dsc_get_opa(this.ptr)
    }
    setOpa(opa){
        return Module._lv_draw_label_dsc_set_opa(this.ptr, opa)
    }
    bidiDir(){
        return Module._lv_draw_label_dsc_get_bidi_dir(this.ptr)
    }
    setBidiDir(bidi_dir){
        return Module._lv_draw_label_dsc_set_bidi_dir(this.ptr, bidi_dir)
    }
    align(){
        return Module._lv_draw_label_dsc_get_align(this.ptr)
    }
    setAlign(align){
        return Module._lv_draw_label_dsc_set_align(this.ptr, align)
    }
    flag(){
        return Module._lv_draw_label_dsc_get_flag(this.ptr)
    }
    setFlag(flag){
        return Module._lv_draw_label_dsc_set_flag(this.ptr, flag)
    }
    decor(){
        return Module._lv_draw_label_dsc_get_decor(this.ptr)
    }
    setDecor(decor){
        return Module._lv_draw_label_dsc_set_decor(this.ptr, decor)
    }
    blendMode(){
        return Module._lv_draw_label_dsc_get_blend_mode(this.ptr)
    }
    setBlendMode(blend_mode){
        return Module._lv_draw_label_dsc_set_blend_mode(this.ptr, blend_mode)
    }
// AUTO GENERATE CODE END [DrawLabelDsc METHODS] --------
}

class DrawLabelHint extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_draw_label_hint_create()
    }
// AUTO GENERATE CODE START [DrawLabelHint METHODS] --------
    lineStart(){
        return Module._lv_draw_label_hint_get_line_start(this.ptr)
    }
    setLineStart(line_start){
        return Module._lv_draw_label_hint_set_line_start(this.ptr, line_start)
    }
    y(){
        return Module._lv_draw_label_hint_get_y(this.ptr)
    }
    setY(y){
        return Module._lv_draw_label_hint_set_y(this.ptr, y)
    }
    coordY(){
        return Module._lv_draw_label_hint_get_coord_y(this.ptr)
    }
    setCoordY(coord_y){
        return Module._lv_draw_label_hint_set_coord_y(this.ptr, coord_y)
    }
// AUTO GENERATE CODE END [DrawLabelHint METHODS] --------
}

class DrawImgDsc extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_draw_img_dsc_create()
    }
// AUTO GENERATE CODE START [DrawImgDsc METHODS] --------
    angle(){
        return Module._lv_draw_img_dsc_get_angle(this.ptr)
    }
    setAngle(angle){
        return Module._lv_draw_img_dsc_set_angle(this.ptr, angle)
    }
    zoom(){
        return Module._lv_draw_img_dsc_get_zoom(this.ptr)
    }
    setZoom(zoom){
        return Module._lv_draw_img_dsc_set_zoom(this.ptr, zoom)
    }
    pivot(){
        return Module._lv_draw_img_dsc_get_pivot(this.ptr)
    }
    setPivot(pivot){
        return Module._lv_draw_img_dsc_set_pivot(this.ptr, pivot)
    }
    recolor(){
        return Module._lv_draw_img_dsc_get_recolor(this.ptr)
    }
    setRecolor(recolor){
        return Module._lv_draw_img_dsc_set_recolor(this.ptr, recolor)
    }
    recolorOpa(){
        return Module._lv_draw_img_dsc_get_recolor_opa(this.ptr)
    }
    setRecolorOpa(recolor_opa){
        return Module._lv_draw_img_dsc_set_recolor_opa(this.ptr, recolor_opa)
    }
    opa(){
        return Module._lv_draw_img_dsc_get_opa(this.ptr)
    }
    setOpa(opa){
        return Module._lv_draw_img_dsc_set_opa(this.ptr, opa)
    }
    blendMode(){
        return Module._lv_draw_img_dsc_get_blend_mode(this.ptr)
    }
    setBlendMode(blend_mode){
        return Module._lv_draw_img_dsc_set_blend_mode(this.ptr, blend_mode)
    }
    frameId(){
        return Module._lv_draw_img_dsc_get_frame_id(this.ptr)
    }
    setFrameId(frame_id){
        return Module._lv_draw_img_dsc_set_frame_id(this.ptr, frame_id)
    }
    antialias(){
        return Module._lv_draw_img_dsc_get_antialias(this.ptr)
    }
    setAntialias(antialias){
        return Module._lv_draw_img_dsc_set_antialias(this.ptr, antialias)
    }
// AUTO GENERATE CODE END [DrawImgDsc METHODS] --------
}

class DrawLineDsc extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_draw_line_dsc_create()
    }
// AUTO GENERATE CODE START [DrawLineDsc METHODS] --------
    color(){
        return Module._lv_draw_line_dsc_get_color(this.ptr)
    }
    setColor(color){
        return Module._lv_draw_line_dsc_set_color(this.ptr, color)
    }
    width(){
        return Module._lv_draw_line_dsc_get_width(this.ptr)
    }
    setWidth(width){
        return Module._lv_draw_line_dsc_set_width(this.ptr, width)
    }
    dashWidth(){
        return Module._lv_draw_line_dsc_get_dash_width(this.ptr)
    }
    setDashWidth(dash_width){
        return Module._lv_draw_line_dsc_set_dash_width(this.ptr, dash_width)
    }
    dashGap(){
        return Module._lv_draw_line_dsc_get_dash_gap(this.ptr)
    }
    setDashGap(dash_gap){
        return Module._lv_draw_line_dsc_set_dash_gap(this.ptr, dash_gap)
    }
    opa(){
        return Module._lv_draw_line_dsc_get_opa(this.ptr)
    }
    setOpa(opa){
        return Module._lv_draw_line_dsc_set_opa(this.ptr, opa)
    }
    blendMode(){
        return Module._lv_draw_line_dsc_get_blend_mode(this.ptr)
    }
    setBlendMode(blend_mode){
        return Module._lv_draw_line_dsc_set_blend_mode(this.ptr, blend_mode)
    }
    roundStart(){
        return Module._lv_draw_line_dsc_get_round_start(this.ptr)
    }
    setRoundStart(round_start){
        return Module._lv_draw_line_dsc_set_round_start(this.ptr, round_start)
    }
    roundEnd(){
        return Module._lv_draw_line_dsc_get_round_end(this.ptr)
    }
    setRoundEnd(round_end){
        return Module._lv_draw_line_dsc_set_round_end(this.ptr, round_end)
    }
    rawEnd(){
        return Module._lv_draw_line_dsc_get_raw_end(this.ptr)
    }
    setRawEnd(raw_end){
        return Module._lv_draw_line_dsc_set_raw_end(this.ptr, raw_end)
    }
// AUTO GENERATE CODE END [DrawLineDsc METHODS] --------
}

class DrawArcDsc extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_draw_arc_dsc_create()
    }
// AUTO GENERATE CODE START [DrawArcDsc METHODS] --------
    color(){
        return Module._lv_draw_arc_dsc_get_color(this.ptr)
    }
    setColor(color){
        return Module._lv_draw_arc_dsc_set_color(this.ptr, color)
    }
    width(){
        return Module._lv_draw_arc_dsc_get_width(this.ptr)
    }
    setWidth(width){
        return Module._lv_draw_arc_dsc_set_width(this.ptr, width)
    }
    imgSrc(){
        return Module._lv_draw_arc_dsc_get_img_src(this.ptr)
    }
    setImgSrc(img_src){
        return Module._lv_draw_arc_dsc_set_img_src(this.ptr, img_src)
    }
    opa(){
        return Module._lv_draw_arc_dsc_get_opa(this.ptr)
    }
    setOpa(opa){
        return Module._lv_draw_arc_dsc_set_opa(this.ptr, opa)
    }
    blendMode(){
        return Module._lv_draw_arc_dsc_get_blend_mode(this.ptr)
    }
    setBlendMode(blend_mode){
        return Module._lv_draw_arc_dsc_set_blend_mode(this.ptr, blend_mode)
    }
    rounded(){
        return Module._lv_draw_arc_dsc_get_rounded(this.ptr)
    }
    setRounded(rounded){
        return Module._lv_draw_arc_dsc_set_rounded(this.ptr, rounded)
    }
// AUTO GENERATE CODE END [DrawArcDsc METHODS] --------
}

class ObjDrawPartDsc extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_obj_draw_part_dsc_create()
    }
// AUTO GENERATE CODE START [ObjDrawPartDsc METHODS] --------
    clipArea(){
        return Module._lv_obj_draw_part_dsc_get_clip_area(this.ptr)
    }
    setClipArea(clip_area){
        return Module._lv_obj_draw_part_dsc_set_clip_area(this.ptr, clip_area)
    }
    classP(){
        return Module._lv_obj_draw_part_dsc_get_class_p(this.ptr)
    }
    setClassP(class_p){
        return Module._lv_obj_draw_part_dsc_set_class_p(this.ptr, class_p)
    }
    type(){
        return Module._lv_obj_draw_part_dsc_get_type(this.ptr)
    }
    setType(type){
        return Module._lv_obj_draw_part_dsc_set_type(this.ptr, type)
    }
    drawArea(){
        return Module._lv_obj_draw_part_dsc_get_draw_area(this.ptr)
    }
    setDrawArea(draw_area){
        return Module._lv_obj_draw_part_dsc_set_draw_area(this.ptr, draw_area)
    }
    rectDsc(){
        return Module._lv_obj_draw_part_dsc_get_rect_dsc(this.ptr)
    }
    setRectDsc(rect_dsc){
        return Module._lv_obj_draw_part_dsc_set_rect_dsc(this.ptr, rect_dsc)
    }
    labelDsc(){
        return Module._lv_obj_draw_part_dsc_get_label_dsc(this.ptr)
    }
    setLabelDsc(label_dsc){
        return Module._lv_obj_draw_part_dsc_set_label_dsc(this.ptr, label_dsc)
    }
    lineDsc(){
        return Module._lv_obj_draw_part_dsc_get_line_dsc(this.ptr)
    }
    setLineDsc(line_dsc){
        return Module._lv_obj_draw_part_dsc_set_line_dsc(this.ptr, line_dsc)
    }
    imgDsc(){
        return Module._lv_obj_draw_part_dsc_get_img_dsc(this.ptr)
    }
    setImgDsc(img_dsc){
        return Module._lv_obj_draw_part_dsc_set_img_dsc(this.ptr, img_dsc)
    }
    arcDsc(){
        return Module._lv_obj_draw_part_dsc_get_arc_dsc(this.ptr)
    }
    setArcDsc(arc_dsc){
        return Module._lv_obj_draw_part_dsc_set_arc_dsc(this.ptr, arc_dsc)
    }
    p1(){
        return Module._lv_obj_draw_part_dsc_get_p1(this.ptr)
    }
    setP1(p1){
        return Module._lv_obj_draw_part_dsc_set_p1(this.ptr, p1)
    }
    p2(){
        return Module._lv_obj_draw_part_dsc_get_p2(this.ptr)
    }
    setP2(p2){
        return Module._lv_obj_draw_part_dsc_set_p2(this.ptr, p2)
    }
    text(){
        return Module._lv_obj_draw_part_dsc_get_text(this.ptr)
    }
    setText(text){
        return Module._lv_obj_draw_part_dsc_set_text(this.ptr, text)
    }
    textLength(){
        return Module._lv_obj_draw_part_dsc_get_text_length(this.ptr)
    }
    setTextLength(text_length){
        return Module._lv_obj_draw_part_dsc_set_text_length(this.ptr, text_length)
    }
    part(){
        return Module._lv_obj_draw_part_dsc_get_part(this.ptr)
    }
    setPart(part){
        return Module._lv_obj_draw_part_dsc_set_part(this.ptr, part)
    }
    id(){
        return Module._lv_obj_draw_part_dsc_get_id(this.ptr)
    }
    setId(id){
        return Module._lv_obj_draw_part_dsc_set_id(this.ptr, id)
    }
    radius(){
        return Module._lv_obj_draw_part_dsc_get_radius(this.ptr)
    }
    setRadius(radius){
        return Module._lv_obj_draw_part_dsc_set_radius(this.ptr, radius)
    }
    value(){
        return Module._lv_obj_draw_part_dsc_get_value(this.ptr)
    }
    setValue(value){
        return Module._lv_obj_draw_part_dsc_set_value(this.ptr, value)
    }
    subPartPtr(){
        return Module._lv_obj_draw_part_dsc_get_sub_part_ptr(this.ptr)
    }
    setSubPartPtr(sub_part_ptr){
        return Module._lv_obj_draw_part_dsc_set_sub_part_ptr(this.ptr, sub_part_ptr)
    }
// AUTO GENERATE CODE END [ObjDrawPartDsc METHODS] --------
}

class ObjClass extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_obj_class_create()
    }
// AUTO GENERATE CODE START [ObjClass METHODS] --------
    baseClass(){
        return Module._lv_obj_class_get_base_class(this.ptr)
    }
    setBaseClass(base_class){
        return Module._lv_obj_class_set_base_class(this.ptr, base_class)
    }
    constructorCb(){
        return Module._lv_obj_class_get_constructor_cb(this.ptr)
    }
    setConstructorCb(constructor_cb){
        return Module._lv_obj_class_set_constructor_cb(this.ptr, constructor_cb)
    }
    destructorCb(){
        return Module._lv_obj_class_get_destructor_cb(this.ptr)
    }
    setDestructorCb(destructor_cb){
        return Module._lv_obj_class_set_destructor_cb(this.ptr, destructor_cb)
    }
    userData(){
        return Module._lv_obj_class_get_user_data(this.ptr)
    }
    setUserData(user_data){
        return Module._lv_obj_class_set_user_data(this.ptr, user_data)
    }
    eventCb(){
        return Module._lv_obj_class_get_event_cb(this.ptr)
    }
    setEventCb(event_cb){
        return Module._lv_obj_class_set_event_cb(this.ptr, event_cb)
    }
    widthDef(){
        return Module._lv_obj_class_get_width_def(this.ptr)
    }
    setWidthDef(width_def){
        return Module._lv_obj_class_set_width_def(this.ptr, width_def)
    }
    heightDef(){
        return Module._lv_obj_class_get_height_def(this.ptr)
    }
    setHeightDef(height_def){
        return Module._lv_obj_class_set_height_def(this.ptr, height_def)
    }
    editable(){
        return Module._lv_obj_class_get_editable(this.ptr)
    }
    setEditable(editable){
        return Module._lv_obj_class_set_editable(this.ptr, editable)
    }
    groupDef(){
        return Module._lv_obj_class_get_group_def(this.ptr)
    }
    setGroupDef(group_def){
        return Module._lv_obj_class_set_group_def(this.ptr, group_def)
    }
    instanceSize(){
        return Module._lv_obj_class_get_instance_size(this.ptr)
    }
    setInstanceSize(instance_size){
        return Module._lv_obj_class_set_instance_size(this.ptr, instance_size)
    }
// AUTO GENERATE CODE END [ObjClass METHODS] --------
}

class Event extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_event_create()
    }
// AUTO GENERATE CODE START [Event METHODS] --------
    setTarget(target){
        return Module._lv_event_set_target(this.ptr, target)
    }
    setCurrentTarget(current_target){
        return Module._lv_event_set_current_target(this.ptr, current_target)
    }
    setCode(code){
        return Module._lv_event_set_code(this.ptr, code)
    }
    setUserData(user_data){
        return Module._lv_event_set_user_data(this.ptr, user_data)
    }
    setParam(param){
        return Module._lv_event_set_param(this.ptr, param)
    }
    prev(){
        return Module._lv_event_get_prev(this.ptr)
    }
    setPrev(prev){
        return Module._lv_event_set_prev(this.ptr, prev)
    }
    deleted(){
        return Module._lv_event_get_deleted(this.ptr)
    }
    setDeleted(deleted){
        return Module._lv_event_set_deleted(this.ptr, deleted)
    }
// AUTO GENERATE CODE END [Event METHODS] --------
}

class HitTestInfo extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_hit_test_info_create()
    }
// AUTO GENERATE CODE START [HitTestInfo METHODS] --------
    point(){
        return Module._lv_hit_test_info_get_point(this.ptr)
    }
    setPoint(point){
        return Module._lv_hit_test_info_set_point(this.ptr, point)
    }
    res(){
        return Module._lv_hit_test_info_get_res(this.ptr)
    }
    setRes(res){
        return Module._lv_hit_test_info_set_res(this.ptr, res)
    }
// AUTO GENERATE CODE END [HitTestInfo METHODS] --------
}

class Group extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_group_create()
    }
// AUTO GENERATE CODE START [Group METHODS] --------
    objLl(){
        return Module._lv_group_get_obj_ll(this.ptr)
    }
    setObjLl(obj_ll){
        return Module._lv_group_set_obj_ll(this.ptr, obj_ll)
    }
    objFocus(){
        return Module._lv_group_get_obj_focus(this.ptr)
    }
    setObjFocus(obj_focus){
        return Module._lv_group_set_obj_focus(this.ptr, obj_focus)
    }
    userData(){
        return Module._lv_group_get_user_data(this.ptr)
    }
    setUserData(user_data){
        return Module._lv_group_set_user_data(this.ptr, user_data)
    }
    frozen(){
        return Module._lv_group_get_frozen(this.ptr)
    }
    setFrozen(frozen){
        return Module._lv_group_set_frozen(this.ptr, frozen)
    }
    refocusPolicy(){
        return Module._lv_group_get_refocus_policy(this.ptr)
    }
// AUTO GENERATE CODE END [Group METHODS] --------
}

class _ObjSpecAttr extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv__obj_spec_attr_create()
    }
// AUTO GENERATE CODE START [_ObjSpecAttr METHODS] --------
    children(){
        return Module.__lv_obj_spec_attr_get_children(this.ptr)
    }
    setChildren(children){
        return Module.__lv_obj_spec_attr_set_children(this.ptr, children)
    }
    childCnt(){
        return Module.__lv_obj_spec_attr_get_child_cnt(this.ptr)
    }
    setChildCnt(child_cnt){
        return Module.__lv_obj_spec_attr_set_child_cnt(this.ptr, child_cnt)
    }
    groupP(){
        return Module.__lv_obj_spec_attr_get_group_p(this.ptr)
    }
    setGroupP(group_p){
        return Module.__lv_obj_spec_attr_set_group_p(this.ptr, group_p)
    }
    eventDsc(){
        return Module.__lv_obj_spec_attr_get_event_dsc(this.ptr)
    }
    setEventDsc(event_dsc){
        return Module.__lv_obj_spec_attr_set_event_dsc(this.ptr, event_dsc)
    }
    scroll(){
        return Module.__lv_obj_spec_attr_get_scroll(this.ptr)
    }
    setScroll(scroll){
        return Module.__lv_obj_spec_attr_set_scroll(this.ptr, scroll)
    }
    extClickPad(){
        return Module.__lv_obj_spec_attr_get_ext_click_pad(this.ptr)
    }
    setExtClickPad(ext_click_pad){
        return Module.__lv_obj_spec_attr_set_ext_click_pad(this.ptr, ext_click_pad)
    }
    extDrawSize(){
        return Module.__lv_obj_spec_attr_get_ext_draw_size(this.ptr)
    }
    setExtDrawSize(ext_draw_size){
        return Module.__lv_obj_spec_attr_set_ext_draw_size(this.ptr, ext_draw_size)
    }
    scrollbarMode(){
        return Module.__lv_obj_spec_attr_get_scrollbar_mode(this.ptr)
    }
    setScrollbarMode(scrollbar_mode){
        return Module.__lv_obj_spec_attr_set_scrollbar_mode(this.ptr, scrollbar_mode)
    }
    scrollSnapX(){
        return Module.__lv_obj_spec_attr_get_scroll_snap_x(this.ptr)
    }
    setScrollSnapX(scroll_snap_x){
        return Module.__lv_obj_spec_attr_set_scroll_snap_x(this.ptr, scroll_snap_x)
    }
    scrollSnapY(){
        return Module.__lv_obj_spec_attr_get_scroll_snap_y(this.ptr)
    }
    setScrollSnapY(scroll_snap_y){
        return Module.__lv_obj_spec_attr_set_scroll_snap_y(this.ptr, scroll_snap_y)
    }
    scrollDir(){
        return Module.__lv_obj_spec_attr_get_scroll_dir(this.ptr)
    }
    setScrollDir(scroll_dir){
        return Module.__lv_obj_spec_attr_set_scroll_dir(this.ptr, scroll_dir)
    }
    eventDscCnt(){
        return Module.__lv_obj_spec_attr_get_event_dsc_cnt(this.ptr)
    }
    setEventDscCnt(event_dsc_cnt){
        return Module.__lv_obj_spec_attr_set_event_dsc_cnt(this.ptr, event_dsc_cnt)
    }
// AUTO GENERATE CODE END [_ObjSpecAttr METHODS] --------
}

class Theme extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_theme_create()
    }
// AUTO GENERATE CODE START [Theme METHODS] --------
    applyCb(){
        return Module._lv_theme_get_apply_cb(this.ptr)
    }
    parent(){
        return Module._lv_theme_get_parent(this.ptr)
    }
    userData(){
        return Module._lv_theme_get_user_data(this.ptr)
    }
    setUserData(user_data){
        return Module._lv_theme_set_user_data(this.ptr, user_data)
    }
    disp(){
        return Module._lv_theme_get_disp(this.ptr)
    }
    setDisp(disp){
        return Module._lv_theme_set_disp(this.ptr, disp)
    }
    setColorPrimary(color_primary){
        return Module._lv_theme_set_color_primary(this.ptr, color_primary)
    }
    setColorSecondary(color_secondary){
        return Module._lv_theme_set_color_secondary(this.ptr, color_secondary)
    }
    setFontSmall(font_small){
        return Module._lv_theme_set_font_small(this.ptr, font_small)
    }
    setFontNormal(font_normal){
        return Module._lv_theme_set_font_normal(this.ptr, font_normal)
    }
    setFontLarge(font_large){
        return Module._lv_theme_set_font_large(this.ptr, font_large)
    }
    flags(){
        return Module._lv_theme_get_flags(this.ptr)
    }
    setFlags(flags){
        return Module._lv_theme_set_flags(this.ptr, flags)
    }
// AUTO GENERATE CODE END [Theme METHODS] --------
}

class FontFmtTxtGlyphDsc extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_font_fmt_txt_glyph_dsc_create()
    }
// AUTO GENERATE CODE START [FontFmtTxtGlyphDsc METHODS] --------
    bitmapIndex(){
        return Module._lv_font_fmt_txt_glyph_dsc_get_bitmap_index(this.ptr)
    }
    setBitmapIndex(bitmap_index){
        return Module._lv_font_fmt_txt_glyph_dsc_set_bitmap_index(this.ptr, bitmap_index)
    }
    advW(){
        return Module._lv_font_fmt_txt_glyph_dsc_get_adv_w(this.ptr)
    }
    setAdvW(adv_w){
        return Module._lv_font_fmt_txt_glyph_dsc_set_adv_w(this.ptr, adv_w)
    }
    boxW(){
        return Module._lv_font_fmt_txt_glyph_dsc_get_box_w(this.ptr)
    }
    setBoxW(box_w){
        return Module._lv_font_fmt_txt_glyph_dsc_set_box_w(this.ptr, box_w)
    }
    boxH(){
        return Module._lv_font_fmt_txt_glyph_dsc_get_box_h(this.ptr)
    }
    setBoxH(box_h){
        return Module._lv_font_fmt_txt_glyph_dsc_set_box_h(this.ptr, box_h)
    }
    ofsX(){
        return Module._lv_font_fmt_txt_glyph_dsc_get_ofs_x(this.ptr)
    }
    setOfsX(ofs_x){
        return Module._lv_font_fmt_txt_glyph_dsc_set_ofs_x(this.ptr, ofs_x)
    }
    ofsY(){
        return Module._lv_font_fmt_txt_glyph_dsc_get_ofs_y(this.ptr)
    }
    setOfsY(ofs_y){
        return Module._lv_font_fmt_txt_glyph_dsc_set_ofs_y(this.ptr, ofs_y)
    }
// AUTO GENERATE CODE END [FontFmtTxtGlyphDsc METHODS] --------
}

class FontFmtTxtCmap extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_font_fmt_txt_cmap_create()
    }
// AUTO GENERATE CODE START [FontFmtTxtCmap METHODS] --------
    rangeStart(){
        return Module._lv_font_fmt_txt_cmap_get_range_start(this.ptr)
    }
    setRangeStart(range_start){
        return Module._lv_font_fmt_txt_cmap_set_range_start(this.ptr, range_start)
    }
    rangeLength(){
        return Module._lv_font_fmt_txt_cmap_get_range_length(this.ptr)
    }
    setRangeLength(range_length){
        return Module._lv_font_fmt_txt_cmap_set_range_length(this.ptr, range_length)
    }
    glyphIdStart(){
        return Module._lv_font_fmt_txt_cmap_get_glyph_id_start(this.ptr)
    }
    setGlyphIdStart(glyph_id_start){
        return Module._lv_font_fmt_txt_cmap_set_glyph_id_start(this.ptr, glyph_id_start)
    }
    unicodeList(){
        return Module._lv_font_fmt_txt_cmap_get_unicode_list(this.ptr)
    }
    setUnicodeList(unicode_list){
        return Module._lv_font_fmt_txt_cmap_set_unicode_list(this.ptr, unicode_list)
    }
    glyphIdOfsList(){
        return Module._lv_font_fmt_txt_cmap_get_glyph_id_ofs_list(this.ptr)
    }
    setGlyphIdOfsList(glyph_id_ofs_list){
        return Module._lv_font_fmt_txt_cmap_set_glyph_id_ofs_list(this.ptr, glyph_id_ofs_list)
    }
    listLength(){
        return Module._lv_font_fmt_txt_cmap_get_list_length(this.ptr)
    }
    setListLength(list_length){
        return Module._lv_font_fmt_txt_cmap_set_list_length(this.ptr, list_length)
    }
    type(){
        return Module._lv_font_fmt_txt_cmap_get_type(this.ptr)
    }
    setType(type){
        return Module._lv_font_fmt_txt_cmap_set_type(this.ptr, type)
    }
// AUTO GENERATE CODE END [FontFmtTxtCmap METHODS] --------
}

class FontFmtTxtGlyphCache extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_font_fmt_txt_glyph_cache_create()
    }
// AUTO GENERATE CODE START [FontFmtTxtGlyphCache METHODS] --------
    lastLetter(){
        return Module._lv_font_fmt_txt_glyph_cache_get_last_letter(this.ptr)
    }
    setLastLetter(last_letter){
        return Module._lv_font_fmt_txt_glyph_cache_set_last_letter(this.ptr, last_letter)
    }
    lastGlyphId(){
        return Module._lv_font_fmt_txt_glyph_cache_get_last_glyph_id(this.ptr)
    }
    setLastGlyphId(last_glyph_id){
        return Module._lv_font_fmt_txt_glyph_cache_set_last_glyph_id(this.ptr, last_glyph_id)
    }
// AUTO GENERATE CODE END [FontFmtTxtGlyphCache METHODS] --------
}

class _BarAnim extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv__bar_anim_create()
    }
// AUTO GENERATE CODE START [_BarAnim METHODS] --------
    bar(){
        return Module.__lv_bar_anim_get_bar(this.ptr)
    }
    setBar(bar){
        return Module.__lv_bar_anim_set_bar(this.ptr, bar)
    }
    animStart(){
        return Module.__lv_bar_anim_get_anim_start(this.ptr)
    }
    setAnimStart(anim_start){
        return Module.__lv_bar_anim_set_anim_start(this.ptr, anim_start)
    }
    animEnd(){
        return Module.__lv_bar_anim_get_anim_end(this.ptr)
    }
    setAnimEnd(anim_end){
        return Module.__lv_bar_anim_set_anim_end(this.ptr, anim_end)
    }
    animState(){
        return Module.__lv_bar_anim_get_anim_state(this.ptr)
    }
    setAnimState(anim_state){
        return Module.__lv_bar_anim_set_anim_state(this.ptr, anim_state)
    }
// AUTO GENERATE CODE END [_BarAnim METHODS] --------
}

class CalendarDate extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_calendar_date_create()
    }
// AUTO GENERATE CODE START [CalendarDate METHODS] --------
    year(){
        return Module._lv_calendar_date_get_year(this.ptr)
    }
    setYear(year){
        return Module._lv_calendar_date_set_year(this.ptr, year)
    }
    month(){
        return Module._lv_calendar_date_get_month(this.ptr)
    }
    setMonth(month){
        return Module._lv_calendar_date_set_month(this.ptr, month)
    }
    day(){
        return Module._lv_calendar_date_get_day(this.ptr)
    }
    setDay(day){
        return Module._lv_calendar_date_set_day(this.ptr, day)
    }
// AUTO GENERATE CODE END [CalendarDate METHODS] --------
}

class ChartSeries extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_chart_series_create()
    }
// AUTO GENERATE CODE START [ChartSeries METHODS] --------
    xPoints(){
        return Module._lv_chart_series_get_x_points(this.ptr)
    }
    setXPoints(x_points){
        return Module._lv_chart_series_set_x_points(this.ptr, x_points)
    }
    yPoints(){
        return Module._lv_chart_series_get_y_points(this.ptr)
    }
    setYPoints(y_points){
        return Module._lv_chart_series_set_y_points(this.ptr, y_points)
    }
    color(){
        return Module._lv_chart_series_get_color(this.ptr)
    }
    setColor(color){
        return Module._lv_chart_series_set_color(this.ptr, color)
    }
    startPoint(){
        return Module._lv_chart_series_get_start_point(this.ptr)
    }
    setStartPoint(start_point){
        return Module._lv_chart_series_set_start_point(this.ptr, start_point)
    }
    hidden(){
        return Module._lv_chart_series_get_hidden(this.ptr)
    }
    setHidden(hidden){
        return Module._lv_chart_series_set_hidden(this.ptr, hidden)
    }
    xExtBufAssigned(){
        return Module._lv_chart_series_get_x_ext_buf_assigned(this.ptr)
    }
    setXExtBufAssigned(x_ext_buf_assigned){
        return Module._lv_chart_series_set_x_ext_buf_assigned(this.ptr, x_ext_buf_assigned)
    }
    yExtBufAssigned(){
        return Module._lv_chart_series_get_y_ext_buf_assigned(this.ptr)
    }
    setYExtBufAssigned(y_ext_buf_assigned){
        return Module._lv_chart_series_set_y_ext_buf_assigned(this.ptr, y_ext_buf_assigned)
    }
    xAxisSec(){
        return Module._lv_chart_series_get_x_axis_sec(this.ptr)
    }
    setXAxisSec(x_axis_sec){
        return Module._lv_chart_series_set_x_axis_sec(this.ptr, x_axis_sec)
    }
    yAxisSec(){
        return Module._lv_chart_series_get_y_axis_sec(this.ptr)
    }
    setYAxisSec(y_axis_sec){
        return Module._lv_chart_series_set_y_axis_sec(this.ptr, y_axis_sec)
    }
// AUTO GENERATE CODE END [ChartSeries METHODS] --------
}

class ChartCursor extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_chart_cursor_create()
    }
// AUTO GENERATE CODE START [ChartCursor METHODS] --------
    pos(){
        return Module._lv_chart_cursor_get_pos(this.ptr)
    }
    setPos(pos){
        return Module._lv_chart_cursor_set_pos(this.ptr, pos)
    }
    pointId(){
        return Module._lv_chart_cursor_get_point_id(this.ptr)
    }
    setPointId(point_id){
        return Module._lv_chart_cursor_set_point_id(this.ptr, point_id)
    }
    color(){
        return Module._lv_chart_cursor_get_color(this.ptr)
    }
    setColor(color){
        return Module._lv_chart_cursor_set_color(this.ptr, color)
    }
    ser(){
        return Module._lv_chart_cursor_get_ser(this.ptr)
    }
    setSer(ser){
        return Module._lv_chart_cursor_set_ser(this.ptr, ser)
    }
    dir(){
        return Module._lv_chart_cursor_get_dir(this.ptr)
    }
    setDir(dir){
        return Module._lv_chart_cursor_set_dir(this.ptr, dir)
    }
    posSet(){
        return Module._lv_chart_cursor_get_pos_set(this.ptr)
    }
    setPosSet(pos_set){
        return Module._lv_chart_cursor_set_pos_set(this.ptr, pos_set)
    }
// AUTO GENERATE CODE END [ChartCursor METHODS] --------
}

class ChartTickDsc extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_chart_tick_dsc_create()
    }
// AUTO GENERATE CODE START [ChartTickDsc METHODS] --------
    majorLen(){
        return Module._lv_chart_tick_dsc_get_major_len(this.ptr)
    }
    setMajorLen(major_len){
        return Module._lv_chart_tick_dsc_set_major_len(this.ptr, major_len)
    }
    minorLen(){
        return Module._lv_chart_tick_dsc_get_minor_len(this.ptr)
    }
    setMinorLen(minor_len){
        return Module._lv_chart_tick_dsc_set_minor_len(this.ptr, minor_len)
    }
    drawSize(){
        return Module._lv_chart_tick_dsc_get_draw_size(this.ptr)
    }
    setDrawSize(draw_size){
        return Module._lv_chart_tick_dsc_set_draw_size(this.ptr, draw_size)
    }
    minorCnt(){
        return Module._lv_chart_tick_dsc_get_minor_cnt(this.ptr)
    }
    setMinorCnt(minor_cnt){
        return Module._lv_chart_tick_dsc_set_minor_cnt(this.ptr, minor_cnt)
    }
    majorCnt(){
        return Module._lv_chart_tick_dsc_get_major_cnt(this.ptr)
    }
    setMajorCnt(major_cnt){
        return Module._lv_chart_tick_dsc_set_major_cnt(this.ptr, major_cnt)
    }
    labelEn(){
        return Module._lv_chart_tick_dsc_get_label_en(this.ptr)
    }
    setLabelEn(label_en){
        return Module._lv_chart_tick_dsc_set_label_en(this.ptr, label_en)
    }
// AUTO GENERATE CODE END [ChartTickDsc METHODS] --------
}

class MeterScale extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_meter_scale_create()
    }
// AUTO GENERATE CODE START [MeterScale METHODS] --------
    tickColor(){
        return Module._lv_meter_scale_get_tick_color(this.ptr)
    }
    setTickColor(tick_color){
        return Module._lv_meter_scale_set_tick_color(this.ptr, tick_color)
    }
    tickCnt(){
        return Module._lv_meter_scale_get_tick_cnt(this.ptr)
    }
    setTickCnt(tick_cnt){
        return Module._lv_meter_scale_set_tick_cnt(this.ptr, tick_cnt)
    }
    tickLength(){
        return Module._lv_meter_scale_get_tick_length(this.ptr)
    }
    setTickLength(tick_length){
        return Module._lv_meter_scale_set_tick_length(this.ptr, tick_length)
    }
    tickWidth(){
        return Module._lv_meter_scale_get_tick_width(this.ptr)
    }
    setTickWidth(tick_width){
        return Module._lv_meter_scale_set_tick_width(this.ptr, tick_width)
    }
    tickMajorColor(){
        return Module._lv_meter_scale_get_tick_major_color(this.ptr)
    }
    setTickMajorColor(tick_major_color){
        return Module._lv_meter_scale_set_tick_major_color(this.ptr, tick_major_color)
    }
    tickMajorNth(){
        return Module._lv_meter_scale_get_tick_major_nth(this.ptr)
    }
    setTickMajorNth(tick_major_nth){
        return Module._lv_meter_scale_set_tick_major_nth(this.ptr, tick_major_nth)
    }
    tickMajorLength(){
        return Module._lv_meter_scale_get_tick_major_length(this.ptr)
    }
    setTickMajorLength(tick_major_length){
        return Module._lv_meter_scale_set_tick_major_length(this.ptr, tick_major_length)
    }
    tickMajorWidth(){
        return Module._lv_meter_scale_get_tick_major_width(this.ptr)
    }
    setTickMajorWidth(tick_major_width){
        return Module._lv_meter_scale_set_tick_major_width(this.ptr, tick_major_width)
    }
    labelGap(){
        return Module._lv_meter_scale_get_label_gap(this.ptr)
    }
    setLabelGap(label_gap){
        return Module._lv_meter_scale_set_label_gap(this.ptr, label_gap)
    }
    labelColor(){
        return Module._lv_meter_scale_get_label_color(this.ptr)
    }
    setLabelColor(label_color){
        return Module._lv_meter_scale_set_label_color(this.ptr, label_color)
    }
    min(){
        return Module._lv_meter_scale_get_min(this.ptr)
    }
    setMin(min){
        return Module._lv_meter_scale_set_min(this.ptr, min)
    }
    max(){
        return Module._lv_meter_scale_get_max(this.ptr)
    }
    setMax(max){
        return Module._lv_meter_scale_set_max(this.ptr, max)
    }
    rMod(){
        return Module._lv_meter_scale_get_r_mod(this.ptr)
    }
    setRMod(r_mod){
        return Module._lv_meter_scale_set_r_mod(this.ptr, r_mod)
    }
    angleRange(){
        return Module._lv_meter_scale_get_angle_range(this.ptr)
    }
    setAngleRange(angle_range){
        return Module._lv_meter_scale_set_angle_range(this.ptr, angle_range)
    }
    rotation(){
        return Module._lv_meter_scale_get_rotation(this.ptr)
    }
    setRotation(rotation){
        return Module._lv_meter_scale_set_rotation(this.ptr, rotation)
    }
// AUTO GENERATE CODE END [MeterScale METHODS] --------
}

class MeterIndicator extends WAMSObject {
    constructor() {
        super()
        this.ptr = Module._lv_meter_indicator_create()
    }
// AUTO GENERATE CODE START [MeterIndicator METHODS] --------
    scale(){
        return Module._lv_meter_indicator_get_scale(this.ptr)
    }
    setScale(scale){
        return Module._lv_meter_indicator_set_scale(this.ptr, scale)
    }
    type(){
        return Module._lv_meter_indicator_get_type(this.ptr)
    }
    setType(type){
        return Module._lv_meter_indicator_set_type(this.ptr, type)
    }
    opa(){
        return Module._lv_meter_indicator_get_opa(this.ptr)
    }
    setOpa(opa){
        return Module._lv_meter_indicator_set_opa(this.ptr, opa)
    }
    startValue(){
        return Module._lv_meter_indicator_get_start_value(this.ptr)
    }
    setStartValue(start_value){
        return Module._lv_meter_indicator_set_start_value(this.ptr, start_value)
    }
    endValue(){
        return Module._lv_meter_indicator_get_end_value(this.ptr)
    }
    setEndValue(end_value){
        return Module._lv_meter_indicator_set_end_value(this.ptr, end_value)
    }
// AUTO GENERATE CODE END [MeterIndicator METHODS] --------
}

function requireLVGL(exports) {
    if(!exports) {
        exports = {}
    }

    exports.EventEmitter = EventEmitter
    exports.WAMSObject = WAMSObject

    exports.init = function() {
        Module._lv_init()
    }

// AUTO GENERATE CODE START [EXPORT JS CLASS] --------
    exports.Obj = Obj
    exports.Label = Label
    exports.Arc = Arc
    exports.Bar = Bar
    exports.Btn = Btn
    exports.BtnMatrix = BtnMatrix
    exports.Canvas = Canvas
    exports.Checkbox = Checkbox
    exports.Dropdown = Dropdown
    exports.Img = Img
    exports.Line = Line
    exports.Roller = Roller
    exports.Slider = Slider
    exports.Switch = Switch
    exports.Table = Table
    exports.TextArea = TextArea
    exports.MsgBox = MsgBox
    exports.Keyboard = Keyboard
    exports.TileView = TileView
    exports.List = List
    exports.DispDrawBuf = DispDrawBuf
    exports.DispDrv = DispDrv
    exports.Disp = Disp
    exports.Animimg = Animimg
    exports.Calendar = Calendar
    exports.Chart = Chart
    exports.Colorwheel = Colorwheel
    exports.Imgbtn = Imgbtn
    exports.Led = Led
    exports.Menu = Menu
    exports.Meter = Meter
    exports.Span = Span
    exports.Spinbox = Spinbox
    exports.Spinner = Spinner
    exports.Tabview = Tabview
    exports.Win = Win
    exports.Timer = Timer
    exports.SqrtRes = SqrtRes
    exports.MemMonitor = MemMonitor
    exports.MemBuf = MemBuf
    exports.Anim = Anim
    exports.Color8 = Color8
    exports.Color16 = Color16
    exports.Color32 = Color32
    exports.ColorHsv = ColorHsv
    exports.ColorFilterDsc = ColorFilterDsc
    exports.Point = Point
    exports.Area = Area
    exports.ImgHeader = ImgHeader
    exports.ImgDsc = ImgDsc
    exports.ImgTransformDsc = ImgTransformDsc
    exports.Ll = Ll
    exports.IndevData = IndevData
    exports._IndevProc = _IndevProc
    exports.Indev = Indev
    exports.FontGlyphDsc = FontGlyphDsc
    exports.Font = Font
    exports.StyleValue = StyleValue
    exports.StyleTransitionDsc = StyleTransitionDsc
    exports.StyleConstProp = StyleConstProp
    exports.Style = Style
    exports._ObjStyle = _ObjStyle
    exports._ObjStyleTransitionDsc = _ObjStyleTransitionDsc
    exports.FsDrv = FsDrv
    exports.FsFile = FsFile
    exports.FsDir = FsDir
    exports.ImgDecoder = ImgDecoder
    exports.ImgDecoderDsc = ImgDecoderDsc
    exports._ImgCacheEntry = _ImgCacheEntry
    exports._DrawMaskSaved = _DrawMaskSaved
    exports._DrawMaskCommonDsc = _DrawMaskCommonDsc
    exports.DrawMaskLineParam = DrawMaskLineParam
    exports.DrawMaskAngleParam = DrawMaskAngleParam
    exports._DrawMaskRadiusCircleDsc = _DrawMaskRadiusCircleDsc
    exports.DrawMaskRadiusParam = DrawMaskRadiusParam
    exports.DrawMaskFadeParam = DrawMaskFadeParam
    exports.DrawMaskMapParam = DrawMaskMapParam
    exports.DrawRectDsc = DrawRectDsc
    exports.DrawLabelDsc = DrawLabelDsc
    exports.DrawLabelHint = DrawLabelHint
    exports.DrawImgDsc = DrawImgDsc
    exports.DrawLineDsc = DrawLineDsc
    exports.DrawArcDsc = DrawArcDsc
    exports.ObjDrawPartDsc = ObjDrawPartDsc
    exports.ObjClass = ObjClass
    exports.Event = Event
    exports.HitTestInfo = HitTestInfo
    exports.Group = Group
    exports._ObjSpecAttr = _ObjSpecAttr
    exports.Theme = Theme
    exports.FontFmtTxtGlyphDsc = FontFmtTxtGlyphDsc
    exports.FontFmtTxtCmap = FontFmtTxtCmap
    exports.FontFmtTxtGlyphCache = FontFmtTxtGlyphCache
    exports._BarAnim = _BarAnim
    exports.CalendarDate = CalendarDate
    exports.ChartSeries = ChartSeries
    exports.ChartCursor = ChartCursor
    exports.ChartTickDsc = ChartTickDsc
    exports.MeterScale = MeterScale
    exports.MeterIndicator = MeterIndicator
// AUTO GENERATE CODE END [EXPORT JS CLASS] --------

    return exports
}