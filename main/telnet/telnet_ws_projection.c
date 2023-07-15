#include "telnet_ws.h"

#include "driver_camera.h"
#include "module_wifi.h"
#include "module_lvgl.h"
#include "debug.h"
#include "utils.h"
#include "esp_http_server.h"
#include "display.h"
#include "module_serial_i2s.h"
#include "driver/i2s.h"
#include "freertos/ringbuf.h"

#define TJPG_WORK_SIZE 3100

#define WS_PROJ_VIDEO_FRAME   1     // cmd[8], x[16], y[16], len[24], ... data
#define WS_PROJ_AUDIO_INIT  2       // cmd[8], sampleRate[16], bit[8], channels[8]
#define WS_PROJ_AUDIO_FRAME  3      // cmd[8], ... data


static uint8_t i2s_num = 255 ;      // 255 表示无效


typedef struct {
    uint16_t x1 ;
    uint16_t y1 ;
    uint16_t x2 ;
    uint16_t y2 ;
    uint8_t * raw ;
} frame_video_block_t ;


typedef struct {
    uint32_t len ;
    uint8_t * ptr ;
} frame_audio_t ;

typedef struct {

    bool enable ;

    // 解码任务
    TaskHandle_t task_jdec ;
    QueueHandle_t jdec_que ;
    SemaphoreHandle_t jdec_working ;

    uint32_t read_buf_size ;
    uint32_t readed ;
    uint8_t * read_buf ;
    JDEC jdec ;
    void * jdeca_pool ;

    frame_video_rect_t * frame_video ;
    
    // 显示任务
    TaskHandle_t task_disp ;
    QueueHandle_t disp_que ;

    st77xx_dev_t * dispdev ;

    // 放音任务
    TaskHandle_t task_audio ;
    RingbufHandle_t audio_ring ;
    QueueHandle_t audio_que ;
    SemaphoreHandle_t audio_data_semaphore ;


} telws_proj_sess_t ;


telws_proj_sess_t * sess = NULL ;

static uint32_t tjpgd_reader (JDEC* jd, uint8_t * buff, uint32_t nbyte) {

    telws_proj_sess_t *sess = (telws_proj_sess_t*)jd->device;

    // printf("%d, read_buff=%p, buff=%p, sess=%p\n",nbyte, sess->read_buf, buff,sess) ;

    uint32_t rb ;

    if( sess->readed+nbyte <= sess->read_buf_size) {
        rb = nbyte ;
    }
    else {
        rb = sess->read_buf_size - sess->readed;
    }

    if (buff) {
        memcpy(buff, sess->read_buf+sess->readed, rb) ;
        sess->readed+= rb ;
        return rb ;
    }

    else {
        sess->readed+= rb ;
        return nbyte ;
    }
}

// typedef struct {
//     int16_t x1 ;
//     int16_t y1 ;
//     int16_t x2 ;
//     int16_t y2 ;
//     uint8_t * bitmap ;
// } trans_buff_t ;

static uint32_t tjpgd_writer (JDEC* jd, void* bitmap, JRECT* rect) {
    frame_video_block_t block ;
    telws_proj_sess_t *sess = (telws_proj_sess_t*)jd->device;
    if(sess && sess->dispdev) {

        int size = (rect->right - rect->left + 1) * (rect->bottom - rect->top + 1) * 2 ;
        // uint8_t * HMALLOC(data, size+8) ;
        // *((int16_t*)data) = rect->left + sess->frame_video->x ;
        // *((int16_t*)(data+2)) = rect->top + sess->frame_video->y ;
        // *((int16_t*)(data+4)) = rect->right + sess->frame_video->x ;
        // *((int16_t*)(data+6)) = rect->bottom + sess->frame_video->y ;
        // memcpy(data+8, bitmap, size) ;

        HMALLOC(block.raw, size) ;
        memcpy(block.raw, bitmap, size) ;
        block.x1 = rect->left + sess->frame_video->x ;
        block.y1 = rect->top + sess->frame_video->y ;
        block.x2 = rect->right + sess->frame_video->x ;
        block.y2 = rect->bottom + sess->frame_video->y ;

        xQueueSend(sess->disp_que, &block, portMAX_DELAY) ;

        vTaskDelay(0) ;
    }
    return 1 ;
}


