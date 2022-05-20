#include "module_lvgl.h"
#include "display.h"
#include "widgets.h"
#include "widgets_gen.h"
#include "style.h"
#include "be_lv_struct_wrapper.h"
#include "be_lv_draggable.h"
#include "lvgl.h"
#include "lv_conf.h"
#include "utils.h"
#include "cutils.h"
#include "be_gl.h"
#include "be_gl_js.h"
#include "indev.h"


#ifndef SIMULATION
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#else
#include <signal.h>
#include <sys/time.h>
#endif


int16_t indev_input_x = 0 ;
int16_t indev_input_y = 0 ;
bool indev_input_pressed = false ;

bool lv_has_inited = false ;
#define LV_TICK_PERIOD_MS 1


static JSValue js_lv_coord_pct(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT16(0,val)
    return JS_NewUint32(ctx,LV_PCT(val)) ;
}

static JSValue js_lv_coord_is_pct(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT16(0,val)
    return JS_NewBool(ctx,LV_COORD_IS_PCT(val)) ;
}


static JSValue js_lvgl_load_screen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    
    CHECK_INSOF_LVOBJ("Obj", argv[0], "arg screen must a lvgl.Obj")
    lv_obj_t* cobj = (lv_obj_t*)JS_GetOpaqueInternal(argv[0]) ;

    if(argc>1) {

        lv_scr_load_anim_t anim ;
        if(!lv_scr_load_anim_jsstr_to_const(ctx, argv[1], &anim)) {
            THROW_EXCEPTION("invalid screen load animation type")
        }

        ARGV_TO_UINT32(2, dur)

        uint32_t delay = 0 ;    
        if(argc>3) {
            if(JS_ToUint32(ctx, &delay, argv[3])!=0) {
                THROW_EXCEPTION("invalid delay")
            }
        }

        bool autoDel = false ;
        if(argc>4) {
            autoDel = JS_ToBool(ctx, argv[4])? true: false ;
        }
        
        lv_scr_load_anim(cobj, anim, dur, delay, autoDel) ;
    }
    else {
        lv_disp_load_scr(cobj) ;
    }

    return JS_UNDEFINED ;
}


static JSValue js_lvgl_input_point(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    JSValue obj = JS_NewObject(ctx) ;
    JS_SetPropertyStr(ctx, obj, "x", JS_NewInt32(ctx, indev_input_x)) ;
    JS_SetPropertyStr(ctx, obj, "y", JS_NewInt32(ctx, indev_input_y)) ;
    JS_SetPropertyStr(ctx, obj, "press", indev_input_pressed? JS_TRUE: JS_FALSE) ;
    return obj ;
}

static JSValue js_lv_rgb(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_color_t color ;
    uint8_t r, g, b ;
    if(JS_ToUint32(ctx, &r, argv[0] ) ){
        THROW_EXCEPTION("arg red must be a int")
    }
    if(argc>=3) {
        if(JS_ToUint32(ctx, &g, argv[1] ) ){
            THROW_EXCEPTION("arg green must be a int")
        }
        if(JS_ToUint32(ctx, &b, argv[2] ) ){
            THROW_EXCEPTION("arg blue must be a int")
        }
    }
    else if(argc==1) {
        g = r;
        b = r ;
    }
    else {
        THROW_EXCEPTION("Missing argv")
    }
    
    color.ch.red = r * 31 / 255.0 + 0.5 ;
    color.ch.blue = b * 31 / 255.0 + 0.5 ;
#if LV_COLOR_16_SWAP == 0
    color.ch.green = g * 63 / 255.0 + 0.5 ;
#else
    g = (g * 63 / 255.0 + 0.5) ;
    color.ch.green_h = (g >> 3) & 7 ;
    color.ch.green_l = g & 7 ;
#endif

    return JS_NewUint32(ctx, color.full) ;
}

static JSValue js_lv_set_indev_active_obj(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)

    lv_obj_t * obj ;
    if( JS_IsNull(argv[0]) ) {
        obj = NULL ;
    }
    else {
        CHECK_INSOF_LVOBJ("Obj", argv[0], "arg widget of lvgl.setIndevActiveObj() must be a lvgl.Object method")
        obj = JS_GetOpaqueInternal(argv[0]) ;
        if(!obj) {
            THROW_EXCEPTION("invalid lv widget obj")
            // return JS_FALSE ;
        }
    }

    lv_indev_t * indev = lv_indev_get_act() ;
    if(!indev) {
        THROW_EXCEPTION("there is no active indev")
        // return JS_FALSE ;   
    }

    if(indev->driver->type != LV_INDEV_TYPE_POINTER) {
        THROW_EXCEPTION("active indev's type is not pointer")
        // return JS_FALSE ;
    }

    indev->proc.types.pointer.act_obj = obj ;

    return JS_UNDEFINED ;
}

