#include <stdio.h>
#include <string.h>
#include "module_media.h"
#include "module_fs.h"
#include "widgets_gen.h"
#include "cutils.h"
#include "utils.h"
#include "eventloop.h"

#ifndef SIMULATION

#include "freertos/event_groups.h"
#include "esp_log.h"
#include "psram.h"
#include <sys/stat.h>
#include "module_serial.h"
#include "driver/i2s.h"

#else

// fake for pc simulation 
#define configTICK_RATE_HZ 10
#define portTICK_PERIOD_MS 10

#define portMAX_DELAY 0
#define xQueueReceive(a,b,c)
#define xRingbufferSend(a,b,c,d) pdTRUE
#define xRingbufferPrintInfo(a)
#define xRingbufferReceiveUpTo(a,b,c,d) NULL
#define vRingbufferReturnItem(a,b)
#define vTaskDelay(a)
#define xRingbufferCreate(a,b) NULL ;
#define vRingbufferDelete(a) ;
#define xQueueCreate(a,b) NULL ;
#define vQueueDelete(a) ;
#define xTaskCreatePinnedToCore(a,b,c,d,e,f,g)
#define vTaskDelete(a) ;
#define xQueueSend(a,b,c) ;

#define echo_DMA(a) ;

#define i2s_set_clk(a,b,c,d)
#define i2s_zero_dma_buffer(a)
#define i2s_write_expand(a,b,c,d,e,f,g)

typedef void * QueueHandle_t;
typedef void * TaskHandle_t;
typedef void * RingbufHandle_t;

typedef int8_t BaseType_t;
typedef uint8_t UBaseType_t;

enum {
    RINGBUF_TYPE_BYTEBUF
} ;

typedef void (*TaskFunction_t)(void * d);

#define pdPASS 0
#define pdTRUE 1

#define MALLOC_CAP_DMA 0
#define heap_caps_malloc(s, a) malloc(s)

#endif

#include "mp3dec.h"


#define echo_time(msg, codes)                                   \
    {                                                           \
        int64_t __tt = gettime() ;                              \
        codes                                                   \
        printf(msg": %lldms\n", gettime()-__tt) ;               \
    }
#define echof_time(msg, codes, ...)                             \
    {                                                           \
        int64_t __tt = gettime() ;                              \
        codes                                                   \
        printf(msg": %lldms\n", __VA_ARGS__, gettime()-__tt) ;  \
    }
#define necho_time(msg, codes)  codes
#define nechof_time(msg, codes, ...)  codes


#define BUFF_SRC_SIZE   MAINBUF_SIZE
#define BUFF_FRAME_SIZE       1024*5



#define STAT_RUNNING        BIT1
#define STAT_STOPPING       BIT2
#define STAT_STOPPED        BIT3
#define STAT_DRAIN          BIT4

typedef struct _stream_elment{

    TaskHandle_t task ;
    RingbufHandle_t ring ;
    EventGroupHandle_t stats ;

    struct _stream_elment * upstream;
    struct _stream_elment * downstream;
    void * pipe ;

} stream_element_t ;


typedef struct {

    stream_element_t base ;

    char src_path[256] ;
    FILE * file ;

} el_src_t ;

typedef struct {

    stream_element_t base ;

    HMP3Decoder decoder ;
    MP3FrameInfo info ;

#ifdef SIMULATION
    FILE * fin ;
    FILE * fout ;
#endif

} el_mp3_t ;

typedef struct {
    stream_element_t base ;
    uint8_t i2s ;
} el_i2s_t ;

typedef struct _player {

    bool running:1 ;
    bool paused:1 ;

    int samplerate ;

    el_src_t * src ;
    el_mp3_t * decode ;
    el_i2s_t * playback ;

    stream_element_t * first ;
    stream_element_t * last ;
    
    JSContext * ctx ;
    JSValue jsobj ;
    
} player_t ;



