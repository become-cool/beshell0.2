#include "module_camera.h"
#include "module_metadata.h"
#include "utils.h"
#include "esp_camera.h"
#include <string.h>

static bool default_config(camera_config_t * config, char partId) {

    config->ledc_channel = LEDC_CHANNEL_0;
    config->ledc_timer = LEDC_TIMER_0;
    config->pixel_format = PIXFORMAT_JPEG;
    config->jpeg_quality = 12;
    config->frame_size = FRAMESIZE_QVGA ;
    config->fb_count = 1;
    config->fb_location = CAMERA_FB_IN_PSRAM;
    config->grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config->xclk_freq_hz = 20000000;

    switch(partId) {
        // BeVision
        case 19: 
            config->pin_d0 = 39;
            config->pin_d1 = 37;
            config->pin_d2 = 36;
            config->pin_d3 = 38;
            config->pin_d4 = 34;
            config->pin_d5 = 19;
            config->pin_d6 = 9;
            config->pin_d7 = 22;
            config->pin_xclk = 5;
            config->pin_pclk = 35;
            config->pin_vsync = 23;
            config->pin_href = 26;
            config->pin_sscb_sda = 21;
            config->pin_sscb_scl = 18;
            config->pin_pwdn = -1;
            config->pin_reset = -1;

            config->fb_count = 2;

            return true ;

        default:
            config->pin_d0 = -1;
            config->pin_d1 = -1;
            config->pin_d2 = -1;
            config->pin_d3 = -1;
            config->pin_d4 = -1;
            config->pin_d5 = -1;
            config->pin_d6 = -1;
            config->pin_d7 = -1;
            config->pin_xclk = -1;
            config->pin_pclk = -1;
            config->pin_vsync = -1;
            config->pin_href = -1;
            config->pin_sscb_sda = -1;
            config->pin_sscb_scl = -1;
            config->pin_pwdn = -1;
            config->pin_reset = -1;
            return false ;
    }
}


static JSValue js_camera_default_config(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    unsigned char partId ;
    if(argc>0) {
        if( JS_ToInt32(ctx, &partId, argv[0])!=0 ) {
            THROW_EXCEPTION("Invalid param type of part id")
        }
    }
    else {
        partId = readPartId() ;
    }

    camera_config_t config ;
    memset(&config, 0, sizeof(camera_config_t));

    if(!default_config(&config, partId)) {
        THROW_EXCEPTION("Not a known camera host, part id: %d", partId)
    }

    JSValue obj = JS_NewObject(ctx) ;

    JS_SetPropertyStr(ctx, obj, "pin_d0", JS_NewInt32(ctx, config.pin_d0)) ;
    JS_SetPropertyStr(ctx, obj, "pin_d1", JS_NewInt32(ctx, config.pin_d1)) ;
    JS_SetPropertyStr(ctx, obj, "pin_d2", JS_NewInt32(ctx, config.pin_d2)) ;
    JS_SetPropertyStr(ctx, obj, "pin_d3", JS_NewInt32(ctx, config.pin_d3)) ;
    JS_SetPropertyStr(ctx, obj, "pin_d4", JS_NewInt32(ctx, config.pin_d4)) ;
    JS_SetPropertyStr(ctx, obj, "pin_d5", JS_NewInt32(ctx, config.pin_d5)) ;
    JS_SetPropertyStr(ctx, obj, "pin_d6", JS_NewInt32(ctx, config.pin_d6)) ;
    JS_SetPropertyStr(ctx, obj, "pin_d7", JS_NewInt32(ctx, config.pin_d7)) ;
    JS_SetPropertyStr(ctx, obj, "pin_xclk", JS_NewInt32(ctx, config.pin_xclk)) ;
    JS_SetPropertyStr(ctx, obj, "pin_pclk", JS_NewInt32(ctx, config.pin_pclk)) ;
    JS_SetPropertyStr(ctx, obj, "pin_vsync", JS_NewInt32(ctx, config.pin_vsync)) ;
    JS_SetPropertyStr(ctx, obj, "pin_href", JS_NewInt32(ctx, config.pin_href)) ;
    JS_SetPropertyStr(ctx, obj, "pin_sscb_sda", JS_NewInt32(ctx, config.pin_sscb_sda)) ;
    JS_SetPropertyStr(ctx, obj, "pin_sscb_scl", JS_NewInt32(ctx, config.pin_sscb_scl)) ;
    JS_SetPropertyStr(ctx, obj, "pin_pwdn", JS_NewInt32(ctx, config.pin_pwdn)) ;
    JS_SetPropertyStr(ctx, obj, "pin_reset", JS_NewInt32(ctx, config.pin_reset)) ;
    
    JS_SetPropertyStr(ctx, obj, "ledc_channel", JS_NewInt32(ctx, config.ledc_channel)) ;
    JS_SetPropertyStr(ctx, obj, "ledc_timer", JS_NewInt32(ctx, config.ledc_timer)) ;
    JS_SetPropertyStr(ctx, obj, "pixel_format", JS_NewInt32(ctx, config.pixel_format)) ;
    JS_SetPropertyStr(ctx, obj, "jpeg_quality", JS_NewInt32(ctx, config.jpeg_quality)) ;
    JS_SetPropertyStr(ctx, obj, "frame_size", JS_NewInt32(ctx, config.frame_size)) ;
    JS_SetPropertyStr(ctx, obj, "fb_count", JS_NewInt32(ctx, config.fb_count)) ;
    JS_SetPropertyStr(ctx, obj, "fb_location", JS_NewInt32(ctx, config.fb_location)) ;
    JS_SetPropertyStr(ctx, obj, "grab_mode", JS_NewInt32(ctx, config.grab_mode)) ;
    JS_SetPropertyStr(ctx, obj, "xclk_freq_hz", JS_NewInt32(ctx, config.xclk_freq_hz)) ;

    return obj ;
}

