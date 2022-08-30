#include "module_media.h"
#include "module_serial.h"
#include "module_fs.h"
#include "widgets_gen.h"
#include "utils.h"
#include "driver/i2s.h"
#include "pwm_audio.h"
#include "dac_audio.h"

#include "lv_demo_music_main.h"
#include "lv_demo_music_list.h"


#include "audio_element.h"
#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "audio_mem.h"
#include "audio_common.h"
#include "fatfs_stream.h"
// #include "spiffs_stream.h"
#include "vfs_stream.h"
#include "i2s_stream.h"
#include "esp_peripherals.h"

// decoder 
#include "mp3_decoder.h"
#include "amr_decoder.h"
#include "opus_decoder.h"
#include "ogg_decoder.h"
#include "flac_decoder.h"
#include "wav_decoder.h"
#include "aac_decoder.h"

#include "mp3dec.h"




#include "esp_log.h"
#include <stdio.h>
#include <string.h>
#include "psram.h"
#include <sys/stat.h>

static const char *TAG = "media";

// typedef struct {
//     // The "RIFF" chunk descriptor
//     uint8_t ChunkID[4];
//     int32_t ChunkSize;
//     uint8_t Format[4];
//     // The "fmt" sub-chunk
//     uint8_t Subchunk1ID[4];
//     int32_t Subchunk1Size;
//     int16_t AudioFormat;
//     int16_t NumChannels;
//     int32_t SampleRate;
//     int32_t ByteRate;
//     int16_t BlockAlign;
//     int16_t BitsPerSample;
//     // The "data" sub-chunk
//     uint8_t Subchunk2ID[4];
//     int32_t Subchunk2Size;
// } wav_header_t;

// static esp_err_t play_wav(const char *filepath)
// {
//     FILE *fd = NULL;
//     struct stat file_stat;

//     if (stat(filepath, &file_stat) == -1) {
//         printf("Failed to stat file : %s\n", filepath);
//         return ESP_FAIL;
//     }

//     printf("file stat info: %s (%ld bytes)...\n", filepath, file_stat.st_size);
//     fd = fopen(filepath, "r");

//     if (NULL == fd) {
//         printf("Failed to read existing file : %s\n", filepath);
//         return ESP_FAIL;
//     }
//     const size_t chunk_size = 1024;
//     uint8_t *buffer = malloc(chunk_size);

//     if (NULL == buffer) {
//         printf("audio data buffer malloc failed\n");
//         fclose(fd);
//         return ESP_FAIL;
//     }

//     /**
//      * read head of WAV file
//      */
//     wav_header_t wav_head;
//     int len = fread(&wav_head, 1, sizeof(wav_header_t), fd);
//     if (len <= 0) {
//         printf("Read wav header failed\n");
//         fclose(fd);
//         return ESP_FAIL;
//     }
//     if (NULL == strstr((char *)wav_head.Subchunk1ID, "fmt") &&
//             NULL == strstr((char *)wav_head.Subchunk2ID, "data")) {
//         printf("Header of wav format error\n");
//         fclose(fd);
//         return ESP_FAIL;
//     }

//     printf("frame_rate=%d, ch=%d, width=%d\n", wav_head.SampleRate, wav_head.NumChannels, wav_head.BitsPerSample);

//     pwm_audio_set_param(wav_head.SampleRate, wav_head.BitsPerSample, wav_head.NumChannels);
//     pwm_audio_start();

	
// 	// dac_audio_set_param(wav_head.SampleRate, wav_head.BitsPerSample, wav_head.NumChannels);
//     // dac_audio_start();

//     // i2s_start(i2s);

//     /**
//      * read wave data of WAV file
//      */
//     size_t write_num = 0;
//     size_t cnt;

//     do {
//         /* Read file in chunks into the scratch buffer */
//         len = fread(buffer, 1, chunk_size, fd);
//         if (len <= 0) {
//             break;
//         }
//         pwm_audio_write(buffer, len, &cnt, 1000 / portTICK_PERIOD_MS);
//         // dac_audio_write(buffer, len, &cnt, 1000 / portTICK_PERIOD_MS);
//         // i2s_write(i2s, buffer, len, &cnt, 1000 / portTICK_PERIOD_MS);

