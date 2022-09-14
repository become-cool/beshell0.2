
// #include <freertos/task.h>
// #include <freertos/queue.h>

#include "module_media.h"
#include "module_serial.h"
#include "module_fs.h"
#include "widgets_gen.h"
#include "cutils.h"
#include "utils.h"
#include "driver/i2s.h"
#include "pwm_audio.h"
#include "dac_audio.h"


#include "mp3dec.h"


#include "esp_log.h"
#include <stdio.h>
#include <string.h>
#include "psram.h"
#include <sys/stat.h>


#define echo_time(msg, codes)                          \
    {                                                       \
        int64_t t = gettime() ;                             \
        codes                                               \
        printf(msg": %lldms\n", gettime()-t) ; \
    }
#define echof_time(msg, codes, ...)                         \
    {                                                       \
        int64_t t = gettime() ;                             \
        codes                                               \
        printf(msg": %lldms\n", __VA_ARGS__, gettime()-t) ; \
    }
#define necho_time(msg, codes)  codes
#define nechof_time(msg, codes, ...)  codes


#define BUFF_SRC_SIZE   MAINBUF_SIZE
#define BUFF_FRAME_SIZE       1024*10




typedef struct {

    char * buff ;
    char * pread ;
    char * pwrite ;
    uint32_t size ;

    // SemaphoreHandle_t writable ;   // 有效数据信号量，task_decode give, task_play take
    SemaphoreHandle_t readable ;   // 有效数据信号量，task_decode give, task_play take
    SemaphoreHandle_t mutex ;       // pread / pwrite 操作互斥量

} ringbuff_t ;

static bool ring_create(ringbuff_t * ring, uint32_t size) {
    memset(ring, 0, sizeof(ringbuff_t));
    ring->buff = heap_caps_malloc(size, MALLOC_CAP_DMA) ;
    if(!ring->buff) {
        return false ;
    }
    ring->size = size ;
    ring->pread = ring->buff ;
    ring->pwrite = ring->buff ;
    ring->readable = xSemaphoreCreateBinary() ;
    ring->mutex = xSemaphoreCreateMutex() ;
    return true ;
}

static void ring_delete(ringbuff_t * ring) {
    free(ring->buff) ;
    vSemaphoreDelete(ring->readable) ;
    vSemaphoreDelete(ring->mutex) ;
    memset(ring, 0, sizeof(ringbuff_t));
}

#define POS_R(ring)  (ring->pread - ring->buff)
#define POS_W(ring)  (ring->pwrite - ring->buff)


static inline uint32_t ring_read(ringbuff_t * ring, char * buff, uint32_t bufflen) {

    xSemaphoreTake(ring->mutex, portMAX_DELAY) ;

    uint32_t freelen = 0 ;

    // write 在前
    if( ring->pwrite > ring->pread) {
        freelen = ring->pwrite - ring->pread ;
    }
    // read 在前
    else if( ring->pwrite < ring->pread ) {
        freelen = ring->size - (ring->pread - ring->buff) ;
    }
    
    if(bufflen>freelen) {
        bufflen = freelen ;
    }

    if(bufflen) {
        memcpy(buff, ring->pread, bufflen) ;
        ring->pread+= bufflen ;

        if( POS_R(ring)==ring->size ) {
            ring->pread = ring->buff ;
        }
    }

    xSemaphoreGive(ring->mutex) ;

    return bufflen ;
}

static inline uint32_t ring_write(ringbuff_t * ring, char * buff, uint32_t length) {
    
    uint32_t freelen = 0 ;

    xSemaphoreTake(ring->mutex, portMAX_DELAY) ;

    // read 在前
    if( ring->pwrite < ring->pread ) {
        freelen = ring->pread - ring->pwrite - 4 ;  // 比实际可写空间少 4个字节，避免套圈
    }

    // write 在前 或 相同位置
    else {
        freelen = ring->size - (ring->pwrite - ring->buff) ;
    }
    
    if(length>freelen) {
        length = freelen ;
    }

    if(length>0) {
        memcpy(ring->pwrite,buff,length) ;
        ring->pwrite+= length ;
    }

    // 回头
    if(ring->pwrite-ring->buff>=ring->size) {
        ring->pwrite = ring->buff ;
    }

    xSemaphoreGive(ring->mutex) ;

    if(length>0) {
        xSemaphoreGive(ring->readable) ;
    }

    return length ;
}

