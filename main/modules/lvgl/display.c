#include "display.h"
#include "widgets_gen.h"
#include "widgets.h"
#include "module_lvgl.h"
#include "utils.h"
#include "cutils.h"

#include "lvgl.h"
#include "lv_conf.h"

#ifndef SIMULATION
// #include "touch_driver.h"
#include "xpt2046.h"
#include "tp_spi.h"
#include "display_ws.h"
#include "disp_st77xx.h"
#else
#include "http_lws.h"
#endif

uint8_t * dma_buff = NULL ;

lv_indev_drv_t indev_drv;

#ifndef SIMULATION

void ws_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) {}

void disp_st7789_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) {
    // printf("disp_st7789_flush()\n") ;
    if(!disp->user_data) {
        printf("spidev is NULL\n") ;
        return ;
    }
    st77xx_draw_rect(disp->user_data, area->x1,area->y1, area->x2, area->y2, color_p) ;
    // disp_virtual_flush(disp, area, color_p) ;
    lv_disp_flush_ready(disp) ;
}
void input_driver_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
    // if(ws_driver_input_read(drv, data))
    //     return ;

    data->continue_reading = xpt2046_read(drv, data) ;
    if(data->state == LV_INDEV_STATE_PRESSED) {
        data->point.x -= 10 ;
    }

    indev_input_x = data->point.x ;
    indev_input_y = data->point.y ;
    indev_input_pressed = (data->state == LV_INDEV_STATE_PRESSED) ;
}
#endif

static JSClassID js_lvgl_disp_class_id ;

static void js_lvgl_disp_finalizer(JSRuntime *rt, JSValue val) {

    printf("js_lvgl_disp_finalizer()\n") ;

    lv_disp_t * disp = (lv_disp_t *)JS_GetOpaque(val, js_lvgl_disp_class_id) ;
    lv_disp_remove(disp) ;

    // spi 设备
#ifndef SIMULATION
    if(disp->driver->user_data) {
        st77xx_dev_t * dev = (st77xx_dev_t*)disp->driver->user_data ;
        spi_bus_remove_device(dev->spi_dev) ;
        free(dev) ;
    }
#endif

    if(disp->driver->draw_buf) {
        if(disp->driver->draw_buf->buf1) {
            free(disp->driver->draw_buf->buf1) ;
        }
        if(disp->driver->draw_buf->buf2) {
            free(disp->driver->draw_buf->buf2) ;
        }
        free(disp->driver->draw_buf) ;
    }

    free(disp->driver) ;
    free(disp) ;
}

static JSClassDef js_lvgl_disp_class = {
    "lvgl.Display",
    .finalizer = js_lvgl_disp_finalizer,
};


#define THIS_DISP                                                                               \
    lv_disp_t * thisdisp = JS_GetOpaque(this_val, js_lvgl_disp_class_id);                       \
    if(!thisdisp) {                                                                             \
        THROW_EXCEPTION("lvgl.Display.activeScreen() must be called as a lvgl.Display method")  \
    }


static JSValue js_lvgl_disp_active_screen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_DISP
    lv_obj_t * scr = lv_disp_get_scr_act(thisdisp) ;
    if(!scr) {
        return JS_NULL ;
    }

    void * objptr = lv_obj_get_user_data(scr) ;
    JSValue jsobj = JS_MKPTR(JS_TAG_OBJECT, objptr) ;

    return JS_DupValue(ctx,jsobj) ;
}

static JSValue js_lvgl_disp_get_screens(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_DISP
    JSValue arr = JS_NewArray(ctx) ;
    for(int i=0;i<thisdisp->screen_cnt; i++) {
        JSValue jsscr = js_lv_obj_wrapper(ctx, thisdisp->screens[i], JS_UNDEFINED, lv_obj_js_class_id())  ;
        JS_SetPropertyUint32(ctx, arr, i, jsscr);
    }
    return arr ;
}

static const JSCFunctionListEntry js_display_proto_funcs[] = {
    JS_CFUNC_DEF("activeScreen", 0, js_lvgl_disp_active_screen),
    JS_CFUNC_DEF("getScreens", 0, js_lvgl_disp_get_screens),
};



static JSValue js_lvgl_get_default_display(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    lv_disp_t * disp = lv_disp_get_default() ;
    if(!disp) {
        
    }
    return JS_UNDEFINED ;
}
static JSValue js_lvgl_set_default_display(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    // lv_disp_set_default
    return JS_UNDEFINED ;
}


/**
 * 
 * @param type "ST7789"
 * @param options
 * @return JSValue 
 */