//         write_num += len;
//     } while (1);

//     fclose(fd);
//     printf("File reading complete, total: %d bytes\n", write_num);
//     // i2s_driver_uninstall(i2s);
//     return ESP_OK;
// }

// static JSValue js_i2s_test(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

//     esp_err_t ret;

//     pwm_audio_config_t pac;
//     pac.duty_resolution    = LEDC_TIMER_10_BIT;
//     pac.gpio_num_left      = 26;
//     pac.ledc_channel_left  = LEDC_CHANNEL_0;
//     pac.gpio_num_right     = 25;
//     pac.ledc_channel_right = LEDC_CHANNEL_1;
//     pac.ledc_timer_sel     = LEDC_TIMER_0;
//     pac.tg_num             = TIMER_GROUP_0;
//     pac.timer_num          = TIMER_0;
//     pac.ringbuf_len        = 1024 * 8;
//     pwm_audio_init(&pac);

// 	//  dac_audio_config_t dac_cfg;
//     // dac_cfg.i2s_num = i2s;
//     // dac_cfg.sample_rate = 22050;
//     // dac_cfg.bits_per_sample = 16;
//     // dac_cfg.dac_mode = I2S_DAC_CHANNEL_LEFT_EN;
//     // dac_cfg.dma_buf_count = 8;
//     // dac_cfg.dma_buf_len = 1024;
//     // dac_cfg.max_data_size = 16 * 1024;
//     // if(ESP_OK != dac_audio_init(&dac_cfg)) {
// 	// 	printf("dac_audio_init() failed.\n") ;
// 	// 	return ;
// 	// }

//     // echo_DMA("before i2s_driver_install()") ;
// 	// i2s_config_t cfg = {
//     //     .mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN,
//     //     .sample_rate = 16000,
//     //     .bits_per_sample = 16, /* must be 8 for built-in DAC */
//     //     .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
//     //     .communication_format = I2S_COMM_FORMAT_STAND_PCM_SHORT,
//     //     // .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // high interrupt priority
//     //     .intr_alloc_flags = 0, // high interrupt priority
//     //     .dma_buf_count = 2,
//     //     .dma_buf_len = 1024
//     // } ;
//     // esp_err_t res = i2s_driver_install(i2s, &cfg, 0, NULL) ;
// 	// if(res!=ESP_OK) {
//     //     printf("i2s_driver_install() failed: %d\n",res) ;
//     //     return ;
//     // }
//     // echo_DMA("after i2s_driver_install()") ;
//     // // if(i2s_set_sample_rates(i2s, 22050)!=ESP_OK) {
//     // //     printf("i2s_set_sample_rates() failed\n") ;
//     // //     return ;
//     // // }
// 	// if(i2s_set_pin(0, NULL)!=ESP_OK) {
//     //     printf("i2s_set_pin() failed\n") ;
//     //     return ;
//     // }
// 	// if(i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN)!=ESP_OK) {
//     //     printf("i2s_set_dac_mode() failed\n") ;
//     //     return ;
//     // }

//     // if(i2s_start(i2s)!=ESP_OK) {
//     //     printf("i2s_start() failed\n") ;
//     //     return ;
//     // }

// 	// //I2S enables *both* DAC channels; we only need DAC1.
// 	// //ToDo: still needed now I2S supports set_dac_mode?
// 	// CLEAR_PERI_REG_MASK(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC_XPD_FORCE_M);
// 	// CLEAR_PERI_REG_MASK(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_XPD_DAC_M);


// 	play_wav("/fs/home/become/music.wav");

// 	return JS_UNDEFINED ;
// }


/***************参数定义***************/
#define STATEconfirm		0x0C			//状态机确认 回读STATEconfirm的寄存值确认IC正常工作状态
#define NORMAL_I2S			0x00
#define NORMAL_LJ			0x01
#define NORMAL_DSPA			0x03
#define NORMAL_DSPB			0x07
#define Format_Len24		0x00
#define Format_Len20		0x01
#define Format_Len18		0x02
#define Format_Len16		0x03
#define Format_Len32		0x04

