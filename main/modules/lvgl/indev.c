#include "indev.h"
#include "display.h"
#include "lvgl.h"
#include "widgets_gen.h"
#include "utils.h"
#include "cutils.h"
#include "module_serial.h"


#ifndef SIMULATION

#include "xpt2046.h"
#include "tp_spi.h"

// @todo 由用户校正
#define OFFSET_X 11

#endif

static JSClassID js_lv_indev_pointer_class_id ;
static JSClassID js_lv_indev_nav_class_id ;

typedef enum {
    INDEV_DRIVER_FAKE ,

    INDEV_DRIVER_XPT2046 ,
    INDEV_DRIVER_JOYPAD ,
} indev_driver_t ;

typedef struct {

    indev_driver_t driver ;
    union  {
        struct {
            spi_host_device_t handle ;
        } spi ;
        struct {
            uint8_t bus ;
            uint8_t addr ;
        } i2c ;
    } conf ;

    bool fake ;
    union  {
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


#define THIS_INDEV(thisobj)     \
    lv_indev_t * thisobj = (lv_indev_t *)JS_GetOpaque(this_val, js_lv_indev_pointer_class_id) ; \
    if(!thisobj) {  \
        thisobj = (lv_indev_t *)JS_GetOpaque(this_val, js_lv_indev_nav_class_id) ; \
    } \
    if(!thisobj) { \
        THROW_EXCEPTION("invalid indev obj") \
    }

#define THIS_SPEC(thisspec)                                 \
    THIS_INDEV(thisobj)                                     \
    if(!thisobj->driver || !thisobj->driver->user_data) {   \
        THROW_EXCEPTION("invalid indev obj")                \
    }                                                       \
    indev_driver_spec_t * thisspec = (indev_driver_spec_t *) thisobj->driver->user_data ;


inline static void indev_emit_js_event(lv_indev_drv_t * drv, indev_driver_spec_t * drv_spec, const char * event_name, const char * key) {
    if( !drv->disp || !drv->disp->driver || !drv->disp->driver->user_data ) {
        return ;
    }
    disp_drv_spec_t * disp_spec = (disp_drv_spec_t*)drv->disp->driver->user_data ;
    if(!disp_spec->enable_input_event || !disp_spec->jsobj) {
        return ;
    }
    JSValue jsobj = JS_MKPTR(JS_TAG_OBJECT, disp_spec->jsobj) ;
    JSValue emit = JS_GetPropertyStr(disp_spec->ctx,jsobj,"emit") ;
    if( JS_IsFunction(disp_spec->ctx, emit) ) {
        
        MAKE_ARGV2(argv, JS_NewString(disp_spec->ctx, event_name), JS_NewString(disp_spec->ctx, key)) 

        JSValue ret = JS_Call(disp_spec->ctx, emit, jsobj, 2, argv ) ;
        if (JS_IsException(ret)) {
            js_std_dump_error(disp_spec->ctx);
        }

        free(argv) ;
    }
    
    JS_FreeValue(disp_spec->ctx, emit);
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
static void indev_global_cb_proc(lv_indev_data_t *data) {
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
    } else {
        if(data->state == LV_INDEV_STATE_PRESSED) {
            if( !JS_IsUndefined(js_indev_global_cb_pressing) ) {
                JSValue ret = JS_Call(js_indev_global_cb_ctx, js_indev_global_cb_pressing, JS_UNDEFINED, 0, NULL);
                if(JS_IsException(ret)) {
                    js_std_dump_error(js_indev_global_cb_ctx) ;
                }
                JS_FreeValue(js_indev_global_cb_ctx, ret) ;
            }
        } else {
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



// ------------
// InDevPointer 

static void indev_pointer_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
    if(!drv->user_data) {
        return ;
    }
    indev_driver_spec_t * driver_spec = (indev_driver_spec_t *) drv->user_data ;

    if(driver_spec->fake) {
        driver_spec->fake = false ;
        data->point.x = driver_spec->data.pointer.x ;
        data->point.y = driver_spec->data.pointer.y ;
        data->state = driver_spec->data.pointer.state ;
        data->continue_reading = false ;
    }
#ifndef SIMULATION
    else if(INDEV_DRIVER_XPT2046 == driver_spec->driver) {
        data->continue_reading = xpt2046_read(drv, data) ;
        if( data->point.x > OFFSET_X ) {
            data->point.x -= OFFSET_X ;
        }
    }
#endif
    else {
        return ;
    }

    indev_global_cb_proc(data) ;
}

static JSValue js_lv_indev_pointer_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    ARGV_TO_STRING_E(0, driver, "arg driver must be a string")

    indev_driver_spec_t * driver_spec = NULL ;
    if(strcmp(driver, "fake")==0) {
        driver_spec = malloc(sizeof(indev_driver_spec_t)) ;
        driver_spec->driver = INDEV_DRIVER_FAKE ;
    }
#ifndef SIMULATION
    else if(strcmp(driver, "xpt2046")==0) {
        if(argc<3) {
            THROW_EXCEPTION("Missing argv")
        }

        ARGV_TO_UINT8(2, cs)
        ARGV_TO_UINT8(1, busnum)

        driver_spec = malloc(sizeof(indev_driver_spec_t)) ;
        driver_spec->driver = INDEV_DRIVER_XPT2046 ;

        esp_err_t res = tp_spi_add_device(busnum, cs, &driver_spec->conf.spi.handle) ;
        if(res!=ESP_OK) {
            free(driver_spec) ;
            THROW_EXCEPTION("spi_add_device failed: %d",res)
        }
        xpt2046_init() ;
    }
#endif
    else {
        THROW_EXCEPTION("unknow InDevPointer driver")
    }
    
    driver_spec->fake = false ;
    driver_spec->data.pointer.x = 0 ;
    driver_spec->data.pointer.y = 0 ;
    driver_spec->data.pointer.state = LV_INDEV_STATE_RELEASED ;

    lv_indev_drv_t * indev_drv = malloc(sizeof(lv_indev_drv_t)) ;
    lv_indev_drv_init(indev_drv);
    indev_drv->type = LV_INDEV_TYPE_POINTER;
    indev_drv->read_cb = indev_pointer_read ;
    indev_drv->user_data = driver_spec ;

    lv_indev_t * indev = lv_indev_drv_register(indev_drv) ;
    indev->driver->gesture_limit = 30 ;

    lv_task_handler() ;

    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_indev_pointer_class_id) ;
    JS_SetOpaque(jsobj, indev) ;
    return jsobj ;
}
static void js_lv_indev_pointer_finalizer(JSRuntime *rt, JSValue this_val){
    printf("js_lv_indev_pointer_finalizer()\n") ;
    lv_indev_t * thisobj = (lv_indev_t *)JS_GetOpaque(this_val, js_lv_indev_pointer_class_id) ;
    if(thisobj) {
        lv_indev_remove(thisobj) ;
        if(thisobj->driver) {
            if(thisobj->driver->user_data) {

                indev_driver_spec_t * pointer = (indev_driver_spec_t *) thisobj->driver->user_data ;
                #ifndef SIMULATION
                if(pointer->driver == INDEV_DRIVER_XPT2046) {
                    spi_bus_remove_device(& pointer->conf.spi.handle) ;
                }
                #endif

                free(thisobj->driver->user_data) ;
                thisobj->driver->user_data = NULL ;
                return ;
            }
            free(thisobj->driver) ;
            thisobj->driver = NULL ;
        }
        free(thisobj) ;
    }


}
static JSClassDef js_lv_indev_pointer_class = {
    "InDevPointer",
    .finalizer = js_lv_indev_pointer_finalizer,
} ;


static JSValue js_lv_indev_tick(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_INDEV(thisobj)
    lv_indev_read_timer_cb(thisobj->driver->read_timer) ;
    return JS_UNDEFINED ;
}


static JSValue js_lv_indev_pointer_set(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_OBJ("lv.InDevNav", "set", thisobj, lv_indev_t)
    CHECK_ARGC(3)
    ARGV_TO_INT32(0, x)
    ARGV_TO_INT32(1, y)
    bool state = JS_ToBool(ctx, argv[2]) ;

    indev_driver_spec_t * driver_spec = (indev_driver_spec_t *)thisobj->driver->user_data ;
    if(!driver_spec) {
        THROW_EXCEPTION("invalid indev")
    }
    driver_spec->fake = true ;
    driver_spec->data.pointer.x = x ;
    driver_spec->data.pointer.y = y ;
    driver_spec->data.pointer.state = state? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;

    return JS_UNDEFINED ;
}


static const JSCFunctionListEntry js_lv_indev_pointer_proto_funcs[] = {
    JS_CFUNC_DEF("set", 0, js_lv_indev_pointer_set),
    JS_CFUNC_DEF("tick", 0, js_lv_indev_tick),
} ;



// ------------
// InDevNav 
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

static void _indev_nav_set_value(indev_driver_spec_t * driver_spec, uint32_t v) {
    driver_spec->data.buttons.release = driver_spec->data.buttons.release | ((~v) & driver_spec->data.buttons.state) ;
    driver_spec->data.buttons.press = driver_spec->data.buttons.press | ((~driver_spec->data.buttons.state) & v) ;
    driver_spec->data.buttons.state = v ;
}

#define PROC_EVENT(mem, KEY, INDEV_KEY, STATE, keyName, eventName)      \
    if(driver_spec->data.buttons.mem&NAVKEY_##KEY) {                    \
        data->key = INDEV_KEY ;                                         \
        data->state = LV_INDEV_STATE_##STATE ;                          \
        driver_spec->data.buttons.mem&= ~NAVKEY_##KEY ;                 \
        indev_emit_js_event(drv, driver_spec, "ipt.btn."eventName, keyName ) ; \
        return ;                                                        \
    }
#define PROC_PRESS(key, NAV_KEY, INDEV_KEY)      PROC_EVENT(press,NAV_KEY, INDEV_KEY,PRESSED, key, "press")
#define PROC_RELEASE(key, NAV_KEY, INDEV_KEY)    PROC_EVENT(release,NAV_KEY, INDEV_KEY,RELEASED, key, "release")                        

static void indev_nav_read_cb(struct _lv_indev_drv_t * drv, lv_indev_data_t * data) {
    if(!drv->user_data) {
        return ;
    }
    indev_driver_spec_t * driver_spec = (indev_driver_spec_t*) drv->user_data ;

#ifndef SIMULATION
    if(driver_spec->driver==INDEV_DRIVER_JOYPAD) {
        uint8_t value = 0 ;
        I2C_BEGIN_READ(driver_spec->conf.i2c.addr) ;
        I2C_RECV(&value,1) ;
        I2C_COMMIT(driver_spec->conf.i2c.bus) ;
        if(ESP_OK==res) {
            _indev_nav_set_value(driver_spec, value) ;
        }
    }
#endif

    if(driver_spec->data.buttons.press) {
        PROC_PRESS("up", UP, LV_KEY_UP)
        PROC_PRESS("down", DOWN, LV_KEY_DOWN)
        PROC_PRESS("left", LEFT, LV_KEY_LEFT)
        PROC_PRESS("right", RIGHT, LV_KEY_RIGHT)
        PROC_PRESS("a", A, LV_KEY_ENTER)
        PROC_PRESS("b", B, LV_KEY_ESC)
        PROC_PRESS("start", START, LV_KEY_ENTER)
        PROC_PRESS("select", SELECT, LV_KEY_NEXT)
    }
    
    if(driver_spec->data.buttons.release) {
        PROC_RELEASE("up", UP, LV_KEY_UP)
        PROC_RELEASE("down", DOWN, LV_KEY_DOWN)
        PROC_RELEASE("left", LEFT, LV_KEY_LEFT)
        PROC_RELEASE("right", RIGHT, LV_KEY_RIGHT)
        PROC_RELEASE("a", A, LV_KEY_ENTER)
        PROC_RELEASE("b", B, LV_KEY_ESC)
        PROC_RELEASE("start", START, LV_KEY_ENTER)
        PROC_RELEASE("select", SELECT, LV_KEY_NEXT)
    }
}

static JSValue js_lv_indev_nav_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){

    indev_driver_spec_t driver_spec ;
    memset(&driver_spec, 0, sizeof(indev_driver_spec_t)) ;
    
    if(argc>0) {

        CHECK_ARGC(3)

        ARGV_TO_STRING_E(0, driverName, "arg driver must be a string")
        if(strcmp(driverName, "joypad")==0) {
            driver_spec.driver = INDEV_DRIVER_JOYPAD ;
        }
        else {
            THROW_EXCEPTION("unknow nav input device driver")
        }
        
        ARGV_TO_UINT8(2, addr)
        ARGV_TO_UINT8(1, bus)

        driver_spec.conf.i2c.bus = bus ;
        driver_spec.conf.i2c.addr = addr ;
    }
    
    lv_indev_drv_t * indev_drv = malloc(sizeof(lv_indev_drv_t));
    lv_indev_drv_init(indev_drv);
    indev_drv->type = LV_INDEV_TYPE_KEYPAD;
    indev_drv->read_cb = indev_nav_read_cb ;
    
    indev_drv->user_data = malloc(sizeof(indev_driver_spec_t)) ;
    memcpy(indev_drv->user_data, &driver_spec, sizeof(indev_driver_spec_t)) ;

    lv_indev_t * indev = lv_indev_drv_register(indev_drv);
    if(!indev) {
        THROW_EXCEPTION("register indev drv failded")
    }
    
    lv_task_handler() ;

    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_indev_nav_class_id) ;
    JS_SetOpaque(jsobj, indev) ;
    return jsobj ;
}
static void js_lv_indev_nav_finalizer(JSRuntime *rt, JSValue this_val){
    printf("js_lv_indev_nav_finalizer()\n") ;
    lv_indev_t * thisobj = (lv_indev_t *)JS_GetOpaque(this_val, js_lv_indev_nav_class_id) ;
    if(thisobj) {
        lv_indev_remove(thisobj) ;
        if(thisobj->driver) {
            if(thisobj->driver->user_data) {
                free(thisobj->driver->user_data) ;
                thisobj->driver->user_data = NULL ;
                return ;
            }
            free(thisobj->driver) ;
            thisobj->driver = NULL ;
        }
        free(thisobj) ;
    }
}
static JSClassDef js_lv_indev_nav_class = {
    "lv.InDevNav",
    .finalizer = js_lv_indev_nav_finalizer,
} ;
static JSValue js_lv_indev_nav_set(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, v)

    THIS_OBJ("lv.InDevNav", "set", thisobj, lv_indev_t)
    if(!thisobj->driver->user_data) {
        THROW_EXCEPTION("invalid indev")
    }

    _indev_nav_set_value((indev_driver_spec_t *) thisobj->driver->user_data, v) ;

    return JS_UNDEFINED ;
}

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 
static JSValue js_lv_indev_nav_state(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    THIS_OBJ("lv.InDevNav", "set", thisobj, lv_indev_t)
    indev_driver_spec_t * driver_spec = (indev_driver_spec_t *) thisobj->driver->user_data ;
    if(!driver_spec) {
        return JS_NULL ;
    }

    printf(
        "state=" BYTE_TO_BINARY_PATTERN ", press=" BYTE_TO_BINARY_PATTERN ", release=" BYTE_TO_BINARY_PATTERN "\n" ,
        BYTE_TO_BINARY(driver_spec->data.buttons.state), BYTE_TO_BINARY(driver_spec->data.buttons.press), BYTE_TO_BINARY(driver_spec->data.buttons.release)
    ) ;

    return JS_UNDEFINED ;
}