static void el_init(
        stream_element_t * el
        , TaskFunction_t pvTaskCode
        , const char *const pcName
        , const uint32_t usStackDepth
        , UBaseType_t uxPriority
        , const BaseType_t xCoreID
        , size_t ringSz
    ) {

    if(ringSz) {
        el->ring = xRingbufferCreate(ringSz, RINGBUF_TYPE_BYTEBUF);
    }
    else {
        el->ring = NULL ;
    }

    el->stats = xEventGroupCreate() ;
    xEventGroupClearBits(el->stats, 0xFFFF) ;
    xEventGroupSetBits(el->stats, STAT_STOPPED | STAT_DRAIN) ;

    xTaskCreatePinnedToCore(pvTaskCode, pcName, usStackDepth, el, uxPriority, &el->task, xCoreID);
}


#define ELEMENT_CREATE(_pipe, type, var, func,stack,prio,core,ringsize) \
    var = heap_caps_malloc(sizeof(type), MALLOC_CAP_DMA) ;              \
    memset(var, 0, sizeof(type)) ;                                      \
    var->base.pipe = _pipe ;                                            \
    el_init(var, func, #func, stack, prio, core, ringsize ) ;

static void task_mp3_decoder(el_mp3_t * el) ;
static stream_element_t * el_mp3_create(player_t * pipe) {    
    el_mp3_t * el ;
    ELEMENT_CREATE(pipe, el_mp3_t, el, task_mp3_decoder, 1024*2+BUFF_SRC_SIZE+BUFF_FRAME_SIZE, 5, 1, BUFF_FRAME_SIZE)

    el->decoder = MP3InitDecoder();

    return el ;
}
static void el_delete(stream_element_t * el) {
    vTaskDelete(el->task) ;
    vEventGroupDelete(el->stats) ;
    if(el->ring) {
        vRingbufferDelete(el->ring) ;
    }
    free(el) ;
}
static void el_src_delete(el_src_t * el) {
    if(el->file) {
        fclose(el->file) ;
        el->file = NULL ;
    }
    el_delete(el) ;
}
static void el_mp3_delete(el_mp3_t * el) {
    MP3FreeDecoder(el->decoder);
    el_delete(el) ;
}
static void el_i2s_delete(el_i2s_t * el) {
    i2s_zero_dma_buffer(el->i2s);
    el_delete(el) ;
}


static void pipe_link(player_t * pipe, int el_cnt, ...) {

    stream_element_t * el ;
	va_list argptr;
	va_start(argptr, el_cnt);

    pipe->first = pipe->last = NULL ;

	for (int i=0;i<el_cnt; i++) {
		el = va_arg(argptr, stream_element_t *);
        if(i==0) {
            el->upstream = NULL ;
            pipe->first = pipe->last = el ;
        }
        else {
            el->upstream = pipe->last ;
            pipe->last->downstream = el ;
            pipe->last = el ;
        }

        el->downstream = NULL ;

	}
	va_end(argptr);
}


static void pipe_set_stats(player_t * pipe, int stats) {
    for(stream_element_t * el=pipe->last; el; el=el->upstream) {
        xEventGroupSetBits(el->stats, stats) ;
    }
}
static void pipe_clear_stats(player_t * pipe, int stats) {
    for(stream_element_t * el=pipe->last; el; el=el->upstream) {
        xEventGroupClearBits(el->stats, stats) ;
    }
}

static inline void emit_event(JSContext * ctx, JSValue thisobj, const char * event) {

    JSValue emit = js_get_glob_prop(ctx, 4, "beapi", "EventEmitter", "prototype", "emit") ;

    JSValue eventName = JS_NewString(ctx, event) ;
    MAKE_ARGV1(argv, eventName)
    eventloop_push_with_argv(ctx, emit, thisobj, 1, argv) ;

    JS_FreeValue(ctx, eventName) ;
    JS_FreeValue(ctx, emit) ;

}

static void pipe_stop(player_t * player) {

    // 从前到后依次关闭
    for(stream_element_t * el=player->first; el; el=el->downstream) {
        xEventGroupSetBits(el->stats, STAT_STOPPING) ;
        xEventGroupWaitBits(el->stats, STAT_STOPPED, false, true, portMAX_DELAY) ;
    }

    player->running = false ;

    printf("audio stop\n") ;

    if( player->ctx && JS_IsObject(player->jsobj) ){
        emit_event(player->ctx, player->jsobj, "stop") ;
    }
}

static bool el_src_strip_pcm(el_src_t * el) {

    if(el->file){
        fclose(el->file) ;
        el->file = NULL ;
    }
    el->file = fopen(el->src_path,"rb") ;
    if(!el->file) {
        printf("can not open file: %s", el->src_path) ;
        return false ;
    }

    return true ;
}


static bool el_src_strip_mp3(el_src_t * el) {
    if(!el_src_strip_pcm(el)){
        return false ;
    }

    char tag[10];
    int tag_len = 0;
    int read_bytes = fread(tag, 1, 10, el->file);

    if(read_bytes != 10) {
        printf("mp3 file length invalid (%d)\n",read_bytes) ; // @TODO: post js event
        return false ;
    }
    
    if (memcmp(tag,"ID3",3) == 0)  {
        tag_len = ((tag[6] & 0x7F)<< 21)|((tag[7] & 0x7F) << 14) | ((tag[8] & 0x7F) << 7) | (tag[9] & 0x7F);
        fseek(el->file, tag_len - 10, SEEK_SET);
    }
    else  {
        fseek(el->file, 0, SEEK_SET);
    }

    printf("mp3 valid \n") ;

    return true ;
}


#define TASK_STOP(el, code)                                                                 \
    if( xEventGroupGetBits(el->base.stats) & STAT_STOPPING ) {              \
        xEventGroupWaitBits(el->base.stats, STAT_DRAIN, false, true, 100);                  \
        code                                                                                \
        xEventGroupClearBits(el->base.stats, STAT_RUNNING) ;                \
        xEventGroupClearBits(el->base.stats, STAT_STOPPING) ;                \
        xEventGroupSetBits(el->base.stats, STAT_STOPPED) ;                                  \
        vTaskDelay(0) ;                                                                     \
    }


// 任务线程：文件读入
static void task_src(el_src_t * el) {
    printf("task_src()\n") ;
    // int cmd ;

    char buff[1024] ;
    size_t read_bytes ;

    while(1) {

        // 停止任务
        TASK_STOP(el, {
            if(el->file) {
                fclose(el->file) ;
                el->file = NULL ;
            }
            dd
        })

        // 等待开始状态
        xEventGroupWaitBits(el->base.stats, STAT_RUNNING, false, true, portMAX_DELAY);

        if(!el->file) {
            pipe_stop((player_t *)el->base.pipe) ;
            continue ;
        }

        nechof_time("fs read, bytes %d", {
            read_bytes = fread(buff, 1, sizeof(buff), el->file) ;
        }, read_bytes)
        if(!read_bytes) {
            pipe_stop((player_t *)el->base.pipe) ;
            continue ;
        }

        if(pdTRUE != xRingbufferSend(el->base.ring, buff, read_bytes, portMAX_DELAY)) {
            printf("task src xRingbufferSend() faild ???") ;
        }
        else {
            xEventGroupClearBits(el->base.stats, STAT_DRAIN) ;
        }
        vTaskDelay(0) ;
        continue;
    }
}



// 解码任务线程
static void task_mp3_decoder(el_mp3_t * el) {

    printf("task_mp3_decoder()\n") ;

    int res ;

    size_t data_size = 0 ;
    unsigned char buff_src[BUFF_SRC_SIZE] ;
    unsigned char buff_pcm[BUFF_FRAME_SIZE] ;

    unsigned char * psrc = buff_src ;
    int decode_left = 0 ;
    int errs = 0 ;

    int samprate = 0 ;

    for(int i=0;i<10;) {
        vTaskDelay(0) ;
        
        // 停止任务
        TASK_STOP(el, {
            psrc = buff_src ;
            decode_left = 0 ;
            dd
        })

        // 等待开始状态
        xEventGroupWaitBits(el->base.stats, STAT_RUNNING, false, true, portMAX_DELAY);

        if (decode_left >= BUFF_SRC_SIZE) {
            printf("????") ;
            continue ;
        }

        if(!el->base.upstream || !el->base.upstream->ring) {
            vTaskDelay(10) ;
            continue ;
        }

        necho_time("memmove", {
        memmove(buff_src, psrc, decode_left);
        })

#ifndef SIMULATION
        data_size = 0 ;
        psrc = xRingbufferReceiveUpTo(el->base.upstream->ring, &data_size, 10, sizeof(buff_src)-decode_left);
        if(psrc) {
            memcpy(buff_src+decode_left, psrc, data_size) ;
            vRingbufferReturnItem(el->base.upstream->ring, psrc) ;   
        }
        if ( data_size==0 || !psrc ) {
            dd
            xEventGroupSetBits(el->base.upstream->stats, STAT_DRAIN) ;  // 前级流干
            vTaskDelay(1) ;

            if ( decode_left==0 ) {
                continue ;
            }
        }
#else
        data_size = fread(buff_src+decode_left, 1, sizeof(buff_src)-decode_left, el->fin) ;
        if(!data_size) {
            break ;
        }
#endif

        decode_left = decode_left + data_size;
        psrc = buff_src;
        

        int offset = MP3FindSyncWord(psrc, decode_left);
        if (offset < 0) {
            decode_left = 0;
            vTaskDelay(0) ;
            continue;

        } else {
            psrc += offset;                         //data start point
            decode_left -= offset;                 //in buffer

            necho_time("MP3Decode", {
            errs = MP3Decode(el->decoder, &psrc, &decode_left, buff_pcm, 0);
            })

            if (errs != 0) {
                // printf("MP3Decode failed ,code is %d \n",errs);
                continue ;
            }

            MP3GetLastFrameInfo(el->decoder, &el->info);
            if(el->info.samprate!=samprate) {
                dn(el->info.samprate)

                // 等这一帧以前的数据播放完成
                xEventGroupWaitBits(el->base.stats, STAT_DRAIN, false, true, 100);

                i2s_set_clk(
                    ((player_t*)el->base.pipe)->playback->i2s
                    , el->info.samprate
                    , 32
                    , el->info.nChans
                );

                printf("i2snum:%d, samprate:%d, bps:%d, chans:%d \n",
                    ((player_t*)el->base.pipe)->playback->i2s
                    , el->info.samprate
                    , 32
                    , el->info.nChans) ;

                samprate = el->info.samprate ;
            }

            int used = data_size - decode_left ;


#ifndef SIMULATION
            if(pdTRUE != xRingbufferSend(el->base.ring, buff_pcm, el->info.outputSamps * el->info.nChans, portMAX_DELAY)) {
                printf("task mp3 decode xRingbufferSend() wrong ?????\n") ;
            }
            else {
                xEventGroupClearBits(el->base.stats, STAT_DRAIN) ;
            }

#else
            fwrite(buff_pcm,1,el->info.outputSamps * el->info.nChans,el->fout) ;
#endif

            vTaskDelay(0) ;
        }
    }

    while(1) {}
}



// 任务线程：放音
static void task_pcm_playback(el_i2s_t * el) {

    printf("task_pcm_playback()\n") ;

    int idle_ticks = 10/ portTICK_PERIOD_MS ;
    int64_t t0 = gettime(), t = 0 ;

    char buff[1024] ;
    uint32_t data_size ;
    size_t data_wroten ;
    char * pwrite = NULL ;

    for(int i=0;i<10;) {
        
        vTaskDelay(0) ;

        // 停止任务
        TASK_STOP(el, {
            i2s_zero_dma_buffer(el->i2s) ;
            dd
        })

        // 等待开始状态
        xEventGroupWaitBits(el->base.stats, STAT_RUNNING, false, true, portMAX_DELAY);


        // dp(el->base.upstream->ring)
        // EL_RECV_UPSTREAM(el, pwrite, buff, sizeof(buff), data_size)

        if(!el->base.upstream || !el->base.upstream->ring) {
            vTaskDelay(10) ;
            continue ;
        }
        data_size = 0 ;
        pwrite = xRingbufferReceiveUpTo(el->base.upstream->ring, &data_size, 10, sizeof(buff));
        if(data_size==0 || !pwrite) {
            dd
            xEventGroupSetBits(el->base.upstream->stats, STAT_DRAIN) ;  // 前级流干
            vTaskDelay(1) ;
            continue ;
        }
        
        memcpy(buff, pwrite, data_size) ;
        vRingbufferReturnItem(el->base.upstream->ring, pwrite) ;
        
        pwrite = buff ;
            
        t = gettime()-t0 ;

        while(data_size) {

            data_wroten = 0 ;
            nechof_time("delay:%lld,%d->%d", {
                // 扩展到 32 sample                
                i2s_write_expand(el->i2s, pwrite, data_size, 16, 32, &data_wroten, portMAX_DELAY );
            }, t, data_size, data_wroten)

            data_size-= data_wroten ;
            pwrite+= data_wroten ;
        }

        t0 = gettime() ;
        // i++ ;

    }

    while(1){}
}

#define THIS_PLAYER(thisobj)                                                \
    player_t * thisobj = JS_GetOpaque(this_val, js_audio_player_class_id) ; \
    if(!thisobj) {                                                          \
        THROW_EXCEPTION("must be called as a Player method")           \
    }


static JSClassID js_audio_player_class_id ;
static JSValue js_audio_player_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    JSValue jsobj = JS_NewObjectClass(ctx, js_audio_player_class_id) ;
    JS_SetPropertyStr(ctx, jsobj, "_handlers", JS_NewObject(ctx));

    player_t * player = heap_caps_malloc(sizeof(player_t), MALLOC_CAP_DMA) ;
    if(!player) {
        THROW_EXCEPTION("out of memory?")
    }
    memset(player, 0, sizeof(player_t)) ;
    JS_SetOpaque(jsobj, player) ;

    // 初始化
    ELEMENT_CREATE(player, el_src_t, player->src, task_src, 1024*3, 5, 1, 1024)
    player->decode = el_mp3_create(player) ;
    ELEMENT_CREATE(player, el_i2s_t, player->playback, task_pcm_playback, 1024*3, 5, 1, 0)

    player->last = player->playback ;

    player->ctx = ctx ;
    player->jsobj = jsobj ;

    // 避免 gc 销毁对象
    // 调用 detach() 方法释放这里的引用
    JS_DupValue(ctx, jsobj) ;

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

    el_src_delete(player->src) ;
    el_mp3_delete(player->decode) ;
    el_i2s_delete(player->playback) ;

    free(player) ;
    JS_SetOpaque(this_val, NULL) ;
}
static JSClassDef js_audio_player_class = {
    "audio.Player",
    .finalizer = js_audio_player_finalizer,
} ;