frame_video_rect_t jdec_frame ;
static void task_jpeg_decode(telws_proj_sess_t * sess) {
    vTaskDelay(1) ;
    // int64_t t = 0 ;
    sess->frame_video = &jdec_frame ;
    while(1) {
        vTaskDelay(0) ;

        xQueueReceive(sess->jdec_que, &jdec_frame, portMAX_DELAY);
        if(!jdec_frame.rawlen) {
            continue ;
        }
        // t = gettime() ;
        xSemaphoreTake( sess->jdec_working, portMAX_DELAY ) ;

        if(sess->enable) {
            sess->readed = 0 ;
            sess->read_buf = jdec_frame.raw ;
            sess->read_buf_size = jdec_frame.rawlen ;

            JRESULT res = jd_prepare(&(sess->jdec), tjpgd_reader, sess->jdeca_pool, TJPG_WORK_SIZE, sess);

            if(res != JDR_OK) {
                printf("jd_prepare() failed: %d\n", res);
            }

            else {
                res = jd_decomp(&(sess->jdec), tjpgd_writer, 0);
                if (res != JDR_OK) {
                    printf("jd_decomp() failed: %d\n", res);
                }
            }
        }

        if(jdec_frame.raw) {
            // printf("free() jpgraw:%p\n",jdec_frame.raw) ;
            free(jdec_frame.raw) ;
            jdec_frame.raw = NULL ;
            jdec_frame.rawlen = 0 ;
        }
        
        xSemaphoreGive( sess->jdec_working );

        // printf("%lldms\n", gettime()-t) ;
    }
}

// uint8_t * buff = NULL ;
static void task_disp(telws_proj_sess_t * sess) {
    frame_video_block_t block ;
    int16_t left = 0 ;
    int16_t top = 0 ;
    int16_t right = 0 ;
    int16_t bottom = 0 ;
    while(1) {
        vTaskDelay(0) ;
        xQueueReceive(sess->disp_que, &block, portMAX_DELAY);
        if(!block.raw) {
            continue ;
        }

        st77xx_draw_rect(sess->dispdev, block.x1,block.y1,block.x2,block.y2, block.raw) ;

        free(block.raw) ;
        block.raw = NULL ;
    }
}

#define BUFF_ROWS 30
static void telnet_ws_projection_clear() {
    lv_disp_t * disp = lv_disp_get_default() ;
    if(!disp || !disp->driver || !disp->driver->user_data ) {
        return ;
    }
    disp_drv_spec_t * drvspec = (disp_drv_spec_t*) disp->driver->user_data ;
    if(!drvspec->spi_dev) {
        return ;
    }
    if(drvspec->is_virtual){
        return ;
    }
    lv_coord_t w = lv_disp_get_hor_res(disp) ;
    lv_coord_t h = lv_disp_get_ver_res(disp) ;

    uint8_t * HMALLOC(buff, w*BUFF_ROWS*2) ;
    memset(buff,0,w*BUFF_ROWS*2) ;
    
    int64_t t = gettime() ;

    uint16_t y1 = 0 ;
    uint16_t y2 = 0 ;
    while(1) {
        y2 = y1+BUFF_ROWS-1 ;
        if(y2>=h) {
            y2 = h-1;
        }
        st77xx_draw_rect(drvspec->spi_dev, 0,y1,w-1,y2, buff) ;
        if(y2==h-1) {
            break;
        }
        y1 = y2+1 ;
    }

    // printf("clear screen:%lld ms\n", gettime()-t) ;

    free(buff) ;
}