#define VDDA_3V3			0x00
#define VDDA_1V8			0x01
#define MCLK_PIN			0x00
#define SCLK_PIN			0x01
/***************参数定义***************/

/**************************************************/
#define MSMode_MasterSelOn	0				//产品主从模式选择:默认选择0为SlaveMode,打开为1选择MasterMode
#define Ratio 				64				//实际Ratio=MCLK/LRCK比率,需要和实际时钟比例匹配
#define Format 				NORMAL_I2S		//数据格式选择,需要和实际时序匹配
#define Format_Len			Format_Len16	//数据长度选择,需要和实际时序匹配
#define SCLK_DIV			4				//SCLK分频选择:(选择范围1~128),SCLK=MCLK/SCLK_DIV
#define SCLK_INV			0				//默认对齐方式为下降沿,1为上升沿对齐,需要和实际时序匹配
#define MCLK_SOURCE			MCLK_PIN		//是否硬件没接MCLK需要用SCLK当作MCLK
#define EQ7bandOn			0				//开启七段EQ下配置1，三段或者不使用配置0
#define VDDA_VOLTAGE		VDDA_3V3		//模拟电压选择为3V3还是1V8,需要和实际硬件匹配
#define DAC_Volume			191				//DAC数字增益:(选择范围0~255),191:0DB,±0.5dB/Step
#define DACHPModeOn			0				//输出负载开启HP驱动:默认选择关闭0,打开为1

/**************************************************/

#define I2CWRNBYTE_CODEC(r,b) i2c_write_byte(0,8,r,b)


static JSValue js_i2s_setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)

    int i2s = 0 ;
    int bit = I2S_BITS_PER_SAMPLE_32BIT ;
    int sample_rate = 44100 ;

i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX),
    .sample_rate = sample_rate,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL2 | ESP_INTR_FLAG_IRAM,
    .dma_buf_count = 2,
    .dma_buf_len = 1024,
    .use_apll = true,
    .tx_desc_auto_clear = true,
    .fixed_mclk = 0
};

i2s_pin_config_t pin_config = {
    .ws_io_num = 33,
    .data_out_num = 4,
    .bck_io_num = 26,
    .data_in_num = -1                                                       //Not used
};
i2s_driver_install(i2s, &i2s_config, 0, NULL);
i2s_set_pin(i2s, &pin_config);
    //clk out
    // PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
    //REG_SET_FIELD(PIN_CTRL, CLK_OUT1, 0)
    // REG_WRITE(PIN_CTRL, 0xFFFFFFF0);
    //i2s_set_clk(i2s_num, rate, bits, ch);



    
    
	I2CWRNBYTE_CODEC(0x02,(MCLK_SOURCE<<7) + (SCLK_INV<<4) +  (EQ7bandOn<<3) + 0x04 + MSMode_MasterSelOn);	
	
	if(DACHPModeOn == 0)//接PA或者LOUT,内部不开启负载驱动,省功耗
    {
        I2CWRNBYTE_CODEC(0x20,0x2A);	
        I2CWRNBYTE_CODEC(0x21,0x3C);	
        I2CWRNBYTE_CODEC(0x22,0x02);	
        I2CWRNBYTE_CODEC(0x24,0x07);	
        I2CWRNBYTE_CODEC(0x23,0x40 + (0x30*VDDA_VOLTAGE));	
    }
	if(DACHPModeOn == 1)//接耳机,内部开启负载驱动,支持耳机输出
    {
        I2CWRNBYTE_CODEC(0x20,0x16);	
        I2CWRNBYTE_CODEC(0x21,0x3F);	
        I2CWRNBYTE_CODEC(0x22,0x0A);	
        I2CWRNBYTE_CODEC(0x24,0x01);	
        I2CWRNBYTE_CODEC(0x23,0xCA + (0x30*VDDA_VOLTAGE));	
    }
	I2CWRNBYTE_CODEC(0x0A,0x01);	
	I2CWRNBYTE_CODEC(0x0B,0x01);	
	I2CWRNBYTE_CODEC(0x11,NORMAL_I2S + (Format_Len<<4));
	I2CWRNBYTE_CODEC(0x14,DAC_Volume);
	if(Ratio == 128)//Ratio=MCLK/LRCK=128：6M144/48K；
    {
        I2CWRNBYTE_CODEC(0x01,0x61 + (0x40*EQ7bandOn));//128 Ratio(MCLK/LRCK)  256
        I2CWRNBYTE_CODEC(0x09,0x00);//128 Ratio(MCLK/LRCK)					
        I2CWRNBYTE_CODEC(0x03,0x00);//LRCK H		
        I2CWRNBYTE_CODEC(0x04,0x80);//LRCK=MCLK/128
        I2CWRNBYTE_CODEC(0x05,SCLK_DIV);//BCLK=MCLK/4		
    }
	else if(Ratio == 64)//Ratio=MCLK/LRCK=64：3M072/48K；   512
    {
        I2CWRNBYTE_CODEC(0x01,0xA1);//64 Ratio(MCLK/LRCK)	
        I2CWRNBYTE_CODEC(0x09,0x02);//64 Ratio(MCLK/LRCK)	
        I2CWRNBYTE_CODEC(0x03,0x00);//LRCK H
        I2CWRNBYTE_CODEC(0x04,0x40);//LRCK=MCLK/64
        I2CWRNBYTE_CODEC(0x05,SCLK_DIV);//BCLK=MCLK/2
    }

	I2CWRNBYTE_CODEC(0x0D,0x14);
	I2CWRNBYTE_CODEC(0x18,0x00);
	I2CWRNBYTE_CODEC(0x08,0x3F);
	I2CWRNBYTE_CODEC(0x00,0x02);
	I2CWRNBYTE_CODEC(0x00,0x03);
	I2CWRNBYTE_CODEC(0x25,0x20);



    return JS_UNDEFINED ;
}


