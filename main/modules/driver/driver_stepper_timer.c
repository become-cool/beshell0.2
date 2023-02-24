#include "driver_stepper_timer.h"
#include "utils.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include <string.h>
#include "eventloop.h"


typedef struct {

    JSContext * ctx ;
    void * jsobj ;

    int8_t io_step ;
    int8_t io_dir ;
    int8_t io_limit ;
    int8_t io_en ;

    esp_timer_handle_t timer ;

    bool is_running:1 ;
    bool level:1 ;      // io step 的脉冲电平
    bool run_dir:1 ;
    bool use_dest:1 ;
    bool use_passing_by:1 ; // "passing-by" 事件

    uint16_t freq ;             // 当前频率 (电机当前速度)
    uint16_t freq_target ;      // 目标频率 (电机目标速度)
    uint16_t freq_v ;           // 频率速度 (电机加速度)
    
    int64_t pos ;       // 当前位置
    int64_t dest ;      // 目标位置 
    int64_t passing_by ;    // 触发 "passing-by" 事件的位置

} driver_stepper_timer_t ;

#define MAX_FREQ 3500


#define THIS_STEPPER(var)                                                   \
    driver_stepper_timer_t * var = JS_GetOpaque(this_val, js_stepper_timer_class_id);   \
    if(!var) {                                                              \
        THROW_EXCEPTION("must be called as a driver.Stepper method")        \
    }


static void emit_stop_event(driver_stepper_timer_t * stepper, bool sync) {

    JSValue eventName = JS_NewString(stepper->ctx, "stop") ;
    MAKE_ARGV3( argv, eventName, JS_NewInt64(stepper->ctx, stepper->pos), JS_NewUint32(stepper->ctx, stepper->run_dir) ) ;

    JSValue jsobj = JS_MKPTR(JS_TAG_OBJECT,stepper->jsobj) ;
    JSValue emit = JS_GetPropertyStr(stepper->ctx, jsobj, "emit") ;

    if(sync) {
        JS_Call(stepper->ctx, emit, jsobj, 3, argv) ;
        JS_FreeValue(stepper->ctx, eventName) ;
        free(argv) ;
    }
    else {
        eventloop_push_with_argv(stepper->ctx, emit, JS_DupValue(stepper->ctx, jsobj), 3, argv) ;
    }

    JS_FreeValue(stepper->ctx, emit) ;
}

static void emit_passing_by_event(driver_stepper_timer_t * stepper) {

    JSValue eventName = JS_NewString(stepper->ctx, "passing-by") ;
    MAKE_ARGV2( argv, eventName, JS_NewInt64(stepper->ctx, stepper->passing_by) ) ;

    JSValue jsobj = JS_MKPTR(JS_TAG_OBJECT,stepper->jsobj) ;
    JSValue emit = JS_GetPropertyStr(stepper->ctx, jsobj, "emit") ;

    eventloop_push_with_argv(stepper->ctx, emit, JS_DupValue(stepper->ctx, jsobj), 2, argv) ;

    JS_FreeValue(stepper->ctx, emit) ;
}

static void stepper_timer(void *arg) {
    driver_stepper_timer_t * stepper = (driver_stepper_timer_t*)arg ;
    if(!stepper || !stepper->is_running) {
        return ;
    }

    gpio_set_level(stepper->io_step, stepper->level) ;

    if(stepper->level) {
        stepper->pos+= stepper->run_dir? 1: -1 ;

        // passing_by 事件
        if(stepper->use_passing_by) {
            if(stepper->pos == stepper->passing_by) {
                emit_passing_by_event(stepper) ;
            }
        }

        // 停止
        if(stepper->use_dest) {
            if(stepper->run_dir) {
                stepper->is_running = stepper->dest > stepper->pos ;
            }
            else {
                stepper->is_running = stepper->dest < stepper->pos ;
            }
            if(!stepper->is_running) {
                esp_timer_stop(stepper->timer) ;
                emit_stop_event(stepper, false) ;
            }
        }
    }

    stepper->level = !stepper->level ;
}

