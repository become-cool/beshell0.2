// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// #include <freertos/FreeRTOS.h>
// #include <freertos/timers.h>
// #include <freertos/task.h>
// #include <freertos/queue.h>
// #include <time.h>
// //Nes stuff wants to define this as well...
// #undef false
// #undef true
// #undef bool


// #include <math.h>
// #include <string.h>
// // #include "noftypes.h"
// #include <bitmap.h>
// #include <nofconfig.h>
// #include <event.h>
// #include <gui.h>
// #include <log.h>
// #include <nes.h>
// #include <nes_pal.h>
// #include <nesinput.h>
// #include <osd.h>
// #include <stdint.h>
// #include "driver/i2s.h"
// #include "sdkconfig.h"
// // #include <spi_lcd.h>

// #include "esp_partition.h"

// // #include "st7789.h"
// // #include "input.h"
// // #include "config.h"

// #define  DEFAULT_SAMPLERATE   22100
// #define  DEFAULT_FRAGSIZE     400

// // // for 1.30'
// // #if SCREEN_SIZE==SCRENN_1I30
// // 	#define  VIDEO_WIDTH        256
// // 	#define  VIDEO_HEIGHT       NES_VISIBLE_HEIGHT

// // // for 3.2'
// // #elif SCREEN_SIZE==SCRENN_3I20
// 	#define  VIDEO_WIDTH        256
// 	#define  VIDEO_HEIGHT       240
// // #endif

// // #define AUDIO_PIN_DATA 2
// // #define AUDIO_PIN_BCLK 4
// // #define AUDIO_PIN_LRC 25


// // // 竖屏
// // #define CONFIG_DC_GPIO 12
// // #define CONFIG_MOSI_GPIO 13
// // #define CONFIG_RESET_GPIO 0
// // #define CONFIG_SCLK_GPIO 14
// // #define CONFIG_BL_GPIO 0
// // #define CONFIG_CS_GPIO 15


// // TFT_t lcd;

// // typedef struct BottonEvent {
// // 	uint8_t btn ;
// // 	uint8_t event ;
// // };
// // struct BottonEvent _btn_events[] = {
// // 	{ JOYPAD1_BTN_DOWN, event_joypad1_down } ,
// // 	{ JOYPAD1_BTN_UP, event_joypad1_up } ,
// // 	{ JOYPAD1_BTN_RIGHT, event_joypad1_right } ,
// // 	{ JOYPAD1_BTN_LEFT, event_joypad1_left } ,
// // 	{ JOYPAD1_BTN_A, event_joypad1_a } ,
// // 	{ JOYPAD1_BTN_B, event_joypad1_b } ,
// // 	{ JOYPAD1_BTN_START, event_joypad1_start } ,
// // 	{ JOYPAD1_BTN_SELECT, event_joypad1_select } ,
	
// // 	{ JOYPAD2_BTN_DOWN, event_joypad2_down } ,
// // 	{ JOYPAD2_BTN_UP, event_joypad2_up } ,
// // 	{ JOYPAD2_BTN_RIGHT, event_joypad2_right } ,
// // 	{ JOYPAD2_BTN_LEFT, event_joypad2_left } ,
// // 	{ JOYPAD2_BTN_A, event_joypad2_a } ,
// // 	{ JOYPAD2_BTN_B, event_joypad2_b } ,
// // 	{ JOYPAD2_BTN_START, event_joypad2_start } ,
// // 	{ JOYPAD2_BTN_SELECT, event_joypad2_select } ,
// // } ;
// // uint8_t _btn_cnt = sizeof(_btn_events)/sizeof(struct BottonEvent);


// QueueHandle_t vidQueue;
// // QueueHandle_t queue;

// TimerHandle_t timer;

// //Seemingly, this will be called only once. Should call func with a freq of frequency,
// int osd_installtimer(int frequency, void *func, int funcsize, void *counter, int countersize) {
//     printf("Timer install, freq=%d\n", frequency);
//     timer=xTimerCreate("nes",configTICK_RATE_HZ/frequency, pdTRUE, NULL, func);
//     xTimerStart(timer, 0);
//     return 0;
// }


// /**
//  * Audio
//  */
// static void (*audio_callback)(void *buffer, int length) = NULL;
// // #if AUDIO_ENA
// static uint16_t *audio_frame;
// // #endif
// void osd_setsound(void (*playfunc)(void *buffer, int length)) {
//    //Indicates we should call playfunc() to get more data.
//    audio_callback = playfunc;
// }