#define I2S_BUFF_SIZE 512
static void task_audio(telws_proj_sess_t * sess) {

    size_t data_size = 0 ;
    size_t data_wroten = 0 ;
    char * data = NULL ;
    char buff[I2S_BUFF_SIZE] ;
    esp_err_t err ;

    // frame_audio_t frame ;
    // frame.ptr = NULL ;
    // frame.len = 0 ;
    
    while(1) {
        if(i2s_num==255 || !sess->enable) {
            vTaskDelay(1) ;            
            continue;
        }

        vTaskDelay(1) ;

        data = xRingbufferReceiveUpTo(sess->audio_ring, &data_size, 20, I2S_BUFF_SIZE);
        if(data_size==0 || !data) {
            i2s_zero_dma_buffer(i2s_num) ;
            continue ;
        }

        memcpy(buff, data, data_size) ;
        vRingbufferReturnItem(sess->audio_ring, data) ;

        data = buff ;
        
        while(data_size) {
            err = i2s_write(i2s_num, data, data_size, &data_wroten, portMAX_DELAY);
            // 扩展到 32 sample (标准I2S)
            // err = i2s_write_expand(i2s_num, data, data_size, 16, 32, &data_wroten, portMAX_DELAY ) ;
            if(err!=ESP_OK) {
                printf("i2s_write_expand() failed: %s(%d)\n", esp_err_to_name(err), err);
            }
            // dn(data_wroten)
            
            data_size-= data_wroten ;
            data+= data_wroten ;
        }


        // xQueueReceive(sess->audio_que, &frame, portMAX_DELAY);
        // if(i2s_num==255) {
        //     vTaskDelay(1) ;
        //     continue ;
        // }
        // if(frame.len==0 || !frame.ptr) {
        //     i2s_zero_dma_buffer(i2s_num) ;
        //     continue ;
        // }

        // if(frame.len) {
        //     err = i2s_write(i2s_num, frame.ptr, frame.len, &data_wroten, portMAX_DELAY);
        //     // 扩展到 32 sample (标准I2S)
        //     // err = i2s_write_expand(i2s_num, data, data_size, 16, 32, &data_wroten, portMAX_DELAY ) ;
        //     if(err!=ESP_OK) {
        //         printf("i2s_write_expand() failed: %s(%d)\n", esp_err_to_name(err), err);
        //     }

        //     if(frame.ptr) {
        //         free(frame.ptr) ;
        //         frame.ptr = NULL ;
        //         frame.len = 0 ;
        //     }
        // }
    }
}

bool telnet_ws_projection_sessn_init() {

    printf("telnet_ws_projection_sessn_init\n") ;

    telnet_ws_projection_clear() ;

    disp_drv_spec_t * drvspec = default_disp_drv_spec() ;
    if(!drvspec || !drvspec->spi_dev) {
        printf("no disp to project\n");
        return false ;
    }
    if(drvspec->is_virtual){
        printf("default disp is virtual\n");
        return false ;
    }

    if(!sess) {
        // printf("malloc proj sess\n") ;
        HMALLOC(sess, sizeof(telws_proj_sess_t)) ;
        memset(sess, 0, sizeof(telws_proj_sess_t)) ;
        sess->jdec.device = sess ;
    }

    if(!sess->jdeca_pool) {
        // printf("create jdeca_pool\n") ;
        HMALLOC(sess->jdeca_pool, TJPG_WORK_SIZE)
        if(!sess->jdeca_pool) {
            printf("out of memory?\n") ;
            goto fail ;
        }
    }

    if(!sess->jdec_que) {
        // printf("create jdec_que\n") ;
        sess->jdec_que = xQueueCreate(5, sizeof(frame_video_rect_t));
        if(!sess->jdec_que) {
            printf("create queue for tjpeg failed\n") ;
            goto fail ;
        }
    }
    
    if(!sess->jdec_working) {
        // printf("create jdec_working\n") ;
        sess->jdec_working = xSemaphoreCreateMutex() ;
    }
    
    if(!sess->disp_que) {
        // printf("create disp_que\n") ;
        sess->disp_que = xQueueCreate(5, sizeof(frame_video_block_t));
        if(!sess->disp_que) {
            printf("create queue for display failed\n") ;
            goto fail ;
        }
    }
    
    if(!sess->audio_ring) {
        // printf("create audio_ring\n") ;
        sess->audio_ring = xRingbufferCreate(1024*10, RINGBUF_TYPE_BYTEBUF);
        if(!sess->audio_ring) {
            goto fail ;
        }
    }
    if(!sess->audio_que) {
        // printf("create audio_que\n") ;
        sess->audio_que = xQueueCreate(5, sizeof(frame_audio_t));
        if(!sess->audio_que) {
            printf("create queue for audio failed\n") ;
            goto fail ;
        }
    }

    if(!sess->audio_data_semaphore) {
        // printf("create audio_data_semaphore\n") ;
        sess->audio_data_semaphore = xSemaphoreCreateBinary() ;
    }
    
    if(!sess->task_jdec) {
        // printf("create task_jpeg_decode\n") ;
        if (xTaskCreatePinnedToCore(task_jpeg_decode, "task_jpeg_decode", 3*1024, sess, 5, &sess->task_jdec, 1) != pdPASS) {
            printf("create task for tjpeg failed\n") ;
            goto fail ;
        }
    }
    if(!sess->task_disp) {
        // printf("create task_disp\n") ;
        if (xTaskCreatePinnedToCore(task_disp, "task_disp", 2*1024, sess, 5, &sess->task_disp, 0) != pdPASS) {
            printf("create task for display failed\n") ;
            goto fail ;
        }
    }
    if(!sess->task_audio) {
        // printf("create task_audio\n") ;
        if (xTaskCreatePinnedToCore(task_audio, "task_audio", 3*1024, sess, 5, &sess->task_audio, 0) != pdPASS) {
            printf("create task for display failed\n") ;
            goto fail ;
        }
    }

    sess->enable = true ;
    sess->dispdev = drvspec->spi_dev ;

    be_lv_pause() ;

    return true ;

fail:
    telnet_ws_projection_sess_release(sess) ;
    return false;
}

