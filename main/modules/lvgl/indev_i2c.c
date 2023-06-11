#include "indev.h"
#include "indev_i2c.h"
#include "utils.h"
#include "widgets_gen.h"
#include "eventloop.h"
#include "be_list.h"

#ifdef PLATFORM_ESP32
#include "module_serial.h"
#endif

void indev_keypad_proc(lv_indev_t * i, lv_indev_data_t * data);
void indev_proc_reset_query_handler(lv_indev_t * indev);

typedef struct {
    nav_key_t btn ;
    lv_key_t key ;
    const char * btnName ;
} btn_key_conf_t ;

btn_key_conf_t btn_key_conf[8] = {
    {NAVKEY_UP,LV_KEY_UP, "up"} ,
    {NAVKEY_DOWN,LV_KEY_DOWN, "down"} ,
    {NAVKEY_LEFT,LV_KEY_LEFT, "left"} ,
    {NAVKEY_RIGHT,LV_KEY_RIGHT, "right"} ,
    {NAVKEY_A,LV_KEY_ENTER, "a"} ,
    {NAVKEY_B,LV_KEY_ESC, "b"} ,
    {NAVKEY_START,LV_KEY_ENTER, "start"} ,
    {NAVKEY_SELECT,LV_KEY_NEXT, "select"} ,
} ;


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

#define dbb(byte) printf("%s="BYTE_TO_BINARY_PATTERN"\n", #byte, BYTE_TO_BINARY(byte)) ;

// ------------
// InDevNav List

static be_list_t * lst_devs = NULL ;

typedef struct {
    be_list_item_t base ;
    indev_driver_spec_t * spec ;
    uint8_t last_value ;
} indev_item_t ;

#ifdef PLATFORM_ESP32

TaskHandle_t task_nav_read_handle = NULL ;

static void task_nav_read(void * argv) {

    uint8_t value = 0 ;

    while(1) {

        if(!lst_devs->count) {
            vTaskDelay(50) ;
            continue;
        }
        
        FOREACH_TYPE_LIST(lst_devs, indev_item_t, item, {

            value = 0 ;
            indev_nav_read_i2c(item->spec, &value) ;

            if(value!=item->last_value) {

                xQueueSend( item->spec->data.buttons.queue, &value, 0 );

                // printf(BYTE_TO_BINARY_PATTERN "->" BYTE_TO_BINARY_PATTERN "\r\n"
                //         , BYTE_TO_BINARY(item->last_value)
                //         , BYTE_TO_BINARY(value)) ;

                item->last_value = value ;
            }
        }) 

        vTaskDelay(1) ;
    }
}


static void i2c_indev_create_task() {
    if(!task_nav_read_handle) {
        xTaskCreatePinnedToCore(&task_nav_read, "task_nav_read", 1024*3, NULL, 5, &task_nav_read_handle, 1) ;
    }
}
#endif

static void i2c_indev_init(indev_driver_spec_t * spec) {
    indev_item_t * item = malloc(sizeof(indev_item_t)) ;
    memset(item, 0, sizeof(indev_item_t)) ;

    item->spec = spec ;
    be_list_append(lst_devs, &item->base) ;

#ifdef PLATFORM_ESP32
    spec->data.buttons.queue = xQueueCreate(1, sizeof(uint8_t));
    i2c_indev_create_task() ;
#endif
}

static void i2c_indev_deinit(indev_driver_spec_t * spec) {

#ifdef PLATFORM_ESP32
    if(spec->data.buttons.queue) {
        vQueueDelete( spec->data.buttons.queue );
        spec->data.buttons.queue = NULL ;
    }
#endif

    FOREACH_TYPE_LIST(lst_devs, indev_item_t, item, {
        if(item->spec==spec) {
            be_list_remove(lst_devs, item) ;
            return ;
        }
    }) 
}

// ------------
// InDevNav 

JSClassID js_lv_indev_nav_class_id = 0 ;


#ifdef PLATFORM_ESP32

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

#endif


