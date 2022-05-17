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

char * romdata = NULL ;
st77xx_dev_t * st77xx = NULL ;

int input_joypad1_bus = -1 ;
int input_joypad1_addr = -1 ;

int input_joypad2_bus = -1 ;
int input_joypad2_addr = -1 ;

unsigned char input_joypad1_byte = 0 ;
unsigned char input_joypad2_byte = 0 ;




static JSValue js_gameplayer_set_display(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(1)
    lv_disp_t * disp = (lv_disp_t *)JS_GetOpaque(argv[0], lv_disp_js_class_id()) ;
    if(!disp || !disp->driver || !disp->driver->user_data) {
        THROW_EXCEPTION("arg disp is a invalid lv.Disp object")
    }

    st77xx = (st77xx_dev_t*) ((disp_drv_data_t*)disp->driver->user_data)->spi_dev ;

    return JS_UNDEFINED;
}



static JSValue js_gameplayer_set_joypad(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    
    if( JS_ToInt32(ctx, &input_joypad1_bus, argv[0])!=0 ) {
        THROW_EXCEPTION("Invalid param type")
	}
    if(argc>1) {
        if( JS_ToInt32(ctx, &input_joypad1_addr, argv[1])!=0 ) {
            THROW_EXCEPTION("Invalid param type")
        }
    }
    else {
        input_joypad1_addr = 51 ;
    }
    
    if(argc>2) {
        if( JS_ToInt32(ctx, &input_joypad2_bus, argv[2])!=0 ) {
            THROW_EXCEPTION("Invalid param type")
        }
        if(argc>3) {
            if( JS_ToInt32(ctx, &input_joypad2_addr, argv[3])!=0 ) {
                THROW_EXCEPTION("Invalid param type")
            }
        }
        else {
            input_joypad2_addr = 52 ;
        }
    }

    return JS_UNDEFINED ;
}

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
    
    romdata = heap_caps_malloc(statbuf.st_size, MALLOC_CAP_SPIRAM) ;
    if(!romdata) {
        free(rompath) ;
        THROW_EXCEPTION("out of memory?");
    }

    fread(romdata, 1, statbuf.st_size, fd) ;
    fclose(fd) ;

    // xTaskCreatePinnedToCore(&start_nofrendo, "start_nofrendo", 16*1024, NULL, 5, NULL, 0);
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
    JS_SetPropertyStr(ctx, gameplayer, "setJoypad", JS_NewCFunction(ctx, js_gameplayer_set_joypad, "setJoypad", 1));
    JS_SetPropertyStr(ctx, gameplayer, "setButtons", JS_NewCFunction(ctx, js_gameplayer_set_display, "setButtons", 1));
    JS_SetPropertyStr(ctx, gameplayer, "setAudio", JS_NewCFunction(ctx, js_gameplayer_set_display, "setAudio", 1));

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


#include "pwm_audio.h"

#define  EMULATOR_SAMPLERATE   44200
#define  EMULATOR_FRAGSIZE     1024

#define  VIDEO_WIDTH        256
#define  VIDEO_HEIGHT       240


#define JOYPAD1_BTNS 0
#define JOYPAD1_BTN_UP 0
#define JOYPAD1_BTN_DOWN 1
#define JOYPAD1_BTN_LEFT 2
#define JOYPAD1_BTN_RIGHT 3
#define JOYPAD1_BTN_A 4
#define JOYPAD1_BTN_B 5
#define JOYPAD1_BTN_X 6
#define JOYPAD1_BTN_Y 7
#define JOYPAD1_BTN_START 8
#define JOYPAD1_BTN_SELECT 9
#define JOYPAD1_BTNS_END 9

#define JOYPAD2_BTNS 10
#define JOYPAD2_BTN_UP 10
#define JOYPAD2_BTN_DOWN 11
#define JOYPAD2_BTN_LEFT 12
#define JOYPAD2_BTN_RIGHT 13
#define JOYPAD2_BTN_A 14
#define JOYPAD2_BTN_B 15
#define JOYPAD2_BTN_X 16
#define JOYPAD2_BTN_Y 17
#define JOYPAD2_BTN_START 18
#define JOYPAD2_BTN_SELECT 19
#define JOYPAD2_BTNS_END 19



