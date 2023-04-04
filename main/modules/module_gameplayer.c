#include "module_gameplayer.h"
#include "module_serial.h"
#include "display.h"
#include "disp_st77XX.h"
#include "utils.h"
#include "nofrendo.h"
#include "module_fs.h"
#include <errno.h>
#include "psram.h"
#include "esp_system.h"
#include "indev.h"
#include "indev_i2c.h"

#include "player_nofrendo.h"
#include "player_gnuboy.h"



#define EMULATOR_NOFRENDO 1
#define EMULATOR_GUNBOY 2


static JSValue js_gameplayer_set_display(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(1)
    lv_disp_t * disp = (lv_disp_t *)JS_GetOpaque(argv[0], lv_disp_js_class_id()) ;
    if(!disp || !disp->driver || !disp->driver->user_data) {
        THROW_EXCEPTION("arg disp is a invalid lv.Disp object")
    }
    lv_disp_drv_t * drv = (lv_disp_drv_t*)disp->driver ;
    if(VIDEO_HEIGHT!=drv->ver_res) {
        THROW_EXCEPTION("disp height must be %d", VIDEO_HEIGHT)
    }

    uint16_t buff_width = 0 ;
    uint16_t buff_xr = 0 ;
    uint16_t buff_xw = 0 ;

    if(VIDEO_WIDTH>drv->hor_res) {
        buff_xr = (VIDEO_WIDTH-drv->hor_res)/2 ;
        buff_xw = 0 ;
        buff_width = drv->hor_res ;
    }
    else {
        buff_xr = 0 ;
        buff_xw = (drv->hor_res-VIDEO_WIDTH)/2 ;
        buff_width = VIDEO_WIDTH ;
    }

    disp_drv_spec_t * drvspec = (disp_drv_spec_t *)drv->user_data ;

    player_nofrendo_set_video(
        drvspec->spi_dev
        , (uint16_t *)drvspec->buff1
        , buff_width
        , (uint16_t)drvspec->buff_lines
        , buff_xr
        , buff_xw
    ) ;

    if(!drvspec->spi_dev) {
        THROW_EXCEPTION("arg disp is a invalid lv.Disp object")
    }

    // 黑屏
    st77xx_fill_rect((st77xx_dev_t*)drvspec->spi_dev,0,0,drv->hor_res-1,drv->ver_res, 0) ;

    return JS_UNDEFINED;
}

// static JSValue js_gameplayer_set_audio(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

//     CHECK_ARGC(1)
//     ARGV_TO_UINT8(0, _i2s)
//     if(_i2s>2) {
//         THROW_EXCEPTION("arg i2s must be 0 or 1")
//     }

//     i2s = (int8_t)_i2s ;

//     return JS_UNDEFINED;
// }


static JSValue js_gameplayer_set_buttons(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_UINT8(0, pin)
    ARGV_TO_UINT8(1, key)

    dn2(pin, key)

    return JS_UNDEFINED;
}

/**
 * 
 * @param rom path 
 * @param emulator: 1: nofrendo; 2: gunboy
 */

static JSValue js_gameplayer_play(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    char * rompath = js_arg_to_vfspath(ctx, argv[0]) ;

    int emulator = EMULATOR_NOFRENDO ;
    if(argc>1) {
        if(JS_ToInt32(ctx, &emulator, argv[1])!=0 || (emulator!=EMULATOR_NOFRENDO && emulator!=EMULATOR_GUNBOY) ) {
            THROW_EXCEPTION("invalid emulator")
        }
    }

    echo_DMA("start game ...") ;
    
    switch(emulator) {
        case EMULATOR_NOFRENDO :
            if(!player_nofrendo_load_rom(rompath)) {
                free(rompath) ;
                THROW_EXCEPTION("load rom failed") ;
            }
            int retcode = player_nofrendo_main();
            break ;
#if CONFIG_IDF_TARGET_ESP32S3
        case EMULATOR_GUNBOY :
            player_gnuboy_main(rompath);
            break ;
#endif
        default :
            free(rompath) ;
            THROW_EXCEPTION("unknown emulator")
            break ;
    }

    free(rompath) ;
    return JS_UNDEFINED ;
}

// void be_module_gameplayer_init() {
// }

void be_module_gameplayer_require(JSContext *ctx) {

    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue gameplayer = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "gameplayer", gameplayer);

    JS_SetPropertyStr(ctx, gameplayer, "play", JS_NewCFunction(ctx, js_gameplayer_play, "play", 1));
    JS_SetPropertyStr(ctx, gameplayer, "setBtns", JS_NewCFunction(ctx, js_gameplayer_set_buttons, "setBtns", 1));
    JS_SetPropertyStr(ctx, gameplayer, "setDisplay", JS_NewCFunction(ctx, js_gameplayer_set_display, "setDisplay", 1));

    // JS_SetPropertyStr(ctx, gameplayer, "setJoypad", JS_NewCFunction(ctx, js_gameplayer_set_joypad, "setJoypad", 1));
    // JS_SetPropertyStr(ctx, gameplayer, "setButtons", JS_NewCFunction(ctx, js_gameplayer_set_display, "setButtons", 1));
    // JS_SetPropertyStr(ctx, gameplayer, "setAudio", JS_NewCFunction(ctx, js_gameplayer_set_audio, "setAudio", 1));

    JS_FreeValue(ctx, beapi);
}

// void be_module_gameplayer_loop(JSContext *ctx) {
// }

// void be_module_gameplayer_reset(JSContext *ctx) {
// }