// static lv_obj_t * ctrl;
// static lv_obj_t * list;


// static JSValue js_music_player_create(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
//     CHECK_ARGC(1)

//     // audio_pipeline_handle_t pipeline;
//     // audio_element_handle_t i2s_stream_writer, mp3_decoder;

//     // audio_pipeline_cfg_t pipeline_cfg = DEFAULT_AUDIO_PIPELINE_CONFIG();
//     // pipeline = audio_pipeline_init(&pipeline_cfg);

//     // mp3_decoder_cfg_t mp3_cfg = DEFAULT_MP3_DECODER_CONFIG();
//     // mp3_decoder = mp3_decoder_init(&mp3_cfg);
//     // // audio_element_set_read_cb(mp3_decoder, mp3_music_read_cb, NULL);

//     // i2s_stream_cfg_t i2s_cfg = I2S_STREAM_CFG_DEFAULT();
//     // i2s_cfg.type = AUDIO_STREAM_WRITER;
//     // i2s_stream_writer = i2s_stream_init(&i2s_cfg);

//     // audio_pipeline_register(pipeline, mp3_decoder, "mp3");
//     // audio_pipeline_register(pipeline, i2s_stream_writer, "i2s");

//     // const char *link_tag[2] = {"mp3", "i2s"};
//     // audio_pipeline_link(pipeline, &link_tag[0], 2);





    
//     // audio_pipeline_run(pipeline);

//     // CHECK_INSOF_LVOBJ("Obj", argv[0], "arg parent must a lv.Obj object")
//     // lv_obj_t * cparent = JS_GetOpaqueInternal(argv[0]) ;

//     // lv_obj_set_style_bg_color(cparent, lv_color_hex(0x343247), 0);

//     // list = _lv_demo_music_list_create(cparent);
//     // ctrl = _lv_demo_music_main_create(cparent);

//     return JS_UNDEFINED ;
// }



