#ifndef __H_INDEV__
#define __H_INDEV__

#include "quickjs-libc.h"
#include <stdbool.h>
#include <stdint.h>
#include "lvgl.h"

#ifndef SIMULATION
#include "driver/spi_master.h"
#endif

typedef enum {
    INDEV_DRIVER_FAKE ,
    INDEV_DRIVER_XPT2046 ,
    INDEV_DRIVER_JOYPAD ,
} indev_driver_t ;

typedef struct {

    uint8_t id ;
    indev_driver_t driver ;
    
#ifndef SIMULATION
    union  {
        struct {
            spi_host_device_t handle ;
        } spi ;
        struct {
            uint8_t bus ;
            uint8_t addr ;
        } i2c ;
    } conf ;
#endif

    bool fake ;
    union{
        struct {
            lv_coord_t x ;
            lv_coord_t y ;
            lv_indev_state_t state ;
        } pointer ;
        struct {
            uint32_t state ;
            uint32_t press ;
            uint32_t release ;
        } buttons ;
    } data ;
} indev_driver_spec_t ;

typedef struct {
    indev_driver_spec_t spec ;

    lv_coord_t max_x ;
    lv_coord_t max_y ;
    bool swap_xy:1 ;
    bool inv_x:1 ;
    bool inv_y:1 ;

} indev_driver_spec_touch_t ;

indev_driver_spec_t * find_indev_spec_by_id(uint8_t id) ;

void be_lv_indev_reset(JSContext * ctx) ;
void be_lv_indev_init() ;
void be_lv_indev_require(JSContext *ctx, JSValue lvgl) ;

#endif