static JSValue js_lvgl_loop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    lv_task_handler() ;
    return JS_UNDEFINED ;
}

static JSValue js_lvgl_tick(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    lv_tick_inc(LV_TICK_PERIOD_MS);
    return JS_UNDEFINED ;
}

static JSValue js_lvgl_refresh(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_refr_now(lv_disp_get_default()) ;
    return JS_UNDEFINED ;
}

#ifndef SIMULATION
void lv_tick_task(void *arg) {
    (void) arg;
    if(lv_has_inited) {
        lv_tick_inc(LV_TICK_PERIOD_MS);
    }
}
#else
struct sigaction tact;
struct itimerval value;
void sig_alm_handler(int sig_num) {
    if(lv_has_inited) {
        lv_tick_inc(LV_TICK_PERIOD_MS);
    }
}
#endif


void be_lv_font_symbol_require(JSContext *ctx, JSValue lvgl) {
    JSValue symbol = JS_NewObject(ctx) ;
    JS_SetPropertyStr(ctx, lvgl, "symbol", symbol);  

    JS_SetPropertyStr(ctx, symbol, "audio", JS_NewString(ctx, LV_SYMBOL_AUDIO));  
    JS_SetPropertyStr(ctx, symbol, "video", JS_NewString(ctx, LV_SYMBOL_VIDEO));  
    JS_SetPropertyStr(ctx, symbol, "list", JS_NewString(ctx, LV_SYMBOL_LIST));  
    JS_SetPropertyStr(ctx, symbol, "ok", JS_NewString(ctx, LV_SYMBOL_OK));  
    JS_SetPropertyStr(ctx, symbol, "close", JS_NewString(ctx, LV_SYMBOL_CLOSE));  
    JS_SetPropertyStr(ctx, symbol, "power", JS_NewString(ctx, LV_SYMBOL_POWER));  
    JS_SetPropertyStr(ctx, symbol, "settings", JS_NewString(ctx, LV_SYMBOL_SETTINGS));  
    JS_SetPropertyStr(ctx, symbol, "home", JS_NewString(ctx, LV_SYMBOL_HOME));  
    JS_SetPropertyStr(ctx, symbol, "download", JS_NewString(ctx, LV_SYMBOL_DOWNLOAD));  
    JS_SetPropertyStr(ctx, symbol, "drive", JS_NewString(ctx, LV_SYMBOL_DRIVE));  
    JS_SetPropertyStr(ctx, symbol, "refresh", JS_NewString(ctx, LV_SYMBOL_REFRESH));  
    JS_SetPropertyStr(ctx, symbol, "mute", JS_NewString(ctx, LV_SYMBOL_MUTE));  
    JS_SetPropertyStr(ctx, symbol, "volume_mid", JS_NewString(ctx, LV_SYMBOL_VOLUME_MID));  
    JS_SetPropertyStr(ctx, symbol, "volume_max", JS_NewString(ctx, LV_SYMBOL_VOLUME_MAX));  
    JS_SetPropertyStr(ctx, symbol, "image", JS_NewString(ctx, LV_SYMBOL_IMAGE));  
    JS_SetPropertyStr(ctx, symbol, "envelope", JS_NewString(ctx, LV_SYMBOL_ENVELOPE));  
    JS_SetPropertyStr(ctx, symbol, "edit", JS_NewString(ctx, LV_SYMBOL_EDIT));  
    JS_SetPropertyStr(ctx, symbol, "tint", JS_NewString(ctx, LV_SYMBOL_TINT));  
    JS_SetPropertyStr(ctx, symbol, "prev", JS_NewString(ctx, LV_SYMBOL_PREV));  
    JS_SetPropertyStr(ctx, symbol, "play", JS_NewString(ctx, LV_SYMBOL_PLAY));  
    JS_SetPropertyStr(ctx, symbol, "pause", JS_NewString(ctx, LV_SYMBOL_PAUSE));  
    JS_SetPropertyStr(ctx, symbol, "stop", JS_NewString(ctx, LV_SYMBOL_STOP));  
    JS_SetPropertyStr(ctx, symbol, "next", JS_NewString(ctx, LV_SYMBOL_NEXT));  
    JS_SetPropertyStr(ctx, symbol, "eject", JS_NewString(ctx, LV_SYMBOL_EJECT));  
    JS_SetPropertyStr(ctx, symbol, "left", JS_NewString(ctx, LV_SYMBOL_LEFT));  
    JS_SetPropertyStr(ctx, symbol, "right", JS_NewString(ctx, LV_SYMBOL_RIGHT));  
    JS_SetPropertyStr(ctx, symbol, "plus", JS_NewString(ctx, LV_SYMBOL_PLUS));  
    JS_SetPropertyStr(ctx, symbol, "minus", JS_NewString(ctx, LV_SYMBOL_MINUS));  
    JS_SetPropertyStr(ctx, symbol, "eye_open", JS_NewString(ctx, LV_SYMBOL_EYE_OPEN));  
    JS_SetPropertyStr(ctx, symbol, "eye_close", JS_NewString(ctx, LV_SYMBOL_EYE_CLOSE));  
    JS_SetPropertyStr(ctx, symbol, "warning", JS_NewString(ctx, LV_SYMBOL_WARNING));  
    JS_SetPropertyStr(ctx, symbol, "shuffle", JS_NewString(ctx, LV_SYMBOL_SHUFFLE));  
    JS_SetPropertyStr(ctx, symbol, "up", JS_NewString(ctx, LV_SYMBOL_UP));  
    JS_SetPropertyStr(ctx, symbol, "down", JS_NewString(ctx, LV_SYMBOL_DOWN));  
    JS_SetPropertyStr(ctx, symbol, "loop", JS_NewString(ctx, LV_SYMBOL_LOOP));  
    JS_SetPropertyStr(ctx, symbol, "directory", JS_NewString(ctx, LV_SYMBOL_DIRECTORY));  
    JS_SetPropertyStr(ctx, symbol, "upload", JS_NewString(ctx, LV_SYMBOL_UPLOAD));  
    JS_SetPropertyStr(ctx, symbol, "call", JS_NewString(ctx, LV_SYMBOL_CALL));  
    JS_SetPropertyStr(ctx, symbol, "cut", JS_NewString(ctx, LV_SYMBOL_CUT));  
    JS_SetPropertyStr(ctx, symbol, "copy", JS_NewString(ctx, LV_SYMBOL_COPY));  
    JS_SetPropertyStr(ctx, symbol, "save", JS_NewString(ctx, LV_SYMBOL_SAVE));  
    JS_SetPropertyStr(ctx, symbol, "bars", JS_NewString(ctx, LV_SYMBOL_BARS));  
    JS_SetPropertyStr(ctx, symbol, "charge", JS_NewString(ctx, LV_SYMBOL_CHARGE));  
    JS_SetPropertyStr(ctx, symbol, "paste", JS_NewString(ctx, LV_SYMBOL_PASTE));  
    JS_SetPropertyStr(ctx, symbol, "bell", JS_NewString(ctx, LV_SYMBOL_BELL));  
    JS_SetPropertyStr(ctx, symbol, "keyboard", JS_NewString(ctx, LV_SYMBOL_KEYBOARD));  
    JS_SetPropertyStr(ctx, symbol, "gps", JS_NewString(ctx, LV_SYMBOL_GPS));  
    JS_SetPropertyStr(ctx, symbol, "file", JS_NewString(ctx, LV_SYMBOL_FILE));  
    JS_SetPropertyStr(ctx, symbol, "wifi", JS_NewString(ctx, LV_SYMBOL_WIFI));  
    JS_SetPropertyStr(ctx, symbol, "battery_full", JS_NewString(ctx, LV_SYMBOL_BATTERY_FULL));  
    JS_SetPropertyStr(ctx, symbol, "battery_3", JS_NewString(ctx, LV_SYMBOL_BATTERY_3));  
    JS_SetPropertyStr(ctx, symbol, "battery_2", JS_NewString(ctx, LV_SYMBOL_BATTERY_2));  
    JS_SetPropertyStr(ctx, symbol, "battery_1", JS_NewString(ctx, LV_SYMBOL_BATTERY_1));  
    JS_SetPropertyStr(ctx, symbol, "battery_empty", JS_NewString(ctx, LV_SYMBOL_BATTERY_EMPTY));  
    JS_SetPropertyStr(ctx, symbol, "usb", JS_NewString(ctx, LV_SYMBOL_USB));  
    JS_SetPropertyStr(ctx, symbol, "bluetooth", JS_NewString(ctx, LV_SYMBOL_BLUETOOTH));  
    JS_SetPropertyStr(ctx, symbol, "trash", JS_NewString(ctx, LV_SYMBOL_TRASH));  
    JS_SetPropertyStr(ctx, symbol, "backspace", JS_NewString(ctx, LV_SYMBOL_BACKSPACE));  
    JS_SetPropertyStr(ctx, symbol, "sd_card", JS_NewString(ctx, LV_SYMBOL_SD_CARD));  
    JS_SetPropertyStr(ctx, symbol, "new_line", JS_NewString(ctx, LV_SYMBOL_NEW_LINE));  

    // s16
    JS_SetPropertyStr(ctx, symbol, "search", JS_NewString(ctx, "\xEF\x80\x82"));  
}

