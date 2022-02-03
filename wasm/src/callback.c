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