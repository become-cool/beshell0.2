#include "display.h"
#include "widgets_gen.h"
#include "widgets.h"
#include "module_lvgl.h"
#include "utils.h"
#include "cutils.h"

#include "lv_conf.h"

#ifndef SIMULATION
// #include "touch_driver.h"
#include "xpt2046.h"
#include "tp_spi.h"
#include "display_ws.h"
#include "disp_st77xx.h"
#include <freertos/queue.h>

#define OFFSET_X 11

#else
#include "http_lws.h"
#endif

uint8_t * disp_buff1 = NULL ;
uint8_t * disp_buff2 = NULL ;

lv_indev_drv_t indev_drv ;
lv_indev_t * indev = NULL ;
bool indev_fake = false ;
lv_coord_t indev_fake_x = 0 ;
lv_coord_t indev_fake_y = 0 ;
bool indev_fake_press = false ;

bool be_lv_fake_indev(lv_indev_data_t *data) {
    if(!indev_fake) {
        return false ;
    }
    indev_fake = false ;
    data->point.x = indev_fake_x ;
    data->point.y = indev_fake_y ;
    data->state = (indev_fake_press? LV_INDEV_STATE_PRESSED: LV_INDEV_STATE_RELEASED) ;
    data->continue_reading = false ;
    return true ;
}


JSContext * js_indev_global_cb_ctx = NULL ;
JSValue js_indev_global_cb_pressed ;
JSValue js_indev_global_cb_released ;
JSValue js_indev_global_cb_pressing ;

static JSValue js_set_indev_global_cb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_STRING(0, event) ;
    if( !JS_IsFunction(ctx, argv[1]) ) {
        JS_FreeCString(ctx, event) ;
        THROW_EXCEPTION("arg callback must be a function")
    }
    if( strcmp("pressed", event)==0 ) {
        js_indev_global_cb_ctx = ctx ;
        js_indev_global_cb_pressed = JS_DupValue(ctx, argv[1]) ;
    }
    else if( strcmp("pressing", event)==0 ) {
        js_indev_global_cb_ctx = ctx ;
        js_indev_global_cb_pressing = JS_DupValue(ctx, argv[1]) ;
    }
    else if( strcmp("released", event)==0 ) {
        js_indev_global_cb_ctx = ctx ;
        js_indev_global_cb_released = JS_DupValue(ctx, argv[1]) ;
    }
    else {
        JS_FreeCString(ctx, event) ;
        THROW_EXCEPTION("unknow event")
    }

    JS_FreeCString(ctx, event) ;

    return JS_UNDEFINED ;
}

static JSValue js_clear_indev_global_cb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_STRING(0, event) ;
    if( strcmp("pressed", event)==0 ) {
        js_indev_global_cb_ctx = NULL ;
        JS_FreeValue(ctx, js_indev_global_cb_pressed) ;
        js_indev_global_cb_pressed = JS_UNDEFINED ;
    }
    else if( strcmp("pressing", event)==0 ) {
        js_indev_global_cb_ctx = NULL ;
        JS_FreeValue(ctx, js_indev_global_cb_pressing) ;
        js_indev_global_cb_pressing = JS_UNDEFINED ;
    }
    else if( strcmp("released", event)==0 ) {
        js_indev_global_cb_ctx = NULL ;
        JS_FreeValue(ctx, js_indev_global_cb_released) ;
        js_indev_global_cb_released = JS_UNDEFINED ;
    }
    else {
        JS_FreeCString(ctx, event) ;
        THROW_EXCEPTION("unknow event")
    }

    JS_FreeCString(ctx, event) ;
    return JS_UNDEFINED ;
}


