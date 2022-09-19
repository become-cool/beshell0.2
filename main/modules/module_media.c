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


#define echo_alloc(msg, codes)                                  \
    {                                                           \
        size_t __m = heap_caps_get_free_size(MALLOC_CAP_DMA) ;  \
        codes                                                   \
        printf(msg" alloc: %d\n", __m - heap_caps_get_free_size(MALLOC_CAP_DMA)) ; \
    }
#define necho_alloc(msg, codes, ...)  codes

#define BUFF_SRC_SIZE   MAINBUF_SIZE/2
#define BUFF_SRC_MEMTYPE   MALLOC_CAP_DMA


#define STAT_RUNNING        BIT1
#define STAT_STOPPING       BIT2
#define STAT_STOPPED        BIT3
#define STAT_ALL_PHASE      (STAT_RUNNING | STAT_STOPPING | STAT_STOPPED)
// 以上 bit 是互斥的

#define STAT_DRAIN          BIT4

typedef struct _stream_elment{

    TaskHandle_t task ;
    RingbufHandle_t ring ;
    size_t ringSz ;
    EventGroupHandle_t stats ;

    struct _stream_elment * upstream;
    struct _stream_elment * downstream;
    void * pipe ;

} el_t ;

static inline void el_set_stat(el_t * el, EventBits_t stat) {
    xEventGroupClearBits(el->stats, STAT_ALL_PHASE&(~stat)) ;
    xEventGroupSetBits(el->stats, stat) ;
}

typedef struct {

    el_t base ;

    char src_path[256] ;
    FILE * file ;

} el_src_t ;

typedef struct {

    el_t base ;

    uint8_t * undecode_buff ;
    MP3DecInfo * decoder ;

    int samprate ;
    int channels ;

} el_mp3_t ;

typedef struct {
    el_t base ;
    uint8_t i2s ;
} el_i2s_t ;

typedef struct _player {

    bool running:1 ;
    bool paused:1 ;
    bool finished: 1 ;   // 正常播放完毕: finished=true， 中途停止: finished=false

    int8_t error ;

    int samplerate ;

    el_src_t * src ;
    el_mp3_t * decode ;
    el_i2s_t * playback ;

    el_t * first ;
    el_t * last ;
    
    JSContext * ctx ;
    JSValue jsobj ;
    
} player_t ;



static void el_init(
        el_t * el
        , TaskFunction_t pvTaskCode
        , const char *const pcName
        , const uint32_t usStackDepth
        , UBaseType_t uxPriority
        , const BaseType_t xCoreID
        , size_t ringSz
    ) {

    el->ringSz = ringSz ;
    if(ringSz) {
        el->ring = xRingbufferCreate(ringSz, RINGBUF_TYPE_BYTEBUF);
    }
    else {
        el->ring = NULL ;
    }

    el->stats = xEventGroupCreate() ;
    el_set_stat(el, STAT_STOPPED | STAT_DRAIN) ;

    xTaskCreatePinnedToCore(pvTaskCode, pcName, usStackDepth, el, uxPriority, &el->task, xCoreID);
}