// static void osd_stopsound(void) {
// }


// static int osd_init_sound(void) {
// // #if AUDIO_ENA
// // 	printf("enable audio\n") ;

// // 	audio_frame=malloc(4*DEFAULT_FRAGSIZE);

// // #if AUDIO_DEV == AUDIO_DEV_BUILDIN_DAC

// // 	printf("init audio buildin dac\n") ;

// // 	i2s_config_t cfg={
// // 		.mode=I2S_MODE_DAC_BUILT_IN|I2S_MODE_TX|I2S_MODE_MASTER,
// // 		.sample_rate=DEFAULT_SAMPLERATE,
// // 		.bits_per_sample=I2S_BITS_PER_SAMPLE_16BIT,
// // 		.channel_format=I2S_CHANNEL_FMT_RIGHT_LEFT,
// // 		.communication_format=I2S_COMM_FORMAT_I2S_MSB,
// // 		.intr_alloc_flags=0,
// // 		.dma_buf_count=4,
// // 		.dma_buf_len=512
// // 	};
// // 	i2s_driver_install(0, &cfg, 0, NULL);
// // 	i2s_set_pin(0, NULL);
// // 	i2s_set_dac_mode(I2S_DAC_CHANNEL_LEFT_EN); 

// //     i2s_set_sample_rates(0, 22050); //set sample rates

// // 	//I2S enables *both* DAC channels; we only need DAC1.
// // 	//ToDo: still needed now I2S supports set_dac_mode?
// // 	CLEAR_PERI_REG_MASK(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC_XPD_FORCE_M);
// // 	CLEAR_PERI_REG_MASK(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_XPD_DAC_M);
	

// // #elif AUDIO_DEV == AUDIO_DEV_I2S

// // 	printf("init audio i2s dev\n") ;

// // 	i2s_config_t i2s_config = {
// // 		.mode = I2S_MODE_MASTER | I2S_MODE_TX,
// // 		.sample_rate=DEFAULT_SAMPLERATE,
// // 		.bits_per_sample=I2S_BITS_PER_SAMPLE_16BIT,
// // 		.channel_format=I2S_CHANNEL_FMT_RIGHT_LEFT,
// // 		.communication_format=I2S_COMM_FORMAT_I2S_MSB,
// // 		.intr_alloc_flags = 0, // default interrupt priority
// // 		.dma_buf_count=4,
// // 		.dma_buf_len=512,
// // 		.use_apll = false
// // 	};

// // 	i2s_pin_config_t pin_config = {
// // 		.bck_io_num = AUDIO_PIN_BCLK,
// // 		.ws_io_num = AUDIO_PIN_LRC,
// // 		.data_out_num = AUDIO_PIN_DATA,
// // 		.data_in_num = I2S_PIN_NO_CHANGE
// // 	};
// // 	printf("i2s data:%d, bclk:%d, lrc:%d\n",AUDIO_PIN_DATA,AUDIO_PIN_BCLK,AUDIO_PIN_LRC) ;

// //     i2s_driver_install(0, &i2s_config, 0, NULL);   //install and start i2s driver
// //     i2s_set_pin(0, &pin_config);
// //     i2s_set_sample_rates(0, 22050); //set sample rates

// // #endif

// // #else
// // 	printf("disable audio\n") ;
// // #endif

// 	audio_callback = NULL;

// 	return 0;
// }

// void osd_getsoundinfo(sndinfo_t *info) {
//    info->sample_rate = DEFAULT_SAMPLERATE;
//    info->bps = 16;
// }

// // /*
// // ** Video
// // */

// static int init(int width, int height);
// static void shutdown(void);
// static int set_mode(int width, int height);
// static void set_palette(rgb_t *pal);
// static void clear(uint8 color);
// static bitmap_t *lock_write(void);
// static void free_write(int num_dirties, rect_t *dirty_rects);
// static void custom_blit(bitmap_t *bmp, int num_dirties, rect_t *dirty_rects);
// static char fb[1]; //dummy


// viddriver_t sdlDriver = {
//    "MediaLayer",         /* name */
//    init,          /* init */
//    shutdown,      /* shutdown */
//    set_mode,      /* set_mode */
//    set_palette,   /* set_palette */
//    clear,         /* clear */
//    lock_write,    /* lock_write */
//    free_write,    /* free_write */
//    custom_blit,   /* custom_blit */
//    false          /* invalidate flag */
// };


// bitmap_t * myBitmap ;