static JSClassID js_stepper_timer_class_id ;
static JSValue js_stepper_timer_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    JSValue jsobj = JS_NewObjectClass(ctx, js_stepper_timer_class_id) ;

    driver_stepper_timer_t * HMALLOC(stepper, sizeof(driver_stepper_timer_t))
    if(!stepper) {
        THROW_EXCEPTION("out of memory?")
    }
    memset(stepper, 0, sizeof(driver_stepper_timer_t)) ;
    stepper->freq = 200 ;
    
    CHECK_ARGC(1)

    // gpio
    ARGV_TO_INT8(0, io_step)
    ARGV_TO_INT8_OPT(1, io_dir, -1)
    ARGV_TO_INT8_OPT(2, io_limit, -1)
    ARGV_TO_INT8_OPT(3, io_en, -1)
    
    stepper->io_step = io_step ;
    stepper->io_dir = io_dir ;
    stepper->io_limit = io_limit ;
    stepper->io_en = io_en ;

    // stepup gpio
    gpio_config_t cfg = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL<<io_step),
        .pull_down_en = false,
        .pull_up_en = false
    };
    CALL_IDF_API( gpio_config(&cfg) )

    if(io_dir>-1) {
        cfg.intr_type = GPIO_INTR_DISABLE ;
        cfg.mode = GPIO_MODE_OUTPUT ;
        cfg.pin_bit_mask = (1ULL<<io_dir) ;
        cfg.pull_down_en = false ;
        cfg.pull_up_en = false ;
        CALL_IDF_API( gpio_config(&cfg) )
    }


    const esp_timer_create_args_t timer_args = {
        .callback = &stepper_timer,
        .arg = stepper ,
        .name = "soft-stepper"
    };
    CALL_IDF_API( esp_timer_create(&timer_args, &(stepper->timer)) )

    JS_SetOpaque(jsobj, stepper) ;
    JS_SetPropertyStr(ctx, jsobj, "_handlers", JS_NewObject(ctx));

    stepper->ctx = ctx ;
    stepper->jsobj = JS_VALUE_GET_PTR(jsobj) ;

    return jsobj ;
}
static void js_stepper_timer_finalizer(JSRuntime *rt, JSValue this_val){
    driver_stepper_timer_t * stepper = JS_GetOpaque(this_val, js_stepper_timer_class_id);
    if(!stepper) {
        printf("must be called as a driver.SoftStepper method\n") ;
    }

    if(stepper->is_running) {
        stepper->is_running = false ;
        emit_stop_event(stepper, true) ;
    }

    esp_timer_stop(stepper->timer) ;
    esp_timer_delete(stepper->timer) ;

    if(stepper) {
        free(stepper) ;
        stepper = NULL ;
        JS_SetOpaque(this_val, NULL) ;
    }

}
static JSClassDef js_stepper_timer_class = {
    "driver.Stepper",
    .finalizer = js_stepper_timer_finalizer,
} ;


static inline JSValue set_freq(JSContext *ctx, driver_stepper_timer_t * stepper, int freq) {
    if(freq<=0 || freq>MAX_FREQ) {
        THROW_EXCEPTION("arg freq must >0 && <%d", MAX_FREQ)
    }
    stepper->freq = freq ;
    stepper->freq_target = freq ;
    
    if(stepper->is_running) {
        CALL_IDF_API( esp_timer_stop(stepper->timer) )
        CALL_IDF_API( esp_timer_start_periodic(stepper->timer, 500000/stepper->freq) )
    }

    return JS_UNDEFINED ;
}

static JSValue run(JSContext *ctx, driver_stepper_timer_t * stepper) {

    if(stepper->io_dir>-1) {
        CALL_IDF_API( gpio_set_level(stepper->io_dir, stepper->run_dir) )
    }

    stepper->level = 0 ;
    CALL_IDF_API( esp_timer_start_periodic(stepper->timer, 500000/stepper->freq) )

    stepper->is_running = true ;

    return JS_UNDEFINED ;
}

/**
 * @param forward = true
 * @param freq ?
 */
static JSValue js_stepper_timer_run(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    THIS_STEPPER(stepper)
    if(stepper->is_running) {
        THROW_EXCEPTION("stepper is running already")
    }
    stepper->run_dir = 1 ;
    if(argc>0) {
        stepper->run_dir = JS_ToBool(ctx, argv[0]) ;
    }
    if(argc>1) {
        ARGV_TO_INT32(1, freq)
        if( JS_IsException(set_freq(ctx, stepper, freq)) ) {
            return JS_EXCEPTION ;
        }
    }
    stepper->use_dest = false ;
    return run(ctx, stepper) ;
}

/**
 * @param steps
 * @param freq ?
 */
static JSValue js_stepper_timer_run_steps(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(1)
    THIS_STEPPER(stepper)
        if(stepper->is_running) {
        THROW_EXCEPTION("stepper is running already")
    }
    ARGV_TO_INT64(0, steps) ;

    stepper->run_dir = steps<0? 0: 1 ;
    stepper->use_dest = true ;
    stepper->dest = stepper->pos + steps ;

    if(argc>1) {
        ARGV_TO_INT32(1, freq)
        if( JS_IsException(set_freq(ctx, stepper, freq)) ) {
            return JS_EXCEPTION ;
        }
    }

    return run(ctx, stepper) ;
}

/**
 * @param steps
 * @param freq ?
 */
