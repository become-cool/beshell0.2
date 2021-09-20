#include "module_gpio.h"
#include "utils.h"
#include "task_js.h"
#include "eventloop.h"
#include "arduino/esp32-hal-gpio.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include <driver/dac.h>
#include <esp_adc_cal.h>


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



JSValue js_adc_set_bits(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_UINT8(0, adc)
    ARGV_TO_UINT8(1, bits)
    if(adc!=1) {
        THROW_EXCEPTION("adcConfigBits() only set adc1")
    }
    if( bits<9 || bits>12 ) {
        THROW_EXCEPTION("adcConfigBits() arg bits must be 9-12")
    }
    return (adc1_config_width(bits-9) == ESP_OK)? JS_TRUE: JS_FALSE ;
}

#define MAPCHANNEL(gpionum, channelnum, adcnum, vpin, vchannel, vadc)   \
    if(vpin==gpionum) {                                                 \
        vchannel = channelnum ;                                         \
        vadc = adcnum ;                                                 \
    }

#define GPIO2ADCCHANNEL(vpin, vchannel, vadc)               \
    adc_channel_t vchannel = 0 ;                            \
    uint8_t vadc = 0 ;                                      \
    MAPCHANNEL(36, 0, 1, vpin, vchannel, vadc)              \
    else MAPCHANNEL(37, 1, 1, vpin, vchannel, vadc)         \
    else MAPCHANNEL(38, 2, 1, vpin, vchannel, vadc)         \
    else MAPCHANNEL(39, 3, 1, vpin, vchannel, vadc)         \
    else MAPCHANNEL(32, 4, 1, vpin, vchannel, vadc)         \
    else MAPCHANNEL(33, 5, 1, vpin, vchannel, vadc)         \
    else MAPCHANNEL(34, 6, 1, vpin, vchannel, vadc)         \
    else MAPCHANNEL(35, 7, 1, vpin, vchannel, vadc)         \
    else MAPCHANNEL(4, 0, 2, vpin, vchannel, vadc)          \
    else MAPCHANNEL(0, 1, 2, vpin, vchannel, vadc)          \
    else MAPCHANNEL(2, 2, 2, vpin, vchannel, vadc)          \
    else MAPCHANNEL(15, 3, 2, vpin, vchannel, vadc)         \
    else MAPCHANNEL(13, 4, 2, vpin, vchannel, vadc)         \
    else MAPCHANNEL(12, 5, 2, vpin, vchannel, vadc)         \
    else MAPCHANNEL(14, 6, 2, vpin, vchannel, vadc)         \
    else MAPCHANNEL(27, 7, 2, vpin, vchannel, vadc)         \
    else MAPCHANNEL(25, 8, 2, vpin, vchannel, vadc)         \
    else MAPCHANNEL(26, 9, 2, vpin, vchannel, vadc)         \
    else {                                                  \
        THROW_EXCEPTION("pin is not a valid adc pin, must be 0, 2, 4, 12-15, 25-27, 32-39.")   \
    }

JSValue js_adc_set_channel_atten(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_UINT8(0, pin)
    ARGV_TO_UINT8(1, atten)

    if( atten<0 || atten>3 ) {
        THROW_EXCEPTION("adcConfigBits() arg atten must be 0-3")
    }

    GPIO2ADCCHANNEL(pin, channel, adc)

    esp_err_t ret = ESP_FAIL ;
    if(adc==1) {
        ret = adc1_config_channel_atten(channel, atten) ;
    }
    else if(adc==2) {
        ret = adc2_config_channel_atten(channel, atten) ;
    }
    
    return (ret==ESP_OK)? JS_TRUE: JS_FALSE ;
}



