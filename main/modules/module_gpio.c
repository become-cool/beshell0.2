#include "module_gpio.h"
#include "utils.h"
#include "esp32-hal-gpio.h"

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

void require_module_gpio(JSContext *ctx) {
    
    JSValue global = JS_GetGlobalObject(ctx);
    
    JS_SetPropertyStr(ctx, global, "pinMode", JS_NewCFunction(ctx, js_gpio_pin_mode, "pinMode", 1));
    JS_SetPropertyStr(ctx, global, "digitalRead", JS_NewCFunction(ctx, js_gpio_digital_read, "digitalRead", 1));
    JS_SetPropertyStr(ctx, global, "digitalWrite", JS_NewCFunction(ctx, js_gpio_digital_write, "digitalWrite", 1));
    JS_SetPropertyStr(ctx, global, "analogRead", JS_NewCFunction(ctx, js_gpio_analog_read, "analogRead", 1));
    JS_SetPropertyStr(ctx, global, "analogWrite", JS_NewCFunction(ctx, js_gpio_analog_write, "analogWrite", 1));

    JS_FreeValue(ctx, global);
}