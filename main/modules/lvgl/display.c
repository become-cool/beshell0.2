#include "display.h"
#include "widgets_gen.h"
#include "widgets.h"
#include "module_lvgl.h"
#include "utils.h"
#include "cutils.h"

#include "lv_conf.h"
#include "telnet_ws.h"

#ifndef SIMULATION
#include "psram.h"
#include "disp_st77xx.h"
#include <freertos/queue.h>
#endif

static uint8_t _disp_id = 0 ;

JSValue js_lv_disp_wrapper(JSContext *ctx, lv_disp_t * disp) {
    if(!disp || !disp->driver) {
        return JS_NULL ;
    }
    disp_drv_spec_t * spec = (disp_drv_spec_t*) disp->driver->user_data ;

    // 该屏幕对象由 js 创建
    if(spec) {
        if(!spec->jsobj) {
            THROW_EXCEPTION("spec->jsobj is null ?") ;
        }
        return JS_DupValue(ctx, JS_MKPTR(JS_TAG_OBJECT,spec->jsobj)) ;
    }

    // 屏幕对象 不是 js 创建
    else {
        // @todo 包装成 js 对象 (不在 js_lv_disp_finalizer 中 free)
        THROW_EXCEPTION("disp not created by js runtime") ;
    }
}

#ifndef SIMULATION
typedef struct {
    lv_coord_t x1 ;
    lv_coord_t x2 ;
    lv_coord_t y1 ;
    lv_coord_t y2 ;
    uint16_t * buff ;
    lv_disp_drv_t * disp ;

    // spi 输出 和 websocket 输出可能在不同的 core 
    // 当有一边先完成时， flushed = true
    // 后完成的一边 调用 lv_disp_flush_ready()
    // bool flushed ;   
} disp_spi_param_t ;


static void disp_st77XX_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) {
    if(!disp->user_data) {
        printf("spidev is NULL\n") ;
        return ;
    }

    st77xx_draw_rect(((disp_drv_spec_t*)disp->user_data)->spi_dev, area->x1,area->y1, area->x2, area->y2, color_p) ;
    // flush_disp_if_other_ready() ;
    
    ws_disp_flush(disp, area, color_p) ;

    // flush_disp_if_other_ready() ;
    lv_disp_flush_ready(disp) ;
}


#endif

void free_disp_drv(JSContext * ctx, lv_disp_t * disp) {
    printf("free_disp_drv()") ;
    if(disp->driver){
        if(disp->driver->draw_buf) {
            free(disp->driver->draw_buf) ;
        }

        if(disp->driver->user_data) {
            disp_drv_spec_t * drv_spec = (disp_drv_spec_t*)disp->driver->user_data ;
            if(drv_spec->buff1) {
                printf("free disp buff1\n") ;
                free(drv_spec->buff1) ;
                drv_spec->buff1 = NULL ;
            }
            if(drv_spec->buff2) {
                printf("free disp buff2\n") ;
                free(drv_spec->buff2) ;
                drv_spec->buff2 = NULL ;
            }
#ifndef SIMULATION
            st77xx_dev_t * dev = drv_spec->spi_dev ;
            if(dev) {
                if(dev->spi_dev) {
                    spi_bus_remove_device(dev->spi_dev) ;
                }
                free(dev) ;
            }
#endif

            JSValue jsvar = JS_MKPTR(JS_TAG_OBJECT, drv_spec->jsobj) ;
            JS_SetOpaque(jsvar, NULL) ;
            JS_FreeValue(ctx,jsvar) ;
            drv_spec->jsobj = NULL ;

            free(disp->driver->user_data) ;
            disp->driver->user_data = NULL ;
        }

        free(disp->driver) ;
        disp->driver = NULL ;
    }
}

static JSClassID js_lv_disp_class_id ;

static void js_lv_disp_finalizer(JSRuntime *rt, JSValue val) {
    printf("js_lv_disp_finalizer\n") ;
    lv_disp_t * disp = (lv_disp_t *)JS_GetOpaque(val, js_lv_disp_class_id) ;
    if(!disp) {
        printf("disp is NULL\n") ;
        return ;
    }
    free_disp_drv(NULL, disp) ;
    lv_disp_remove(disp) ;
}

static JSClassDef js_lv_disp_class = {
    "lvgl.Display",
    .finalizer = js_lv_disp_finalizer,
};


#define THIS_DISP                                                           \
    lv_disp_t * thisdisp = JS_GetOpaque(this_val, js_lv_disp_class_id);     \
    if(!thisdisp) {                                                         \
        THROW_EXCEPTION("must be called as a lvgl.Display method")          \
    }


