#include "indev.h"
#include "indev_i2c.h"
#include "utils.h"
#include "widgets_gen.h"


#include "module_serial.h"


// ------------
// InDevNav 

JSClassID js_lv_indev_nav_class_id = 0 ;

static inline void _indev_nav_set_value(indev_driver_spec_t * driver_spec, uint32_t v) {
    driver_spec->data.buttons.release = driver_spec->data.buttons.release | ((~v) & driver_spec->data.buttons.state) ;
    driver_spec->data.buttons.press = driver_spec->data.buttons.press | ((~driver_spec->data.buttons.state) & v) ;
    driver_spec->data.buttons.state = v ;
}

#define PROC_EVENT(mem, KEY, INDEV_KEY, STATE, keyName, keyStat)        \
    if(driver_spec->data.buttons.mem&NAVKEY_##KEY) {                    \
        data->key = INDEV_KEY ;                                         \
        data->state = LV_INDEV_STATE_##STATE ;                          \
        driver_spec->data.buttons.mem&= ~NAVKEY_##KEY ;                 \
        indev_emit_js_event(drv, driver_spec, "ipt.btn."keyStat, keyName ) ; \
        return ;                                                        \
    }
#define PROC_PRESS(key, NAV_KEY, INDEV_KEY)      PROC_EVENT(press,NAV_KEY, INDEV_KEY,PRESSED, key, "press")
#define PROC_RELEASE(key, NAV_KEY, INDEV_KEY)    PROC_EVENT(release,NAV_KEY, INDEV_KEY,RELEASED, key, "release")                        


#define INDEV_I2C_READ(spec, value)         \
    I2C_BEGIN_READ(spec->conf.i2c.addr) ;   \
    I2C_RECV(&value,1) ;                    \
    I2C_COMMIT(spec->conf.i2c.bus) ;

static void indev_nav_read_cb(struct _lv_indev_drv_t * drv, lv_indev_data_t * data) {
    if(!drv->user_data) {
        return ;
    }
    indev_driver_spec_t * driver_spec = (indev_driver_spec_t*) drv->user_data ;
    
    uint8_t value ;
    INDEV_I2C_READ(driver_spec, value)
    if(ESP_OK==res) {
        _indev_nav_set_value(driver_spec, value) ;
    }

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

static void task_nav_read(indev_driver_spec_t * spec) {

    const TickType_t xDelay = 5/portTICK_PERIOD_MS ;
    uint8_t value = 0 ;
    uint8_t last = 0 ;

    while(1) {

        INDEV_I2C_READ(spec, value)
        if(ESP_OK==res) {
            if(value!=last) {
                printf("i2c indev:%d\n",value) ;
                last = value ;
            }
            _indev_nav_set_value(spec, value) ;
        }

        vTaskDelay(xDelay) ;
    }
}

static JSValue js_lv_indev_nav_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){

    if(lv_disp_get_default()==NULL) {
        THROW_EXCEPTION("There is no default display.")
    }
    if(_indev_id>=255){
        THROW_EXCEPTION("max indev count 255") ;
    }

    indev_driver_spec_t driver_spec ;
    memset(&driver_spec, 0, sizeof(indev_driver_spec_t)) ;

    driver_spec.id = _indev_id ++ ;

    if(argc>0) {

        CHECK_ARGC(3)

        ARGV_TO_STRING_E(0, driverName, "arg driver must be a string")
        if(strcmp(driverName, "joypad")==0) {
            driver_spec.driver = INDEV_DRIVER_JOYPAD ;
        }
        else {
            THROW_EXCEPTION("unknow nav input device driver")
        }
        
        ARGV_TO_UINT8(1, bus)
        ARGV_TO_UINT8(2, addr)

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

    // xTaskCreatePinnedToCore(&task_nav_read, "task_nav_read", 1024*2, (void *)indev_drv->user_data, 5, NULL, 1) ;
    
    lv_task_handler() ;

    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_indev_nav_class_id) ;
    JS_SetOpaque(jsobj, indev) ;
    return jsobj ;
}
static void js_lv_indev_nav_finalizer(JSRuntime *rt, JSValue this_val){
    // printf("js_lv_indev_nav_finalizer()\n") ;
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
    // JS_CFUNC_DEF("tick", 0, js_lv_indev_tick),
    // JS_CFUNC_DEF("setGroup", 0, js_lv_indev_set_group),
    // JS_CFUNC_DEF("id", 0, js_lv_indev_id),

    JS_CFUNC_DEF("state", 0, js_lv_indev_nav_state),
} ;




void be_indev_i2c_init() {
    JS_NewClassID(&js_lv_indev_nav_class_id);
}

void be_indev_i2c_require(JSContext *ctx, JSValue lvgl, JSValue baseProto) {

    QJS_DEF_CLASS(lv_indev_nav, "InDevNav", "lv.InDevNav", baseProto, lvgl)
    
}

void be_indev_i2c_loop(JSContext *ctx) {
}

void be_indev_i2c_reset(JSContext *ctx) {
}