JSValue js_gpio_analog_read(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, pin)

    uint8_t bits = 12 ;
    if(argc>1) {
        if( JS_ToUint32(ctx, &bits, argv[1]) ) {
            THROW_EXCEPTION("Invalid param type");
        }
	}
    
    GPIO2ADCCHANNEL(pin, channel, adc)
    
    int val = 0 ;
    if(adc==1) {
        adc1_config_width(bits-9) ;
        val = adc1_get_raw(channel) ;
    }
    else if(adc==2) {
        adc2_get_raw(channel, bits-9, &val) ;
    }
    return JS_NewInt32(ctx, val) ;
}
JSValue js_gpio_analog_write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_UINT8(0, pin)
    ARGV_TO_UINT8(1, value)

    if(pin<25 || pin>26) {
        THROW_EXCEPTION("analogWrite() arg pin must be 25 or 26")  
    }
    pin-= 25 ; // DAC_CHANNEL_1 or DAC_CHANNEL_2
    
    dac_output_enable(pin);
    dac_output_voltage(pin, value);

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
        THROW_EXCEPTION("Arg not a function")
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
    
    // printf("gpio=%d, duty=%d, channel=%d, timer=%d, speedMode=%d \n",gpio,duty,channel,timer, speedMode) ;

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


// 释放没有被占用的 timer
void free_timer(uint8_t speedMode, uint8_t timer){
    for(uint8_t c=0; c<LEDC_CHANNEL_MAX; c++) {
        if(channelHolds[speedMode][c].timer == timer) {
            return ;
        }
    }

    // printf("free timer %d\n",timer) ;

    timerHolds[speedMode][timer].freq = 255 ;
    timerHolds[speedMode][timer].resolution = 255 ;
}

JSValue pwm_config(JSContext *ctx, uint8_t gpio, uint32_t freq, uint8_t resolution, uint32_t duty, int8_t * outSpeedMode, int8_t * outChannel){
    
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

        // 如果该 channal 已经被设置，其使用的timer 没有其他 channel 使用
        // 则先释放这个timer
        if(channelHolds[speedMode][channel].timer!=255){
            uint8_t _t = channelHolds[speedMode][channel].timer ;
            channelHolds[speedMode][channel].timer = 255 ;  // 解除此 channel 对 timer 的引用
            free_timer( speedMode, _t ) ;
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

    if(outSpeedMode)
        *outSpeedMode = speedMode ;
    if(outChannel)
        *outChannel = channel ;

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
    return pwm_config(ctx, gpio, freq, resolution, duty, NULL, NULL) ;
}

/**
 * gpio
 * duty
 */
JSValue js_pwm_write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(2)
    ARGV_TO_UINT8(0, gpio)
    ARGV_TO_DOUBLE(1, value)
    uint32_t duty = 0 ;

    int8_t speedMode = 255 ;
    int8_t channel = 255 ;
    for(speedMode=0; speedMode<LEDC_SPEED_MODE_MAX; speedMode++) {
        for(uint8_t c=0; c<LEDC_CHANNEL_MAX; c++) {
            // 已经分给给该 gpio 的 channel
            if(channelHolds[speedMode][c].gpio==gpio) {
                channel = c ;
                goto endloop ;
            }
        }
    }
    // 设置为默认的 freq/resolution
    if( JS_IsException(pwm_config(ctx, gpio, 50, 16, 0, &speedMode, &channel)) ) {
        return JS_EXCEPTION ;
    }
endloop:

    duty = value * pow(2,timerHolds[speedMode][ channelHolds[speedMode][channel].timer ].resolution) + 0.5 ;
    // printf("speed mode:%d, channel:%d, duty: %d\n", speedMode, channel, duty) ;

    esp_err_t ret = ledc_set_duty(speedMode, channel, duty) ;
    if(ret<0) {
        return JS_NewInt32(ctx, ret) ;
    }
    ret = ledc_update_duty(speedMode, channel) ;
    if(ret<0) {
        return JS_NewInt32(ctx, ret-1000) ;
    }

    return JS_NewInt32(ctx, 0) ;
}