static JSValue js_camera_setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    // camera_config_t config ;
    // memset(&config, 0, sizeof(camera_config_t));

    // unsigned char partId = readPartId() ;
    // default_config( &config, partId ) ;

    // if( argv>0 ) {
    //     if( !JS_IsObject(argv[0]) ) {
    //         THROW_EXCEPTION("param config must be a object")
    //     }
    //     ASSIGN_INT_PROP(argv[0], "pin_d0", config.pin_d0, error)
    //     ASSIGN_INT_PROP(argv[0], "pin_d1", config.pin_d1, error)
    //     ASSIGN_INT_PROP(argv[0], "pin_d2", config.pin_d2, error)
    //     ASSIGN_INT_PROP(argv[0], "pin_d3", config.pin_d3, error)
    //     ASSIGN_INT_PROP(argv[0], "pin_d4", config.pin_d4, error)
    //     ASSIGN_INT_PROP(argv[0], "pin_d5", config.pin_d5, error)
    //     ASSIGN_INT_PROP(argv[0], "pin_d6", config.pin_d6, error)
    //     ASSIGN_INT_PROP(argv[0], "pin_d7", config.pin_d7, error)
        
    //     ASSIGN_INT_PROP(argv[0], "pin_xclk", config.pin_xclk, error)
    //     ASSIGN_INT_PROP(argv[0], "pin_pclk", config.pin_pclk, error)
    //     ASSIGN_INT_PROP(argv[0], "pin_vsync", config.pin_vsync, error)
    //     ASSIGN_INT_PROP(argv[0], "pin_href", config.pin_href, error)
    //     ASSIGN_INT_PROP(argv[0], "pin_sscb_sda", config.pin_sscb_sda, error)
    //     ASSIGN_INT_PROP(argv[0], "pin_sscb_scl", config.pin_sscb_scl, error)
    //     ASSIGN_INT_PROP(argv[0], "pin_pwdn", config.pin_pwdn, error)
    //     ASSIGN_INT_PROP(argv[0], "pin_reset", config.pin_reset, error)

    //     ASSIGN_INT_PROP(argv[0], "ledc_channel", config.ledc_channel, error)
    //     ASSIGN_INT_PROP(argv[0], "ledc_timer", config.ledc_timer, error)
    //     ASSIGN_INT_PROP(argv[0], "pixel_format", config.pixel_format, error)
    //     ASSIGN_INT_PROP(argv[0], "jpeg_quality", config.jpeg_quality, error)
    //     ASSIGN_INT_PROP(argv[0], "frame_size", config.frame_size, error)
    //     ASSIGN_UINT_PROP(argv[0], "fb_count", config.fb_count, error)
    //     ASSIGN_UINT_PROP(argv[0], "fb_location", config.fb_location, error)
    //     ASSIGN_UINT_PROP(argv[0], "grab_mode", config.grab_mode, error)
    //     ASSIGN_INT_PROP(argv[0], "xclk_freq_hz", config.xclk_freq_hz, error)
    // }


  camera_config_t config;
  
    config.pin_d0 = 39;
    config.pin_d1 = 37;
    config.pin_d2 = 36;
    config.pin_d3 = 38;
    config.pin_d4 = 34;
    config.pin_d5 = 19;
    config.pin_d6 = 9;
    config.pin_d7 = 22;
    config.pin_xclk = 5;
    config.pin_pclk = 35;
    config.pin_vsync = 23;
    config.pin_href = 26;
    config.pin_sscb_sda = 21;
    config.pin_sscb_scl = 18;
    config.pin_pwdn = -1;
    config.pin_reset = -1;

    config.ledc_channel = 1;
    config.ledc_timer = 1;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    config.frame_size = FRAMESIZE_CIF;
    config.jpeg_quality = 10;
    config.fb_count = 2;

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        THROW_EXCEPTION("Camera Init Failed, %s (%d)", esp_err_to_name(err), err)
    }

    return JS_TRUE ;

// error:
//     return JS_EXCEPTION ;
}


static JSValue js_camera_fb_get(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)

    return JS_UNDEFINED ;
}

static JSValue js_camera_fb_return(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)

    return JS_UNDEFINED ;
}

void be_module_camera_init() {
}

void be_module_camera_require(JSContext *ctx) {

    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue camera = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "camera", camera);

    JS_SetPropertyStr(ctx, camera, "setup", JS_NewCFunction(ctx, js_camera_setup, "setup", 1));
    JS_SetPropertyStr(ctx, camera, "defaultConfig", JS_NewCFunction(ctx, js_camera_default_config, "defaultConfig", 1));
    JS_SetPropertyStr(ctx, camera, "fbGet", JS_NewCFunction(ctx, js_camera_fb_get, "fbGet", 1));
    JS_SetPropertyStr(ctx, camera, "fbReturn", JS_NewCFunction(ctx, js_camera_fb_return, "fbReturn", 1));

    JS_FreeValue(ctx, beapi);
}

void be_module_camera_loop(JSContext *ctx) {
}

void be_module_camera_reset(JSContext *ctx) {
}

