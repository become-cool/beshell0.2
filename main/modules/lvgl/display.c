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



JSValue js_lv_disp_wrapper(JSContext *ctx, lv_disp_t * disp) {
    if(!disp->driver) {
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
    uint8_t * buff ;
    lv_disp_drv_t * disp ;
} draw_param_t ;

draw_param_t draw_param ;

QueueHandle_t disp_queue;

static void disp_st77XX_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) {
    if(!disp->user_data) {
        printf("spidev is NULL\n") ;
        return ;
    }
    // printf("disp_st77XX_flush(%d,%d,%d,%d)\n",area->x1,area->y1,area->x2,area->y2) ;

    // draw_param.x1 = area->x1 ;
    // draw_param.x2 = area->x2 ;
    // draw_param.y1 = area->y1 ;
    // draw_param.y2 = area->y2 ;
    // draw_param.buff = (uint8_t*)color_p ;
    // draw_param.disp = disp ;

    // draw_param_t * pp = &draw_param ;
	// xQueueSend(disp_queue, (void *)&pp, 0);

    st77xx_draw_rect(((disp_drv_spec_t*)disp->user_data)->spi_dev, area->x1,area->y1, area->x2, area->y2, color_p) ;
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
    if(!thisdisp->driver->user_data) {
        return ;
    }
    disp_drv_spec_t * thisspec = (disp_drv_spec_t *) thisdisp->driver->user_data ;
    thisspec->enable_input_event = true ;
    thisspec->ctx = ctx ;
    return JS_UNDEFINED ;
}

static JSValue js_lv_disp_disable_input_event(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_DISP
    if(!thisdisp->driver->user_data) {
        return ;
    }
    disp_drv_spec_t * thisspec = (disp_drv_spec_t *) thisdisp->driver->user_data ;
    thisspec->enable_input_event = false ;
    thisspec->ctx = ctx ;
    return JS_UNDEFINED ;
}


static const JSCFunctionListEntry js_lv_disp_proto_funcs[] = {
    JS_CFUNC_DEF("activeScreen", 0, js_lv_disp_active_screen),
    JS_CFUNC_MAGIC_DEF("sysLayer", 0, js_lv_disp_layer, 1),
    JS_CFUNC_MAGIC_DEF("topLayer", 0, js_lv_disp_layer, 2),
    JS_CFUNC_DEF("getScreens", 0, js_lv_disp_get_screens),
    JS_CFUNC_DEF("enableInputEvent", 0, js_lv_disp_enable_input_event),
    JS_CFUNC_DEF("disableInputEvent", 0, js_lv_disp_disable_input_event),
};



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



