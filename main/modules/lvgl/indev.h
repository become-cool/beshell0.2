#ifndef __H_INDEV__
#define __H_INDEV__

#include "quickjs-libc.h"
#include <stdbool.h>
#include <stdint.h>
#include "lvgl.h"
#include "list.h"

#ifndef SIMULATION
#include "driver/spi_master.h"
#include "driver/i2c.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#endif

typedef enum {
    INDEV_DRIVER_FAKE ,
    INDEV_DRIVER_XPT2046 ,
    INDEV_DRIVER_JOYPAD ,
    INDEV_DRIVER_BTN ,
} indev_driver_t ;


typedef enum {
    NAVKEY_UP = 0x1 ,
    NAVKEY_DOWN = 0x2 ,
    NAVKEY_LEFT = 0x4 ,
    NAVKEY_RIGHT = 0x8 ,
    NAVKEY_A = 0x10 ,
    NAVKEY_B = 0x20 ,
    NAVKEY_START = 0x40 ,
    NAVKEY_SELECT = 0x80 ,
} nav_key_t ;


// #define INPUT_QUEUE_SIZE 10

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
        struct {
            uint8_t gpio ;
            uint8_t key ;
            uint8_t trigger ;
        } btn ;
    } conf ;
#endif

    bool fake:1 ;
    bool found:1 ;
    bool enable_jsobj_event:1 ;  // 在 js 对象上触发事件
    
    union{
        struct {
            lv_coord_t x ;
            lv_coord_t y ;
            lv_indev_state_t state ;
        } pointer ;
        struct {
            uint32_t state ;
            uint32_t unread_pressed ;
            uint32_t unread_released ;
            // uint32_t press ;
            // uint32_t release ;

            // uint8_t value ;
#ifndef SIMULATION
            QueueHandle_t queue ;
#endif

            // bool unread ;

            // list_t queue ;
            // uint8_t queue_length ;
            // uint8_t queue[INPUT_QUEUE_SIZE] ;
            // uint8_t queue_length ;  // 如果超出 INPUT_QUEUE_SIZE, 则等于 0xFF

#ifndef SIMULATION
            SemaphoreHandle_t semaphore ;
#endif
        } buttons ;
        struct {
            uint32_t code ;
            lv_indev_state_t state ;
        } key ;
    } data ;

    JSContext * ctx ;
    JSValue jsobj ;

    lv_indev_t * lv_indev ;

} indev_driver_spec_t ;

typedef struct {
    indev_driver_spec_t spec ;

    lv_coord_t max_x ;
    lv_coord_t max_y ;
    lv_coord_t offset_x ;
    lv_coord_t offset_y ;
    bool swap_xy:1 ;
    bool inv_x:1 ;
    bool inv_y:1 ;

} indev_driver_spec_touch_t ;

indev_driver_spec_t * find_indev_spec_by_id(uint8_t id) ;
void indev_emit_js_event(indev_driver_spec_t * drv_spec, const char * event_name, const char * key) ;
void indev_global_cb_proc(lv_indev_data_t *data) ;

extern uint8_t _indev_id ;

extern JSClassID js_lv_indev_pointer_class_id ;
extern JSClassID js_lv_indev_nav_class_id ;

#ifndef SIMULATION
#define THIS_INDEV(thisobj)     \
    lv_indev_t * thisobj = (lv_indev_t *)JS_GetOpaque(this_val, js_lv_indev_pointer_class_id) ; \
    if(!thisobj) {  \
        thisobj = (lv_indev_t *)JS_GetOpaque(this_val, js_lv_indev_nav_class_id) ; \
    } \
    if(!thisobj) { \
        THROW_EXCEPTION("invalid indev obj") \
    }
#else
#define THIS_INDEV(thisobj)     \
    lv_indev_t * thisobj = (lv_indev_t *)JS_GetOpaque(this_val, js_lv_indev_pointer_class_id) ; \
    if(!thisobj) { \
        THROW_EXCEPTION("invalid indev obj") \
    }
#endif

#define THIS_SPEC(thisspec)                                 \
    THIS_INDEV(thisobj)                                     \
    if(!thisobj->driver || !thisobj->driver->user_data) {   \
        THROW_EXCEPTION("invalid indev obj")                \
    }                                                       \
    indev_driver_spec_t * thisspec = (indev_driver_spec_t *) thisobj->driver->user_data ;




void be_lv_indev_reset(JSContext * ctx) ;
void be_lv_indev_init() ;
void be_lv_indev_require(JSContext *ctx, JSValue lvgl) ;

#endif