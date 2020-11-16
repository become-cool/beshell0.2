#include "module_gpio.h"
#include "utils.h"
#include "task_js.h"
#include "eventloop.h"
#include "esp32-hal-gpio.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

#define SET_PIN_MODE(name, cst)  if(strcmp(mode,name)==0) {                     \
                                    pinMode(pin, cst) ;                         \
                                }
JSValue js_gpio_pin_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    
    CHECK_ARGC(2)
    ARGV_TO_UINT8(0, pin)
    char * mode = JS_ToCString(ctx, argv[1]) ;

    SET_PIN_MODE("input", INPUT)
    else SET_PIN_MODE("output", OUTPUT)
    else SET_PIN_MODE("input-pullup", INPUT_PULLUP)
    else SET_PIN_MODE("input-pulldown", INPUT_PULLDOWN)
    else SET_PIN_MODE("analog", ANALOG)
    else SET_PIN_MODE("pullup", PULLUP)
    else SET_PIN_MODE("pulldown", PULLDOWN)
    else SET_PIN_MODE("open-drain", OPEN_DRAIN)
    else SET_PIN_MODE("output-open-drain", OUTPUT_OPEN_DRAIN)
    else {
        THROW_EXCEPTION("unknow pin mode")
    }

    JS_FreeCString(ctx, mode) ;
    return JS_UNDEFINED ;
}


JSValue js_gpio_digital_read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, pin)
    uint8_t value = digitalRead(pin) ;
    return JS_NewInt32(ctx, value) ;
}
JSValue js_gpio_digital_write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_UINT8(0, pin)
    ARGV_TO_UINT8(1, value)
    digitalWrite(pin, value) ;
    return JS_UNDEFINED ;
}
JSValue js_gpio_analog_read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}
JSValue js_gpio_analog_write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}

JSValue _gpio_isr_js_callback = NULL ;

static void IRAM_ATTR _gpio_isr_handler(void* arg) {
    if(!_gpio_isr_js_callback)
        return ;
    JSValue * argv = malloc(sizeof(JSValue));       // 由 eventloop free
    JSContext * ctx = task_current_context() ;
    *argv = JS_NewInt32(ctx, (uint32_t) arg) ;
    eventloop_push_with_argv(ctx, _gpio_isr_js_callback, 1, argv) ;
}


JSValue js_gpio_set_pin_isr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_UINT8(0, pin)
    ARGV_TO_UINT8(1, mode)

    gpio_set_intr_type(pin, mode);
    gpio_isr_handler_remove(pin);
    if(mode>0){
        gpio_isr_handler_add(pin, _gpio_isr_handler, (void *)pin);
    }

    return JS_UNDEFINED;
}
JSValue js_gpio_unset_pin_isr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, pin)

    gpio_isr_handler_remove(pin);

    return JS_UNDEFINED;
}
JSValue js_gpio_set_pin_isr_callback(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    if(!JS_IsFunction(ctx, argv[0])) {
        return JS_UNDEFINED;
    }

    if(_gpio_isr_js_callback)
        JS_FreeValue(ctx, _gpio_isr_js_callback) ;
    _gpio_isr_js_callback = JS_DupValue(ctx, argv[0]) ;

    return JS_UNDEFINED;
}

/**
 * timer 0-3
 * freq
 * resolution  1-20
 * speedMode 0:high, 1:low
 */
JSValue js_pwm_config_timer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(4)
    ARGV_TO_UINT8(0, timer)
    ARGV_TO_UINT32(1, freq)
    ARGV_TO_UINT8(2, resolution)
    ARGV_TO_UINT8(3, speedMode)

    printf("timer=%d, freq=%d, resolution=%d, speedMode=%d \n",timer,freq,resolution,speedMode) ;

    if(timer<0 || timer>4) {
        THROW_EXCEPTION("pwm timer must be 0-4")
    }
    if(resolution<1 || resolution>20) {
        THROW_EXCEPTION("pwm resolution must be 1-20(bit)")
    }
    if(speedMode<0 || speedMode>1) {
        THROW_EXCEPTION("pwm speed mode must be 0(high) or 1(low)")
    }

    ledc_timer_config_t timerConf = {
        .duty_resolution = resolution,      // resolution of PWM duty
        .freq_hz = freq,                    // frequency of PWM signal
        .speed_mode = speedMode,            // timer mode
        .timer_num = timer,                 // timer index
    } ;
    return JS_NewInt32(ctx, ledc_timer_config(&timerConf)) ;
}

/**
 * gpio 0-3
 * duty
 * channel 0-7
 * timer 0-3
 * speedMode 0:high, 1:low
 */