typedef struct {

    uint8_t i2s:4 ;

    bool playing:1 ;
    bool paused:1 ;

    FILE * file ;
    char src_path[256] ;

    HMP3Decoder decoder ;
    int samplerate ;

    QueueHandle_t queue_decoder;
    QueueHandle_t queue_pcm_player;

    TaskHandle_t task_decoder ;
    TaskHandle_t task_player ;

    uint8_t * buff_src ;
    uint8_t * read_ptr ;
    int decode_left ;

    uint32_t outputcnt ;
    uint32_t inputcnt ;
    
    ringbuff_t ring ;
    

    JSContext * ctx ;
    JSValue jsobj ;
    
} player_t ;



void player_set_playing(player_t * player, bool playing) {
    if(player->playing == playing) {
        return ;
    }
    player->playing = playing ;
    if(playing) {
        JS_DupValue(player->ctx, player->jsobj) ;
    }
    else {
        JS_FreeValue(player->ctx, player->jsobj) ;
    }
}

// 放音线程
static void task_pcm_player(player_t * player) {

    printf("task_pcm_player()\n") ;

    int idle_ticks = 10/ portTICK_PERIOD_MS ;
    size_t cnt ;
    int64_t t0 = gettime(), t = 0 ;

    char buff[2048] ;
    uint32_t size ;
    char * pwrite = NULL ;

    while(1) {
        
        vTaskDelay(0) ;

        xSemaphoreTake(player->ring.readable, portMAX_DELAY) ;

        necho_time("ring get", {
        size = ring_read(&player->ring, buff, sizeof(buff)) ;
        })
        if(size==0) {
            continue;
        }
            
            player->outputcnt+= size ;

        pwrite = buff ;

        t = gettime()-t0 ;

        while(size) {
            cnt = 0 ;
            echof_time("delay: %lld, write: %d, wroten: %d, i2s", {
                // 扩展到 32 sample                
                i2s_write_expand(0, pwrite, size, 16, 32, &cnt, portMAX_DELAY );
            }, t, size,cnt)
            // dn2()

            size-= cnt ;
            pwrite+= cnt ;
        }

        t0 = gettime() ;
    }

}

                
// static inline bool mp3_decode_start(player_t * player) {

//     printf("mp3_decode_start()\n") ;

//     if(!player->file) {    
//         ds(player->src_path)
//         player->file = fopen(player->src_path,"rb") ;
//     }
//     if(!player->file) {
//         printf("can not open file %s\n",player->src_path) ; // @TODO: post js event
//         return false ;
//     }

//     char tag[10];
//     int tag_len = 0;
//     int read_bytes = fread(tag, 1, 10, player->file);
//     // dn5(read_bytes, tag[0], tag[1], tag[2], tag[3])

//     // DN(read_bytes)
//     if(read_bytes != 10) {
//         printf("mp3 file length invalid (%d)\n",read_bytes) ; // @TODO: post js event
//         return false ;
//     }
    
//     if (memcmp(tag,"ID3",3) == 0)  {
//         tag_len = ((tag[6] & 0x7F)<< 21)|((tag[7] & 0x7F) << 14) | ((tag[8] & 0x7F) << 7) | (tag[9] & 0x7F);
//             // printf("tag_len: %d %x %x %x %x", tag_len,tag[6],tag[7],tag[8],tag[9]);
//         fseek(player->file, tag_len - 10, SEEK_SET);
//     }
//     else  {
//         fseek(player->file, 0, SEEK_SET);
//     }

//     player->read_ptr = player->buff_src;
//     player->decode_left = 0 ;

//     return true ;
// }


// // 读文件并解码一帧
// // 返回：错误代码 <0; 文件结束 0; 成功 1
// static inline int mp3_decode_frame(player_t * player) {

//     if(player->file==NULL){
//         printf("file not open\n");
//         return -20 ;
//     }
    
//     size_t cnt = 0 ;
//     int errs = 0 ;

//     while (1) {

//         vTaskDelay(0) ;

//         if (player->decode_left < BUFF_SRC_SIZE) {
//             necho_time("memmove", {
//             memmove(player->buff_src, player->read_ptr, player->decode_left);
//             })
//             int br = fread(player->buff_src + player->decode_left, 1, BUFF_SRC_SIZE - player->decode_left, player->file);
//             // printf("read(%d): %lld\n", ts, gettime() - ts);