static void * malloc_buffer(size_t size) {
#ifndef SIMULATION
    // echo_DMA("before malloc_buffer()") ;
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
 * @param type "st7789"|"st7789v"|"virtual-display"
 * @param options: {cs,dc,spi,freq,width,height}
 * @return JSValue 
 */
JSValue js_lvgl_create_display(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(2)

    ARGV_TO_STRING_LEN_E(0, typestr, typelen, "argv type must be a string")
    if(!JS_IsObject(argv[1])) {
        THROW_EXCEPTION("argv options must be a object")
    }

    lv_disp_draw_buf_t * drawbuf = NULL ;
    lv_disp_drv_t * dispdrv = NULL ;

    GET_INT_PROP(argv[1], "width", width, excp)
    GET_INT_PROP(argv[1], "height", height, excp)
    
    disp_drv_spec_t * dvrdata = malloc(sizeof(disp_drv_spec_t)) ;

    // 创建缓冲区
#ifndef SIMULATION
    size_t bufsize = width * DISP_BUFF_LINES ;
    dvrdata->buff1 = malloc_buffer(bufsize*2) ;
    if(getPsramTotal()>0) {
        dvrdata->buff2 = malloc_buffer(bufsize*2) ;
    }
    else {
        dvrdata->buff2 = NULL ;
    }
#else
    size_t bufsize = width * DISP_BUFF_LINES_ENOUGH ;
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

    if( strncmp(typestr, "st7789", 6)==0 ) {

#ifndef SIMULATION

        GET_INT_PROP(argv[1], "cs", cs, excp)
        GET_INT_PROP(argv[1], "dc", dc, excp)
        GET_INT_PROP_DEFAULT(argv[1], "spi", spi, 1, excp)
        GET_INT_PROP_DEFAULT(argv[1], "freq", freq, 26000000, excp)

        printf("spi=%d, cs=%d, dc=%d, freq=%d, width=%d, height=%d\n",spi,cs,dc,freq,width,height) ;

        // 初始化 spi
        st77xx_dev_t * spidev = malloc(sizeof(st77xx_dev_t));
        st77xx_spi_init(spidev, spi, cs, dc, freq);

        if( strcmp(typestr, "st7789v")==0 ) {
            GET_INT_PROP_DEFAULT(argv[1], "MADCTL", MADCTL, 0x20|0x80, excp)
            st7789v_init(spidev, width, height, 0, 0, (uint8_t)MADCTL);
        }
        else if(strcmp(typestr, "st7789")==0) {
            GET_INT_PROP_DEFAULT(argv[1], "MADCTL", MADCTL, 0, excp)
            st7789_init(spidev, width, height, 0, 0, (uint8_t)MADCTL);
        }
        else {
            JS_ThrowReferenceError(ctx, "unknow disp driver: %s", typestr);
            goto excp ;
        }

        // 注册设备驱动对象
        dispdrv->flush_cb = disp_st77XX_flush ;
        dvrdata->spi_dev = spidev ;
#endif
    }

    // 虚拟屏幕
    else if( strcmp(typestr, "virtual-display")==0 ) {

        // 注册设备驱动对象
        dispdrv->flush_cb = ws_disp_flush ;
    }
    else {
        JS_ThrowReferenceError(ctx, "unknow disp driver: %s", typestr);
        goto excp ;
    }

    lv_disp_t * disp = lv_disp_drv_register(dispdrv); 
    
    JSValue jsdisp = JS_NewObjectClass(ctx, js_lv_disp_class_id) ;
    JS_SetPropertyStr(ctx, jsdisp, "type", argv[0]) ;
    JS_SetPropertyStr(ctx, jsdisp, "_handles", JS_NewObject(ctx)) ;
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
    // vlgl_js_display_ws_init() ;
	// disp_queue = xQueueCreate(1, sizeof(draw_param_t *));
	// xTaskCreatePinnedToCore(task_disp, "task_disp", 2048, NULL, 5, NULL, 1);
#endif
}

void be_lv_display_require(JSContext *ctx, JSValue lvgl) {
    
    // lvgl.Display
    JSValue EventEmitterProto = js_get_glob_prop(ctx, 3, "beapi", "EventEmitter", "prototype") ;
    qjs_def_class(
        ctx, "Display"
        , js_lv_disp_class_id, &js_lv_disp_class
        , "lv.Display", NULL
        , js_lv_disp_proto_funcs, countof(js_lv_disp_proto_funcs)
        , EventEmitterProto, JS_UNDEFINED
    ) ;
    JS_FreeValue(ctx, EventEmitterProto) ;
    
    // JS_NewClass(JS_GetRuntime(ctx), js_lv_disp_class_id, &js_lv_disp_class);
    // JSValue dispDriverProto = JS_NewObject(ctx);
    // JS_SetPropertyFunctionList(ctx, dispDriverProto, js_display_proto_funcs, countof(js_display_proto_funcs));
    // JS_SetClassProto(ctx, js_lv_disp_class_id, dispDriverProto);




    JS_SetPropertyStr(ctx, lvgl, "createDisplay", JS_NewCFunction(ctx, js_lvgl_create_display, "createDisplay", 1));
    // JS_SetPropertyStr(ctx, lvgl, "actionDisplay", JS_NewCFunction(ctx, js_lvgl_set_default_display, "actionDisplay", 1));
}

void be_lv_display_reset(JSContext * ctx) {
    
#ifndef SIMULATION
    multi_heap_info_t info;
    heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);
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
    heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);
    // dn2(info.total_free_bytes, info.total_allocated_bytes)
#endif
}


JSClassID lv_disp_js_class_id() {
    return js_lv_disp_class_id ;
}
