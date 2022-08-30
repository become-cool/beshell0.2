#include "module_nvs.h"
#include "utils.h"
#include <sys/stat.h>


#ifndef SIMULATION

#include "esp_system.h"
#include "nvs_flash.h"


#define NVS_OPEN(h, failed)                                 \
    nvs_handle_t handle;                                    \
    esp_err_t err = nvs_open("beshell", NVS_READWRITE, &h); \
    if(err!=ESP_OK) {                                       \
        failed                                              \
    }

#define NVS_INT_GETTER(type,ctype,jstype)                         \
    static JSValue js_nvs_read_##type(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {    \
        CHECK_ARGC(1)                                       \
        ARGV_TO_STRING(0,key)                               \
        JSValue result = JS_NULL;                           \
        NVS_OPEN(handle, {                                  \
            goto ret ;                                      \
        })                                                  \
        ctype v ;                                           \
        if(nvs_get_##type(handle, key, &v)==ESP_OK) {       \
            result = JS_New##jstype(ctx, v) ;               \
        }                                                   \
        nvs_close(handle) ;                                 \
    ret:                                                    \
        JS_FreeCString(ctx, key) ;                          \
        return result ;                                     \
    }

NVS_INT_GETTER(i8, int8_t, Int32)
NVS_INT_GETTER(i16, int16_t, Int32)
NVS_INT_GETTER(i32, int32_t, Int32)
NVS_INT_GETTER(i64, int64_t, Int64)
NVS_INT_GETTER(u8, uint8_t, Uint32)
NVS_INT_GETTER(u16, uint16_t, Uint32)
NVS_INT_GETTER(u32, uint32_t, Uint32)


static JSValue js_nvs_erase_key(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_STRING(0,key)

    JSValue result = JS_FALSE;
    
    NVS_OPEN(handle, {
        goto ret ;
    })
    result = nvs_erase_key(handle, key)==ESP_OK? JS_TRUE: JS_FALSE;
    nvs_close(handle) ;

ret:
    JS_FreeCString(ctx, key) ;
    return result ;
}

#endif


void load_boot_level_from_nvs(uint8_t * pvalue) {
#ifndef SIMULATION
    NVS_OPEN(handle, {
        return ;
    })
    if(nvs_get_u8(handle, "next_boot_lv", pvalue)==ESP_OK) {
        nvs_erase_key(handle, "next_boot_lv") ;
    }
    nvs_close(handle) ;
#else
    FILE * h = fopen("./nvs/next_boot_lv.bin","r") ;
    if(!h) {
        return ;
    }
    fread(pvalue, 1, 1, h) ;
    fclose(h) ;
    unlink("./nvs/next_boot_lv.bin") ;
#endif
}

static JSValue js_nvs_set_next_boot_level(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0,level)
    JSValue result = JS_FALSE;
#ifndef SIMULATION
    NVS_OPEN(handle, {
        goto ret ;
    })
    result = nvs_set_u8(handle, "next_boot_lv", level)==ESP_OK? JS_TRUE: JS_FALSE; ;
    nvs_close(handle) ;
#else
#ifdef WIN32
        mkdir("./nvs") ;
#else
        mkdir("./nvs", ACCESSPERMS) ;
#endif
    FILE * h = fopen("./nvs/next_boot_lv.bin","w") ;
    if(!h) {
        return JS_FALSE ;
    }
    result = fwrite(&level, 1, 1, h) == 1? JS_TRUE: JS_FALSE ;
    fclose(h) ;
#endif

ret:
    return result ;
}

void be_module_nvs_init() {}

void be_module_nvs_require(JSContext *ctx) {

    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue nvs = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "nvs", nvs);


    JS_SetPropertyStr(ctx, nvs, "setNextBootLevel", JS_NewCFunction(ctx, js_nvs_set_next_boot_level, "setNextBootLevel", 1));

#ifndef SIMULATION

    JS_SetPropertyStr(ctx, nvs, "readInt8", JS_NewCFunction(ctx, js_nvs_read_i8, "readInt8", 1));
    JS_SetPropertyStr(ctx, nvs, "readInt16", JS_NewCFunction(ctx, js_nvs_read_i16, "readInt16", 1));
    JS_SetPropertyStr(ctx, nvs, "readInt32", JS_NewCFunction(ctx, js_nvs_read_i32, "readInt32", 1));
    JS_SetPropertyStr(ctx, nvs, "readInt64", JS_NewCFunction(ctx, js_nvs_read_i64, "readInt64", 1));
    JS_SetPropertyStr(ctx, nvs, "readUint8", JS_NewCFunction(ctx, js_nvs_read_u8, "readUint8", 1));
    JS_SetPropertyStr(ctx, nvs, "readUint16", JS_NewCFunction(ctx, js_nvs_read_u16, "readUint16", 1));
    JS_SetPropertyStr(ctx, nvs, "readUint32", JS_NewCFunction(ctx, js_nvs_read_u32, "readUint32", 1));

#endif


    JS_FreeValue(ctx, beapi);
}

void be_module_nvs_loop(JSContext *ctx) {}

void be_module_nvs_reset(JSContext *ctx) {}