static JSValue js_audio_play_pcm(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    if(player->running) {
        THROW_EXCEPTION("player is running")
    }
    CHECK_ARGC(1)
    ARGV_TO_STRING(0, path) ;

    if(strlen(path) + strlen(vfs_path_prefix) + 1 > sizeof(player->src->src_path)) {
        free(path) ;
        THROW_EXCEPTION("path too long")
    }

    sprintf(player->src->src_path, "%s%s", vfs_path_prefix, path) ;
    JS_FreeCString(ctx, path) ;

    i2s_set_clk(0,44100,32,2);

    if(!el_src_strip_pcm(player->src)) {
        THROW_EXCEPTION("file not exists") ;
    }

    // src -> playback
    pipe_link( player, 2, player->src, player->playback ) ;

    pipe_clear_stats(player, STAT_STOPPING) ;
    pipe_clear_stats(player, STAT_STOPPED) ;
    pipe_set_stats(player, STAT_RUNNING) ;

    player->paused = false ;
    player->running = true ;
    emit_event(ctx, player->jsobj, "play") ;

    return JS_UNDEFINED ;
}



static JSValue js_audio_play_mp3(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    if(player->running) {
        THROW_EXCEPTION("player is running")
    }
    CHECK_ARGC(1)
    ARGV_TO_STRING(0, path) ;

    if(strlen(path) + strlen(vfs_path_prefix) + 1 > sizeof(player->src->src_path)) {
        free(path) ;
        THROW_EXCEPTION("path too long")
    }
    sprintf(player->src->src_path, "%s%s", vfs_path_prefix, path) ;
    JS_FreeCString(ctx, path) ;

#ifndef SIMULATION
    if(!el_src_strip_mp3(player->src)) {
        THROW_EXCEPTION("file not exists") ;
    }

    // src -> decode -> playback
    pipe_link( player, 3, player->src, player->decode, player->playback ) ;
    // dp(player->src)
    // dp(player->decode)
    // dp(player->playback)

    player->paused = false ;
    player->running = true ;
    emit_event(ctx, player->jsobj, "play") ;

    pipe_clear_stats(player, STAT_STOPPING) ;
    pipe_clear_stats(player, STAT_STOPPED) ;
    pipe_set_stats(player, STAT_RUNNING) ;

#else
    player->decode->fin = fopen("../filesystem/root/mnt/sd/music-16b-2c-44100hz.mp3", "rb") ;
    player->decode->fout = fopen("../filesystem/root/mnt/sd/test2.pcm", "wb") ;
    task_mp3_decoder(NULL) ;
    fclose(player->decode->fin) ;
    fclose(player->decode->fout) ;
#endif
    return JS_UNDEFINED ;
}