bool indev_last_pressed = false ;
void indev_global_cb_proc(lv_indev_data_t *data) {
    if( indev_last_pressed==false ) {
        // pressed
        if(data->state==LV_INDEV_STATE_PRESSED) {
            if( !JS_IsUndefined(js_indev_global_cb_pressed) ) {
                JSValue ret = JS_Call(js_indev_global_cb_ctx, js_indev_global_cb_pressed, JS_UNDEFINED, 0, NULL);
                if(JS_IsException(ret)) {
                    js_std_dump_error(js_indev_global_cb_ctx) ;
                }
                JS_FreeValue(js_indev_global_cb_ctx, ret) ;
            }
        }
    }
    else {
        if(data->state == LV_INDEV_STATE_PRESSED) {
            if( !JS_IsUndefined(js_indev_global_cb_pressing) ) {
                JSValue ret = JS_Call(js_indev_global_cb_ctx, js_indev_global_cb_pressing, JS_UNDEFINED, 0, NULL);
                if(JS_IsException(ret)) {
                    js_std_dump_error(js_indev_global_cb_ctx) ;
                }
                JS_FreeValue(js_indev_global_cb_ctx, ret) ;
            }
        }
        else {
            if( !JS_IsUndefined(js_indev_global_cb_released) ) {
                JSValue ret = JS_Call(js_indev_global_cb_ctx, js_indev_global_cb_released, JS_UNDEFINED, 0, NULL);
                if(JS_IsException(ret)) {
                    js_std_dump_error(js_indev_global_cb_ctx) ;
                }
                JS_FreeValue(js_indev_global_cb_ctx, ret) ;
            }
        }
    }
    indev_last_pressed = (data->state==LV_INDEV_STATE_PRESSED) ;
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

bool lock_spi_transfering = false ;
uint8_t * dma_buff = NULL ;

QueueHandle_t disp_queue;


spi_device_handle_t device_touch = NULL;

// static void task_disp(void *arg) {

//     draw_param_t * param ;
    
//     while(1) {
// 		xQueueReceive(disp_queue, &param, portMAX_DELAY);

//         // printf("draw %d,%d - %d,%d \n", param->x1,param->y1, param->x2, param->y2) ;

//         memcpy(dma_buff, param->buff, DMA_BUFF_LEN) ;
        
//         lock_spi_transfering = true ;
//         st77xx_draw_rect(param->disp->user_data, param->x1,param->y1, param->x2, param->y2, dma_buff) ;
//         lock_spi_transfering = false ;

//         lv_disp_flush_ready(param->disp) ;
// 	}
// }

void ws_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) {}

void disp_st7789_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) {
    if(!disp->user_data) {
        printf("spidev is NULL\n") ;
        return ;
    }

    // draw_param.x1 = area->x1 ;
    // draw_param.x2 = area->x2 ;
    // draw_param.y1 = area->y1 ;
    // draw_param.y2 = area->y2 ;
    // draw_param.buff = (uint8_t*)color_p ;
    // draw_param.disp = disp ;

    // draw_param_t * pp = &draw_param ;
	// xQueueSend(disp_queue, (void *)&pp, 0);

    st77xx_draw_rect(disp->user_data, area->x1,area->y1, area->x2, area->y2, color_p) ;
    lv_disp_flush_ready(disp) ;
}


void input_driver_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
    // if(ws_driver_input_read(drv, data))
    //     return ;

    if(!be_lv_fake_indev(data)) {

        if(lock_spi_transfering) {
            // printf("spi lock") ;
            data->point.x = indev_input_x ;
            data->point.y = indev_input_y ;
            data->state = indev_input_pressed? LV_INDEV_STATE_PRESSED: LV_INDEV_STATE_RELEASED ;
            return ;
        }
        else {
            data->continue_reading = xpt2046_read(drv, data) ;
            if( data->point.x > OFFSET_X ) {
                data->point.x -= OFFSET_X ;
            }
        }
    }

    indev_input_x = data->point.x ;
    indev_input_y = data->point.y ;
    indev_input_pressed = (data->state == LV_INDEV_STATE_PRESSED) ;

    indev_global_cb_proc(data) ;
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
        // if(disp->driver->draw_buf->buf1) {
        //     free(disp->driver->draw_buf->buf1) ;
        // }
        // if(disp->driver->draw_buf->buf2) {
        //     free(disp->driver->draw_buf->buf2) ;
        // }
        free(disp->driver->draw_buf) ;
    }

    // free(disp->driver) ;
    // free(disp) ;
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

static JSValue js_lvgl_disp_layer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, int layer_type) {
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
    JSValue jslayer = js_lv_obj_wrapper(ctx, layer, JS_UNDEFINED, lv_obj_js_class_id()) ;
    return JS_DupValue(ctx, jslayer) ;
    // return js_lv_obj_wrapper(ctx, layer, JS_UNDEFINED, lv_obj_js_class_id()) ;
}


