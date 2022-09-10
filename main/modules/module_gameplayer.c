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

static char * romdata = NULL ;
static st77xx_dev_t * st77xx = NULL ;
uint16_t * disp_buff = NULL ;

uint16_t buff_width = 0 ;
uint16_t buff_lines = 0 ;
uint16_t buff_xr = 0 ;
uint16_t buff_xw = 0 ;

static int8_t i2s = -1 ;


static QueueHandle_t queVideo;
static QueueHandle_t queAudio;
// static QueueHandle_t queCmd;


#define  EMULATOR_SAMPLERATE   44200
// #define  EMULATOR_FRAGSIZE     400
#define  EMULATOR_FRAGSIZE     1024

#define  VIDEO_WIDTH        256
#define  VIDEO_HEIGHT       240


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
    disp_buff = (uint16_t *)drvspec->buff1 ;
    buff_lines = (uint16_t)drvspec->buff_lines ;

    dn4(buff_width,buff_lines,buff_xr,buff_xw)

    st77xx = (st77xx_dev_t*) drvspec->spi_dev ;
    if(!st77xx) {
        THROW_EXCEPTION("arg disp is a invalid lv.Disp object")
    }


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


static JSValue js_gameplayer_play(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    char * rompath = js_arg_to_vfspath(ctx, argv[0]) ;

    struct stat statbuf;
    if(stat(rompath,&statbuf)<0) {
        JS_ThrowReferenceError(ctx, "Failed to open rom file: %s", rompath);
        free(rompath) ;
        return JS_EXCEPTION ;
    }

    if(!S_ISREG(statbuf.st_mode)) {
        JS_ThrowReferenceError(ctx, "arg path is a file: %s", rompath);
        free(rompath) ;
        return JS_EXCEPTION ;
    }
    if(statbuf.st_size<1){
        JS_ThrowReferenceError(ctx, "rom file is empty: %s", rompath);
        free(rompath) ;
        return JS_EXCEPTION ;
    }

    printf("rom size: %d\n", (int)statbuf.st_size) ;

	FILE * fd = fopen(rompath, "r");
    free(rompath) ;

    if(NULL==fd) {
        THROW_EXCEPTION("Failed to open rome file (%d).", errno);
    }
    
    romdata = malloc(statbuf.st_size) ;
    if(!romdata) {
        free(rompath) ;
        THROW_EXCEPTION("out of memory?");
    }

    fread(romdata, 1, statbuf.st_size, fd) ;
    fclose(fd) ;

    echo_DMA("start nofrendo ...") ;
    nofrendo_main(0, NULL);
    free(romdata) ;

    return JS_UNDEFINED ;
}

void be_module_gameplayer_init() {
}

void be_module_gameplayer_require(JSContext *ctx) {

    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue gameplayer = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "gameplayer", gameplayer);

    JS_SetPropertyStr(ctx, gameplayer, "play", JS_NewCFunction(ctx, js_gameplayer_play, "play", 1));
    JS_SetPropertyStr(ctx, gameplayer, "setDisplay", JS_NewCFunction(ctx, js_gameplayer_set_display, "setDisplay", 1));
    // JS_SetPropertyStr(ctx, gameplayer, "setJoypad", JS_NewCFunction(ctx, js_gameplayer_set_joypad, "setJoypad", 1));
    // JS_SetPropertyStr(ctx, gameplayer, "setButtons", JS_NewCFunction(ctx, js_gameplayer_set_display, "setButtons", 1));
    // JS_SetPropertyStr(ctx, gameplayer, "setAudio", JS_NewCFunction(ctx, js_gameplayer_set_audio, "setAudio", 1));

    JS_FreeValue(ctx, beapi);
}

void be_module_gameplayer_loop(JSContext *ctx) {
}

void be_module_gameplayer_reset(JSContext *ctx) {
}



// ----------------------------------------------------------------
// 模拟器 Video/Audio 输入, Input 输入



#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <time.h>
#include <math.h>
#include <string.h>

//Nes stuff wants to define this as well...
#undef false
#undef true
#undef bool

// #include "noftypes.h"
#include <bitmap.h>
#include <nofconfig.h>
#include <event.h>
#include <gui.h>
#include <log.h>
#include <nes.h>
#include <nes_pal.h>
#include <nesinput.h>
#include <osd.h>
#include <stdint.h>
#include "driver/i2s.h"
#include "sdkconfig.h"



//Seemingly, this will be called only once. Should call func with a freq of frequency,
int osd_installtimer(int frequency, void *func, int funcsize, void *counter, int countersize) {
    printf("Timer install, freq=%d\n", frequency);

    const esp_timer_create_args_t periodic_timer_args = {
        .callback = func,
        .name = "nes"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 1000*1000/frequency));


    // timer=xTimerCreate("nes",configTICK_RATE_HZ/frequency, pdTRUE, NULL, func);
    // xTimerStart(timer, 0);

    return 0;
}


