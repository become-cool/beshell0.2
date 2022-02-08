#include <stdlib.h>
#include <emscripten.h>
#include "lv_conf.h"
#include "lvgl.h"
#include "../../main/debug.h"

static void _cb_adapter_disp_drv_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p) {
    // printf("lv_wasm_disp_drv_flush_cb()%d,%d,%d,%d, disp: %p\n", area->x1,area->y1, area->x2, area->y2, dispdrv) ;
    EM_ASM_ARGS({
        Module.onDispDrvFlush && Module.onDispDrvFlush($0,$1,$2,$3,$4,$5)
    }, drv, area->x1,area->y1, area->x2, area->y2, color_p);
    lv_disp_flush_ready(drv) ;
}

EMSCRIPTEN_KEEPALIVE void * cb_adapter_disp_drv_flush() {
    return (void *)_cb_adapter_disp_drv_flush;
}


static void _cb_adapter_indev_drv_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
    if(drv->user_data) {
        lv_indev_data_t * input_data = (lv_indev_data_t *) drv->user_data ;
        data->point.x = input_data->point.x ;
        data->point.y = input_data->point.y ;
        data->state = input_data->state ;
        // printf("cb_adapter_indev_drv_read(), %d, %d, %d\n", data->point.x, data->point.y, data->state) ;
    }
}

EMSCRIPTEN_KEEPALIVE void * cb_adapter_indev_drv_read(){
    return (void *)_cb_adapter_indev_drv_read;
}