FILE *fd = NULL;
typedef struct {
    // The "RIFF" chunk descriptor
    uint8_t ChunkID[4];
    int32_t ChunkSize;
    uint8_t Format[4];
    // The "fmt" sub-chunk
    uint8_t Subchunk1ID[4];
    int32_t Subchunk1Size;
    int16_t AudioFormat;
    int16_t NumChannels;
    int32_t SampleRate;
    int32_t ByteRate;
    int16_t BlockAlign;
    int16_t BitsPerSample;
    // The "data" sub-chunk
    uint8_t Subchunk2ID[4];
    int32_t Subchunk2Size;
} wav_header_t;
size_t file_writen = 0 ;
bool start_save = false ;
bool is_open = false ;


typedef struct BottonEvent {
	// uint8_t btn ;
	uint8_t event ;
	uint8_t bit ;
};
struct BottonEvent joypad1_btns[] = {
	{ event_joypad1_a, 0x01 } ,
	{ event_joypad1_b, 0x02 } ,
	{ event_joypad1_up, 0x04 } ,
	{ event_joypad1_down, 0x08 } ,
	{ event_joypad1_left, 0x10 } ,
	{ event_joypad1_right, 0x20 } ,
	{ event_joypad1_start, 0x40 } ,
	{ event_joypad1_select, 0x80 } ,
} ;
struct BottonEvent joypad2_btns[] = {
	{ event_joypad2_a, 0x01 } ,
	{ event_joypad2_b, 0x02 } ,
	{ event_joypad2_up, 0x04 } ,
	{ event_joypad2_down, 0x08 } ,
	{ event_joypad2_left, 0x10 } ,
	{ event_joypad2_right, 0x20 } ,
	{ event_joypad2_start, 0x40 } ,
	{ event_joypad2_select, 0x80 } ,
} ;


QueueHandle_t vidQueue;

TimerHandle_t timer;

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



void osd_setsound(void (*playfunc)(void *buffer, int length)) {
    printf("nes.osd_setsound()\n") ;
   //Indicates we should call playfunc() to get more data.
   audio_callback = playfunc;
}