JSValue js_pwm_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, gpio)

    uint8_t timer = 255 ;
    uint8_t speedMode = 255 ;

    // 找到对应的 channel
    for(uint8_t sm=0; sm<LEDC_SPEED_MODE_MAX; sm++) {
        // reset channels
        for(uint8_t c=0; c<LEDC_CHANNEL_MAX; c++) {
            if( channelHolds[sm][c].gpio == gpio){

                timer = channelHolds[sm][c].timer ;
                speedMode = sm ;

                // printf("channel=%d, timer=%d, speedMode=%d\n", c, timer, speedMode) ;

                ledc_stop(sm, c, 0) ;
                gpio_set_direction(gpio, GPIO_MODE_DEF_OUTPUT);
                channelHolds[sm][c].gpio = 255 ;
                channelHolds[sm][c].timer = 255 ;

                break ;
            }
        }
    }


    if(timer==255){
        return JS_UNDEFINED ;
    }
    
    // 如果没有其他 channel 使用 timer ，则释放 timer
    free_timer(speedMode, timer) ;

    return JS_UNDEFINED ;
}

void require_module_gpio(JSContext *ctx) {
    
    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beapi = JS_GetPropertyStr(ctx, global, "beapi") ;

    JSValue gpio = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "gpio", gpio);
    
    JS_SetPropertyStr(ctx, gpio, "pinMode", JS_NewCFunction(ctx, js_gpio_pin_mode, "pinMode", 1));
    JS_SetPropertyStr(ctx, gpio, "digitalRead", JS_NewCFunction(ctx, js_gpio_digital_read, "digitalRead", 1));
    JS_SetPropertyStr(ctx, gpio, "digitalWrite", JS_NewCFunction(ctx, js_gpio_digital_write, "digitalWrite", 1));
    JS_SetPropertyStr(ctx, gpio, "adcConfigBits", JS_NewCFunction(ctx, js_adc_set_bits, "adcConfigBits", 1));
    JS_SetPropertyStr(ctx, gpio, "adcConfigAtten", JS_NewCFunction(ctx, js_adc_set_channel_atten, "adcConfigAtten", 1));
    JS_SetPropertyStr(ctx, gpio, "analogRead", JS_NewCFunction(ctx, js_gpio_analog_read, "analogRead", 1));
    JS_SetPropertyStr(ctx, gpio, "analogWrite", JS_NewCFunction(ctx, js_gpio_analog_write, "analogWrite", 1));
    JS_SetPropertyStr(ctx, gpio, "pwmConfigTimer", JS_NewCFunction(ctx, js_pwm_config_timer, "pwmConfigTimer", 1));
    JS_SetPropertyStr(ctx, gpio, "pwmConfigChannel", JS_NewCFunction(ctx, js_pwm_config_channel, "pwmConfigChannel", 1));
    JS_SetPropertyStr(ctx, gpio, "pwmConfig", JS_NewCFunction(ctx, js_pwm_config, "pwmConfig", 1));
    JS_SetPropertyStr(ctx, gpio, "pwmWrite", JS_NewCFunction(ctx, js_pwm_write, "pwmWrite", 1));
    JS_SetPropertyStr(ctx, gpio, "pwmStop", JS_NewCFunction(ctx, js_pwm_stop, "pwmStop", 1));
    JS_SetPropertyStr(ctx, gpio, "setPinISR", JS_NewCFunction(ctx, js_gpio_set_pin_isr, "setPinISR", 1));
    JS_SetPropertyStr(ctx, gpio, "unsetPinISR", JS_NewCFunction(ctx, js_gpio_unset_pin_isr, "unsetPinISR", 1));
    JS_SetPropertyStr(ctx, gpio, "setPinISRCallback", JS_NewCFunction(ctx, js_gpio_set_pin_isr_callback, "setPinISRCallback", 1));

    JS_FreeValue(ctx, global);
    JS_FreeValue(ctx, beapi);
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
                ledc_stop(speedMode, c, 0) ;
                gpio_set_direction(channelHolds[speedMode][c].gpio, GPIO_MODE_DEF_OUTPUT);
                channelHolds[speedMode][c].gpio = 255 ;
                channelHolds[speedMode][c].timer = 255 ;
            }
        }
        // reset timers
        for(uint8_t t=0; t<LEDC_TIMER_MAX; t++) {
            timerHolds[speedMode][t].freq = 255 ;
            timerHolds[speedMode][t].resolution = 0 ;
        }
    }
}