//             // 结束
//             if ((br == 0)&&(player->decode_left==0)) {
//                 fclose(player->file) ;
//                 player->file = NULL ;
//                 return 0 ;
//             }

//             player->decode_left = player->decode_left + br;
//             player->read_ptr = player->buff_src;
//         }
//         int offset = MP3FindSyncWord(player->read_ptr, player->decode_left);
//         if (offset < 0) {  

//             printf("MP3FindSyncWord not find\n");
//             player->decode_left=0;
//             vTaskDelay(0) ;
//             continue;

//         } else {

//             player->read_ptr += offset;                         //data start point
//             player->decode_left -= offset;                 //in buffer
//             // dp(player->read_ptr)
//             // dp(player->frame_writing->buff)
            
//             necho_time("MP3Decode", {
//             errs = MP3Decode(player->decoder, &player->read_ptr, &player->decode_left, player->frame_writing->buff, 0);
//             })

//             if (errs != 0) {
//                 printf("MP3Decode failed ,code is %d \n",errs);
//                 return errs ;
//             }
//             MP3GetLastFrameInfo(player->decoder, &player->frame_writing->info);
//             return 1 ;
//         }
//     }
// }




// 解码任务线程
static void task_mp3_decoder(player_t * player) {

    printf("task_mp3_decoder()\n") ;

    int cmd ;
    int idle_ticks = 10/ portTICK_PERIOD_MS ;
    int res ;

    // int64_t t0 = gettime(), t1=0 ;

    while(1) {

        vTaskDelay(0) ;

    }
}

#define THIS_PLAYER(thisobj)                                                \
    player_t * thisobj = JS_GetOpaque(this_val, js_audio_player_class_id) ; \
    if(!thisobj) {                                                          \
        THROW_EXCEPTION("must be called as a Player method")           \
    }


static JSClassID js_audio_player_class_id ;
static JSValue js_audio_player_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    JSValue jsobj = JS_NewObjectClass(ctx, js_audio_player_class_id) ;

    dn(sizeof(player_t))
    player_t * player = heap_caps_malloc(sizeof(player_t), MALLOC_CAP_DMA) ;
    if(!player) {
        THROW_EXCEPTION("out of memory?")
    }
    memset(player, 0, sizeof(player_t)) ;
    JS_SetOpaque(jsobj, player) ;

    // 初始化
    player->i2s = 0 ;  // @ TODO js 参数
    echo_DMA("before MP3InitDecoder") ;
    player->decoder = MP3InitDecoder();
    echo_DMA("after MP3InitDecoder") ;

    player->buff_src = heap_caps_malloc(BUFF_SRC_SIZE, MALLOC_CAP_DMA) ;
    if(!player->buff_src) {
        free(player) ;
        THROW_EXCEPTION("out of memory?")
    }

    if(!ring_create(&player->ring, BUFF_FRAME_SIZE)) {
        free(player->buff_src) ;
        free(player) ;
        THROW_EXCEPTION("out of memory?")
    }

    echo_DMA("before xQueueCreate") ;
    player->queue_decoder = xQueueCreate(1, sizeof(int));
    player->queue_pcm_player = xQueueCreate(1, sizeof(int));

    player->ctx = ctx ;
    player->jsobj = jsobj ;

    echo_DMA("before xTaskCreatePinnedToCore") ;
    // xTaskCreatePinnedToCore(&task_mp3_decoder, "task_mp3_decoder", 1024*2, player, 5, &player->task_decoder, 0);
    xTaskCreatePinnedToCore(&task_pcm_player, "task_pcm_player", 1024*4, player, 10, &player->task_player, 1);

    echo_DMA("all done") ;
    return jsobj ;
}
static void js_audio_player_finalizer(JSRuntime *rt, JSValue this_val){
    printf("deinit all player resource") ;
    
    player_t * player = JS_GetOpaque(this_val, js_audio_player_class_id) ;
    if(!player) {
        printf("?! js_audio_player_finalizer()") ;
        return ;
    }

    vTaskDelete(player->task_decoder) ;
    vTaskDelete(player->task_player) ;

    free(player->buff_src) ;
    ring_delete(&player->ring) ;

    if(player->file) {
        fclose(player->file) ;
        player->file = NULL ;
    }

    i2s_zero_dma_buffer(player->i2s);
    MP3FreeDecoder(player->decoder);

    free(player) ;
    JS_SetOpaque(this_val, NULL) ;
}
static JSClassDef js_audio_player_class = {
    "audio.Player",
    .finalizer = js_audio_player_finalizer,
} ;

