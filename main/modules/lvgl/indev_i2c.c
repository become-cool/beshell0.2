#include "indev.h"
#include "indev_i2c.h"
#include "utils.h"
#include "widgets_gen.h"
#include "eventloop.h"


#include "module_serial.h"


// ------------
// InDevNav 

JSClassID js_lv_indev_nav_class_id = 0 ;

static inline void _indev_nav_set_value(indev_driver_spec_t * driver_spec, uint32_t v) {
    driver_spec->data.buttons.release = driver_spec->data.buttons.release | ((~v) & driver_spec->data.buttons.state) ;
    driver_spec->data.buttons.press = driver_spec->data.buttons.press | ((~driver_spec->data.buttons.state) & v) ;
    driver_spec->data.buttons.state = v ;
}


/*
#define INDEV_I2C_READ(spec, value)         \
    I2C_BEGIN_READ(spec->conf.i2c.addr) ;   \
    I2C_RECV(&value,1) ;                    \
    I2C_COMMIT(spec->conf.i2c.bus) ;
*/
inline bool indev_nav_read_i2c(indev_driver_spec_t* spec, uint8_t * byte) {

    I2C_BEGIN_READ(spec->conf.i2c.addr) ;
    I2C_RECV(byte,1) ;
    I2C_COMMIT(spec->conf.i2c.bus) ;

    spec->found = ESP_OK==res ;

    return spec->found ;
}


static void indev_post_js_event(indev_driver_spec_t * spec, const char * event_name, const char * key) {

    if( !spec->enable_jsobj_event ) {
        return ;
    }

    MAKE_ARGV2(argv
        , JS_NewString(spec->ctx, event_name)
        , JS_NewString(spec->ctx, key)
    )

    JSValue jsobj = JS_MKPTR(JS_TAG_OBJECT, spec->jsobj) ;
    JSValue emit = JS_GetPropertyStr(spec->ctx, jsobj,"emit") ;

    eventloop_push_with_argv(spec->ctx, emit, JS_DupValue(spec->ctx, jsobj), 2, argv) ;

    JS_FreeValue(spec->ctx, emit);
}


#define PROC_EVENT(mem, KEY, INDEV_KEY, STATE, keyName, keyStat)        \
    if(driver_spec->data.buttons.mem&NAVKEY_##KEY) {                    \
        driver_spec->data.buttons.mem&= ~NAVKEY_##KEY ;                 \
        if(data && drv) {                                               \
            data->key = INDEV_KEY ;                                     \
            data->state = LV_INDEV_STATE_##STATE ;                      \
            indev_emit_js_event(drv, driver_spec, "ipt.btn."keyStat, keyName ) ; \
        }                                                               \
        else {                                                          \
            indev_post_js_event(driver_spec ,keyStat, keyName) ;        \
        }                                                               \
        return true ;                                                   \
    }
#define PROC_PRESS(key, NAV_KEY, INDEV_KEY)      PROC_EVENT(press,NAV_KEY, INDEV_KEY,PRESSED, key, "press")
#define PROC_RELEASE(key, NAV_KEY, INDEV_KEY)    PROC_EVENT(release,NAV_KEY, INDEV_KEY,RELEASED, key, "release")