static JSValue js_stepper_timer_run_to(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(1)
    THIS_STEPPER(stepper)
    if(stepper->is_running) {
        THROW_EXCEPTION("stepper is running already")
    }
    ARGV_TO_INT64(0, dest) ;

    stepper->use_dest = true ;
    stepper->dest = dest ;
    
    stepper->run_dir = dest>stepper->pos? 1: 0 ;    

    if(argc>1) {
        ARGV_TO_INT32(1, freq)
        if( JS_IsException(set_freq(ctx, stepper, freq)) ) {
            return JS_EXCEPTION ;
        }
    }

    return run(ctx, stepper) ;
}

static JSValue js_stepper_timer_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)

    esp_timer_stop(stepper->timer) ;

    if(stepper->is_running) {
        emit_stop_event(stepper, true) ;
        stepper->is_running = false ;
    }

    return JS_UNDEFINED ;
}


static JSValue js_stepper_timer_set_freq(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_STEPPER(stepper)
    
    ARGV_TO_INT32(0, freq) ;
    return set_freq(ctx, stepper, freq) ;
}

static JSValue js_stepper_timer_get_freq(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)
    return JS_NewUint32(ctx, stepper->freq) ;
}

static JSValue js_stepper_timer_is_running(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)
    return JS_NewBool(ctx, stepper->is_running) ;
}

static JSValue js_stepper_timer_set_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_STEPPER(stepper)
    ARGV_TO_INT64(0, pos) ;
    stepper->pos = pos ;
    return JS_UNDEFINED ;
}
static JSValue js_stepper_timer_pos(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)
    return JS_NewInt64(ctx, stepper->pos) ;
}

static JSValue js_stepper_timer_get_step_pin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)
    return JS_NewInt32(ctx, stepper->io_step) ;
}
static JSValue js_stepper_timer_get_dir_pin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)
    return JS_NewInt32(ctx, stepper->io_dir) ;
}
static JSValue js_stepper_timer_get_limit_pin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_STEPPER(stepper)
    return JS_NewInt32(ctx, stepper->io_limit) ;
}



static JSValue js_stepper_timer_set_passing_by(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_STEPPER(stepper)
    ARGV_TO_INT64(0, passing_by) ;
    stepper->passing_by = passing_by ;
    stepper->use_passing_by = true ;
    return JS_UNDEFINED ;
}


static JSValue js_stepper_timer_clear_passing_by(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    THIS_STEPPER(stepper)
    stepper->use_passing_by = false ;
    return JS_UNDEFINED ;
}


static const JSCFunctionListEntry js_stepper_timer_proto_funcs[] = {
    JS_CFUNC_DEF("run", 0, js_stepper_timer_run),
    JS_CFUNC_DEF("runSteps", 0, js_stepper_timer_run_steps),
    JS_CFUNC_DEF("runTo", 0, js_stepper_timer_run_to),
    JS_CFUNC_DEF("stop", 0, js_stepper_timer_stop),
    JS_CFUNC_DEF("setFreq", 0, js_stepper_timer_set_freq),
    JS_CFUNC_DEF("freq", 0, js_stepper_timer_get_freq),
    JS_CFUNC_DEF("isRunning", 0, js_stepper_timer_is_running),
    JS_CFUNC_DEF("pos", 0, js_stepper_timer_pos),
    JS_CFUNC_DEF("setPos", 0, js_stepper_timer_set_pos),
    JS_CFUNC_DEF("stepPin", 0, js_stepper_timer_get_step_pin),
    JS_CFUNC_DEF("dirPin", 0, js_stepper_timer_get_dir_pin),
    JS_CFUNC_DEF("limitPin", 0, js_stepper_timer_get_limit_pin),
    JS_CFUNC_DEF("setPassingBy", 0, js_stepper_timer_set_passing_by),
    JS_CFUNC_DEF("clearPassingBy", 0, js_stepper_timer_clear_passing_by),
} ;


void be_module_driver_stepper_timer_init() {
    JS_NewClassID(&js_stepper_timer_class_id);
}

void be_module_driver_stepper_timer_require(JSContext *ctx,JSValue driver) {

    JSValue EventEmitterProto = js_get_glob_prop(ctx, 3, "beapi", "EventEmitter", "prototype") ;
    QJS_DEF_CLASS(stepper_timer, "SoftStepper", "driver.SoftStepper", EventEmitterProto, driver)
    JS_FreeValue(ctx, EventEmitterProto);

    // JS_SetPropertyStr(ctx, driver_stepper_timer, "xxxOoo", JS_NewCFunction(ctx, js_xxx_ooo, "xxxOoo", 1));


}

void be_module_driver_stepper_timer_loop(JSContext *ctx) {
}

void be_module_driver_stepper_timer_reset(JSContext *ctx) {
}