static void play_mp3(char *path)
{
    uint8_t i2s = I2S_NUM_0 ;

    printf("start to decode ...\n");
    HMP3Decoder hMP3Decoder;
    MP3FrameInfo mp3FrameInfo;
    
    uint8_t readBuf[MAINBUF_SIZE];
    if(readBuf==NULL){
        printf("readBuf malloc failed\n");
        return ;
    }
    short *output=malloc(1153*4);
    if(output==NULL){
        // free(readBuf);
        printf("outBuf malloc failed\n");
        return ;
    }


    hMP3Decoder = MP3InitDecoder();
    if (hMP3Decoder == 0){
        // free(readBuf);
        free(output);
        printf("memory is not enough..\n");
        return ;
    }
    

    ds(path)
    FILE *mp3File=fopen( path,"rb");
    free(path) ;
    if(mp3File==NULL){
        MP3FreeDecoder(hMP3Decoder);
        // free(readBuf);
        free(output);
        printf("open file failed\n");
        return ;
    }

    int samplerate=0;
    i2s_zero_dma_buffer(0);
    // DS(path)
    
    char tag[10];
    int tag_len = 0;
    int read_bytes = fread(tag, 1, 10, mp3File);
    // DN(read_bytes)
    if(read_bytes == 10) 
    {
        if (memcmp(tag,"ID3",3) == 0) 
        {
            tag_len = ((tag[6] & 0x7F)<< 21)|((tag[7] & 0x7F) << 14) | ((tag[8] & 0x7F) << 7) | (tag[9] & 0x7F);
                // printf("tag_len: %d %x %x %x %x", tag_len,tag[6],tag[7],tag[8],tag[9]);
            fseek(mp3File, tag_len - 10, SEEK_SET);
        }
        else 
        {
            fseek(mp3File, 0, SEEK_SET);
        }
    }
    unsigned char* input = &readBuf[0];
    int bytesLeft = 0;
    int outOfData = 0;
    unsigned char* readPtr = readBuf;
    uint8_t emptys [4096] = {0} ;
    bool printed = false ;

    uint64_t t0 = gettime() ;
    uint64_t t1 = gettime() ;
    uint64_t t2 = 0 ;


    while (1)
    {
        if (bytesLeft < MAINBUF_SIZE)
        {
            memmove(readBuf, readPtr, bytesLeft);
            int br = fread(readBuf + bytesLeft, 1, MAINBUF_SIZE - bytesLeft, mp3File);
            if ((br == 0)&&(bytesLeft==0)) break;

            bytesLeft = bytesLeft + br;
            readPtr = readBuf;
        }
        int offset = MP3FindSyncWord(readPtr, bytesLeft);
        if (offset < 0)
        {  
            printf("MP3FindSyncWord not find\n");
            bytesLeft=0;
            continue;
        }
        else
        {
            readPtr += offset;                         //data start point
            bytesLeft -= offset;                 //in buffer
            int errs = MP3Decode(hMP3Decoder, &readPtr, &bytesLeft, output, 0);
            if (errs != 0)
            {
                printf("MP3Decode failed ,code is %d \n",errs);
                break;
            }
            MP3GetLastFrameInfo(hMP3Decoder, &mp3FrameInfo);
            if(samplerate!=mp3FrameInfo.samprate)
            {
                samplerate=mp3FrameInfo.samprate;
                // hal_i2s_init(0,samplerate,16,mp3FrameInfo.nChans);
                i2s_set_clk(0,samplerate*(Ratio/64),32,mp3FrameInfo.nChans);
                //wm8978_samplerate_set(samplerate);
                printf("mp3file info---bitrate=%d,layer=%d,bitsPerSample=%d,nChans=%d,samprate=%d,outputSamps=%d\n"
                    ,mp3FrameInfo.bitrate,mp3FrameInfo.layer,mp3FrameInfo.bitsPerSample,
                    mp3FrameInfo.nChans,mp3FrameInfo.samprate,mp3FrameInfo.outputSamps);
            }

            
            // t2 = gettime() ;
            // printf("decode: %lld\n", t2-t1) ;

            size_t cnt = 0 ;
            // ES8156 如果省略 MCLK ，由 SCLK 提供主时钟，
            // 则 SCLK/LRSLK >=64 , 因此 sample 最小 32bit，而 esp-idf 最大支持 32bit
            // 因此统一 32bit/sample
            if(mp3FrameInfo.bitsPerSample!=32) {
                // 扩展到 32 sample
                i2s_write_expand(i2s, output,
                    mp3FrameInfo.outputSamps * mp3FrameInfo.nChans,
                    mp3FrameInfo.bitsPerSample,
                    32, &cnt, portMAX_DELAY );
            }
            else {
                i2s_write(i2s, output, mp3FrameInfo.outputSamps*2, &cnt, 1000 / portTICK_PERIOD_MS);
            }

            // t1 = gettime() ;
            // printf("output: %lld\n", t1-t2) ;
            
        }

    }

    printf("total: %lld\n", gettime()-t0) ;


    i2s_zero_dma_buffer(0);
    //i2s_driver_uninstall(0);
    MP3FreeDecoder(hMP3Decoder);
    // free(readBuf);
    free(output);  
    fclose(mp3File);

    printf("end mp3 decode ..\n");

    return ;
}