inline static void lvgl_proc_btn(indev_driver_spec_t * spec, uint32_t * unread, nav_key_t btn, lv_key_t key, lv_indev_state_t state, lv_indev_data_t *data) {

    data->key = key ;
    data->state = state ;

    (*unread) &= ~btn ;
    data->continue_reading = (*unread)? true: false ;

    printf("btn:%d, key:%d ,state:%d, continue:%d\n", btn, key, state, data->continue_reading) ;
    return ;
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


#ifdef PLATFORM_ESP32
        // echo_time("i2c_ping",{
            driver_spec->found = i2c_ping(bus,addr) ;
        // })
#endif

        if(!driver_spec->found) {
            printf("not found i2c device, bus: %d, addr: %d \n", bus,addr) ;
        }

        i2c_indev_init(driver_spec) ;
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
    indev_drv->read_cb = NULL ;
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

                i2c_indev_deinit(spec) ;
                
                // if(spec->data.buttons.semaphore) {
                //     vSemaphoreDelete(spec->data.buttons.semaphore) ;
                //     spec->data.buttons.semaphore = NULL ;
                // }

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


static void send_btns(indev_driver_spec_t * spec, uint32_t value) {

    lv_indev_data_t lvdata ;
    
    uint32_t changes = spec->data.buttons.state ^ value ;

    uint8_t btn = 0 ;
    for(int i=0;i<8;i++) {

        btn = (1<<i) ;
        
        if( btn & changes ) {

            lvdata.state = (btn & value)? LV_INDEV_STATE_PRESSED: LV_INDEV_STATE_RELEASED ;

            // js 事件
            indev_emit_js_event(spec, lvdata.state? "ipt.btn.press": "ipt.btn.release", btn_key_conf[i].btnName ) ;

            // lvgl 事件
            if( spec->lv_indev )  {

                lvdata.key = btn_key_conf[i].key ; 

                spec->lv_indev->proc.state = lvdata.state ;

                indev_proc_reset_query_handler(spec->lv_indev);                     
                indev_keypad_proc(spec->lv_indev, &lvdata); 
                indev_proc_reset_query_handler(spec->lv_indev); 

                // printf("btn: %d, key: %d, state: %d\n", btn, lvdata.key, lvdata.state) ;
            }

        }
        
    }

    spec->data.buttons.state = value ;
}


static JSValue js_lv_indev_nav_set(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, v)

    THIS_SPEC(spec)
    if(!spec) {
        THROW_EXCEPTION("invalid indev")
    }

    send_btns(spec, v) ;

    return JS_UNDEFINED ;
}
static JSValue js_lv_indev_nav_unset(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, v)

    THIS_SPEC(spec)
    if(!spec) {
        THROW_EXCEPTION("invalid indev")
    }

    send_btns(spec, spec->data.buttons.state&(~v)) ;

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

static JSValue js_lv_indev_nav_state(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_SPEC(spec)
    if(!spec) {
        THROW_EXCEPTION("invalid indev")
    }

    printf(
        "state=" BYTE_TO_BINARY_PATTERN "\n"
        , BYTE_TO_BINARY(spec->data.buttons.state)//, BYTE_TO_BINARY(spec->data.buttons.press), BYTE_TO_BINARY(spec->data.buttons.release)
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

    if(!lst_devs) {
        lst_devs = malloc(sizeof(be_list_t)) ;
        memset(lst_devs,0,sizeof(be_list_t)) ;
        be_list_init(lst_devs) ;
    }

}

void be_indev_i2c_require(JSContext *ctx, JSValue lvgl, JSValue baseProto) {
    QJS_DEF_CLASS(lv_indev_nav, "InDevNav", "lv.InDevNav", baseProto, lvgl)
}

#ifdef PLATFORM_ESP32
inline uint8_t indev_nav_take_value(indev_driver_spec_t * spec) {
    uint8_t value = 0 ;
    if( xQueueReceive(spec->data.buttons.queue, &value, 0) == pdTRUE ) {
        spec->data.buttons.state = value ;
        return value ;
    }
    else {
        return spec->data.buttons.state ;
    }
}

inline void be_indev_i2c_loop(JSContext *ctx) {
    uint8_t value = 0 ;
    FOREACH_TYPE_LIST(lst_devs, indev_item_t, item, {
        if( xQueueReceive(item->spec->data.buttons.queue, &value, 0) == pdTRUE ) {
            send_btns(item->spec,value) ;
        }
    })
}
#endif

void be_indev_i2c_reset(JSContext *ctx) {

    // @todo 再次 deinit 所有 lst_devs 中的item

    // @todo delete task_nav_read_handle 任务
}

