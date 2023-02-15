#include "driver_stepper.h"
#include "utils.h"
#include "rom/gpio.h"
#include "driver/ledc.h"
#include "driver/mcpwm.h"
#include "driver/pcnt.h"
#include "soc/pcnt_struct.h"
#include "hal/ledc_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include <string.h>
#include "eventloop.h"

#define MAX_PCNT_VALUE 30000
#define MIN_PCNT_VALUE -30000

typedef struct {

    JSContext * ctx ;
    void * jsobj ;
    
    int8_t io_step ;
    int8_t io_dir ;
    int8_t io_en ;
    int8_t io_limit ;

    // ledc_timer_t ledc_timer ;
    // ledc_channel_t ledc_channel ;
    // ledc_mode_t speed_mode ;

    mcpwm_unit_t mcpwm_unit ;
    mcpwm_timer_t mcpwm_timer ;

    pcnt_unit_t pcnt_unit ;
    // pcnt_channel_t pcnt_channel ;
    pcnt_isr_handle_t counter_isr_handle ;

    bool running ;

    // 连续运动 run_steps<0 
    int16_t run_steps ;
    uint8_t run_dir ;
    uint32_t steps_over_counter ;

} driver_stepper_t ;

#define THIS_STEPPER(var)                                                   \
    driver_stepper_t * var = JS_GetOpaque(this_val, js_stepper_class_id);   \
    if(!var) {                                                              \
        THROW_EXCEPTION("must be called as a driver.Stepper method")        \
    }

#define STEPPER_LEDC_INIT_BITS ((ledc_timer_bit_t)7)
#define STEPPER_LEDC_DUTY  ((1ULL << STEPPER_LEDC_INIT_BITS) / 2 - 1)
#define STEPPER_LEDC_INIT_FREQ 1000
#define STEPPER_CNT_H  10000
#define STEPPER_CNT_L  -10000

static JSClassID js_stepper_class_id ;


inline static void emit_stop_event(driver_stepper_t * stepper, bool sync) {
    
    int count = 0 ;
    pcnt_get_counter_value(stepper->pcnt_unit, &count) ;

    JSValue eventName = JS_NewString(stepper->ctx, "stop") ;
    MAKE_ARGV3( argv, eventName, JS_NewUint32(stepper->ctx, count), JS_NewUint32(stepper->ctx, stepper->run_dir) ) ;

    JSValue emit = js_get_glob_prop(stepper->ctx, 4, "beapi", "EventEmitter", "prototype", "emit") ;

    if(sync) {
        dd
        // dn( JS_IsUndefined(stepper->jsobj) )
        JS_Call(stepper->ctx, emit, JS_MKPTR(JS_TAG_OBJECT,stepper->jsobj), 3, argv) ;
        JS_FreeValue(stepper->ctx, eventName) ;
    }
    else {
        eventloop_push_with_argv(stepper->ctx, emit, JS_DupValue(stepper->ctx, JS_MKPTR(JS_TAG_OBJECT,stepper->jsobj)), 3, argv) ;
    }

    JS_FreeValue(stepper->ctx, emit) ;
}

static void IRAM_ATTR stepper_pcnt_intr_handler(void *arg) {
    if(!arg) {
        return ;
    }
    driver_stepper_t * stepper = (driver_stepper_t *) arg ;

    uint32_t status ;
    pcnt_get_event_status(stepper->pcnt_unit, &status);

    // 结束
    if(stepper->run_steps>0 && status&PCNT_EVT_THRES_0 ) {
        mcpwm_stop(stepper->mcpwm_unit, stepper->mcpwm_timer);
        stepper->running = false ;
        
        // 事件
        emit_stop_event(stepper, false) ;
    }


    // 计数器溢出
    if( status&PCNT_EVT_H_LIM ) {
        pcnt_counter_clear(stepper->pcnt_unit) ;
        stepper->steps_over_counter ++ ;
    }

    PCNT.int_clr.val = 1ULL<<stepper->pcnt_unit;
}


/**
 * step (0-39)
 * dir (0-39)
 * en (0-39)
 * mcpwm = 0-3
 * pcnt_unit = PCNT_UNIT_0
 * 
 */
