#include "display.h"
#include "display_ws.h"
#include "utils.h"
#include "cutils.h"

#include "lvgl.h"
#include "lv_conf.h"

#include "lvgl_touch/tp_spi.h"
#include "disp_st77xx.h"
#include "touch_driver.h"

#define DMA_BUFF_LEN 320*20*2
uint8_t * dma_buff = NULL ;

lv_indev_drv_t indev_drv;
static void btn_event_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        static uint8_t cnt = 0;
        cnt++;

        /*Get the first child of the button which is the label and change its text*/
        lv_obj_t * label = lv_obj_get_child(btn, 0);
        lv_label_set_text_fmt(label, "Button: %d", cnt);
    }
}


void disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) {
    // printf("disp_flush() (%d,%d) -> (%d,%d) \n", area->x1,area->y1, area->x2, area->y2) ;
    if(!disp->user_data) {
        printf("spidev is NULL\n") ;
        return ;
    }
    st77xx_draw_rect(disp->user_data, area->x1,area->y1, area->x2, area->y2, color_p) ;
    lv_disp_flush_ready(disp) ;
}



static JSClassID js_lvgl_disp_class_id ;

static void js_lvgl_disp_finalizer(JSRuntime *rt, JSValue val) {

    printf("js_lvgl_disp_finalizer()\n") ;

    lv_disp_t * disp = (lv_disp_t *)JS_GetOpaque(val, js_lvgl_disp_class_id) ;
    lv_disp_remove(disp) ;

    // spi 设备
    if(disp->driver->user_data) {
        st77xx_dev_t * dev = (st77xx_dev_t*)disp->driver->user_data ;
        spi_bus_remove_device(dev->spi_dev) ;
        free(dev) ;
    }

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
    "lvgl.DispDriver",
    .finalizer = js_lvgl_disp_finalizer,
};


static JSValue js_lvgl_disp_fillRect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(4)

    ARGV_TO_UINT16(0, x1)
    ARGV_TO_UINT16(1, y1)
    ARGV_TO_UINT16(2, x2)
    ARGV_TO_UINT16(3, y2)
    // ARGV_TO_UINT8(4, r)
    // ARGV_TO_UINT8(5, g)
    // ARGV_TO_UINT8(6, b)

    uint16_t size = (x2-x1+1) * (y2-y1+1) * 2;
    uint8_t * buff = malloc(size) ;
    memset(buff,0,size) ;
    
    st77xx_dev_t * spidev = ((lv_disp_t *)JS_GetOpaque(this_val, js_lvgl_disp_class_id))->driver->user_data ;
    if(!spidev) {
        THROW_EXCEPTION("JS_GetOpaque(this_val, js_lvgl_disp_class_id) is NULL")
    }
    st77xx_draw_rect(spidev, x1,y1, x2, y2, (uint16_t*)buff) ;

    return JS_UNDEFINED ;
}

static JSValue js_lvgl_disp_action_screen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}
static JSValue js_lvgl_disp_load_screen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}

