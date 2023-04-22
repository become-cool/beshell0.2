#include "player_nofrendo.h"
#include "module_serial.h"
#include "display.h"
#include "utils.h"
#include "nofrendo.h"
#include "module_fs.h"
#include <errno.h>
#include "psram.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "esp_task_wdt.h"
#include "indev.h"
#include "indev_i2c.h"
#include "esp32_perfmon.h"

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




static char * romdata = NULL ;
static st77xx_dev_t * st77xx = NULL ;
static uint16_t * disp_buff = NULL ;

static uint16_t buff_width = 0 ;
static uint16_t buff_lines = 0 ;
static uint16_t buff_xr = 0 ;
static uint16_t buff_xw = 0 ;

static int8_t i2s = -1 ;


static QueueHandle_t queVideo;
static QueueHandle_t queAudio;
// static QueueHandle_t queCmd;

#define AUDIO_RECORD 0

#define  EMULATOR_SAMPLERATE   22050
// #define  EMULATOR_FRAGSIZE     400
#define  EMULATOR_FRAGSIZE     400


// FILE * hsound = NULL ;
// size_t sound_smps = 0 ;



void player_nofrendo_set_video(st77xx_dev_t * dev, uint16_t * buff, uint16_t width, uint16_t lines, uint16_t xr, uint16_t xw) {
    st77xx = dev ;
    disp_buff = buff ;
    buff_width = width ;
    buff_lines = lines ;
    buff_xr = xr ;
    buff_xw = xw ;
}

int player_nofrendo_load_rom(char * rompath) {
    
    struct stat statbuf;
    if(stat(rompath,&statbuf)<0) {
        printf("Failed to open rom file: %s\n", rompath);
        return false ;
    }

    if(!S_ISREG(statbuf.st_mode)) {
        printf("arg path is a file: %s\n", rompath);
        return false ;
    }
    if(statbuf.st_size<1){
        printf("rom file is empty: %s\n", rompath);
        return false ;
    }

    printf("rom size: %d\n", (int)statbuf.st_size) ;

	FILE * fd = fopen(rompath, "r");

    if(NULL==fd) {
        printf("Failed to open rome file (%d)\n", errno);
        return false ;
    }
    
    if(romdata) {
        free(romdata) ;
        romdata = NULL ;
    }
    romdata = malloc(statbuf.st_size) ;
    
    if(!romdata) {
        printf("out of memory?\n");
        fclose(fd) ;
        return false ;
    }

    size_t read_bytes = 1024 * 10 ;
    void * read_ptr = romdata ;
    size_t left_bytes = statbuf.st_size ;
    while( left_bytes>0) {

        if(read_bytes>left_bytes) {
            read_bytes = left_bytes ;
        }
        
        fread(read_ptr, 1, read_bytes, fd) ;

        read_ptr +=read_bytes ;
        left_bytes-=read_bytes ;
    }

    fclose(fd) ;

    return true ;
}

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
static int16_t *audio_frame;
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

    // if(i2s_has_setup(0)) {
    //     i2s = 0 ;
    // }
    // else if(i2s_has_setup(1)) {
    //     i2s = 1 ;
    // }
    // else {
    //     i2s = -1 ;
    // }
    
	// printf("init audio i2s dev\n") ;


    i2s = 0 ;
    // if(i2s>=0) {

    //     // if( i2s_set_sample_rates(i2s, EMULATOR_SAMPLERATE)){
    //     //     printf("Failed: %s()\n", "i2s_set_sample_rates") ;
    //     // }


    //     // printf("audio i2c bus:%d\n",i2s) ;
    //     // printf("i2s_set_clk(%d,%d,%d,%d)\n",i2s, EMULATOR_SAMPLERATE, 16, I2S_CHANNEL_FMT_ALL_LEFT) ;
    //     // i2s_set_clk(i2s, EMULATOR_SAMPLERATE, 16, I2S_CHANNEL_FMT_ALL_LEFT);
    // }

    
    // i2s_driver_uninstall(i2s) ;
        
    // i2s_config_t i2s_config = {
    //     .mode = I2S_MODE_MASTER | I2S_MODE_TX,
    //     .sample_rate=EMULATOR_SAMPLERATE,
    //     .bits_per_sample= 16 ,
    //     .channel_format=I2S_CHANNEL_FMT_RIGHT_LEFT,
    //     .communication_format=I2S_COMM_FORMAT_I2S_MSB,
    //     .intr_alloc_flags = 0, // default interrupt priority
    //     .dma_buf_count=4,
    //     .dma_buf_len=512,
    //     .use_apll = false
    // };

    // i2s_pin_config_t pin_config = {
    //     .bck_io_num = 33,       // BCK
    //     .ws_io_num = 32,        // LR
    //     .data_out_num = 22,     // sdata
    //     .data_in_num = I2S_PIN_NO_CHANGE
    // };
    // printf("i2s data:%d, bclk:%d, lrc:%d\n",AUDIO_PIN_DATA,AUDIO_PIN_BCLK,AUDIO_PIN_LRC) ;

    // if( i2s_driver_install(i2s, &i2s_config, 0, NULL) != ESP_OK ){
    //     printf("Failed: %s()\n", "i2s_driver_install") ;
    // }
    // if( i2s_set_pin(i2s, &pin_config) ){
    //     printf("Failed: %s()\n", "i2s_set_pin") ;
    // }

    // hsound = fopen("/fs/home/become/xxx.pcm","w") ;
    // sound_smps = 0 ;
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
    // printf("nes.set_palette()\n") ;
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