static void indev_nav_process_value(indev_driver_spec_t * driver_spec, struct _lv_indev_drv_t * drv, lv_indev_data_t * data) {

    if(driver_spec->data.buttons.semaphore) {
        if( xSemaphoreTake(driver_spec->data.buttons.semaphore, 5) != pdTRUE ){
            return ;
        }
    }

    _indev_nav_set_value(driver_spec, driver_spec->data.buttons.value) ;
    
    if(driver_spec->data.buttons.semaphore) {
        xSemaphoreGive(driver_spec->data.buttons.semaphore) ;
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

static void indev_nav_read_cb(struct _lv_indev_drv_t * drv, lv_indev_data_t * data) {
    if(!drv->user_data) {
        return ;
    }
    indev_driver_spec_t * driver_spec = (indev_driver_spec_t*) drv->user_data ;
    if(driver_spec->fake) {
        driver_spec->fake = false ;
    }
    else if(!driver_spec->found) {
        return ;
    }
    indev_nav_process_value(driver_spec, drv, data) ;
}


static void task_nav_read(indev_driver_spec_t * spec) {
    while(1) {
        if( xSemaphoreTake(spec->data.buttons.semaphore, 5) == pdTRUE ){
            indev_nav_read_i2c(spec, &spec->data.buttons.value) ;
            xSemaphoreGive(spec->data.buttons.semaphore) ;

            // 如果已经注册到 lvgl ，则由 lvgl 处理读到的数据
            if( ! spec->lv_indev ) {
                indev_nav_process_value(spec, NULL, NULL) ;
            }
        }
        vTaskDelay(10/portTICK_PERIOD_MS) ;
    }
}

static JSValue js_lv_indev_nav_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){

    if(_indev_id>=255){
        THROW_EXCEPTION("max indev count 255") ;
    }

    indev_driver_spec_t * HMALLOC(driver_spec, sizeof(indev_driver_spec_t));
    memset(driver_spec, 0, sizeof(indev_driver_spec_t)) ;

    driver_spec->id = _indev_id ++ ;

    CHECK_ARGC(1)
    ARGV_TO_STRING_E(0, driverName, "arg driver must be a string")

    // 设置了 i2c 参数
    if(argc>1) {

        CHECK_ARGC(3)

        if(strcmp(driverName, "joypad")==0) {
            driver_spec->driver = INDEV_DRIVER_JOYPAD ;
        }
        else {
            JS_FreeCString(ctx, driverName) ;
            THROW_EXCEPTION("unknow nav input device driver")
        }
        
        ARGV_TO_UINT8(1, bus)
        ARGV_TO_UINT8(2, addr)

        driver_spec->conf.i2c.bus = bus ;
        driver_spec->conf.i2c.addr = addr ;

        driver_spec->found = i2c_ping(bus,addr) ;

        if(!driver_spec->found) {
            printf("not found i2c device, bus: %d, addr: %d \n", bus,addr) ;
        }

        driver_spec->data.buttons.semaphore = xSemaphoreCreateMutex();
        xTaskCreatePinnedToCore(&task_nav_read, "task_nav_read", 1024*2, (void *)driver_spec, 5, NULL, 1) ;
    }

    // 未设置 i2c 参数, 做为 fake 虚拟输入设备
    else {

        if(strcmp(driverName, "fake")!=0) {
            JS_FreeCString(ctx, driverName) ;
            THROW_EXCEPTION("unknow nav input device driver")
        }

        driver_spec->driver = INDEV_DRIVER_FAKE ;
    }

    JS_FreeCString(ctx, driverName) ;

    // 注册到 lvgl
    lv_indev_drv_t * indev_drv = malloc(sizeof(lv_indev_drv_t));
    lv_indev_drv_init(indev_drv);
    indev_drv->type = LV_INDEV_TYPE_KEYPAD;
    indev_drv->read_cb = indev_nav_read_cb ;
    indev_drv->user_data = driver_spec ;

    lv_indev_t * indev = lv_indev_drv_register(indev_drv);
    if(!indev) {
        THROW_EXCEPTION("register indev drv failded")
    }

    driver_spec->lv_indev = indev ;
    
    lv_task_handler() ;

    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_indev_nav_class_id) ;
    JS_SetOpaque(jsobj, indev) ;
    JS_SetPropertyStr(ctx, jsobj, "_handlers", JS_NewObject(ctx));

    driver_spec->jsobj = JS_VALUE_GET_PTR(jsobj) ;
    driver_spec->ctx = ctx ;

    // 调用 lv_indev_remove() 后解除这个引用
    JS_DupValue(ctx, jsobj) ;

    return jsobj ;
}
static void js_lv_indev_nav_finalizer(JSRuntime *rt, JSValue this_val){
    printf("js_lv_indev_nav_finalizer()\n") ;

    lv_indev_t * indev = (lv_indev_t *)JS_GetOpaque(this_val, js_lv_indev_nav_class_id) ;
    if(indev && indev->driver) {

        // 从 lvgl 中注销
        lv_indev_remove(indev) ;

        if(indev->driver) {

            if(indev->driver->user_data) {
                indev_driver_spec_t * spec = (indev_driver_spec_t *)indev->driver->user_data;       

                spec->lv_indev = NULL ;

                if(spec->data.buttons.semaphore) {
                    vSemaphoreDelete(spec->data.buttons.semaphore) ;
                    spec->data.buttons.semaphore = NULL ;
                }

                free(spec) ;
                spec = NULL ;
            }

            if(indev->driver) {
                free(indev->driver) ;
                indev->driver = NULL ;
            }
        }
            
        free(indev) ;

        JS_SetOpaque(this_val, NULL) ;
    }
}
static JSClassDef js_lv_indev_nav_class = {
    "lv.InDevNav",
    .finalizer = js_lv_indev_nav_finalizer,
} ;

static JSValue js_lv_indev_nav_set(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, v)

    THIS_SPEC(spec)
    if(!spec) {
        THROW_EXCEPTION("invalid indev")
    }

    _indev_nav_set_value(spec, v) ;
    spec->fake = true ;

    return JS_UNDEFINED ;
}

static JSValue js_lv_indev_nav_unset(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, v)

    THIS_SPEC(spec)
    if(!spec) {
        THROW_EXCEPTION("invalid indev")
    }

    v = spec->data.buttons.state & (~v) ;
    _indev_nav_set_value(spec, v) ;
    spec->fake = true ;

    return JS_UNDEFINED ;
}

static JSValue js_lv_indev_nav_enable_event(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_SPEC(spec)
    if(!spec) {
        THROW_EXCEPTION("invalid indev")
    }
    spec->enable_jsobj_event = true ;
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
    THIS_SPEC(spec)
    if(!spec) {
        THROW_EXCEPTION("invalid indev")
    }

    printf(
        "state=" BYTE_TO_BINARY_PATTERN ", press=" BYTE_TO_BINARY_PATTERN ", release=" BYTE_TO_BINARY_PATTERN "\n" ,
        BYTE_TO_BINARY(spec->data.buttons.state), BYTE_TO_BINARY(spec->data.buttons.press), BYTE_TO_BINARY(spec->data.buttons.release)
    ) ;

    return JS_UNDEFINED ;
}

static const JSCFunctionListEntry js_lv_indev_nav_proto_funcs[] = {
    JS_CFUNC_DEF("set", 0, js_lv_indev_nav_set),
    JS_CFUNC_DEF("unset", 0, js_lv_indev_nav_unset),
    JS_CFUNC_DEF("enableEvent", 0, js_lv_indev_nav_enable_event),
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