static JSValue js_lv_disp_active_screen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_DISP
    lv_obj_t * scr = lv_disp_get_scr_act(thisdisp) ;
    return js_lv_obj_wrapper(ctx,scr,JS_UNDEFINED,lv_obj_js_class_id()) ;
}

static JSValue js_lv_disp_get_screens(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_DISP
    JSValue arr = JS_NewArray(ctx) ;
    for(int i=0;i<thisdisp->screen_cnt; i++) {
        JSValue jsscr = js_lv_obj_wrapper(ctx, thisdisp->screens[i], JS_UNDEFINED, lv_obj_js_class_id())  ;
        JS_SetPropertyUint32(ctx, arr, i, jsscr);
    }
    return arr ;
}

static JSValue js_lv_disp_layer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, int layer_type) {
    THIS_DISP
    lv_obj_t * layer ;
    if(layer_type==1) {
        layer = lv_disp_get_layer_sys(thisdisp) ;
    }
    else if(layer_type==2) {
        layer = lv_disp_get_layer_top(thisdisp) ;
    }
    else {
        return JS_NULL ;
    }
    JSValue jslayer = js_lv_obj_wrapper(ctx, layer, JS_UNDEFINED, 0) ;
    return jslayer ;
}

static JSValue js_lv_disp_enable_input_event(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_DISP
    if(!thisdisp->driver || !thisdisp->driver->user_data) {
        THROW_EXCEPTION("invalid lv.Display object")
    }
    disp_drv_spec_t * thisspec = (disp_drv_spec_t *) thisdisp->driver->user_data ;
    thisspec->enable_input_event = true ;
    thisspec->ctx = ctx ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_disp_disable_input_event(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_DISP
    if(!thisdisp->driver || !thisdisp->driver->user_data) {
        THROW_EXCEPTION("invalid lv.Display object")
    }
    disp_drv_spec_t * thisspec = (disp_drv_spec_t *) thisdisp->driver->user_data ;
    thisspec->enable_input_event = false ;
    thisspec->ctx = ctx ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_disp_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_DISP
    if(!thisdisp->driver) {
        THROW_EXCEPTION("invalid lv.Display object")
    }
    return JS_NewInt32(ctx, thisdisp->driver->hor_res) ;
}
static JSValue js_lv_disp_height(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_DISP
    if(!thisdisp->driver) {
        THROW_EXCEPTION("invalid lv.Display object")
    }
    return JS_NewInt32(ctx, thisdisp->driver->ver_res) ;
}
static JSValue js_lv_disp_id(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_DISP
    if(!thisdisp->driver || !thisdisp->driver->user_data) {
        return ;
    }
    return JS_NewInt32(ctx, ((disp_drv_spec_t*)thisdisp->driver->user_data)->id) ;
}

JSValue be_lv_display_inv_area(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_DISP
    CHECK_ARGC(4)
    ARGV_TO_UINT32(0, x1)
    ARGV_TO_UINT32(1, y1)
    ARGV_TO_UINT32(2, x2)
    ARGV_TO_UINT32(3, y2)

    lv_area_t area;
    memset(&area, 0, sizeof(lv_area_t));
    area.x1 = x1 ;
    area.y1 = y1 ;
    area.x2 = x2 ;
    area.y2 = y2 ;
    
    _lv_inv_area(thisdisp, &area);


    uint64_t t0 = gettime() ;
    lv_task_handler();
    printf("lv_task_handler() ms %llu\n",gettime() - t0) ;

    return JS_UNDEFINED;
}

#ifndef SIMULATION
JSValue be_lv_display_st77xx_init(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_DISP
    CHECK_ARGC(4)

    ARGV_TO_STRING(0, typestr)
    ARGV_TO_UINT32(1, width)
    ARGV_TO_UINT32(2, height)
    ARGV_TO_UINT32(3, MADCTL)

    st77xx_conf_t conf ;
    memset(&conf, 0, sizeof(st77xx_conf_t)) ;
    conf.width = width ;
    conf.height = height ;
    conf.MADCTL = MADCTL ;

    // ds(typestr)
    // dn3(width,height,MADCTL)

    disp_drv_spec_t * spec = (disp_drv_spec_t *)thisdisp->driver->user_data ;
    
    if( strcmp(typestr, "ST7789V")==0 ) {
        st7789v_init(spec->spi_dev, conf);
    }
    else if(strcmp(typestr, "ST7789")==0) {
        st7789_init(spec->spi_dev, conf);
    }

    JS_FreeCString(ctx,typestr) ;
    return JS_UNDEFINED ;
}

JSValue js_lv_disp_dev_reg_write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_DISP
    CHECK_ARGC(2)

    ARGV_TO_UINT8(0, reg)
    ARGV_TO_UINT8(1, data)

    return JS_UNDEFINED ;
}