#define DELETE_TASK(task)       \
    if(task) {                  \
        vTaskDelete(task) ;     \
        task = NULL ;           \
    }
#define DELETE_QUEUE(que)       \
    if(que) {                   \
        vQueueDelete(que) ;     \
        que = NULL ;            \
    }
#define DELETE_RING(ring)           \
    if(ring) {                      \
        vRingbufferDelete(ring) ;   \
        ring = NULL ;               \
    }

#define DELETE_SEMAPHORE(semaphore)     \
    if(semaphore) {                     \
        vSemaphoreDelete(semaphore) ;   \
        semaphore = NULL ;              \
    }

void telnet_ws_projection_sess_release() {
    if(!sess) {
        return ;
    }

    printf("telnet_ws_projection_sess_release()\n") ;

    if(sess->jdec_working) {
        xSemaphoreTake( sess->jdec_working, portMAX_DELAY ) ;
    }

    if(sess->jdeca_pool) {
        free(sess->jdeca_pool) ;
        sess->jdeca_pool = NULL ;
    }

    DELETE_TASK(sess->task_jdec)
    DELETE_TASK(sess->task_disp)
    DELETE_TASK(sess->task_audio)

    DELETE_QUEUE(sess->jdec_que)
    DELETE_QUEUE(sess->disp_que)

    DELETE_RING(sess->audio_ring)
    DELETE_QUEUE(sess->audio_que)
    DELETE_SEMAPHORE(sess->audio_data_semaphore) 


    if(i2s_num!=255) {
        i2s_zero_dma_buffer(i2s_num) ;
    }

    sess->enable = false ;

    // 恢复 ui
    be_lv_resume() ;

    if(sess->jdec_working) {
        xSemaphoreGive( sess->jdec_working ) ;
    }
}

inline void post_jpg_rect(uint16_t x, uint16_t y, void * jpgraw, uint32_t jpgraw_len) {
    frame_video_rect_t rect ;
    memset(&rect, 0, sizeof(frame_video_rect_t));

    rect.x = x ;
    rect.y = y ;
    rect.raw = jpgraw ;
    rect.rawlen = jpgraw_len ;

    xQueueSend(sess->jdec_que, &rect, portMAX_DELAY);
}

#define WRAP_QUOTE(string) "\"" string "\""
#define PAIR(key,value)  WRAP_QUOTE(key) ":" WRAP_QUOTE(value)
#define RSPN_JSON(media, act, msg) "{" PAIR("media",media) "," PAIR("act",act) "," PAIR("msg",msg) "}"
#define RSPN_STRING(conn, media, act, msg) \
    mg_ws_send(conn, RSPN_JSON(media, act, msg), sizeof(RSPN_JSON(media, act, msg))-1, WEBSOCKET_OP_TEXT);