static const JSCFunctionListEntry js_display_proto_funcs[] = {
    JS_CFUNC_DEF("activeScreen", 0, js_lvgl_disp_active_screen),
    JS_CFUNC_MAGIC_DEF("sysLayer", 0, js_lvgl_disp_layer, 1),
    JS_CFUNC_MAGIC_DEF("topLayer", 0, js_lvgl_disp_layer, 2),
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

    if(!disp_buff1) {
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
    if(disp_buff2) {
        lv_disp_draw_buf_init(drawbuf, disp_buff1+DMA_BUFF_AUX_SIZE, disp_buff2+DMA_BUFF_AUX_SIZE, buflen/2);
    }
    else {
        lv_disp_draw_buf_init(drawbuf, disp_buff1+DMA_BUFF_AUX_SIZE, NULL, buflen/2);
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
    tp_spi_add_device(1, 18, &device_touch);
    xpt2046_init();
#endif


    lv_indev_drv_init(&indev_drv);
#ifndef SIMULATION
    indev_drv.read_cb = input_driver_read ;
#else
    indev_drv.read_cb = ws_driver_input_read ;
#endif
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev = lv_indev_drv_register(&indev_drv);
    indev->driver->gesture_limit = 30 ;
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

void be_module_lvgl_malloc_buffer() {    
#ifndef SIMULATION
    dma_buff = heap_caps_malloc( DMA_BUFF_LEN + DMA_BUFF_AUX_SIZE, MALLOC_CAP_DMA);
    if(!dma_buff) {
        printf("heap_caps_malloc(%d) faild for display buff.\n", DMA_BUFF_LEN) ;
    }
    disp_buff1 = dma_buff ;
    disp_buff2 = NULL ;
#else
    disp_buff1 = malloc(DMA_BUFF_LEN + DMA_BUFF_AUX_SIZE) ;
    disp_buff2 = malloc(DMA_BUFF_LEN + DMA_BUFF_AUX_SIZE) ;
#endif
}


void be_lv_display_init() {
    // class id 全局, 分配一次
    JS_NewClassID(&js_lvgl_disp_class_id);

#ifndef SIMULATION
    vlgl_js_display_ws_init() ;
    
	// disp_queue = xQueueCreate(1, sizeof(draw_param_t *));
	// xTaskCreatePinnedToCore(task_disp, "task_disp", 2048, NULL, 5, NULL, 1);
#endif
}

static JSValue js_lvgl_tick_indev(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    lv_indev_read_timer_cb(indev_drv.read_timer) ;
    return JS_UNDEFINED ;
}

static JSValue js_lvgl_fake_indev(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(3)

    int32_t x, y ;

    JS_ToInt32(ctx, &x, argv[0]) ;
    indev_fake_x = x ;

    JS_ToInt32(ctx, &y, argv[1]) ;
    indev_fake_y = y ;

    indev_fake_press = JS_ToBool(ctx, argv[2]) ;

    indev_fake = true ;


    return JS_UNDEFINED ;
}



void be_lv_display_require(JSContext *ctx, JSValue lvgl) {
    
    js_indev_global_cb_ctx = NULL ;
    js_indev_global_cb_pressed = JS_UNDEFINED;
    js_indev_global_cb_released = JS_UNDEFINED;
    js_indev_global_cb_pressing = JS_UNDEFINED;
    
    // lvgl.Display
    JS_NewClass(JS_GetRuntime(ctx), js_lvgl_disp_class_id, &js_lvgl_disp_class);
    JSValue dispDriverProto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, dispDriverProto, js_display_proto_funcs, countof(js_display_proto_funcs));
    JS_SetClassProto(ctx, js_lvgl_disp_class_id, dispDriverProto);

    JS_SetPropertyStr(ctx, lvgl, "createDisplay", JS_NewCFunction(ctx, js_lvgl_create_display, "createDisplay", 1));
    JS_SetPropertyStr(ctx, lvgl, "actionDisplay", JS_NewCFunction(ctx, js_lvgl_set_default_display, "actionDisplay", 1));
    JS_SetPropertyStr(ctx, lvgl, "fakeIndev", JS_NewCFunction(ctx, js_lvgl_fake_indev, "fakeIndev", 1));
    JS_SetPropertyStr(ctx, lvgl, "tickIndev", JS_NewCFunction(ctx, js_lvgl_tick_indev, "tickIndev", 1));

    JS_SetPropertyStr(ctx, lvgl, "setIndevCallback", JS_NewCFunction(ctx, js_set_indev_global_cb, "setIndevCallback", 1));
    JS_SetPropertyStr(ctx, lvgl, "clearIndevCallback", JS_NewCFunction(ctx, js_clear_indev_global_cb, "clearIndevCallback", 1));
}

void be_lv_display_reset(JSContext * ctx) {

    js_indev_global_cb_ctx = NULL ;

    JS_FreeValue(ctx, js_indev_global_cb_pressed) ;
    js_indev_global_cb_pressed = JS_UNDEFINED;
    
    JS_FreeValue(ctx, js_indev_global_cb_released) ;
    js_indev_global_cb_released = JS_UNDEFINED;
    
    JS_FreeValue(ctx, js_indev_global_cb_pressing) ;
    js_indev_global_cb_pressing = JS_UNDEFINED;
    

    // 清理 disp (timer,indev,lv_obj_t) / driver 
    lv_disp_t * disp = NULL ;
    while((disp=lv_disp_get_default())!=false) {
        if(disp->driver){
            if(disp->driver->draw_buf) {
                free(disp->driver->draw_buf) ;
            }
#ifndef SIMULATION
            st77xx_dev_t * dev = (st77xx_dev_t*)disp->driver->user_data ;
            if(dev) {
                spi_bus_remove_device(dev->spi_dev) ;
                free(dev) ;
            }
            if(device_touch) {
                spi_bus_remove_device(device_touch) ;
                device_touch = NULL ;
            }
#endif
            free(disp->driver) ;
        }
        lv_disp_remove(disp) ;
    }

    if(indev) {
        lv_indev_remove(indev) ;
        indev = NULL ;
    }
}