static const JSCFunctionListEntry js_lv_indev_nav_proto_funcs[] = {
    JS_CFUNC_DEF("set", 0, js_lv_indev_nav_set),
    JS_CFUNC_DEF("tick", 0, js_lv_indev_tick),
    JS_CFUNC_DEF("state", 0, js_lv_indev_nav_state),
} ;



void be_lv_indev_reset(JSContext * ctx) {

    js_indev_global_cb_ctx = NULL ;

    JS_FreeValue(ctx, js_indev_global_cb_pressed) ;
    js_indev_global_cb_pressed = JS_UNDEFINED;
    
    JS_FreeValue(ctx, js_indev_global_cb_released) ;
    js_indev_global_cb_released = JS_UNDEFINED;
    
    JS_FreeValue(ctx, js_indev_global_cb_pressing) ;
    js_indev_global_cb_pressing = JS_UNDEFINED;

}


void be_lv_indev_init() {
    JS_NewClassID(&js_lv_indev_nav_class_id);
    JS_NewClassID(&js_lv_indev_pointer_class_id);
}

void be_lv_indev_require(JSContext *ctx, JSValue lvgl) {

    QJS_DEF_CLASS(lv_indev_nav, "InDevNav", "lv.InDevNav", JS_UNDEFINED, lvgl)
    QJS_DEF_CLASS(lv_indev_pointer, "InDevPointer", "lv.InDevPointer", JS_UNDEFINED, lvgl)
    
    js_indev_global_cb_ctx = NULL ;
    js_indev_global_cb_pressed = JS_UNDEFINED;
    js_indev_global_cb_released = JS_UNDEFINED;
    js_indev_global_cb_pressing = JS_UNDEFINED;

    JS_SetPropertyStr(ctx, lvgl, "setIndevCallback", JS_NewCFunction(ctx, js_set_indev_global_cb, "setIndevCallback", 1));
    JS_SetPropertyStr(ctx, lvgl, "clearIndevCallback", JS_NewCFunction(ctx, js_clear_indev_global_cb, "clearIndevCallback", 1));

}
