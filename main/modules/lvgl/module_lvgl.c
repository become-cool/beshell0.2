#include "module_lvgl.h"
#include "lvgl.h"
#include "lv_conf.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"


#include "websocket_driver.h"


#define DISP_HOR_RES 320
#define DISP_VER_RES 240

#define LV_TICK_PERIOD_MS 1



void my_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) {
    int32_t x, y;
    /*It's a very slow but simple implementation.
     *`set_pixel` needs to be written by you to a set pixel on the screen*/
    for(y = area->y1; y <= area->y2; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            // set_pixel(x, y, *color_p);
            color_p++;
        }
    }
    lv_disp_flush_ready(disp);         /* Indicate you are ready with the flushing*/
}

void lv_tick_task(void *arg) {
    (void) arg;
    lv_tick_inc(LV_TICK_PERIOD_MS);
}

void init_lvgl() {

    printf("LV_COLOR_DEPTH=%d\n", LV_COLOR_DEPTH) ;

    lv_init();
    websocket_driver_init() ;

    static lv_disp_draw_buf_t draw_buf;                   
    lv_color_t * buf1 = heap_caps_malloc(DISP_HOR_RES * DISP_VER_RES / 10, MALLOC_CAP_DMA);

    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, DISP_HOR_RES * DISP_VER_RES / 10);  


    static lv_disp_drv_t disp_drv;        /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);          /*Basic initialization*/
    disp_drv.flush_cb = websocket_driver_flush;    /*Set your driver function*/
    disp_drv.draw_buf = &draw_buf;        /*Assign the buffer to the display*/
    disp_drv.hor_res = DISP_HOR_RES;   /*Set the horizontal resolution of the display*/
    disp_drv.ver_res = DISP_VER_RES;   /*Set the vertical resolution of the display*/
    lv_disp_drv_register(&disp_drv);      /*Finally register the driver*/



    // lvgl 时钟
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "lvgl"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));
}





JSValue js_lvgl_init(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}

void require_module_lvgl(JSContext *ctx) {


    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beapi = JS_GetPropertyStr(ctx, global, "beapi") ;

    JSValue ui = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "ui", ui);

    JS_SetPropertyStr(ctx, ui, "init", JS_NewCFunction(ctx, js_lvgl_init, "init", 1));


    JS_FreeValue(ctx, global);
    JS_FreeValue(ctx, beapi);

}