#endif



static const JSCFunctionListEntry js_lv_disp_proto_funcs[] = {
    JS_CFUNC_DEF("activeScreen", 0, js_lv_disp_active_screen),
    JS_CFUNC_MAGIC_DEF("sysLayer", 0, js_lv_disp_layer, 1),
    JS_CFUNC_MAGIC_DEF("topLayer", 0, js_lv_disp_layer, 2),
    JS_CFUNC_DEF("getScreens", 0, js_lv_disp_get_screens),
    JS_CFUNC_DEF("enableInputEvent", 0, js_lv_disp_enable_input_event),
    JS_CFUNC_DEF("disableInputEvent", 0, js_lv_disp_disable_input_event),
    JS_CFUNC_DEF("width", 0, js_lv_disp_width),
    JS_CFUNC_DEF("height", 0, js_lv_disp_height),
    JS_CFUNC_DEF("id", 0, js_lv_disp_id),
    JS_CFUNC_DEF("invArea", 0, be_lv_display_inv_area),
#ifndef SIMULATION
    JS_CFUNC_DEF("initST77xx", 0, be_lv_display_st77xx_init),
    JS_CFUNC_DEF("devRegWrite", 0, js_lv_disp_dev_reg_write),
#endif
};

disp_drv_spec_t * default_disp_drv_spec() {

    lv_disp_t * disp = lv_disp_get_default() ;
    if(!disp || !disp->driver || !disp->driver->user_data ) {
        return ;
    }

    return disp->driver->user_data ;
}

// static JSValue js_lvgl_get_default_display(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
//     lv_disp_t * disp = lv_disp_get_default() ;
//     if(!disp) {
        
//     }
//     return JS_UNDEFINED ;
// }
// static JSValue js_lvgl_set_default_display(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
//     // lv_disp_set_default
//     return JS_UNDEFINED ;
// }

disp_drv_spec_t * find_disp_spec_by_id(uint8_t id){
    for(lv_disp_t * d = lv_disp_get_next(NULL); d!=NULL; d = lv_disp_get_next(d)) {
        if(!d->driver || !d->driver->user_data) {
            continue ;
        }
        if(((disp_drv_spec_t*)d->driver->user_data)->id == id) {
            return d->driver->user_data ;
        }
    }
    return NULL ;
}
lv_disp_t * find_disp_by_id(uint8_t id) {
    for(lv_disp_t * d = lv_disp_get_next(NULL); d!=NULL; d = lv_disp_get_next(d)) {
        if(!d->driver || !d->driver->user_data) {
            continue ;
        }
        if(((disp_drv_spec_t*)d->driver->user_data)->id == id) {
            return d ;
        }
    }
    return NULL ;
}


static JSValue js_lvgl_get_display_by_id(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, id)
    disp_drv_spec_t * spec = find_disp_spec_by_id(id) ;
    if(spec) {
        return JS_DupValue(ctx, JS_MKPTR(JS_TAG_OBJECT, spec->jsobj)) ;
    }
    return JS_NULL ;
}

static JSValue js_lvgl_all_displays(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    JSValue array = JS_NewArray(ctx) ;
    int i = 0 ;
    for(lv_disp_t * d = lv_disp_get_next(NULL); d!=NULL; d = lv_disp_get_next(d)) {
        if(!d->driver || !d->driver->user_data) {
            continue ;
        }
        JSValue disp = JS_MKPTR(JS_TAG_OBJECT, ((disp_drv_spec_t*)d->driver->user_data)->jsobj) ;
        JS_SetPropertyUint32(ctx, array, i++, disp) ;
    }
    return array ;
}


static void * malloc_buffer(size_t size) {
#ifndef SIMULATION
    // echo_DMA("before malloc_buffer()") ;
    // void * buff = heap_caps_malloc( size + DISP_BUFF_AUX_SIZE, MALLOC_CAP_SPIRAM); 
    void * buff = heap_caps_malloc( size + DISP_BUFF_AUX_SIZE, MALLOC_CAP_DMA);
    if(!buff) {
        printf("heap_caps_malloc(%d, MALLOC_CAP_DMA) failed, try malloc().\n", size+DISP_BUFF_AUX_SIZE) ;
        buff = malloc(size + DISP_BUFF_AUX_SIZE) ;
    }
    // echo_DMA("after malloc_buffer()") ;
#else
    void * buff = malloc(size + DISP_BUFF_AUX_SIZE) ;
#endif
    return buff ;
}