JSValue js_lvgl_create_display(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    if(!dma_buff) {
        THROW_EXCEPTION("DMA Buff is NULL")
    }

    CHECK_ARGC(2)

    ARGV_TO_STRING_LEN_E(0, typestr, typelen, "argv type must be a string")
    if(!JS_IsObject(argv[1])) {
        THROW_EXCEPTION("argv options must be a object")
    }

    lv_disp_draw_buf_t * drawbuf = NULL ;
    lv_disp_drv_t * dispdrv = NULL ;

    GET_INT_PROP(argv[1], "width", width, excp)
    GET_INT_PROP(argv[1], "height", height, excp)
    
    // 创建缓冲区
    size_t buflen = (DMA_BUFF_LEN / width) * width ;
    drawbuf = malloc(sizeof(lv_disp_draw_buf_t)) ;
    if(!drawbuf){
        JS_ThrowReferenceError(ctx, "out of memory?");
        goto excp ;
    }
    lv_disp_draw_buf_init(drawbuf, dma_buff+DMA_BUFF_AUX_SIZE, NULL, buflen/2);

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

    if( strcmp(typestr, "ST7789")==0 ) {

#ifndef SIMULATION

        GET_INT_PROP(argv[1], "cs", cs, excp)
        GET_INT_PROP(argv[1], "dc", dc, excp)
        GET_INT_PROP_DEFAULT(argv[1], "spi", spi, 1, excp)
        GET_INT_PROP_DEFAULT(argv[1], "freq", freq, 26000000, excp)

        // printf("spi=%d, cs=%d, dc=%d, freq=%d, width=%d, height=%d\n",spi,cs,dc,freq,width,height) ;

        // 初始化 spi
        st77xx_dev_t * spidev = malloc(sizeof(st77xx_dev_t));
        st77xx_spi_init(spidev, spi, cs, dc, freq);
        st77xx_init(spidev, width, height, 0, 0);


        // 注册设备驱动对象
        dispdrv->flush_cb = disp_st7789_flush ;
        dispdrv->user_data = spidev ;
#endif
    }

    // 虚拟屏幕
    else if( strcmp(typestr, "VIRTUAL")==0 ) {

        // 注册设备驱动对象
        dispdrv->flush_cb = ws_disp_flush ;
    }
    else {
        JS_ThrowReferenceError(ctx, "unknow disp driver: %s", typestr);
        goto excp ;
    }

    lv_disp_t * disp = lv_disp_drv_register(dispdrv); 
    
    JSValue jsdisp = JS_NewObjectClass(ctx, js_lvgl_disp_class_id) ;
    JS_SetPropertyStr(ctx, jsdisp, "type", argv[0]) ;
    // JS_SetPropertyStr(ctx, jsdisp, "options", argv[1]) ;
    JS_SetOpaque(jsdisp, disp);

    // 触摸设备
#ifndef SIMULATION
    tp_spi_add_device(1, 18);
    xpt2046_init();
#endif


    lv_indev_drv_init(&indev_drv);
#ifndef SIMULATION
    indev_drv.read_cb = input_driver_read ;
#else
    indev_drv.read_cb = ws_driver_input_read ;
#endif
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    lv_indev_t *indev = lv_indev_drv_register(&indev_drv);
    if(!indev) {
        printf("Cound not create indev\n") ;
    }

    lv_task_handler() ;

    return JS_DupValue(ctx,jsdisp) ;

excp:
    if(drawbuf)
        free(drawbuf) ;
    if(dispdrv)
        free(dispdrv) ;
    JS_FreeCString(ctx, typestr) ;
    return JS_EXCEPTION ;
}

uint8_t * display_dma_buff() {
    return dma_buff ;
}

void init_lvgl_display() {

#ifndef SIMULATION
    dma_buff = heap_caps_malloc( DMA_BUFF_LEN + DMA_BUFF_AUX_SIZE, MALLOC_CAP_DMA);
#else
    dma_buff = malloc(DMA_BUFF_LEN + DMA_BUFF_AUX_SIZE) ;
#endif
    if(!dma_buff) {
        printf("heap_caps_malloc(%d) faild for display buff.\n", DMA_BUFF_LEN) ;
    }

    // class id 全局, 分配一次
    JS_NewClassID(&js_lvgl_disp_class_id);

#ifndef SIMULATION
    vlgl_js_display_ws_init() ;
#endif
}




void require_vlgl_js_display(JSContext *ctx, JSValue lvgl) {
    
    // lvgl.Display
    JS_NewClass(JS_GetRuntime(ctx), js_lvgl_disp_class_id, &js_lvgl_disp_class);
    JSValue dispDriverProto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, dispDriverProto, js_display_proto_funcs, countof(js_display_proto_funcs));
    JS_SetClassProto(ctx, js_lvgl_disp_class_id, dispDriverProto);

    JS_SetPropertyStr(ctx, lvgl, "createDisplay", JS_NewCFunction(ctx, js_lvgl_create_display, "createDisplay", 1));

    JS_SetPropertyStr(ctx, lvgl, "actionDisplay", JS_NewCFunction(ctx, js_lvgl_set_default_display, "actionDisplay", 1));
}
