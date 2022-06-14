#include "module_media.h"
#include "widgets_gen.h"
#include "utils.h"
#include "driver/i2s.h"
#include "pwm_audio.h"
#include "dac_audio.h"

#include "lv_demo_music_main.h"
#include "lv_demo_music_list.h"

#include "esp_log.h"
#include <stdio.h>
#include <string.h>
#include "psram.h"
#include <sys/stat.h>

static const char *TAG = "media";

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

static esp_err_t play_wav(const char *filepath)
{
    FILE *fd = NULL;
    struct stat file_stat;

    if (stat(filepath, &file_stat) == -1) {
        printf("Failed to stat file : %s\n", filepath);
        return ESP_FAIL;
    }

    printf("file stat info: %s (%ld bytes)...\n", filepath, file_stat.st_size);
    fd = fopen(filepath, "r");

    if (NULL == fd) {
        printf("Failed to read existing file : %s\n", filepath);
        return ESP_FAIL;
    }
    const size_t chunk_size = 1024;
    uint8_t *buffer = malloc(chunk_size);

    if (NULL == buffer) {
        printf("audio data buffer malloc failed\n");
        fclose(fd);
        return ESP_FAIL;
    }

    /**
     * read head of WAV file
     */
    wav_header_t wav_head;
    int len = fread(&wav_head, 1, sizeof(wav_header_t), fd);
    if (len <= 0) {
        printf("Read wav header failed\n");
        fclose(fd);
        return ESP_FAIL;
    }
    if (NULL == strstr((char *)wav_head.Subchunk1ID, "fmt") &&
            NULL == strstr((char *)wav_head.Subchunk2ID, "data")) {
        printf("Header of wav format error\n");
        fclose(fd);
        return ESP_FAIL;
    }

    printf("frame_rate=%d, ch=%d, width=%d\n", wav_head.SampleRate, wav_head.NumChannels, wav_head.BitsPerSample);

    pwm_audio_set_param(wav_head.SampleRate, wav_head.BitsPerSample, wav_head.NumChannels);
    pwm_audio_start();

	
	// dac_audio_set_param(wav_head.SampleRate, wav_head.BitsPerSample, wav_head.NumChannels);
    // dac_audio_start();

    // i2s_start(I2S_NUM_0);

    /**
     * read wave data of WAV file
     */
    size_t write_num = 0;
    size_t cnt;

    do {
        /* Read file in chunks into the scratch buffer */
        len = fread(buffer, 1, chunk_size, fd);
        if (len <= 0) {
            break;
        }
        pwm_audio_write(buffer, len, &cnt, 1000 / portTICK_PERIOD_MS);
        // dac_audio_write(buffer, len, &cnt, 1000 / portTICK_PERIOD_MS);
        // i2s_write(I2S_NUM_0, buffer, len, &cnt, 1000 / portTICK_PERIOD_MS);

        write_num += len;
    } while (1);

    fclose(fd);
    printf("File reading complete, total: %d bytes\n", write_num);
    // i2s_driver_uninstall(I2S_NUM_0);
    return ESP_OK;
}

static JSValue js_i2s_test(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    esp_err_t ret;

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

	//  dac_audio_config_t dac_cfg;
    // dac_cfg.i2s_num = I2S_NUM_0;
    // dac_cfg.sample_rate = 22050;
    // dac_cfg.bits_per_sample = 16;
    // dac_cfg.dac_mode = I2S_DAC_CHANNEL_LEFT_EN;
    // dac_cfg.dma_buf_count = 8;
    // dac_cfg.dma_buf_len = 1024;
    // dac_cfg.max_data_size = 16 * 1024;
    // if(ESP_OK != dac_audio_init(&dac_cfg)) {
	// 	printf("dac_audio_init() failed.\n") ;
	// 	return ;
	// }

    // echo_DMA("before i2s_driver_install()") ;
	// i2s_config_t cfg = {
    //     .mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
    //     .sample_rate = 16000,
    //     .bits_per_sample = 16, /* must be 8 for built-in DAC */
    //     .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    //     .communication_format = I2S_COMM_FORMAT_STAND_PCM_SHORT,
    //     // .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // high interrupt priority
    //     .intr_alloc_flags = 0, // high interrupt priority
    //     .dma_buf_count = 2,
    //     .dma_buf_len = 1024
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

	// //I2S enables *both* DAC channels; we only need DAC1.
	// //ToDo: still needed now I2S supports set_dac_mode?
	// CLEAR_PERI_REG_MASK(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC_XPD_FORCE_M);
	// CLEAR_PERI_REG_MASK(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_XPD_DAC_M);


	play_wav("/fs/home/become/music.wav");

	return JS_UNDEFINED ;
}


static JSValue js_i2s_setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)

	// i2s_config_t cfg={
	// 	.mode=I2S_MODE_DAC_BUILT_IN|I2S_MODE_TX|I2S_MODE_MASTER,
	// 	.sample_rate=DEFAULT_SAMPLERATE,
	// 	.bits_per_sample=I2S_BITS_PER_SAMPLE_8BIT,
	// 	.channel_format=I2S_CHANNEL_FMT_RIGHT_LEFT,
	// 	.communication_format=I2S_COMM_FORMAT_I2S_MSB,
	// 	.intr_alloc_flags=0,
	// 	.dma_buf_count=4,
	// 	.dma_buf_len=1024
	// };
	// i2s_driver_install(0, &cfg, 0, NULL);
	// i2s_set_pin(0, NULL);
	// i2s_set_dac_mode(I2S_DAC_CHANNEL_LEFT_EN); 

    // i2s_set_sample_rates(0, DEFAULT_SAMPLERATE); //set sample rates

	// //I2S enables *both* DAC channels; we only need DAC1.
	// //ToDo: still needed now I2S supports set_dac_mode?
	// CLEAR_PERI_REG_MASK(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC_XPD_FORCE_M);
	// CLEAR_PERI_REG_MASK(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_XPD_DAC_M);

    return JS_UNDEFINED ;
}


static lv_obj_t * ctrl;
static lv_obj_t * list;

static JSValue js_music_player_create(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)

    // CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lv.Obj object")
    // lv_obj_t * cparent = JS_GetOpaqueInternal(argv[0]) ;

    // lv_obj_set_style_bg_color(cparent, lv_color_hex(0x343247), 0);

    // list = _lv_demo_music_list_create(cparent);
    // ctrl = _lv_demo_music_main_create(cparent);

    return JS_UNDEFINED ;
}


void be_module_media_require(JSContext *ctx) {

    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue media = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "media", media);

    JS_SetPropertyStr(ctx, media, "test", JS_NewCFunction(ctx, js_i2s_test, "test", 1));
    JS_SetPropertyStr(ctx, media, "setup", JS_NewCFunction(ctx, js_i2s_setup, "setup", 1));
    JS_SetPropertyStr(ctx, media, "createMusicPlayer", JS_NewCFunction(ctx, js_music_player_create, "createMusicPlayer", 1));

    JS_FreeValue(ctx, beapi);
}