JSValue js_pwm_config_channel(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    
    CHECK_ARGC(5)
    ARGV_TO_UINT8(0, gpio)
    ARGV_TO_UINT32(1, duty)
    ARGV_TO_UINT8(2, channel)
    ARGV_TO_UINT8(3, timer)
    ARGV_TO_UINT8(4, speedMode)
    
    printf("gpio=%d, duty=%d, channel=%d, timer=%d, speedMode=%d \n",gpio,duty,channel,timer, speedMode) ;

    if(timer<0 || timer>4) {
        THROW_EXCEPTION("pwm timer must be 0-4")
    }
    if(channel<0 || channel>7) {
        THROW_EXCEPTION("pwm channel must be 0-7")
    }
    if(speedMode<0 || speedMode>1) {
        THROW_EXCEPTION("pwm speed mode must be 0(high) or 1(low)")
    }

    ledc_channel_config_t channelConf = {
        .channel    = channel,
        .duty       = duty,
        .gpio_num   = gpio,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .intr_type  = LEDC_INTR_DISABLE,
        .hpoint     = 0,
        .timer_sel  = timer
    } ;
    return JS_NewInt32(ctx, ledc_channel_config(&channelConf) );
}



typedef struct {
    uint32_t freq ;
    uint8_t resolution ;
} timer_hold_t ;
typedef struct {
    uint8_t gpio ;
    uint8_t timer ;
} channel_hold_t ;

timer_hold_t timerHolds[2][4] = { {{255,0},{255,0},{255,0},{255,0}}, {{255,0},{255,0},{255,0},{255,0}} };
channel_hold_t channelHolds[2][8] = {
    {{255,255},{255,255},{255,255},{255,255},{255,255},{255,255},{255,255},{255,255}}, 
    {{255,255},{255,255},{255,255},{255,255},{255,255},{255,255},{255,255},{255,255}}
};

/**
 * gpio
 * freq
 * resolution  1-20
 * duty
 */
JSValue js_pwm_config(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(4)
    ARGV_TO_UINT8(0, gpio)
    ARGV_TO_UINT32(1, freq)
    ARGV_TO_UINT8(2, resolution)
    ARGV_TO_UINT32(3, duty)

    if(resolution<1 || resolution>20) {
        THROW_EXCEPTION("pwm resolution must be 1-20(bit)")
    }

    uint8_t channel = 255 ;
    uint8_t timer = 255 ;
    uint8_t speedMode=0 ;
    
    for(speedMode=0; speedMode<LEDC_SPEED_MODE_MAX; speedMode++) {

        // 分配 channel
        for(uint8_t c=0; c<LEDC_CHANNEL_MAX; c++) {
            // 已经分给给该 gpio 的 channel
            if(channelHolds[speedMode][c].gpio==gpio) {
                channel = c ;
                break ;
            }
            // 遇到第一个未分配的channel作为备选
            if(channel==255 && channelHolds[speedMode][c].gpio==255) {
                channel = c ;
            }
        }

        // 无可用 channel , 切换 speed mode 再找
        if(channel==255) {
            continue ;
        }

        // 分配 timer
        for(uint8_t t=0; t<LEDC_TIMER_MAX; t++) {
            // 使用相同的 freq 和 resolution
            if( timerHolds[speedMode][t].freq==freq && timerHolds[speedMode][t].resolution==resolution ) {
                timer = t ;
                break ;
            }
            // 第一个遇到未分配timer作为备选
            if( timer==255 && timerHolds[speedMode][t].freq==255) {
                timer = t ;
            }
        }

        // 无可用 timer , 切换 speed mode 再找
        if(timer==255) {
            continue ;
        }

        // bingo
        break ;
    }

    if(speedMode>=LEDC_SPEED_MODE_MAX) {
        THROW_EXCEPTION("There is no free timer or channel for this gpio") ;
    }

    // printf("use speed mode:%d, timer:%d, channel: %d\n", speedMode, timer, channel) ;

    // timer 尚未设置
    if(timerHolds[speedMode][timer].freq==255){
        
        timerHolds[speedMode][timer].freq = freq ;
        timerHolds[speedMode][timer].resolution = resolution ;

        ledc_timer_config_t timerConf = {
            .duty_resolution = resolution,      // resolution of PWM duty
            .freq_hz = freq,                    // frequency of PWM signal
            .speed_mode = speedMode,            // timer mode
            .timer_num = timer,                 // timer index
        } ;
        esp_err_t ret = ledc_timer_config(&timerConf) ;
        if(ret<0) {
            return JS_NewInt32(ctx, ret) ;
        }
    }
    
    channelHolds[speedMode][channel].gpio = gpio ;
    channelHolds[speedMode][channel].timer = timer ;

    ledc_channel_config_t channelConf = {
        .channel    = channel,
        .duty       = duty,
        .gpio_num   = gpio,
        .speed_mode = speedMode,
        .intr_type  = LEDC_INTR_DISABLE,
        .hpoint     = 0,
        .timer_sel  = timer
    } ;
    esp_err_t ret = ledc_channel_config(&channelConf) ;
    if(ret<0) {
        return JS_NewInt32(ctx, ret-1000) ;
    }

    return JS_NewInt32(ctx, 0) ;
}