// void osd_getvideoinfo(vidinfo_t *info) {
//    info->default_width = VIDEO_WIDTH;
//    info->default_height = VIDEO_HEIGHT;
//    info->driver = &sdlDriver;
// }

// /* flip between full screen and windowed */
// void osd_togglefullscreen(int code) {
// }

// /* initialise video */
// static int init(int width, int height) {
// 	return 0;
// }

// static void shutdown(void) {
// }

// /* set a video mode */
// static int set_mode(int width, int height) {
//    return 0;
// }

// // uint16 myPalette[256] ;
// rgb_t myPalette[256] ;

// /* copy nes palette over to hardware */
// static void set_palette(rgb_t *pal) {
//     uint16 c;
//     int i;
//     for (i = 0; i < 256; i++) {
//         myPalette[i].b = pal[i].b ;
//         myPalette[i].g = pal[i].g ;
//         myPalette[i].r = pal[i].r ;
//     }
// }

// /* clear all frames to a particular color */
// static void clear(uint8 color) {
// //   SDL_FillRect(mySurface, 0, color);
// }



// /* acquire the directbuffer for writing */
// static bitmap_t *lock_write(void) {
// //   SDL_LockSurface(mySurface);
//    myBitmap = bmp_createhw((uint8*)fb, VIDEO_WIDTH, VIDEO_HEIGHT, VIDEO_WIDTH*2);
//    return myBitmap;
// }

// /* release the resource */
// static void free_write(int num_dirties, rect_t *dirty_rects) {
//    bmp_destroy(&myBitmap);
// }


// #define __STDC_FORMAT_MACROS
// #include <inttypes.h>
// #include <sys/time.h>

// static uint32_t CLOCKS_PER_MSEC = CLOCKS_PER_SEC/1000 ;

// uint64_t nows(){
//     return (clock() / CLOCKS_PER_SEC);
// }
// uint64_t nowms(){
//     return (clock() * 1000 / CLOCKS_PER_SEC);
// }


// uint64_t last_time = 0 ;
// uint8_t dynamic_frames = 60 ;
// void framerate() {
// 	int64_t _now = nowms() ;
// 	if(last_time==0 || _now<last_time) {
// 		last_time = _now ;
// 		return ;
// 	}

// 	dynamic_frames = 1000 / (_now-last_time) ;
// 	// printf("%dHz\n",dynamic_frames) ;
// 	// printf("1000/(%"PRIu64"-%"PRIu64") = %d\n", _now, last_time, dynamic_frames) ;
// 	last_time = _now ;
// }

// static void custom_blit(bitmap_t *bmp, int num_dirties, rect_t *dirty_rects) {

// 	framerate() ;

// 	xQueueSend(vidQueue, &bmp, 0);

// // #if AUDIO_ENA

// // 	int left=DEFAULT_SAMPLERATE / ((dynamic_frames>10)? dynamic_frames: NES_REFRESH_RATE) ;
// // 	// printf("dfr=%d, left=%d\n",dynamic_frames,left) ;

// // #if AUDIO_DEV == AUDIO_DEV_BUILDIN_DAC

// // 	esp_err_t res = ESP_FAIL ;
// // 	while(left) {

// // 		int n=DEFAULT_FRAGSIZE;
// // 		if (n>left) n=left;
// // 		audio_callback(audio_frame, n); //get more data
// // 		//16 bit mono -> 32-bit (16 bit r+l)
// // 		for (int i=n-1; i>=0; i--) {
// // 			audio_frame[i*2+1]=audio_frame[i];
// // 			audio_frame[i*2]=audio_frame[i];
// // 		}
// // 		// printf("%x,%x,%x,%x,%x,%x,%x,%x...\n", audio_frame[0], audio_frame[1], audio_frame[2], audio_frame[3], audio_frame[4], audio_frame[5], audio_frame[6], audio_frame[7]) ;

// // 		// i2s_write_bytes(0, audio_frame, 4*n, portMAX_DELAY);
// // 		size_t bytes_written = 0 ;
// // 		res = i2s_write(0, audio_frame, 4*n, &bytes_written, 100);
// // 		// printf("i2s written: %d, %d\n", bytes_written, res) ;
// // 		left-=n;
// // 	}

// // 	printf("audio frame end\n") ;

// // #elif AUDIO_DEV == AUDIO_DEV_I2S

// // 	while(left) {
// // 		int n=DEFAULT_FRAGSIZE;
// // 		if (n>left)
// // 			n=left;
// // 		audio_callback(audio_frame, n); //get more data

// // 		// printf("audio %d/%d\n", n, left) ;

