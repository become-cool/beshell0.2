#include <stdlib.h>
#include <emscripten.h>
#include "lv_conf.h"
#include "lvgl.h"
#include "../../main/debug.h"


EMSCRIPTEN_KEEPALIVE lv_coord_t lv_coord_pct(uint16_t pct) {
    return LV_PCT(pct);
}
EMSCRIPTEN_KEEPALIVE bool lv_is_coord_pct(uint16_t pct) {
    return LV_COORD_IS_PCT(pct);
}

EMSCRIPTEN_KEEPALIVE lv_coord_t lv_coord_size_content() {
    return LV_SIZE_CONTENT;
}


EMSCRIPTEN_KEEPALIVE lv_coord_t lv_indev_data_get_point_x(lv_indev_data_t * pdata) {
    return pdata->point.x ;
}
EMSCRIPTEN_KEEPALIVE void lv_indev_data_set_point_x(lv_indev_data_t * pdata, lv_coord_t x) {
    pdata->point.x = x;
}
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_indev_data_get_point_y(lv_indev_data_t * pdata) {
    return pdata->point.y ;
}
EMSCRIPTEN_KEEPALIVE void lv_indev_data_set_point_y(lv_indev_data_t * pdata, lv_coord_t y) {
    pdata->point.y = y;
}



EMSCRIPTEN_KEEPALIVE void lv_obj_set_coord_x(lv_obj_t * obj, lv_coord_t val) {
    lv_coord_t dx = val - obj->coords.x1 ;
    lv_obj_set_x(obj, lv_obj_get_x_aligned(obj) + dx) ;
}
EMSCRIPTEN_KEEPALIVE void lv_obj_set_coord_y(lv_obj_t * obj, lv_coord_t val) {
    lv_coord_t dy = val - obj->coords.y1 ;
    lv_obj_set_y(obj, lv_obj_get_y_aligned(obj) + dy) ;
}
EMSCRIPTEN_KEEPALIVE void lv_obj_set_coords(lv_obj_t * obj, lv_coord_t x, lv_coord_t y) {
    lv_coord_t dx = x - obj->coords.x1 ;
    lv_obj_set_x(obj, lv_obj_get_x_aligned(obj) + dx) ;

    lv_coord_t dy = y - obj->coords.y1 ;
    lv_obj_set_y(obj, lv_obj_get_y_aligned(obj) + dy) ;
}

EMSCRIPTEN_KEEPALIVE lv_coord_t lv_obj_get_coord_x(lv_obj_t * obj) {
    return obj->coords.x1 ;
}
EMSCRIPTEN_KEEPALIVE lv_coord_t lv_obj_get_coord_y(lv_obj_t * obj) {
    return obj->coords.y1 ;
}