/**
 * gpio
 * duty
 */
JSValue js_pwm_write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(2)
    ARGV_TO_UINT8(0, gpio)
    ARGV_TO_UINT32(1, duty)

    esp_err_t ret = 0 ;
    int8_t channel = 255 ;
    int8_t speedMode = 255 ;
    for(speedMode=0; speedMode<LEDC_SPEED_MODE_MAX; speedMode++) {
        for(uint8_t c=0; c<LEDC_CHANNEL_MAX; c++) {
            // 已经分给给该 gpio 的 channel
            if(channelHolds[speedMode][c].gpio==gpio) {
                channel = c ;
                goto endloop ;
            }
        }
    }
    THROW_EXCEPTION("Call pwm.config() first") ;
endloop:

    // printf("speed mode:%d, channel:%d, duty: %d\n", speedMode, channel, duty) ;

    ret = ledc_set_duty(speedMode, channel, duty) ;
    if(ret<0) {
        return JS_NewInt32(ctx, ret) ;
    }
    ret = ledc_update_duty(speedMode, channel) ;
    if(ret<0) {
        return JS_NewInt32(ctx, ret-1000) ;
    }

    return JS_NewInt32(ctx, 0) ;
}

void require_module_gpio(JSContext *ctx) {
    
    JSValue global = JS_GetGlobalObject(ctx);
    
    JS_SetPropertyStr(ctx, global, "pinMode", JS_NewCFunction(ctx, js_gpio_pin_mode, "pinMode", 1));
    JS_SetPropertyStr(ctx, global, "digitalRead", JS_NewCFunction(ctx, js_gpio_digital_read, "digitalRead", 1));
    JS_SetPropertyStr(ctx, global, "digitalWrite", JS_NewCFunction(ctx, js_gpio_digital_write, "digitalWrite", 1));
    JS_SetPropertyStr(ctx, global, "analogRead", JS_NewCFunction(ctx, js_gpio_analog_read, "analogRead", 1));
    JS_SetPropertyStr(ctx, global, "analogWrite", JS_NewCFunction(ctx, js_gpio_analog_write, "analogWrite", 1));
    JS_SetPropertyStr(ctx, global, "setPinISR", JS_NewCFunction(ctx, js_gpio_set_pin_isr, "setPinISR", 1));
    JS_SetPropertyStr(ctx, global, "unsetPinISR", JS_NewCFunction(ctx, js_gpio_unset_pin_isr, "unsetPinISR", 1));
    JS_SetPropertyStr(ctx, global, "setPinISRCallback", JS_NewCFunction(ctx, js_gpio_set_pin_isr_callback, "setPinISRCallback", 1));

    JSValue pwm = JS_NewObject(ctx) ;
    JS_SetPropertyStr(ctx, global, "pwm", pwm);
    JS_SetPropertyStr(ctx, pwm, "configTimer", JS_NewCFunction(ctx, js_pwm_config_timer, "configTimer", 1));
    JS_SetPropertyStr(ctx, pwm, "configChannel", JS_NewCFunction(ctx, js_pwm_config_channel, "configChannel", 1));
    JS_SetPropertyStr(ctx, pwm, "config", JS_NewCFunction(ctx, js_pwm_config, "config", 1));
    JS_SetPropertyStr(ctx, pwm, "write", JS_NewCFunction(ctx, js_pwm_write, "write", 1));

    JS_FreeValue(ctx, global);
}

void gpio_init() {
    gpio_install_isr_service(0);
}

void gpio_on_before_reset(JSContext *ctx) {
    // 清空 js 回调函数
    if(_gpio_isr_js_callback) {
        JS_FreeValue(ctx, _gpio_isr_js_callback) ;
        _gpio_isr_js_callback = NULL ;
    }

    // 停止所有 pwm
    for(uint8_t speedMode=0; speedMode<LEDC_SPEED_MODE_MAX; speedMode++) {
        // reset channels
        for(uint8_t c=0; c<LEDC_CHANNEL_MAX; c++) {
            // printf("speedMode=%d, channel=%d, timer=%d, gpio=%d\n",speedMode,c,channelHolds[speedMode][c].timer,channelHolds[speedMode][c].gpio) ;
            if(channelHolds[speedMode][c].gpio!=255) {
                channelHolds[speedMode][c].gpio = 255 ;
                channelHolds[speedMode][c].timer = 255 ;
                ledc_stop(speedMode, c, 0) ;
            }
        }
        // reset timers
        for(uint8_t t=0; t<LEDC_TIMER_MAX; t++) {
            timerHolds[speedMode][t].freq = 255 ;
            timerHolds[speedMode][t].resolution = 0 ;
        }
    }
}