// // 		//16 bit mono -> 32-bit (16 bit r+l)
// // 		for (int i=n-1; i>=0; i--) {
// // 			audio_frame[i*2+1]=audio_frame[i];
// // 			audio_frame[i*2]=audio_frame[i];
// // 		}
// // 		// i2s_write_bytes(0, audio_frame, 4*n, portMAX_DELAY);
// // 		size_t bytes_written ;
// // 		i2s_write(0, audio_frame, 4*n, &bytes_written, 100);

// // 		left-=n;
// // 	}


// // #endif
// // #endif
// }

// // uint16_t line[240*8] ;
// // void st7789_write_frame(const uint8_t ** data) {

// //     int x, y;
// // 	// int height = 240 ;
// // 	// int width = 240 ;
// // 	int i = 0 ;
// // 	int idx ;

// // 	for(int b=0; b<30; b++) {
// // 		i = 0 ;
// // 		for(int by=0;by<8;by++) {
// // 			y = b*8 + by ;
			
// // 			// nofrendo 的实际宽度为 256, 屏幕宽 240，所以从8开始
// // 			for(x=8;x<248;x++) {
// // 				idx = data[y][x] ;
// // 				line[i++] = rgb565_conv_inv(myPalette[idx].r, myPalette[idx].g, myPalette[idx].b) ;
// // 			}
// // 		}
		
// // 		lcdDrawRectPixels(&lcd, 0, y, 239, y+7, line) ;
// // 	}
// // }

// char *osd_getromdata() {

// 	char* romdata = NULL ;
// 	// const esp_partition_t* part;
// 	// spi_flash_mmap_handle_t hrom;
// 	// esp_err_t err;
// 	// part=esp_partition_find_first(0x40, 0x1, "loadrom");
// 	// if (part==0) printf("Couldn't find rom part!\n");
// 	// err=esp_partition_mmap(part, 0, 2*1024*1024, SPI_FLASH_MMAP_DATA, (const void**)&romdata, &hrom);
// 	// if (err!=ESP_OK) printf("Couldn't map rom part!(error:%d)\n", err);
// 	// printf("Initialized. ROM@%p\n", romdata);
//     // printf("free : %dKB\n", esp_get_free_heap_size()/1024) ;

//     return romdata;

// }

// //This runs on core 1.
// static void videoTask(void *arg) {

// 	bitmap_t *bmp=NULL;
// 	// x = (240-VIDEO_WIDTH)/2;
//     // y = ((240-VIDEO_HEIGHT)/2);
//     while(1) {
// 		// xQueueReceive(vidQueue, &bmp, portMAX_DELAY);//skip one frame to drop to 30
// 		xQueueReceive(vidQueue, &bmp, portMAX_DELAY);

// 		// printf("F:%dx%d\n",bmp->width, bmp->height) ;

// 		// ili9341_write_frame(x, y, VIDEO_WIDTH, VIDEO_HEIGHT, (const uint8_t **)bmp->line);
// 		// st7789_write_frame(bmp->line);
// 	}
// }


// void osd_getinput(void) {
// 	// struct InputGPIO * pins = loop_input() ;
	
// 	// printf("_btn_cnt: %d, start:%d, up: %d,  down: %d\n"
// 	// 	, _btn_cnt
// 	// 	, pins[JOYPAD1_BTN_START].pressdown
// 	// 	, pins[JOYPAD1_BTN_UP].pressdown
// 	// 	, pins[JOYPAD1_BTN_DOWN].pressdown
// 	// ) ;
	
// 	// for(int i=0; i< _btn_cnt; i++ ) {
// 	// 	event_get(_btn_events[i].event)( pins[_btn_events[i].btn].pressdown? INP_STATE_MAKE: INP_STATE_BREAK) ;
// 	// }
// }

// void osd_getmouse(int *x, int *y, int *button) {}

// // /*
// // ** Shutdown
// // */

// /* this is at the bottom, to eliminate warnings */
// void osd_shutdown() {
//    audio_callback = NULL;
// }

// static int logprint(const char *string) {
//    return printf("%s", string);
// }

// /*
// ** Startup
// */
// int osd_init() {

// 	log_chain_logfunc(logprint);

// 	if (osd_init_sound())
// 		return -1;
    
// 	vidQueue=xQueueCreate(1, sizeof(bitmap_t *));
// 	xTaskCreatePinnedToCore(&videoTask, "videoTask", 2048, NULL, 5, NULL, 1);

// 	return 0;
// }