void play_by_adf(char * path)
{
    // Example of linking elements into an audio pipeline -- START
    audio_pipeline_handle_t pipeline;
    audio_element_handle_t vfs_stream_reader, i2s_stream_writer, music_decoder;

    esp_log_level_set("*", ESP_LOG_WARN);
    esp_log_level_set(TAG, ESP_LOG_INFO);


    ESP_LOGI(TAG, "[3.0] Create audio pipeline for playback");
    audio_pipeline_cfg_t pipeline_cfg = DEFAULT_AUDIO_PIPELINE_CONFIG();
    pipeline = audio_pipeline_init(&pipeline_cfg);
    mem_assert(pipeline);

    ESP_LOGI(TAG, "[3.1] Create vfs stream to read data from sdcard");
    vfs_stream_cfg_t vfs_cfg = VFS_STREAM_CFG_DEFAULT();
    vfs_cfg.type = AUDIO_STREAM_READER;
    vfs_stream_reader = vfs_stream_init(&vfs_cfg);

    ESP_LOGI(TAG, "[3.2] Create i2s stream to write data to codec chip");
    i2s_stream_cfg_t i2s_cfg = I2S_STREAM_CFG_DEFAULT();
    i2s_cfg.type = AUDIO_STREAM_WRITER;
    i2s_stream_writer = i2s_stream_init(&i2s_cfg);


    ESP_LOGI(TAG, "[3.3] Create mp3 decoder");
    mp3_decoder_cfg_t mp3_cfg = DEFAULT_MP3_DECODER_CONFIG();
    music_decoder = mp3_decoder_init(&mp3_cfg);


    ESP_LOGI(TAG, "[3.4] Register all elements to audio pipeline");
    audio_pipeline_register(pipeline, vfs_stream_reader, "file");
    audio_pipeline_register(pipeline, music_decoder, "dec");
    audio_pipeline_register(pipeline, i2s_stream_writer, "i2s");

    ESP_LOGI(TAG, "[3.5] Link it together [sdcard]-->vfs_stream-->music_decoder-->i2s_stream-->[codec_chip]");
    const char *link_tag[3] = {"file", "dec", "i2s"};
    audio_pipeline_link(pipeline, &link_tag[0], 3);


    ESP_LOGI(TAG, "[3.6] Set up uri");
    audio_element_set_uri(vfs_stream_reader, path);
dd
    ESP_LOGI(TAG, "[ 4 ] Set up  event listener");
    audio_event_iface_cfg_t evt_cfg = AUDIO_EVENT_IFACE_DEFAULT_CFG();
    audio_event_iface_handle_t evt = audio_event_iface_init(&evt_cfg);

    ESP_LOGI(TAG, "[4.1] Listening event from all elements of pipeline");
    audio_pipeline_set_listener(pipeline, evt);

    ESP_LOGI(TAG, "[4.2] Listening event from peripherals");
    esp_periph_config_t periph_cfg = DEFAULT_ESP_PERIPH_SET_CONFIG();
    esp_periph_set_handle_t set = esp_periph_set_init(&periph_cfg);
    audio_event_iface_set_listener(esp_periph_set_get_event_iface(set), evt);

    ESP_LOGI(TAG, "[ 5 ] Start audio_pipeline");
    audio_pipeline_run(pipeline);
    // Example of linking elements into an audio pipeline -- END

    ESP_LOGI(TAG, "[ 6 ] Listen for all pipeline events");
    while (1) {
        audio_event_iface_msg_t msg;
        esp_err_t ret = audio_event_iface_listen(evt, &msg, portMAX_DELAY);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "[ * ] Event interface error : %d", ret);
            continue;
        }

        if (msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT && msg.source == (void *) music_decoder
            && msg.cmd == AEL_MSG_CMD_REPORT_MUSIC_INFO) {
            audio_element_info_t music_info = {0};
            audio_element_getinfo(music_decoder, &music_info);

            ESP_LOGI(TAG, "[ * ] Receive music info from mp3 decoder, sample_rates=%d, bits=%d, ch=%d",
                     music_info.sample_rates, music_info.bits, music_info.channels);

            audio_element_setinfo(i2s_stream_writer, &music_info);
            i2s_stream_set_clk(i2s_stream_writer, music_info.sample_rates, music_info.bits, music_info.channels);
            continue;
        }

        /* Stop when the last pipeline element (i2s_stream_writer in this case) receives stop event */
        if (msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT && msg.source == (void *) i2s_stream_writer
            && msg.cmd == AEL_MSG_CMD_REPORT_STATUS
            && (((int)msg.data == AEL_STATUS_STATE_STOPPED) || ((int)msg.data == AEL_STATUS_STATE_FINISHED))) {
            ESP_LOGW(TAG, "[ * ] Stop event received");
            break;
        }
    }

    ESP_LOGI(TAG, "[ 7 ] Stop audio_pipeline");
    audio_pipeline_stop(pipeline);
    audio_pipeline_wait_for_stop(pipeline);
    audio_pipeline_terminate(pipeline);

    audio_pipeline_unregister(pipeline, vfs_stream_reader);
    audio_pipeline_unregister(pipeline, i2s_stream_writer);
    audio_pipeline_unregister(pipeline, music_decoder);

    /* Terminal the pipeline before removing the listener */
    audio_pipeline_remove_listener(pipeline);

    /* Stop all periph before removing the listener */
    esp_periph_set_stop_all(set);
    audio_event_iface_remove_listener(esp_periph_set_get_event_iface(set), evt);

    /* Make sure audio_pipeline_remove_listener & audio_event_iface_remove_listener are called before destroying event_iface */
    audio_event_iface_destroy(evt);

    /* Release all resources */
    audio_pipeline_deinit(pipeline);
    audio_element_deinit(vfs_stream_reader);
    audio_element_deinit(i2s_stream_writer);
    audio_element_deinit(music_decoder);
    esp_periph_set_destroy(set);
}