/**
 * Audio
 */
static void (*audio_callback)(void *buffer, int length) = NULL;
static uint16_t *audio_frame;
static bool audio_started = false ;


void osd_setsound(void (*playfunc)(void *buffer, int length)) {
    printf("nes.osd_setsound()\n") ;
   //Indicates we should call playfunc() to get more data.
   audio_callback = playfunc;
}

static void osd_stopsound(void) {
}

static void osd_init_sound_i2s(void) {
    audio_frame = heap_caps_malloc(4*EMULATOR_FRAGSIZE, MALLOC_CAP_DMA);

    if(i2s_has_setup(0)) {
        i2s = 0 ;
    }
    else if(i2s_has_setup(1)) {
        i2s = 1 ;
    }
    else {
        i2s = -1 ;
    }

    if(i2s>=0) {
        printf("audio i2c bus:%d\n",i2s) ;
        printf("i2s_set_clk(%d,%d,%d,%d)\n",i2s, EMULATOR_SAMPLERATE, 16, I2S_CHANNEL_FMT_RIGHT_LEFT) ;
        i2s_set_clk(i2s, EMULATOR_SAMPLERATE, 16, I2S_CHANNEL_FMT_RIGHT_LEFT);
    }
}

void osd_getsoundinfo(sndinfo_t *info) {
    printf("nes.osd_getsoundinfo()\n") ;
   info->sample_rate = EMULATOR_SAMPLERATE;
   info->bps = 16;
}

// /*
// ** Video
// */

static int init(int width, int height);
static void shutdown(void);
static int set_mode(int width, int height);
static void set_palette(rgb_t *pal);
static void clear(uint8 color);
static bitmap_t *lock_write(void);
static void free_write(int num_dirties, rect_t *dirty_rects);
static void custom_blit(bitmap_t *bmp, int num_dirties, rect_t *dirty_rects);
static char fb[1]; //dummy
static void * malloc_dma(int);


viddriver_t sdlDriver = {
   "MediaLayer",         /* name */
   init,          /* init */
   shutdown,      /* shutdown */
   set_mode,      /* set_mode */
   set_palette,   /* set_palette */
   clear,         /* clear */
   lock_write,    /* lock_write */
   free_write,    /* free_write */
   custom_blit,   /* custom_blit */
   malloc_dma,
   false          /* invalidate flag */
};


bitmap_t * myBitmap ;

void osd_getvideoinfo(vidinfo_t *info) {
    printf("nes.osd_getvideoinfo()\n") ;
   info->default_width = VIDEO_WIDTH;
   info->default_height = VIDEO_HEIGHT;
   info->driver = &sdlDriver;
}

/* flip between full screen and windowed */
void osd_togglefullscreen(int code) {
}

/* initialise video */
static int init(int width, int height) {
    printf("nes.init()\n") ;
	return 0;
}

static void shutdown(void) {
    printf("nes.shutdown()\n") ;
}

/* set a video mode */
static int set_mode(int width, int height) {
    printf("nes.set_mode()\n") ;
   return 0;
}


#define PALETTE_SIZE 256
uint16_t * palette = NULL ;
// rgb_t palette[256] ;