static JSValue js_stepper_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){

    CHECK_ARGC(2)

    // gpio
    ARGV_TO_INT8(0, io_step)
    ARGV_TO_INT8(1, io_dir)
    ARGV_TO_INT8_OPT(2, io_en, -1)
    
    // mcpwm
    ARGV_TO_INT32_OPT(3, mcpwm_unit, MCPWM_UNIT_0)
    ARGV_TO_INT32_OPT(4, mcpwm_timer, MCPWM_TIMER_0)

    // // pcnt
    ARGV_TO_INT32_OPT(5, pcnt_unit, PCNT_UNIT_0)

    // malloc
    driver_stepper_t * HMALLOC(stepper, sizeof(driver_stepper_t))
    if(!stepper) {
        THROW_EXCEPTION("out of memory?")
    }
    memset(stepper, 0, sizeof(driver_stepper_t)) ;
    stepper->io_step = io_step ;
    stepper->io_dir = io_dir ;
    stepper->io_en = io_en ;
    stepper->mcpwm_unit = mcpwm_unit ;
    stepper->mcpwm_timer = mcpwm_timer ;
    stepper->pcnt_unit = pcnt_unit ;
    
    JSValue jsobj = JS_NewObjectClass(ctx, js_stepper_class_id) ;

    // setup pcnt
    pcnt_config_t cfg_pcnt = {
        .pulse_gpio_num = stepper->io_step,
        .ctrl_gpio_num = -1, //stepper->io_dir,
        .channel = 0,
        .unit = stepper->pcnt_unit,

        // dir 正反方向，计数器均增长
        // (由于 ctrl_gpio_num=-1 , 以下配置并不需要)
        .pos_mode = PCNT_COUNT_INC,
        .neg_mode = PCNT_COUNT_INC,         
        .lctrl_mode = PCNT_MODE_KEEP,
        .hctrl_mode = PCNT_MODE_KEEP,

        .counter_l_lim = -MIN_PCNT_VALUE,
        .counter_h_lim = MAX_PCNT_VALUE
    };
    CALL_IDF_API( pcnt_unit_config(&cfg_pcnt) )

    CALL_IDF_API( pcnt_set_event_value(stepper->pcnt_unit, PCNT_EVT_THRES_0, 0) )
    // CALL_IDF_API( pcnt_event_enable(stepper->pcnt_unit, PCNT_EVT_THRES_0) )

    CALL_IDF_API( pcnt_event_enable(stepper->pcnt_unit, PCNT_EVT_L_LIM) )
    CALL_IDF_API( pcnt_event_enable(stepper->pcnt_unit, PCNT_EVT_H_LIM) )
    CALL_IDF_API( pcnt_event_enable(stepper->pcnt_unit, PCNT_EVT_ZERO) )

    CALL_IDF_API( pcnt_counter_pause(stepper->pcnt_unit) )

    // This needs to be called on init or count will stay at 0!
    CALL_IDF_API( pcnt_counter_clear(stepper->pcnt_unit) )

    // Register the counter interrupt handler.
    // CALL_IDF_API( pcnt_isr_register(stepper_pcnt_intr_handler, (void*)stepper, 0, &stepper->counter_isr_handle) )
    // CALL_IDF_API( pcnt_intr_enable(stepper->pcnt_unit) )
    
    pcnt_isr_service_install(0);
    pcnt_isr_handler_add(stepper->pcnt_unit, stepper_pcnt_intr_handler, (void *)stepper);

    CALL_IDF_API( pcnt_set_filter_value(stepper->pcnt_unit, 1) )
    CALL_IDF_API( pcnt_filter_enable(stepper->pcnt_unit) )

    CALL_IDF_API( pcnt_counter_resume(stepper->pcnt_unit) )
    
    CALL_IDF_API( pcnt_counter_resume(stepper->pcnt_unit) )


    // setup mcpwm
    CALL_IDF_API( mcpwm_gpio_init(stepper->mcpwm_unit, MCPWM0A, stepper->io_step) )
    mcpwm_config_t cfg_mcpwm = {
        .cmpr_a = 50,
        .cmpr_b = 0,
        .counter_mode = MCPWM_UP_COUNTER,
        .duty_mode = MCPWM_DUTY_MODE_0,
        .frequency = 1000,
    };
    CALL_IDF_API( mcpwm_init(stepper->mcpwm_unit, stepper->mcpwm_timer, &cfg_mcpwm) )
    CALL_IDF_API( mcpwm_stop(stepper->mcpwm_unit,stepper->mcpwm_timer) )

    // 
    // Enable both pins as output.
    REG_SET_BIT(GPIO_ENABLE_REG, (1ULL<<stepper->io_step|1ULL<<stepper->io_dir));
    
    // Enable input on direction pin mux.
    PIN_INPUT_ENABLE(GPIO_PIN_MUX_REG[stepper->io_dir]);

    // Connect MCPWM pulse signal to io step output in-case MCPWM is setup before TICK is registered as an input.
    gpio_matrix_out(stepper->io_step, PWM0_OUT0A_IDX, 0, 0);

    // Enable io step as input in GPIO mux.
    PIN_INPUT_ENABLE(GPIO_PIN_MUX_REG[stepper->io_step]);

    JS_SetOpaque(jsobj, stepper) ;
    JS_SetPropertyStr(ctx, jsobj, "_handlers", JS_NewObject(ctx));

    stepper->ctx = ctx ;
    stepper->jsobj = JS_VALUE_GET_PTR(jsobj) ;

    return jsobj ;
}

static void js_stepper_finalizer(JSRuntime *rt, JSValue this_val){
    driver_stepper_t * thisstepper = JS_GetOpaque(this_val, js_stepper_class_id);
    if(!thisstepper) {
        printf("must be called as a driver.Stepper method\n") ;
    }
    if(thisstepper) {
        free(thisstepper) ;
        thisstepper = NULL ;
        JS_SetOpaque(this_val, NULL) ;
    }
}
static JSClassDef js_stepper_class = {
    "driver.Stepper",
    .finalizer = js_stepper_finalizer,
} ;