#if AUDIO_RECORD

bool record_audio = false ;
FILE * record_audio_fd = NULL ;
static char * record_audio_buff = NULL ;
static char * record_audio_writer = NULL ;
#define RECORD_LEN 1024*1024


static void record_on(){

    printf("record_on()\n") ;

    record_audio = true ;
    // record_audio_buff = heap_caps_malloc(RECORD_LEN, MALLOC_CAP_SPIRAM);
    record_audio_writer = record_audio_buff ;
}



static void record_off(){
    
    printf("record_off()\n") ;

    record_audio_fd = fopen("/fs/mnt/sd/record.pcm", "w") ;
    if(!record_audio_fd) {
        printf("fopen() record.pcm failed\n") ;
    } else {
        size_t pcmlen = record_audio_writer - record_audio_buff ;
        dn(pcmlen)
        fwrite(record_audio_buff, 1, pcmlen, record_audio_fd) ;
        fclose(record_audio_fd) ;
        record_audio_fd = NULL ;
    }

    record_audio = false ;
}

static void record_proc(void * data, size_t datalen){

    size_t wroten = record_audio_writer - record_audio_buff ;
    size_t left = RECORD_LEN - wroten ;
    if(datalen>left) {
        datalen = left ;
    }

    memcpy(record_audio_writer, data, datalen) ;
    record_audio_writer+= datalen ;

    if(record_audio_writer-record_audio_buff>=RECORD_LEN) {
        record_off() ;
    }

    // if(record_audio_fd) {
    //     fwrite(data, 1, datalen, record_audio_fd) ;
    // }
}

#endif

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

static bool printed = false ;

static void audio_play_frame() {

}


static uint8_t print_framerate_counter = 0 ;

static void custom_blit(bitmap_t *bmp, int num_dirties, rect_t *dirty_rects) {

    esp_task_wdt_reset() ;

	xQueueSend(queVideo, &bmp, 0);

    audio_started = true ;
}

void task_audio(void * data) {

    while(!audio_started) {
        vTaskDelay(10) ;
    }

    printf("audio start\n") ;

    while(1){

        framerate() ;
        if(print_framerate_counter++%60==0) {
            printf("fps:%d, cpu0:%d%%, cpu1:%d%%\n", dynamic_frames, cpu0_usage(), cpu1_usage()) ;
        }

        int left = EMULATOR_SAMPLERATE / NES_REFRESH_RATE ;
        size_t written ;

        while(left) {
            int n=EMULATOR_FRAGSIZE;
            if (n>left)
                n=left;
            audio_callback(audio_frame, n); //get more data

            //16 bit mono -> 32-bit (16 bit right+left)
            for (int i=n-1; i>=0; i--) {
                audio_frame[i] = audio_frame[i]/4 ;
                audio_frame[i*2+1]=audio_frame[i];
                audio_frame[i*2]=audio_frame[i];
            }
            
            i2s_write(i2s, audio_frame, 4*n, &written, portMAX_DELAY);
            left-=n;

#if AUDIO_RECORD
            if(record_audio) {
                record_proc(audio_frame, written) ;
            }
#endif
        }

        vTaskDelay(1) ;
    }
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
#if AUDIO_RECORD
        // if(!record_audio)
#endif
        // {
		    st7789_write_frame(bmp->line);
        // }

        vTaskDelay(1) ;
	}
}

#define SET_INPUT(joy, btn, bit)  event_get(event_joypad##joy##_##btn) ( btns_##joy&bit? INP_STATE_MAKE: INP_STATE_BREAK) ;


void osd_getinput(void) {

    uint8_t btns_1 = 0 ;
    uint8_t btns_2 = 0 ;

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
                btns_1 = indev_nav_take_value(spec) ;
                // indev_nav_read_i2c(spec, & btns_1) ;
            }
            else if(spec->conf.i2c.addr==52) {
                btns_2 = indev_nav_take_value(spec) ;
                // indev_nav_read_i2c(spec, & btns_2) ;
            }
        }

        // 192 == select + start
        if( (btns_1&192)==192 || (btns_2&192)==192 ) {
#ifndef SIMULATION
            printf("pressed select + start\n") ;
            esp_restart() ;
#else
            exit(99) ;
#endif
        }

#if AUDIO_RECORD
        // 按 select 停止画面，记录声音
        if(btns_1==0x80 && !record_audio) {
            record_on() ;
        }
        // 按 start 恢复
        else if(btns_1==0x40 && record_audio) {
            record_off() ;
        }
#endif

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
    xTaskCreatePinnedToCore(&task_video, "task_video", 1024*4, NULL, 5, NULL, 1);

    if(i2s>=0) {
#if AUDIO_RECORD
        record_audio_buff = malloc(RECORD_LEN) ;
	    xTaskCreatePinnedToCore(&task_audio, "task_audio", 1024*4, NULL, 5, NULL, 1);
#else
	    xTaskCreatePinnedToCore(&task_audio, "task_audio", 1024*2, NULL, 5, NULL, 1);
#endif
    }

    echo_DMA("end osd_init") ;
	return 0;
}

int player_nofrendo_main() {
    return nofrendo_main(0, NULL);
}