static JSValue js_audio_is_running(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    return player->running? JS_TRUE : JS_FALSE ;
}
static JSValue js_audio_is_paused(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    return player->paused? JS_TRUE : JS_FALSE ;
}
static JSValue js_audio_pause(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    pipe_clear_stats(player, STAT_RUNNING) ;
    player->paused = true ;
    emit_event(ctx, player->jsobj, "pause") ;
    return JS_UNDEFINED ;
}
static JSValue js_audio_resume(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    pipe_set_stats(player, STAT_RUNNING) ;
    player->paused = false ;
    emit_event(ctx, player->jsobj, "resume") ;
    return JS_UNDEFINED ;
}
static JSValue js_audio_track_info(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}
static JSValue js_audio_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    pipe_stop(player) ;
    return JS_UNDEFINED ;
}


static JSValue js_audio_play_detach(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    JS_FreeValue(ctx, player->jsobj) ;
    return JS_UNDEFINED ;
}

static const JSCFunctionListEntry js_audio_player_proto_funcs[] = {
    JS_CFUNC_DEF("playPCM", 0, js_audio_play_pcm),
    JS_CFUNC_DEF("playMP3", 0, js_audio_play_mp3),
    JS_CFUNC_DEF("pause", 0, js_audio_pause),
    JS_CFUNC_DEF("resume", 0, js_audio_resume),
    JS_CFUNC_DEF("stop", 0, js_audio_stop),
    JS_CFUNC_DEF("isRunner", 0, js_audio_is_running),
    JS_CFUNC_DEF("isPaused", 0, js_audio_is_paused),
    JS_CFUNC_DEF("detach", 0, js_audio_play_detach),
} ;

void be_module_media_init() {

    dn(configTICK_RATE_HZ)

    JS_NewClassID(&js_audio_player_class_id);
}

void be_module_media_require(JSContext *ctx) {

    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue audio = JS_NewObject(ctx);
    
    JS_SetPropertyStr(ctx, beapi, "audio", audio);
    // JS_SetPropertyStr(ctx, audio, "crash", JS_NewCFunction(ctx, js_audio_crash, "crash", 1));

    
    JSValue EventEmitterProto = js_get_glob_prop(ctx, 3, "beapi", "EventEmitter", "prototype") ;
    QJS_DEF_CLASS(audio_player, "Player", "audio.Player", EventEmitterProto, audio)

    JS_FreeValue(ctx, beapi);
    JS_FreeValue(ctx, EventEmitterProto);
}


void be_module_media_reset(JSContext *ctx) {
}