void be_module_lvgl_init() {
    be_lv_display_init() ;
    be_lv_widgets_gen_init() ;
    be_lv_widgets_init() ;
    be_lv_styles_init() ;
    be_lv_structs_init() ;
    be_lv_draggable_init() ;
    be_gl_init() ;
    be_lv_indev_init() ;

#ifndef SIMULATION
    // lvgl 时钟
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "lvgl"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));
#else 
    // lvgl 时钟
    tact.sa_handler = sig_alm_handler;
    tact.sa_flags = SA_RESTART ;  // SA_RESTART 在 select() 时遇到中断发生，在中断完成后自动恢复
    sigemptyset(&tact.sa_mask);
    sigaction(SIGALRM, &tact, NULL);
    
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = 1000 * LV_TICK_PERIOD_MS;
    value.it_interval = value.it_value;
    setitimer(ITIMER_REAL, &value, NULL);
#endif
}

void be_module_lvgl_require(JSContext *ctx) {
    if(!lv_has_inited) {
        lv_init();
        lv_png_init() ;
        
        lv_has_inited = true ;
    }

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beapi = JS_GetPropertyStr(ctx, global, "beapi") ;

    JSValue EventEmitterProto = js_get_glob_prop(ctx, 3, "beapi", "EventEmitter", "prototype") ;
    JSValue lvgl = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "lvgl", lvgl);
    JS_SetPropertyStr(ctx, lvgl, "__proto__", EventEmitterProto);
    JS_SetPropertyStr(ctx, lvgl, "_handles", JS_NewObject(ctx));
    

    JS_SetPropertyStr(ctx, lvgl, "loop", JS_NewCFunction(ctx, js_lvgl_loop, "loop", 1));
    JS_SetPropertyStr(ctx, lvgl, "tick", JS_NewCFunction(ctx, js_lvgl_tick, "tick", 1));
    JS_SetPropertyStr(ctx, lvgl, "refresh", JS_NewCFunction(ctx, js_lvgl_refresh, "refresh", 1));
    JS_SetPropertyStr(ctx, lvgl, "loadScreen", JS_NewCFunction(ctx, js_lvgl_load_screen, "loadScreen", 1));
    JS_SetPropertyStr(ctx, lvgl, "inputPoint", JS_NewCFunction(ctx, js_lvgl_input_point, "inputPoint", 1));
    JS_SetPropertyStr(ctx, lvgl, "pct", JS_NewCFunction(ctx, js_lv_coord_pct, "pct", 1));
    JS_SetPropertyStr(ctx, lvgl, "isPct", JS_NewCFunction(ctx, js_lv_coord_is_pct, "isPct", 1));
    JS_SetPropertyStr(ctx, lvgl, "rgb", JS_NewCFunction(ctx, js_lv_rgb, "rgb", 1));
    JS_SetPropertyStr(ctx, lvgl, "setIndevActiveObj", JS_NewCFunction(ctx, js_lv_set_indev_active_obj, "setIndevActiveObj", 1));

    JS_SetPropertyStr(ctx, lvgl, "SizeContent", JS_NewUint32(ctx, LV_SIZE_CONTENT));
    JS_SetPropertyStr(ctx, lvgl, "MaxCoord", JS_NewUint32(ctx, LV_COORD_MAX));
    JS_SetPropertyStr(ctx, lvgl, "MinCoord", JS_NewUint32(ctx, LV_COORD_MIN));

    be_lv_display_require(ctx, lvgl) ;
    be_lv_widgets_require(ctx, lvgl) ;
    be_lv_widgets_gen_require(ctx, lvgl) ;
    be_lv_styles_require(ctx, lvgl) ;
    be_lv_font_symbol_require(ctx, lvgl) ;
    be_lv_structs_require(ctx, lvgl) ;
    be_lv_draggable_require(ctx, lvgl) ;
    be_gl_require(ctx, lvgl) ;
    be_lv_indev_require(ctx, lvgl) ;

    JS_FreeValue(ctx, global);
    JS_FreeValue(ctx, beapi);
    JS_FreeValue(ctx, EventEmitterProto);
}



void be_module_lvgl_reset(JSContext *ctx) {
    be_lv_display_reset(ctx) ;
    be_lv_indev_reset(ctx) ;
}

void be_module_lvgl_loop(JSContext *ctx)  {
    if(lv_has_inited) {
        lv_task_handler() ;
    }
}