static JSValue js_media_play_mp3(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(2)

    ARGV_TO_UINT8(0, i2s)
    if(i2s!=0 && i2s!=1) {
        THROW_EXCEPTION("arg %s must be 0 or 1", "i2s")
    }
    char * path = js_arg_to_vfspath(ctx,argv[1]) ;
    if(!path) {
        THROW_EXCEPTION("out of memory?")
    }

    ds(path)
    play_by_adf(path) ;
    // play_mp3(path) ;

    free(path) ;

	// vidQueue=xQueueCreate(1, sizeof(bitmap_t *));
	// xTaskCreatePinnedToCore(&play_mp3, "play_mp3", 2048, path, 5, NULL, 1);


    return JS_UNDEFINED ;
}

void be_module_media_require(JSContext *ctx) {

    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue media = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "media", media);

    // JS_SetPropertyStr(ctx, media, "test", JS_NewCFunction(ctx, js_i2s_test, "test", 1));

    JS_SetPropertyStr(ctx, media, "setup", JS_NewCFunction(ctx, js_i2s_setup, "setup", 1));
    // JS_SetPropertyStr(ctx, media, "createMusicPlayer", JS_NewCFunction(ctx, js_music_player_create, "createMusicPlayer", 1));
    JS_SetPropertyStr(ctx, media, "playMP3", JS_NewCFunction(ctx, js_media_play_mp3, "playMP3", 1));

    JS_FreeValue(ctx, beapi);
}