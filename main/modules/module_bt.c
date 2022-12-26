#include "module_bt.h"
#include "utils.h"
#include <string.h>

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"


#define ARGV_BT_MODE(i,var)     \
    ARGV_TO_UINT8(i, var)       \
    if(var!=ESP_BT_MODE_BLE && var!=ESP_BT_MODE_CLASSIC_BT) {         \
        THROW_EXCEPTION("argv mode must be 1 (ble), 2 (class bt)")  \
    }

#define CALL_IDF_API(api, failed_msg, ...)              \
        esp_err_t err = api(__VA_ARGS__) ;            \
        if (err != ESP_OK) {                                    \
            THROW_EXCEPTION(failed_msg, esp_err_to_name(err));  \
        }                                                       \
        

#define API_IDF2JS_NO_ARGV(jsfunc, api, failed_msg, ...)                    \
    static JSValue jsfunc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) { \
        CALL_IDF_API(api, failed_msg, ##__VA_ARGS__)                   \
        return JS_TRUE ;                                   \
    }


static JSValue js_bt_controller_init_as_default(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_BT_MODE(0, mode)
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    bt_cfg.bluetooth_mode = mode ;
    CALL_IDF_API(esp_bt_controller_init, "bt controller init failed: %s", &bt_cfg)
    return JS_TRUE ;
}

API_IDF2JS_NO_ARGV(js_bt_controller_deinit, esp_bt_controller_deinit, "bt controller deinit failed: %s")

static JSValue js_bt_controller_mem_release(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, mode)
    CALL_IDF_API(esp_bt_controller_mem_release, "controller mem release failed: %s", mode)
    return JS_TRUE ;
}

static JSValue js_bt_controller_enable(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_BT_MODE(0, mode)
    CALL_IDF_API(esp_bt_controller_enable, "controller enable failed: %s", mode)
    return JS_TRUE ;
}

static JSValue js_bt_controller_get_status(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    esp_bt_controller_status_t status = esp_bt_controller_get_status() ;
    return JS_NewInt32(ctx, status) ;
}

API_IDF2JS_NO_ARGV(js_bt_controller_disable, esp_bt_controller_disable, "controller disable failed: %s")
API_IDF2JS_NO_ARGV(js_bt_bluedroid_init, esp_bluedroid_init, "bluedroid init failed: %s")
API_IDF2JS_NO_ARGV(js_bt_bluedroid_deinit, esp_bluedroid_deinit, "bluedroid deinit failed: %s")
API_IDF2JS_NO_ARGV(js_bt_bluedroid_enable, esp_bluedroid_enable, "bluedroid enable failed: %s")
API_IDF2JS_NO_ARGV(js_bt_bluedroid_disable, esp_bluedroid_disable, "bluedroid disable failed: %s")

static JSValue js_bt_bluedroid_get_status(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    esp_bluedroid_status_t status = esp_bluedroid_get_status() ;
    return JS_NewInt32(ctx, status) ;
}

static JSValue js_bt_set_pin(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_STRING_LEN_E(0, pin, pinlen, "argv len must be a string")

    if(pinlen!=4) {
        JS_FreeCString(ctx, pin) ;
        THROW_EXCEPTION("pin length must be 4")
    }

    esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_FIXED;
    esp_bt_pin_code_t pin_code;
    memcpy(pin_code, pin, 4) ;
    esp_bt_gap_set_pin(pin_type, 4, pin_code);

    JS_FreeCString(ctx, pin) ;
    return JS_UNDEFINED ;
}

void be_module_bt_init() {
}

void be_module_bt_require(JSContext *ctx) {

    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue bt = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "bt", bt);

    JS_SetPropertyStr(ctx, bt, "controllerMemRelease", JS_NewCFunction(ctx, js_bt_controller_mem_release, "controllerMemRelease", 1));
    JS_SetPropertyStr(ctx, bt, "controllerInitAsDefault", JS_NewCFunction(ctx, js_bt_controller_init_as_default, "controllerInitAsDefault", 1));
    JS_SetPropertyStr(ctx, bt, "controllerDeinit", JS_NewCFunction(ctx, js_bt_controller_deinit, "controllerDeinit", 1));
    JS_SetPropertyStr(ctx, bt, "controllerEnable", JS_NewCFunction(ctx, js_bt_controller_enable, "controllerEnable", 1));
    JS_SetPropertyStr(ctx, bt, "controllerDisable", JS_NewCFunction(ctx, js_bt_controller_disable, "controllerDisable", 1));
    JS_SetPropertyStr(ctx, bt, "controllerStatus", JS_NewCFunction(ctx, js_bt_controller_get_status, "controllerStatus", 1));
    
    JS_SetPropertyStr(ctx, bt, "bluedroidInit", JS_NewCFunction(ctx, js_bt_bluedroid_init, "bluedroidInit", 1));
    JS_SetPropertyStr(ctx, bt, "bluedroidDeinit", JS_NewCFunction(ctx, js_bt_bluedroid_deinit, "bluedroidDeinit", 1));
    JS_SetPropertyStr(ctx, bt, "bluedroidEnable", JS_NewCFunction(ctx, js_bt_bluedroid_enable, "bluedroidEnable", 1));
    JS_SetPropertyStr(ctx, bt, "bluedroidDisable", JS_NewCFunction(ctx, js_bt_bluedroid_disable, "bluedroidDisable", 1));
    JS_SetPropertyStr(ctx, bt, "bluedroidStatus", JS_NewCFunction(ctx, js_bt_bluedroid_get_status, "bluedroidStatus", 1));

    JS_FreeValue(ctx, beapi);
}

void be_module_bt_loop(JSContext *ctx) {
}

void be_module_bt_reset(JSContext *ctx) {
}