/**
 * 
 * @param type "ST7789"|"ST7789v"|"virtual-display"
 * @param options: {cs,dc,spi,freq,MADCTL,width,height}
 * @return JSValue 
 */
JSValue js_lvgl_create_display(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    if(_disp_id>=255){
        THROW_EXCEPTION("max display count 255") ;
    }

    CHECK_ARGC(2)

    ARGV_TO_STRING_LEN_E(0, typestr, typelen, "argv type must be a string")
    if(!JS_IsObject(argv[1])) {
        THROW_EXCEPTION("argv options must be a object")
    }

    lv_disp_draw_buf_t * drawbuf = NULL ;
    lv_disp_drv_t * dispdrv = NULL ;

    GET_INT_PROP(argv[1], "width", width, { goto excp ;})
    GET_INT_PROP(argv[1], "height", height, { goto excp ;})
    
    disp_drv_spec_t * dvrdata = malloc(sizeof(disp_drv_spec_t)) ;
    memset(dvrdata, 0, sizeof(disp_drv_spec_t)) ;
    dvrdata->id = _disp_id ++ ;

    // 创建缓冲区
#ifndef SIMULATION
    dvrdata->buff_lines = DISP_BUFF_LINES ;
    size_t bufsize = width * dvrdata->buff_lines ;
    dvrdata->buff1 = malloc_buffer(bufsize*2) ;
    if(getPsramTotal()>0) {
        dvrdata->buff2 = malloc_buffer(bufsize*2) ;
    }
    else {
        dvrdata->buff2 = NULL ;
    }
#else
    dvrdata->buff_lines = DISP_BUFF_LINES_ENOUGH ;
    size_t bufsize = width * dvrdata->buff_lines ;
    dvrdata->buff1 = malloc_buffer(bufsize*2) ;
    dvrdata->buff2 = malloc_buffer(bufsize*2) ;
#endif

    dvrdata->enable_input_event = true ;
    dvrdata->ctx = ctx ;

    drawbuf = malloc(sizeof(lv_disp_draw_buf_t)) ;
    if(!drawbuf){
        JS_ThrowReferenceError(ctx, "out of memory?");
        goto excp ;
    }
    if(dvrdata->buff2) {
        lv_disp_draw_buf_init(drawbuf, dvrdata->buff1+DISP_BUFF_AUX_SIZE, dvrdata->buff2+DISP_BUFF_AUX_SIZE, bufsize);
    }
    else {
        lv_disp_draw_buf_init(drawbuf, dvrdata->buff1+DISP_BUFF_AUX_SIZE, NULL, bufsize);
    }

    // 创建设备驱动对象
    dispdrv = malloc(sizeof(lv_disp_drv_t)) ;
    if(!dispdrv){
        JS_ThrowReferenceError(ctx, "out of memory?");
        goto excp ;
    }
    lv_disp_drv_init(dispdrv); 
    dispdrv->draw_buf = drawbuf;
    dispdrv->hor_res = width;
    dispdrv->ver_res = height;

    dispdrv->user_data = dvrdata ;

    // ST7789/ST7789V
    if( strncmp(typestr, "ST7789", 6)==0 ) {

#ifndef SIMULATION

        GET_INT_PROP(argv[1], "cs", cs, { goto excp ;})
        GET_INT_PROP(argv[1], "dc", dc, { goto excp ;})
        GET_INT_PROP_DEFAULT(argv[1], "spi", spi, 1)
        GET_INT_PROP_DEFAULT(argv[1], "freq", freq, 26000000)
        GET_INT_PROP_DEFAULT(argv[1], "MADCTL", MADCTL, 0)
        bool invColor = JS_ToBool(ctx, JS_GetPropertyStr(ctx, argv[1], "invColor")) ;

        printf("spi=%d, cs=%d, dc=%d, freq=%d, width=%d, height=%d, MADCTL=%d, invColor=%d\n",spi,cs,dc,freq,width,height, MADCTL, invColor) ;

        // 初始化 spi
        st77xx_dev_t * spidev = malloc(sizeof(st77xx_dev_t));
        st77xx_spi_init(spidev, spi, cs, dc, freq);

        st77xx_conf_t conf ;
        memset(&conf, 0, sizeof(st77xx_conf_t)) ;
        conf.width = width ;
        conf.height = height ;
        conf.MADCTL = MADCTL ;
        conf.invColor = invColor ;

        if( strcmp(typestr, "ST7789V")==0 ) {
            st7789v_init(spidev, conf);
        }
        else if(strcmp(typestr, "ST7789")==0) {
            dd
            st7789_init(spidev, conf);
        }
        else {
            JS_ThrowReferenceError(ctx, "unknow disp driver: %s", typestr);
            goto excp ;
        }

        // 注册设备驱动对象
        dispdrv->flush_cb = disp_st77XX_flush ;
        dvrdata->spi_dev = spidev ;

        // 启动 spi 传输任务
        // disp_spi_start() ;
#endif
    }

    // 虚拟屏幕
    else if( strcmp(typestr, "virtual-display")==0 ) {

        // 注册设备驱动对象
        dispdrv->flush_cb = ws_disp_flush ;
        dvrdata->is_virtual = true ;
    }
    else {
        JS_ThrowReferenceError(ctx, "unknow disp driver: %s", typestr);
        goto excp ;
    }

    lv_disp_t * disp = lv_disp_drv_register(dispdrv); 
    
    JSValue jsdisp = JS_NewObjectClass(ctx, js_lv_disp_class_id) ;
    JS_SetPropertyStr(ctx, jsdisp, "type", argv[0]) ;
    JS_SetPropertyStr(ctx, jsdisp, "_handlers", JS_NewObject(ctx)) ;
    JS_SetOpaque(jsdisp, disp);

    dvrdata->jsobj = JS_VALUE_GET_PTR(jsdisp) ;

    lv_task_handler() ;

    return jsdisp ;

excp:
    if(drawbuf)
        free(drawbuf) ;
    if(dispdrv)
        free(dispdrv) ;
    JS_FreeCString(ctx, typestr) ;
    return JS_EXCEPTION ;
}