static uint16_t rgb565_conv_inv(uint16_t r,uint16_t g,uint16_t b) {
	uint16_t color = (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
	return ((color>>8) & 0xFF) | ((color&0xFF)<<8) ;
}

/* copy nes palette over to hardware */
static void set_palette(rgb_t *pal) {
    printf("nes.set_palette()\n") ;
    uint16 c;
    int i;
    for (i = 0; i < PALETTE_SIZE; i++) {
        palette[i] = rgb565_conv_inv(pal[i].r, pal[i].g, pal[i].b) ;
    }
}

/* clear all frames to a particular color */
static void clear(uint8 color) {
}



/* acquire the directbuffer for writing */
static bitmap_t *lock_write(void) {
//   SDL_LockSurface(mySurface);
   myBitmap = bmp_createhw((uint8*)fb, VIDEO_WIDTH, VIDEO_HEIGHT, VIDEO_WIDTH*2);
   return myBitmap;
}


void * malloc_dma(int size){
    printf("malloc_dma(%d) dma free: %d;\n",size, heap_caps_get_free_size(MALLOC_CAP_DMA)) ;
    // echo_DMA("before malloc_dma()") ;
    void * data = heap_caps_malloc(size, MALLOC_CAP_DMA) ;
    if(!data) {
        data = malloc(size) ;
    }
    echo_DMA("after malloc_dma()") ;
    
    return data ;
}

/* release the resource */
static void free_write(int num_dirties, rect_t *dirty_rects) {
   bmp_destroy(&myBitmap);
}


#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <sys/time.h>

static uint32_t CLOCKS_PER_MSEC = CLOCKS_PER_SEC/1000 ;

uint64_t nows(){
    return (clock() / CLOCKS_PER_SEC);
}
uint64_t nowms(){
    return (clock() * 1000 / CLOCKS_PER_SEC);
}

static uint64_t next_time = 0 ;
static uint64_t last_time = 0 ;
static uint8_t frames = 0 ;
static uint8_t dynamic_frames = 60 ;
void framerate() {

    frames ++ ;

	int64_t _now = esp_timer_get_time() ;
	if(last_time==0 || _now<last_time) {
		last_time = _now ;
        next_time = _now+1000 ;
        frames = 0 ;
		return ;
	}
    
    if( _now>next_time) {
        // printf("fps: %d\n", frames) ;
        frames = 0 ;
		next_time = _now + 1000000;
    }

	dynamic_frames = 1000000 / (_now-last_time) ;
	// printf("%dHz\n",dynamic_frames) ;
	// printf("1000/(%"PRIu64"-%"PRIu64") = %d\n", _now, last_time, dynamic_frames) ;
	last_time = _now ;
}


static void custom_blit(bitmap_t *bmp, int num_dirties, rect_t *dirty_rects) {

    vTaskDelay(0) ;

    framerate() ;

	xQueueSend(queVideo, &bmp, 0);

    audio_started = true ;

    // return ;

    // if(i2s>=0) {
    
    //     // dn2(dynamic_frames, NES_REFRESH_RATE)
    //     int left = EMULATOR_SAMPLERATE / ((dynamic_frames>10)? dynamic_frames: NES_REFRESH_RATE) ;

    //     while(left) {
    //         int n=EMULATOR_FRAGSIZE;
    //         if (n>left)
    //             n=left;
    //         audio_callback(audio_frame, n); //get more data

    //         // printf("audio %d/%d\n", n, left) ;

    //         //16 bit mono -> 32-bit (16 bit right+left)
    //         for (int i=n-1; i>=0; i--) {
    //             audio_frame[i*2+1]=audio_frame[i];
    //             audio_frame[i*2]=audio_frame[i];
    //         }
    //         size_t bytes_written ;
            
    //         int64_t t0 = gettime() ;
    //         i2s_write_expand(i2s, audio_frame, 4*n, 16,32, &bytes_written, portMAX_DELAY);
    //         // i2s_write(i2s, audio_frame, 4*n, &bytes_written, 10);
    //         printf("i2s_write_expand() t:%lld, bytes: %d\n", gettime()-t0, bytes_written) ;
    //         // dn(bytes_written)

    //         left-=n;
    //     }
    // }
}

void task_audio(void * data) {

    while(!audio_started) {
        vTaskDelay(10) ;
    }

    printf("audio start\n") ;

    while(1){
        audio_callback(audio_frame, EMULATOR_FRAGSIZE) ;

        //16 bit mono -> 32-bit (16 bit right+left)
        for (int i=EMULATOR_FRAGSIZE-1; i>=0; i--) {
            audio_frame[i*2+1]=audio_frame[i];
            audio_frame[i*2]=audio_frame[i];
        }

        size_t bytes_written ;
        // int64_t t0 = gettime() ;
        // i2s_write_expand(i2s, audio_frame, 4*EMULATOR_FRAGSIZE, 16,32, &bytes_written, portMAX_DELAY);
        i2s_write(i2s, audio_frame, 4*EMULATOR_FRAGSIZE, &bytes_written, 10);
        // printf("i2s_write() t:%lld, bytes: %d\n", gettime()-t0, bytes_written) ;
        // dn(bytes_written)

        vTaskDelay(0) ;
    }
    
        // // dn2(dynamic_frames, NES_REFRESH_RATE)
        // int left = EMULATOR_SAMPLERATE / ((dynamic_frames>10)? dynamic_frames: NES_REFRESH_RATE) ;

        // while(left) {
        //     int n=EMULATOR_FRAGSIZE;
        //     if (n>left)
        //         n=left;
        //     audio_callback(audio_frame, n); //get more data

        //     // printf("audio %d/%d\n", n, left) ;

        //     //16 bit mono -> 32-bit (16 bit right+left)
        //     for (int i=n-1; i>=0; i--) {
        //         audio_frame[i*2+1]=audio_frame[i];
        //         audio_frame[i*2]=audio_frame[i];
        //     }
        //     size_t bytes_written ;
            
        //     // int64_t t0 = gettime() ;
        //     i2s_write_expand(i2s, audio_frame, 4*n, 16,32, &bytes_written, portMAX_DELAY);
        //     // i2s_write(i2s, audio_frame, 4*n, &bytes_written, 10);
        //     // printf("i2s_write() t:%lld, bytes: %d\n", gettime()-t0, bytes_written) ;
        //     // dn(bytes_written)

        //     left-=n;
        // }
}

void st7789_write_frame(const uint8_t ** data) {

    uint16_t * to = NULL ;
    uint16_t x, y, yy ;

    for(int y = 0; y<VIDEO_HEIGHT; y+=buff_lines) {
        to = disp_buff ;

        for(yy = 0; yy<buff_lines; yy++ ) {
            for(x=buff_xr;x<buff_xr+buff_width;x++) {
				*(to++) = palette[ data[y+yy][x] ] ;
			}
        }

        st77xx_draw_rect(st77xx, buff_xw, y, buff_xw+buff_width-1, y+buff_lines-1, disp_buff) ;
    }
}

char *osd_getromdata() {
    return romdata;
}

//This runs on core 1.
static void task_video(void *arg) {

	bitmap_t *bmp=NULL;
    while(1) {
		// xQueueReceive(queVideo, &bmp, portMAX_DELAY);//skip one frame to drop to 30
		xQueueReceive(queVideo, &bmp, portMAX_DELAY);

		// printf("F:%dx%d\n",bmp->width, bmp->height) ;

        // dp(bmp->line)
		st7789_write_frame(bmp->line);

        vTaskDelay(1) ;
	}
}

#define SET_INPUT(joy, btn, bit)  event_get(event_joypad##joy##_##btn) ( btns_##joy&bit? INP_STATE_MAKE: INP_STATE_BREAK) ;


void osd_getinput(void) {

    uint32_t btns_1 = 0 ;
    uint32_t btns_2 = 0 ;

    lv_indev_t * indev=NULL ;
    indev_driver_spec_t * spec = NULL ;
    while((indev=lv_indev_get_next(indev))) {
        if( !indev->driver || !indev->driver->user_data ) {
            continue;
        }
        spec = (indev_driver_spec_t *) indev->driver->user_data ;
        if(spec->driver==INDEV_DRIVER_JOYPAD) {
            // dn(spec->conf.i2c.addr)
            if(spec->conf.i2c.addr==51) {
                btns_1 = spec->data.buttons.state ;
            }
            else if(spec->conf.i2c.addr==52) {
                btns_2 = spec->data.buttons.state ;
            }
        }
    }

    // dn2(btns_1,btns_2)

    SET_INPUT(1, up, 0x01)
    SET_INPUT(1, down, 0x02)
    SET_INPUT(1, left, 0x04)
    SET_INPUT(1, right, 0x08)
    SET_INPUT(1, a, 0x10)
    SET_INPUT(1, b, 0x20)
    SET_INPUT(1, start, 0x40)
    SET_INPUT(1, select, 0x80)

    SET_INPUT(2, up, 0x01)
    SET_INPUT(2, down, 0x02)
    SET_INPUT(2, left, 0x04)
    SET_INPUT(2, right, 0x08)
    SET_INPUT(2, a, 0x10)
    SET_INPUT(2, b, 0x20)
    SET_INPUT(2, start, 0x40)
    SET_INPUT(2, select, 0x80)
}



void osd_getmouse(int *x, int *y, int *button) {}

// /*
// ** Shutdown
// */

/* this is at the bottom, to eliminate warnings */
void osd_shutdown() {
   audio_callback = NULL;
}

static int logprint(const char *string) {
   return printf("nes:%s", string);
}

/*
** Startup
*/
int osd_init() {

    printf("osd_init()\n") ;

    if(!palette) {
        palette = heap_caps_malloc( sizeof(uint16)*PALETTE_SIZE, MALLOC_CAP_DMA ) ;
        if(!palette) {
            printf("can not malloc palette use DMA\n") ;
            palette = malloc( sizeof(uint16)*PALETTE_SIZE ) ;
        }
    }
    echo_DMA("after malloc palette") ;

	log_chain_logfunc(logprint);

	osd_init_sound_i2s() ;
    
	queVideo=xQueueCreate(1, sizeof(bitmap_t *));
    xTaskCreatePinnedToCore(&task_video, "task_video", 2048, NULL, 5, NULL, 1);

    if(i2s>=0) {
	    xTaskCreatePinnedToCore(&task_audio, "task_audio", 2048, NULL, 2, NULL, 1);
    }
    

    echo_DMA("end osd_init") ;
	return 0;
}
