#include "module_gpio.h"
#include "utils.h"
#include "task_js.h"
#include "eventloop.h"
#include "esp32-hal-gpio.h"
#include "driver/gpio.h"

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

    JS_FreeValue(ctx, global);
}

void gpio_init() {
    gpio_install_isr_service(0);
}

void gpio_on_before_reset(JSContext *ctx) {
    if(_gpio_isr_js_callback) {
        JS_FreeValue(ctx, _gpio_isr_js_callback) ;
        _gpio_isr_js_callback = NULL ;
    }
}