static JSValue js_audio_play_pcm(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    CHECK_ARGC(1)
    char * path = js_arg_to_vfspath(ctx,argv[0]) ;
    ds(path)
    FILE *h=fopen(path,"rb");
    if(!h) {
        THROW_EXCEPTION("can not open file path")
    }

    char buff[1024] ;
    char * writeptr ;
    int read_bytes = 0 ;
    int write_bytes = 0 ;
    size_t cnt = 0 ;

    i2s_set_clk(0,44100,32,2);

    player->inputcnt = 0 ;

    while( 1 ) {

        nechof_time("fs read, bytes %d", {
        read_bytes = fread(buff, 1, sizeof(buff), h) ;
        player->inputcnt+= read_bytes ;
        }, read_bytes)

        if(!read_bytes) {
            break ;
        }

        writeptr = buff ;

        while(read_bytes) {

            write_bytes = ring_write(&player->ring, writeptr, read_bytes) ;
            writeptr+= write_bytes;
            read_bytes-= write_bytes ;

            if(read_bytes) {
                printf("write block, %d, pos:%d/%d\n",
                    read_bytes
                    , player->ring.pwrite-player->ring.buff
                    , player->ring.pread-player->ring.buff
                ) ;
            
                vTaskDelay(1) ;
            }

        }

        vTaskDelay(0) ;
    }

    dn(player->outputcnt)
    dn(player->inputcnt)

    fclose(h) ;

    free(path) ;
    return JS_UNDEFINED ;
}

static JSValue js_audio_play_pcm_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    char * path = js_arg_to_vfspath(ctx,argv[0]) ;
    ds(path)
    FILE *h=fopen(path,"rb");
    if(!h) {
        THROW_EXCEPTION("can not open file path")
    }

    char buff[2048] ;
    int read_bytes = 0 ;
    size_t cnt = 0 ;

    i2s_set_clk(0,44100,32,2);

    
    uint32_t outputcnt = 0 ;

    int64_t t = gettime() ;
    while( (read_bytes = fread(buff, 1, 2048, h)) ) {
        printf("delay : %lld\n", gettime()-t) ;
        echo_time("write", {
        i2s_write_expand(0, buff, read_bytes, 16, 32, &cnt, portMAX_DELAY );
        })
        t = gettime() ;

        outputcnt+= cnt ;
    }

    dn(outputcnt)

    fclose(h) ;

    free(path) ;
    return JS_UNDEFINED ;
}

static JSValue js_audio_play_mp3(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    if(player->playing) {
        THROW_EXCEPTION("player is playing")
    }
    CHECK_ARGC(1)
    ARGV_TO_STRING(0, path) ;

    if(strlen(path) + strlen(vfs_path_prefix) + 1 > sizeof(player->src_path)) {
        free(path) ;
        THROW_EXCEPTION("path too long")
    }

    sprintf(player->src_path, "%s%s", vfs_path_prefix, path) ;
    JS_FreeCString(ctx, path) ;

    ds(player->src_path)

    player_set_playing(player, true) ;

    // int cmd = CMD_START ;
    // xQueueSend(player->queue_decoder, &cmd, 0) ;

    return JS_UNDEFINED ;
}


static JSValue js_audio_get_status(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    return JS_UNDEFINED ;
}
static JSValue js_audio_pause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}
static JSValue js_audio_resume(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}
static JSValue js_audio_track_info(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}
static JSValue js_audio_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}




static const JSCFunctionListEntry js_audio_player_proto_funcs[] = {
    JS_CFUNC_DEF("playPCM", 0, js_audio_play_pcm),
    JS_CFUNC_DEF("playMP3", 0, js_audio_play_mp3),
} ;

void be_module_media_init() {

    dn(configTICK_RATE_HZ)

    JS_NewClassID(&js_audio_player_class_id);
}

void be_module_media_require(JSContext *ctx) {

    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue audio = JS_NewObject(ctx);
    
    JS_SetPropertyStr(ctx, beapi, "audio", audio);
    JS_SetPropertyStr(ctx, audio, "playPCM", JS_NewCFunction(ctx, js_audio_play_pcm_sync, "playPCM", 1));

    QJS_DEF_CLASS(audio_player, "Player", "audio.Player", JS_UNDEFINED, audio)

    JS_FreeValue(ctx, beapi);
}


void be_module_media_reset(JSContext *ctx) {
}