static JSValue js_stepper_set_freq(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)

    THIS_STEPPER(stepper)
    ARGV_TO_UINT32(0, freq)

    CALL_IDF_API( mcpwm_set_frequency(stepper->mcpwm_unit,stepper->mcpwm_timer,freq) )

    return JS_UNDEFINED ;
}


static JSValue run_steps(JSContext *ctx, driver_stepper_t * stepper, int16_t steps, uint8_t dir) {

    if(stepper->running) {
        THROW_EXCEPTION("this stepper is running alread")
    }

    // 方向
    CALL_IDF_API( gpio_set_level(stepper->io_dir, dir) )
    stepper->run_dir = dir ;

    // 限步
    if(steps>0) {
        CALL_IDF_API( pcnt_set_event_value(stepper->pcnt_unit, PCNT_EVT_THRES_0, steps) )
        CALL_IDF_API( pcnt_event_enable(stepper->pcnt_unit, PCNT_EVT_THRES_0) )

        CALL_IDF_API( pcnt_counter_pause(stepper->pcnt_unit) )
        CALL_IDF_API( pcnt_counter_resume(stepper->pcnt_unit) )
        
        stepper->run_steps = steps;
    }
    else {
        stepper->run_steps = -1 ;
    }

    // 计数清零
    stepper->steps_over_counter = 0 ;
    CALL_IDF_API( pcnt_counter_clear(stepper->pcnt_unit) )
    
    // 开始
    CALL_IDF_API( mcpwm_set_duty(stepper->mcpwm_unit,stepper->mcpwm_timer, MCPWM_GEN_A, 50.0f) )
    CALL_IDF_API( mcpwm_start(stepper->mcpwm_unit,stepper->mcpwm_timer) )

    stepper->running = true ;

    // dn(stepper->running)
    // dn(stepper->steps_over_counter)
    // dn(stepper->run_steps)

    return JS_UNDEFINED ;
}

/**
 * @param dir=1
 */
static JSValue js_stepper_run(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    uint8_t dir = 1 ;
    if(argc>0) {
        ARGV_TO_UINT8_VAR(0, dir)
    }

    THIS_STEPPER(stepper)

    return run_steps(ctx, stepper, -1, dir) ;
}
static JSValue js_stepper_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    THIS_STEPPER(stepper)
    CALL_IDF_API( mcpwm_stop(stepper->mcpwm_unit,stepper->mcpwm_timer) )
    stepper->running = false ;
    
    // 事件
    emit_stop_event(stepper, true) ;

    return JS_UNDEFINED ;
}

/**
 * @param steps 
 * @param dir=1
 */
static JSValue js_stepper_run_steps(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT16(0, steps)

    uint8_t dir = 1 ;
    if(argc>1) {
        ARGV_TO_UINT8_VAR(1, dir)
    }

    THIS_STEPPER(stepper)

    return run_steps(ctx, stepper, steps, dir) ;
}


static JSValue js_stepper_get_steps(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)

    ARGV_TO_UINT8_OPT(0, only_pcnt_val, 0)

    THIS_STEPPER(stepper)

    uint32_t count = 0 ;
    pcnt_get_counter_value(stepper->pcnt_unit, &count);

    dn(count)
    dn(stepper->steps_over_counter)

    if(!only_pcnt_val) {
        count = stepper->steps_over_counter * MAX_PCNT_VALUE + count ;
    }

    return JS_NewInt32(ctx,count) ;
}

static const JSCFunctionListEntry js_stepper_proto_funcs[] = {
    JS_CFUNC_DEF("run", 0, js_stepper_run),
    JS_CFUNC_DEF("stop", 0, js_stepper_stop),
    JS_CFUNC_DEF("runSteps", 0, js_stepper_run_steps),
    JS_CFUNC_DEF("getSteps", 0, js_stepper_get_steps),
    JS_CFUNC_DEF("setFreq", 0, js_stepper_set_freq),
} ;


void be_module_driver_stepper_init() {
    JS_NewClassID(&js_stepper_class_id);
}

void be_module_driver_stepper_require(JSContext *ctx, JSValue driver) {

    JSValue EventEmitterProto = js_get_glob_prop(ctx, 3, "beapi", "EventEmitter", "prototype") ;

    QJS_DEF_CLASS(stepper, "Stepper", "driver.Stepper", EventEmitterProto, driver)
    // 
    // JS_SetPropertyStr(ctx, driver, "pcntCount", JS_NewCFunction(ctx, js_driver_pcnt_count, "pcntCount", 1));
    
    JS_FreeValue(ctx, EventEmitterProto) ;
}

void be_module_driver_stepper_loop(JSContext *ctx) {
}

void be_module_driver_stepper_reset(JSContext *ctx) {
}