#define ELEMENT_CREATE(_pipe, type, var, func,stack,prio,core,ringsize) \
    var = heap_caps_malloc(sizeof(type), MALLOC_CAP_DMA) ;              \
    memset(var, 0, sizeof(type)) ;                                      \
    var->base.pipe = _pipe ;                                            \
    el_init(var, func, #func, stack, prio, core, ringsize ) ;

static void task_mp3_decoder(el_mp3_t * el) ;
static el_t * el_mp3_create(player_t * pipe) {    
    el_mp3_t * el ;
    echo_alloc("el_mp3_t",{
        ELEMENT_CREATE(pipe, el_mp3_t, el, task_mp3_decoder, 1024*3, 5, 1, 512)
    })

    echo_alloc("hexli", {
        el->decoder = MP3InitDecoder();
    })

    el->samprate = 0 ;
    el->channels = 0 ;

    el->undecode_buff = heap_caps_malloc(BUFF_SRC_SIZE, BUFF_SRC_MEMTYPE) ;

    return el ;
}
static void el_delete(el_t * el) {
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
    free(el->undecode_buff) ;
    el->undecode_buff = NULL ;
    el_delete(el) ;
}
static void el_i2s_delete(el_i2s_t * el) {
    i2s_zero_dma_buffer(el->i2s);
    el_delete(el) ;
}

static void el_mp3_reset(el_mp3_t * el) {
    MP3ResetDecoder(el->decoder) ;
}


static bool el_is_drain(el_t * el) {
    if(!el->ring) {
        return true ;
    }
    UBaseType_t uxItemsWaiting = 0 ;
    vRingbufferGetInfo(el->ring,NULL,NULL,NULL,NULL,&uxItemsWaiting) ;
    return uxItemsWaiting==0 ;
}

static void el_print_stats(el_t * el) {

    printf("%s: %s\n", "running", (xEventGroupGetBits(el->stats)&STAT_RUNNING)? "true": "false") ;
    printf("%s: %s\n", "stopping", (xEventGroupGetBits(el->stats)&STAT_STOPPING)? "true": "false") ;
    printf("%s: %s\n", "stopped", (xEventGroupGetBits(el->stats)&STAT_STOPPED)? "true": "false") ;
    printf("%s: %s\n", "drain", (xEventGroupGetBits(el->stats)&STAT_DRAIN)? "true": "false") ;

    if(el->ring) {
        UBaseType_t free = NULL ;
        UBaseType_t read = NULL ;
        UBaseType_t write = NULL ;
        UBaseType_t acquire = NULL ;
        UBaseType_t waiting = NULL ;
        vRingbufferGetInfo(el->ring, &free, &read, &write, &acquire,&waiting) ;

        #define print_ring_pos(var)  printf("ring %s@%d\n", #var, var) ;
        print_ring_pos(write)
        print_ring_pos(read)
        print_ring_pos(free)
        print_ring_pos(acquire)
        print_ring_pos(waiting)
    }
}

static void pipe_link(player_t * pipe, int el_cnt, ...) {

    el_t * el ;
	va_list argptr;
	va_start(argptr, el_cnt);

    pipe->first = pipe->last = NULL ;

	for (int i=0;i<el_cnt; i++) {
		el = va_arg(argptr, el_t *);
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
    for(el_t * el=pipe->last; el; el=el->upstream) {
        el_set_stat(el, stats) ;
    }
}
static void pipe_clear_stats(player_t * pipe, int stats) {
    for(el_t * el=pipe->last; el; el=el->upstream) {
        xEventGroupClearBits(el->stats, stats) ;
    }
}

static void el_clear_ring(el_t * el) {
    if(el->ring && el->ringSz) {
        vRingbufferDelete(el->ring) ;
        el->ring = xRingbufferCreate(el->ringSz, RINGBUF_TYPE_BYTEBUF);
    }
}

static void pipe_clear(player_t * pipe) {
    el_clear_ring(pipe->src);
    el_clear_ring(pipe->decode);
    el_clear_ring(pipe->playback);
}

static inline void emit_event(JSContext * ctx, JSValue thisobj, const char * event, JSValue param) {

    JSValue emit = js_get_glob_prop(ctx, 4, "beapi", "EventEmitter", "prototype", "emit") ;

    JSValue eventName = JS_NewString(ctx, event) ;

    if(JS_IsUndefined(param) ) {
        MAKE_ARGV1(argv, eventName)
        eventloop_push_with_argv(ctx, emit, thisobj, 1, argv) ;
    }
    else {
        MAKE_ARGV2(argv, eventName, param)
        eventloop_push_with_argv(ctx, emit, thisobj, 2, argv) ;
    }

    JS_FreeValue(ctx, eventName) ;
    JS_FreeValue(ctx, emit) ;

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

    return true ;
}


#define PLAYER(el) ((player_t*)el->pipe)
static inline void el_stop_when_req(el_t * el) {
    if( ! (xEventGroupGetBits(el->stats) & STAT_STOPPING) ) {
        printf("not set STAT_STOPPING \n") ;
        return ;
    }

    // 等待数据流干
    if(!el_is_drain(el)) {
        if(el->downstream) {
            el_set_stat(el->downstream, STAT_RUNNING);
        }
        xEventGroupWaitBits(el->stats, STAT_DRAIN, false, true, 100);
    }

    el_set_stat(el, STAT_STOPPED) ;

    // 将 STAT_STOPPING 向后级传递
    if(el->downstream) {
        el_set_stat(el->downstream, STAT_STOPPING) ;
    }

    // 最后一个节点：触发 finish 事件
    else {
        PLAYER(el)->running = false ;
        PLAYER(el)->paused = false ;
        if( PLAYER(el)->ctx && JS_IsObject(PLAYER(el)->jsobj) ){
            if(PLAYER(el)->error) {
                emit_event(PLAYER(el)->ctx, PLAYER(el)->jsobj, "error", JS_NewInt32(NULL, PLAYER(el)->error)) ;
            }
            emit_event(PLAYER(el)->ctx, PLAYER(el)->jsobj, PLAYER(el)->finished? "finish": "stop", JS_UNDEFINED) ;
        }

    }

    vTaskDelay(0) ;
}


// 任务线程：文件读入
static void task_src(el_src_t * el) {
    // printf("task_src()\n") ;
    // int cmd ;

    char buff[512] ;
    size_t read_bytes ;
    EventBits_t bits ;

    while(1) {

        // 等待开始状态
        bits = xEventGroupWaitBits(el->base.stats, STAT_RUNNING|STAT_STOPPING, false, false, portMAX_DELAY);
        if( bits&STAT_STOPPING ) {
            el_stop_when_req(el) ;

            if(el->file) {
                fclose(el->file) ;
                el->file = NULL ;
            }

            vTaskDelay(1) ;
            continue ;
        }

        if(!el->file) {
            goto finish ;
            continue ;
        }

        nechof_time("fs read, bytes %d", {
            read_bytes = fread(buff, 1, sizeof(buff), el->file) ;
        }, read_bytes)
        if(!read_bytes) {
            goto finish ;
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
finish:
        ((player_t *)el->base.pipe)->finished = true ;
        el_set_stat( el, STAT_STOPPING ) ;
        vTaskDelay(10) ;
        continue;

    }
}



static void mp3dec_output(el_mp3_t * el, uint8_t * data, size_t size) {

    if( el->decoder->samprate!=el->samprate || el->decoder->nChans!=el->channels ) {
        // dn2(el->decoder->samprate, el->decoder->nChans)

        // 等这一帧以前的数据播放完成
        // xEventGroupWaitBits(el->base.stats, STAT_DRAIN, false, true, 100);

        printf("i2snum:%d, samprate:%d, bps:%d, chans:%d \n",
            ((player_t*)el->base.pipe)->playback->i2s
            , el->decoder->samprate
            , 32
            , el->decoder->nChans) ;
        i2s_set_clk(
            ((player_t*)el->base.pipe)->playback->i2s
            , el->decoder->samprate
            , 32
            , el->decoder->nChans
        );


        el->samprate = el->decoder->samprate ;
        el->channels = el->decoder->nChans ;
    }

#ifndef SIMULATION
    // if(pdTRUE != xRingbufferSend(el->base.ring, buff_pcm, el->info.outputSamps * 2, portMAX_DELAY)) {
    if(pdTRUE != xRingbufferSend(el->base.ring, data, size, portMAX_DELAY)) {
        printf("task mp3 decode xRingbufferSend() wrong ?????\n") ;
    }
    else {
        xEventGroupClearBits(el->base.stats, STAT_DRAIN) ;
    }

#else
    // fwrite(buff_pcm,1,el->info.outputSamps * el->info.nChans,el->fout) ;
    fwrite(data,1,size,el->fout) ;
#endif
}

// 解码任务线程
static void task_mp3_decoder(el_mp3_t * el) {

    // printf("task_mp3_decoder()\n") ;

    EventBits_t bits ;
    int res ;

    size_t data_size = 0 ;
    // unsigned char el->undecode_buff[BUFF_SRC_SIZE] ;

    unsigned char * psrc = el->undecode_buff ;
    int decode_left = 0 ;
    int offset = 0 ;
    int errs = 0 ;

    // hexli_set_ringbuf(el->base.ring, xRingbufferSend) ;
    mp3dec_set_output_func(mp3dec_output) ;

    for(int i=0;i<10;) {
        vTaskDelay(0) ;

        // 等待开始状态
        bits = xEventGroupWaitBits(el->base.stats, STAT_RUNNING|STAT_STOPPING, false, false, portMAX_DELAY);
        if( bits&STAT_STOPPING ) {
            el_stop_when_req(el) ;

            psrc = el->undecode_buff ;
            decode_left = 0 ;

            vTaskDelay(1) ;
            continue ;
        }

        if(decode_left && psrc) {
            memmove(el->undecode_buff, psrc, decode_left);
        }

#ifndef SIMULATION
        data_size = 0 ;
        psrc = xRingbufferReceiveUpTo(el->base.upstream->ring, &data_size, 20, BUFF_SRC_SIZE-decode_left);
        if(psrc) {
            memcpy(el->undecode_buff+decode_left, psrc, data_size) ;
            vRingbufferReturnItem(el->base.upstream->ring, psrc) ;   
        }
        if ( data_size==0 || !psrc ) {

            // 确定前级已流干（ring buffer 里的可读数据可能分在头尾两端，需要两次才能读空）
            if(el_is_drain(el->base.upstream)) {
                // printf("decode's input drain\n") ;
                xEventGroupSetBits(el->base.upstream->stats, STAT_DRAIN) ;

                decode_left = 0 ;
                psrc = NULL ;
                
                vTaskDelay(1) ;
            }
            continue ;
        }

#else
        data_size = fread(el->undecode_buff+decode_left, 1, BUFF_SRC_SIZE-decode_left, el->fin) ;
        if(!data_size) {
            break ;
        }
#endif

        decode_left = decode_left + data_size;
        psrc = el->undecode_buff;
        

        offset = MP3FindSyncWord(psrc, decode_left);
        
        // dn3(data_size, decode_left,offset)
        if (offset < 0) {
            decode_left = 0;
            vTaskDelay(0) ;
            continue;

        } else {
            psrc += offset;                         //data start point
            decode_left -= offset;                 //in buffer

            necho_time("MP3Decode", {
            errs = MP3Decode(el->decoder, &psrc, &decode_left, el, 0);
            })

            if (errs < -1) {
                printf("MP3Decode failed ,code is %d, receive: %d, left data: %d, sync: %d \n",errs, data_size, decode_left, offset);

                // 遇到错误，停止解码
                // emit_event(((player_t *)el->base.pipe)->ctx, ((player_t *)el->base.pipe)->jsobj, "error") ;

                ((player_t *)el->base.pipe)->error = errs ;
                ((player_t *)el->base.pipe)->finished = false ;

                el_set_stat( ((player_t *)el->base.pipe)->first, STAT_STOPPING ) ;
                
                // xEventGroupClearBits( el->base.stats, STAT_RUNNING ) ;
                

                vTaskDelay(0) ;
                continue ;
            }

            vTaskDelay(0) ;
        }
    }

    while(1) {}
}



// 任务线程：放音
static void task_pcm_playback(el_i2s_t * el) {

    // printf("task_pcm_playback()\n") ;

    EventBits_t bits ;
    int idle_ticks = 10/ portTICK_PERIOD_MS ;
    int64_t t0 = gettime(), t = 0 ;

    char buff[512] ;
    uint32_t data_size ;
    size_t data_wroten ;
    char * pwrite = NULL ;

    for(int i=0;i<10;) {
        
        vTaskDelay(0) ;

        // 等待开始状态
        bits = xEventGroupWaitBits(el->base.stats, STAT_RUNNING|STAT_STOPPING, false, false, portMAX_DELAY);
        if( bits&STAT_STOPPING ) {
            el_stop_when_req(el) ;

            // i2s_zero_dma_buffer(el->i2s) ;

            vTaskDelay(1) ;
            continue ;
        }


        // dp(el->base.upstream->ring)
        // EL_RECV_UPSTREAM(el, pwrite, buff, sizeof(buff), data_size)

        if(!el->base.upstream || !el->base.upstream->ring) {
            vTaskDelay(10) ;
            continue ;
        }
        data_size = 0 ;
        pwrite = xRingbufferReceiveUpTo(el->base.upstream->ring, &data_size, 10, sizeof(buff));
        if(data_size==0 || !pwrite) {
            
            // 确定前级已流干（ring buffer 里的可读数据可能分在头尾两端，需要两次才能读空）
            if(el_is_drain(el->base.upstream)) {
                // printf("i2s's input drain\n") ;
                xEventGroupSetBits(el->base.upstream->stats, STAT_DRAIN) ;  
                vTaskDelay(1) ;
            }

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
        THROW_EXCEPTION("must be called as a Player method")                \
    }


static JSClassID js_audio_player_class_id ;
static JSValue js_audio_player_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    size_t mem = heap_caps_get_free_size(MALLOC_CAP_DMA) ;
    JSValue jsobj = JS_NewObjectClass(ctx, js_audio_player_class_id) ;
    JS_SetPropertyStr(ctx, jsobj, "_handlers", JS_NewObject(ctx));

    player_t * player = heap_caps_malloc(sizeof(player_t), MALLOC_CAP_DMA) ;
    if(!player) {
        THROW_EXCEPTION("out of memory?")
    }
    memset(player, 0, sizeof(player_t)) ;
    JS_SetOpaque(jsobj, player) ;

    // 初始化
    echo_alloc("task_src", {
        ELEMENT_CREATE(player, el_src_t, player->src, task_src, 1024*3, 5, 0, 512)  // task_src 需要访问 sd spi bus, 和 screen spi 任务在同一cpu核可以避免 spi bus 抢占错误
    })
    echo_alloc("task_mp3_decode", {
        player->decode = el_mp3_create(player) ;
    })
    echo_alloc("task_playback", {
        ELEMENT_CREATE(player, el_i2s_t, player->playback, task_pcm_playback, 1024*3, 5, 1, 0)
    })

    player->last = player->playback ;

    player->ctx = ctx ;
    player->jsobj = jsobj ;

    // 避免 gc 销毁对象
    // 调用 detach() 方法释放这里的引用
    JS_DupValue(ctx, jsobj) ;

    printf("audio.Player DMA alloc: %d, free:%d\n", mem-heap_caps_get_free_size(MALLOC_CAP_DMA), heap_caps_get_free_size(MALLOC_CAP_DMA)) ;
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

    // 清空管道
    pipe_clear(player) ;

    // src -> playback
    pipe_link( player, 2, player->src, player->playback ) ;

    pipe_set_stats(player, STAT_RUNNING) ;

    player->paused = false ;
    player->running = true ;
    player->finished = false ;
    player->error = 0 ;
    emit_event(ctx, player->jsobj, "play", JS_UNDEFINED) ;

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

    // 重置 hexli 状态
    if(xEventGroupGetBits(player->decode->base.stats) & STAT_RUNNING) {
        THROW_EXCEPTION("decoder not close yet")
    }
    el_mp3_reset(player->decode) ;

    // 清空管道
    pipe_clear(player) ;

    pipe_link( player, 3, player->src, player->decode, player->playback ) ;

    player->paused = false ;
    player->running = true ;
    player->finished = false ;
    player->error = 0 ;
    emit_event(ctx, player->jsobj, "play", JS_UNDEFINED) ;

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
    emit_event(ctx, player->jsobj, "pause", JS_UNDEFINED) ;
    return JS_UNDEFINED ;
}
static JSValue js_audio_resume(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    pipe_set_stats(player, STAT_RUNNING) ;
    player->paused = false ;
    emit_event(ctx, player->jsobj, "resume", JS_UNDEFINED) ;
    return JS_UNDEFINED ;
}
static JSValue js_audio_track_info(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_UNDEFINED ;
}
static JSValue js_audio_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)
    if(!player->running) {
        return JS_UNDEFINED ;
    }
    
    pipe_set_stats(player, STAT_RUNNING) ;
    el_set_stat(player->first, STAT_STOPPING) ;
    vTaskDelay(0) ;

    return JS_UNDEFINED ;
    // EventBits_t bits = xEventGroupWaitBits(player->last->stats, STAT_STOPPED, false, false, 1000);
    // if(!(bits&STAT_STOPPED)) {
    //     printf("stop time out: %d\n", bits) ;
    // }

    // return (bits&STAT_STOPPED)? JS_TRUE : JS_FALSE ;
}

static JSValue js_audio_print_stats(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_PLAYER(player)

    printf("\n[task_src]\n") ;
    el_print_stats(player->src) ;
    
    printf("\n[task_mp3_decode]\n") ;
    el_print_stats(player->decode) ;
    
    printf("\n[task_i2s]\n") ;
    el_print_stats(player->playback) ;

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
    JS_CFUNC_DEF("isRunning", 0, js_audio_is_running),
    JS_CFUNC_DEF("isPaused", 0, js_audio_is_paused),
    JS_CFUNC_DEF("detach", 0, js_audio_play_detach),
    JS_CFUNC_DEF("printStats", 0, js_audio_print_stats),
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