void be_lv_display_init() {
    // class id 全局, 分配一次
    JS_NewClassID(&js_lv_disp_class_id);

#ifndef SIMULATION
	// disp_queue = xQueueCreate(1, sizeof(draw_param_t *));
	// xTaskCreatePinnedToCore(task_disp, "task_disp", 2048, NULL, 5, NULL, 1);
#endif
}



static JSValue js_lv_display_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    THROW_EXCEPTION("create Display by api function beapi.lvgl.createDisplay()")
}


void be_lv_display_require(JSContext *ctx, JSValue lvgl) {

    _disp_id = 0 ;
    
    // lvgl.Display
    JSValue EventEmitterProto = js_get_glob_prop(ctx, 3, "beapi", "EventEmitter", "prototype") ;
    qjs_def_class(
        ctx, "Display"
        , js_lv_disp_class_id, &js_lv_disp_class
        , "lv.Display", js_lv_display_constructor
        , js_lv_disp_proto_funcs, countof(js_lv_disp_proto_funcs)
        , EventEmitterProto, lvgl
    ) ;
    JS_FreeValue(ctx, EventEmitterProto) ;
    
    // JS_NewClass(JS_GetRuntime(ctx), js_lv_disp_class_id, &js_lv_disp_class);
    // JSValue dispDriverProto = JS_NewObject(ctx);
    // JS_SetPropertyFunctionList(ctx, dispDriverProto, js_display_proto_funcs, countof(js_display_proto_funcs));
    // JS_SetClassProto(ctx, js_lv_disp_class_id, dispDriverProto);


    JS_SetPropertyStr(ctx, lvgl, "createDisplay", JS_NewCFunction(ctx, js_lvgl_create_display, "createDisplay", 1));
    JS_SetPropertyStr(ctx, lvgl, "allDisplays", JS_NewCFunction(ctx, js_lvgl_all_displays, "allDisplays", 1));
    JS_SetPropertyStr(ctx, lvgl, "getDisplayById", JS_NewCFunction(ctx, js_lvgl_get_display_by_id, "getDisplayById", 1));
}

void be_lv_display_reset(JSContext * ctx) {
    
#ifndef SIMULATION
    // multi_heap_info_t info;
    // heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);
    // dn2(info.total_free_bytes, info.total_allocated_bytes)
#endif

    // 清理 disp (timer,indev,lv_obj_t) / driver 
    lv_disp_t * disp = NULL ;
    while((disp=lv_disp_get_default())!=false) {
        free_disp_drv(ctx, disp) ;
        lv_disp_remove(disp) ;
    }


// #ifndef SIMULATION
//     if(device_touch) {
//         spi_bus_remove_device(device_touch) ;
//         // device_touch = NULL ;
//     }
// #endif
    // if(indev) {
    //     lv_indev_remove(indev) ;
    //     indev = NULL ;
    // }

    
#ifndef SIMULATION
    // heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);
    // dn2(info.total_free_bytes, info.total_allocated_bytes)
#endif
}


JSClassID lv_disp_js_class_id() {
    return js_lv_disp_class_id ;
}