static void osd_stopsound(void) {
}
static int osd_init_sound(void) {

	audio_frame = malloc(2*EMULATOR_FRAGSIZE);
    if(!audio_frame) {
        printf("audio_frame: %p\n",audio_frame) ;
    }

    // echo_DMA("before i2s_driver_install()") ;
	// i2s_config_t cfg = {
    //     .mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
    //     .sample_rate = EMULATOR_SAMPLERATE,
    //     .bits_per_sample = 16,
    //     .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    //     .communication_format = I2S_COMM_FORMAT_I2S_MSB, //I2S_COMM_FORMAT_STAND_PCM_SHORT,
    //     // .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // high interrupt priority
    //     .intr_alloc_flags = 0, // high interrupt priority
    //     .dma_buf_count = 2,
    //     .dma_buf_len = EMULATOR_FRAGSIZE
    // } ;
    // esp_err_t res = i2s_driver_install(I2S_NUM_0, &cfg, 0, NULL) ;
	// if(res!=ESP_OK) {
    //     printf("i2s_driver_install() failed: %d\n",res) ;
    //     return ;
    // }
    // echo_DMA("after i2s_driver_install()") ;
    // // if(i2s_set_sample_rates(I2S_NUM_0, 22050)!=ESP_OK) {
    // //     printf("i2s_set_sample_rates() failed\n") ;
    // //     return ;
    // // }
	// if(i2s_set_pin(0, NULL)!=ESP_OK) {
    //     printf("i2s_set_pin() failed\n") ;
    //     return ;
    // }
	// if(i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN)!=ESP_OK) {
    //     printf("i2s_set_dac_mode() failed\n") ;
    //     return ;
    // }

    // if(i2s_start(I2S_NUM_0)!=ESP_OK) {
    //     printf("i2s_start() failed\n") ;
    //     return ;
    // }
	

    pwm_audio_config_t pac;
    pac.duty_resolution    = LEDC_TIMER_10_BIT;
    pac.gpio_num_left      = 26;
    pac.ledc_channel_left  = LEDC_CHANNEL_0;
    pac.gpio_num_right     = 25;
    pac.ledc_channel_right = LEDC_CHANNEL_1;
    pac.ledc_timer_sel     = LEDC_TIMER_0;
    pac.tg_num             = TIMER_GROUP_0;
    pac.timer_num          = TIMER_0;
    pac.ringbuf_len        = 1024 * 8;
    pwm_audio_init(&pac);

    pwm_audio_set_param(EMULATOR_SAMPLERATE, 16, 1);

    
	fd = fopen("/fs/home/become/gamemusic.wav", "w");
    is_open = true ;
    char header[44] = {0} ;
    fwrite(header, 1, 44, fd);
    fflush(fd) ;

    file_writen = 0 ;

	audio_callback = NULL;


	return 0;
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

// uint16 myPalette[256] ;
rgb_t myPalette[256] ;

/* copy nes palette over to hardware */
static void set_palette(rgb_t *pal) {
    printf("nes.set_palette()\n") ;
    uint16 c;
    int i;
    for (i = 0; i < 256; i++) {
        myPalette[i].b = pal[i].b ;
        myPalette[i].g = pal[i].g ;
        myPalette[i].r = pal[i].r ;
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

/* release the resource */
static void free_write(int num_dirties, rect_t *dirty_rects) {
   bmp_destroy(&myBitmap);
}


#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <sys/time.h>

// static uint32_t CLOCKS_PER_MSEC = CLOCKS_PER_SEC/1000 ;

// uint64_t nows(){
//     return (clock() / CLOCKS_PER_SEC);
// }
// uint64_t nowms(){
//     return (clock() * 1000 / CLOCKS_PER_SEC);
// }


uint64_t last_time = 0 ;
uint8_t dynamic_frames = 60 ;
void framerate() {
	int64_t _now = esp_timer_get_time() ;
	if(last_time==0 || _now<last_time) {
		last_time = _now ;
		return ;
	}

    // printf("time: %i64d -> %i64d\n", last_time, _now) ;
	dynamic_frames = 10000000 / (_now-last_time) ;
	// printf("%dHz\n",dynamic_frames) ;
	// printf("1000/(%"PRIu64"-%"PRIu64") = %d\n", _now, last_time, dynamic_frames) ;
	last_time = _now ;
}




static void custom_blit(bitmap_t *bmp, int num_dirties, rect_t *dirty_rects) {

    vTaskDelay(1) ;


	xQueueSend(vidQueue, &bmp, 0);

	// framerate() ;
	// int left=EMULATOR_SAMPLERATE / ((dynamic_frames>10)? dynamic_frames: NES_REFRESH_RATE) ;
	int left=EMULATOR_SAMPLERATE / NES_REFRESH_RATE ;
	// printf("dfr=%d, left=%d\n",dynamic_frames,left) ;

	esp_err_t res = ESP_FAIL ;
	while(left) {

		int n=EMULATOR_FRAGSIZE;
		if (n>left) n=left;
		audio_callback(audio_frame, n); //get more data
		// // 8 bit -> 16 bit
		// for (int i=n-1; i>=0; i--) {
		// 	audio_frame[i*2+1]=audio_frame[i];
		// 	audio_frame[i*2]=audio_frame[i];
		// }
		// printf("(%d) ... %x,%x,%x,%x,%x,%x,%x,%x\n", n, audio_frame[n-8], audio_frame[n-7], audio_frame[n-6], audio_frame[n-5], audio_frame[n-4], audio_frame[n-3], audio_frame[n-2], audio_frame[n-1]) ;

        if(!start_save && audio_frame[0]!=0 && audio_frame[2]!=0 && audio_frame[3]!=0) {
            start_save = true ;
            printf("music start \n") ;
        }
        if(start_save) {
            if(file_writen<=1048576) {
                file_writen+= fwrite((void *)audio_frame, 2, n, fd) * 2 ;
                fflush(fd) ;
            }
            else {
                if(is_open) {
                    dn(file_writen)
                    is_open = false ;
                    fclose(fd) ;
                }
            }
        }

		size_t bytes_written = 0 ;
		// res = i2s_write(0, audio_frame, 2*n, &bytes_written, 100);
        // pwm_audio_write(audio_frame, 2*n, &bytes_written, 100);
        if(bytes_written!=2*n) {
		    // printf("i2s written: %d/%d\n", 2*n, bytes_written) ;
        }

        

		left-=n;
	}

// 	printf("audio frame end\n") ;

}

static uint16_t rgb565_conv_inv(uint16_t r,uint16_t g,uint16_t b) {
	uint16_t color = (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
	return ((color>>8) & 0xFF) | ((color&0xFF)<<8) ;
}

uint16_t line[240*8] ;
void st7789_write_frame(const uint8_t ** data) {

    int x, y;
	// int height = 240 ;
	// int width = 240 ;
	int i = 0 ;
	int idx ;

	for(int b=0; b<30; b++) {
		i = 0 ;
		for(int by=0;by<8;by++) {
			y = b*8 + by ;
			
			// nofrendo 的实际宽度为 256, 屏幕宽 240，所以从8开始
			for(x=8;x<248;x++) {
				idx = data[y][x] ;
				line[i++] = rgb565_conv_inv(myPalette[idx].r, myPalette[idx].g, myPalette[idx].b) ;
			}
		}
		
		// lcdDrawRectPixels(&lcd, 0, y, 239, y+7, line) ;
        st77xx_draw_rect(st77xx, 0, y, 239, y+7, line) ;
	}
}

char *osd_getromdata() {
    return romdata;
}

//This runs on core 1.
static void videoTask(void *arg) {
	bitmap_t *bmp=NULL;
    while(1) {
		// xQueueReceive(vidQueue, &bmp, portMAX_DELAY);//skip one frame to drop to 30
		xQueueReceive(vidQueue, &bmp, portMAX_DELAY);

		// printf("F:%dx%d\n",bmp->width, bmp->height) ;

		st7789_write_frame(bmp->line);
	}
}


void osd_getinput(void) {

    input_joypad1_byte = 0 ;
    input_joypad2_byte = 0 ;

    // printf("osd_getinput(),%d,%d\n",input_joypad1_addr,input_joypad1_bus) ;

    if(input_joypad1_bus>-1 && input_joypad1_addr>0) {
        I2C_BEGIN_READ(input_joypad1_addr)
        I2C_RECV(&input_joypad1_byte,1)
        I2C_COMMIT(input_joypad1_bus)
    }
    if(input_joypad2_bus>-1 && input_joypad2_addr>0) {
        I2C_BEGIN_READ(input_joypad2_addr)
        I2C_RECV(&input_joypad2_byte,1)
        I2C_COMMIT(input_joypad2_bus)
    }

	// struct InputGPIO * pins = loop_input() ;
	
	// printf("_btn_cnt: %d, start:%d, up: %d,  down: %d\n"
	// 	, _btn_cnt
	// 	, pins[JOYPAD1_BTN_START].pressdown
	// 	, pins[JOYPAD1_BTN_UP].pressdown
	// 	, pins[JOYPAD1_BTN_DOWN].pressdown
	// ) ;
	
	for(int i=0; i< 8; i++ ) {
		event_get(joypad1_btns[i].event)( (input_joypad1_byte & joypad1_btns[i].bit)? INP_STATE_MAKE: INP_STATE_BREAK ) ;
	}
	for(int i=0; i< 8; i++ ) {
		event_get(joypad2_btns[i].event)( (input_joypad2_byte & joypad2_btns[i].bit)? INP_STATE_MAKE: INP_STATE_BREAK ) ;
	}
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

	log_chain_logfunc(logprint);

	if (osd_init_sound())
		return -1;
    
	vidQueue=xQueueCreate(1, sizeof(bitmap_t *));
	xTaskCreatePinnedToCore(&videoTask, "videoTask", 2048, NULL, 5, NULL, 1);

	return 0;
}