inline static void rspn_video_frame(struct mg_connection *conn, struct mg_ws_message * wm) {

    if( wm->data.len<8 ) {
        RSPN_STRING(conn, "video", "frame", "invalid length")
        return ;
    }

    frame_video_rect_t rect ;
    memset(&rect, 0, sizeof(frame_video_rect_t));

    rect.x = (wm->data.ptr[1] << 8) | (wm->data.ptr[2]) ;
    rect.y = (wm->data.ptr[3] << 8) | (wm->data.ptr[4]) ;
    rect.rawlen =  (wm->data.ptr[5] << 16) | (wm->data.ptr[6] << 8) | wm->data.ptr[7] ;

    if(rect.rawlen+8 < rect.rawlen) {
        RSPN_STRING(conn, "video", "frame", "invalid length")
        return ;
    }

    // dn3( rect.x, rect.y, rect.rawlen )
    // print_block()

    HMALLOC( rect.raw, rect.rawlen ) ;
    memcpy(rect.raw, wm->data.ptr+8, rect.rawlen) ;

    xQueueSend(sess->jdec_que, &rect, portMAX_DELAY);

    RSPN_STRING(conn, "video", "frame", "ok")

}

inline static void rspn_audio_init(struct mg_connection *conn, struct mg_ws_message * wm) {
    if( wm->data.len<5 ) {
        RSPN_STRING(conn, "audio", "init", "invalid length")
        return ;
    }

    if(i2s_has_setup(0)) {
        i2s_num = 0 ;
    } else if(i2s_has_setup(1)) {
        i2s_num = 1 ;
    } else {
        i2s_num = 255 ;
        RSPN_STRING(conn, "audio", "init", "i2s not setup")
        return ;
    }

    uint16_t sample_rate = (wm->data.ptr[1] << 8) | wm->data.ptr[2] ;
    uint8_t bit = wm->data.ptr[3] ;
    uint8_t channels = wm->data.ptr[4] ;

    dn4(i2s_num, sample_rate,bit,channels)

    if(i2s_set_clk(i2s_num, sample_rate, bit, channels)!=ESP_OK) {
        RSPN_STRING(conn, "audio", "init", "failed")
        return ;
    }

    RSPN_STRING(conn, "audio", "init", "ok")
}

inline static void rspn_audio_frame(struct mg_connection *conn, struct mg_ws_message * wm) {

    if(i2s_num==255) {
        RSPN_STRING(conn, "audio", "frame", "i2s invalid")
        return ;
    }

    if(!sess->audio_que) {
        RSPN_STRING(conn, "audio", "frame", "unknow error")
        return ;
    }

    // frame_audio_t frame ;
    // frame.len = wm->data.len-1 ;
    // HMALLOC(frame.ptr, frame.len) ;
    // memcpy(frame.ptr, wm->data.ptr+1, frame.len) ;
    // dn(frame.len)
    // xQueueSend(sess->audio_que, &frame, portMAX_DELAY);


    if(pdTRUE != xRingbufferSend(sess->audio_ring, wm->data.ptr+1, wm->data.len-1, 100)) {
        RSPN_STRING(conn, "audio", "frame", "timeout")
        return ;
    }

    // 通知数据装入
    // xSemaphoreGive( sess->audio_data_semaphore);

    RSPN_STRING(conn, "audio", "frame", "ok")

}

void telnet_ws_response_projection(struct mg_connection *conn, struct mg_ws_message * wm) {

    if(!sess || !sess->jdeca_pool || !sess->enable) {
        return ;
    }

    // 视频帧
    if(wm->data.ptr[0]==WS_PROJ_VIDEO_FRAME) {
        rspn_video_frame(conn,wm) ;
    }
    // 音频初始化
    else if(wm->data.ptr[0]==WS_PROJ_AUDIO_INIT) {
        rspn_audio_init(conn,wm) ;
    }
    // 音频帧
    else if(wm->data.ptr[0]==WS_PROJ_AUDIO_FRAME) {
        rspn_audio_frame(conn,wm) ;
    }
    else {
        RSPN_STRING(conn, "unknow","unknow","invalid command")
    }
        
    vTaskDelay(0) ;
}