static const JSCFunctionListEntry js_display_proto_funcs[] = {
    JS_CFUNC_DEF("actionScreen", 0, js_lvgl_disp_action_screen),
    JS_CFUNC_DEF("loadScreen", 0, js_lvgl_disp_load_screen),
    JS_CFUNC_DEF("fillRect", 0, js_lvgl_disp_fillRect),
};

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

    JSValue jsdisp = JS_NewObjectClass(ctx, js_lvgl_disp_class_id) ;

    JS_SetPropertyStr(ctx, jsdisp, "type", argv[0]) ;
    JS_SetPropertyStr(ctx, jsdisp, "options", argv[1]) ;


    ARGV_TO_STRING_E(0, typestr, typelen, "argv type must be a string")
    if(!JS_IsObject(argv[1])) {
        THROW_EXCEPTION("argv options must be a object")
    }

    if( strcmp(typestr, "ST7789")==0 ) {

        GET_INT_PROP(argv[1], "cs", cs, excp)
        GET_INT_PROP(argv[1], "dc", dc, excp)
        GET_INT_PROP(argv[1], "width", width, excp)
        GET_INT_PROP(argv[1], "height", height, excp)
        GET_INT_PROP_DEFAULT(argv[1], "spi", spi, 1, excp)
        GET_INT_PROP_DEFAULT(argv[1], "freq", freq, 26000000, excp)

        // printf("spi=%d, cs=%d, dc=%d, freq=%d, width=%d, height=%d\n",spi,cs,dc,freq,width,height) ;

        // 初始化 spi
        st77xx_dev_t * spidev = malloc(sizeof(st77xx_dev_t));
        st77xx_spi_init(spidev, spi, cs, dc, freq);
        st77xx_init(spidev, width, height, 0, 0);

        // 创建缓冲区
        lv_disp_draw_buf_t * drawbuf = malloc(sizeof(lv_disp_draw_buf_t)) ;
        size_t buflen = (DMA_BUFF_LEN / width) * width ;
        // printf("dma buff len: %d\n", buflen) ;
        lv_disp_draw_buf_init(drawbuf, dma_buff, NULL, buflen/2);  

        // 创建设备驱动对象
        lv_disp_drv_t * dispdrv = malloc(sizeof(lv_disp_drv_t)) ;
        lv_disp_drv_init(dispdrv); 
        dispdrv->flush_cb = disp_flush ;
        dispdrv->draw_buf = drawbuf;
        dispdrv->hor_res = width;
        dispdrv->ver_res = height;
        dispdrv->user_data = spidev ;
        lv_disp_t * disp = lv_disp_drv_register(dispdrv); 

        JS_SetOpaque(jsdisp, disp);


        // demo ------------
        // 触摸设备
        tp_spi_add_device(1, 18);
        xpt2046_init();

        lv_indev_drv_init(&indev_drv);
        indev_drv.read_cb = touch_driver_read;
        indev_drv.type = LV_INDEV_TYPE_POINTER;
        lv_indev_t *indev = lv_indev_drv_register(&indev_drv);
        if(!indev) {
            printf("Cound create indev\n") ;
        }

        lv_obj_t * btn = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
        lv_obj_set_pos(btn, 10, 10);                            /*Set its position*/
        lv_obj_set_size(btn, 120, 50);                          /*Set its size*/
        lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/

        lv_obj_t * label = lv_label_create(btn);          /*Add a label to the button*/
        lv_label_set_text(label, "Button");                     /*Set the labels text*/
        lv_obj_center(label);

        lv_task_handler() ;

        return jsdisp ;
    }
    else {
        JS_ThrowReferenceError(ctx, "unknow disp driver: %s", typestr);
        goto excp ;
    }

excp:
    JS_FreeCString(ctx, typestr) ;
    return JS_EXCEPTION ;
}



void vlgl_js_display_init() {

    
    printf("heap_caps_get_free_size(MALLOC_CAP_DMA): %d\n", heap_caps_get_free_size(MALLOC_CAP_DMA)) ;
    dma_buff = heap_caps_malloc( DMA_BUFF_LEN, MALLOC_CAP_DMA);
    if(!dma_buff) {
        printf("heap_caps_malloc(%d) faild for display buff.\n", DMA_BUFF_LEN) ;
    }

    // class id 全局, 分配一次
    JS_NewClassID(&js_lvgl_disp_class_id);

    vlgl_js_display_ws_init() ;
}




void require_vlgl_js_display(JSContext *ctx, JSValue lvgl) {

    printf("require_vlgl_js_display()\n") ;

    // lvgl.Display
    JS_NewClass(JS_GetRuntime(ctx), js_lvgl_disp_class_id, &js_lvgl_disp_class);
    JSValue dispDriverProto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, dispDriverProto, js_display_proto_funcs, countof(js_display_proto_funcs));
    JS_SetClassProto(ctx, js_lvgl_disp_class_id, dispDriverProto);

    JS_SetPropertyStr(ctx, lvgl, "createDisplay", JS_NewCFunction(ctx, js_lvgl_create_display, "createDisplay", 1));

    // JS_SetPropertyStr(ctx, lvgl, "setDefaultDisplay", JS_NewCFunction(ctx, js_lvgl_set_default_display, "setDefaultDisplay", 1));
}
