#include "telnet_ws.h"

#include "driver_camera.h"
#include "module_wifi.h"
#include "module_lvgl.h"
#include "debug.h"
#include "utils.h"
#include "esp_http_server.h"
#include "display.h"

#define TJPG_WORK_SIZE 3100


typedef struct {

    bool enable ;

    uint32_t read_buf_size ;
    uint32_t readed ;
    uint8_t * read_buf ;
    st77xx_dev_t * dispdev ;

    JDEC jdec ;
    void * jdeca_pool ;

    TaskHandle_t jdec_task ;
    QueueHandle_t jdec_que ;
    SemaphoreHandle_t jdec_working ;
    
    TaskHandle_t disp_task ;
    QueueHandle_t disp_que ;

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
    telws_proj_sess_t *sess = (telws_proj_sess_t*)jd->device;
    if(sess && sess->dispdev) {

        int size = (rect->right - rect->left + 1) * (rect->bottom - rect->top + 1) * 2 ;
        uint8_t * HMALLOC(data, size+8) ;
        *((int16_t*)data) = rect->left ;
        *((int16_t*)(data+2)) = rect->top ;
        *((int16_t*)(data+4)) = rect->right ;
        *((int16_t*)(data+6)) = rect->bottom ;
        memcpy(data+8, bitmap, size) ;

        xQueueSend(sess->disp_que, &data, portMAX_DELAY) ;

        // st77xx_draw_rect(sess->dispdev, rect->left,rect->top,rect->right,rect->bottom, bitmap) ;
        vTaskDelay(0) ;
    }
    return 1 ;
}

uint8_t * jdec_param ; // 不能做为 task_jpeg_decode() 栈上的变量，避免 xTaskDelay() 清理
static void task_jpeg_decode(telws_proj_sess_t * sess) {
    vTaskDelay(1) ;
    int64_t t = 0 ;
    while(1) {
        vTaskDelay(0) ;

        xQueueReceive(sess->jdec_que, &jdec_param, portMAX_DELAY);
        if(!jdec_param) {
            continue ;
        }
        // t = gettime() ;
        xSemaphoreTake( sess->jdec_working, portMAX_DELAY ) ;

        if(!sess->enable) {
            free(jdec_param) ;
            jdec_param = NULL ;
            goto end ;
        }

        sess->readed = 0 ;
        sess->read_buf_size = * (uint32_t*)jdec_param ;
        sess->read_buf = jdec_param + 4 ;

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
end:
        free(jdec_param) ;
        jdec_param = NULL ;
        
        xSemaphoreGive( sess->jdec_working );

        // printf("%lldms\n", gettime()-t) ;
    }
}

uint8_t * buff = NULL ;
static void task_disp(telws_proj_sess_t * sess) {

    int16_t left = 0 ;
    int16_t top = 0 ;
    int16_t right = 0 ;
    int16_t bottom = 0 ;
    while(1) {
        vTaskDelay(0) ;
        xQueueReceive(sess->disp_que, &buff, portMAX_DELAY);
        if(!buff) {
            continue ;
        }
        // dp(buff)
        // dp(sess)

        left = * ((int16_t*)buff) ;
        top = * ((int16_t*)(buff+2)) ;
        right = * ((int16_t*)(buff+4)) ;
        bottom = * ((int16_t*)(buff+6)) ;

        // dn4(left,top,right,bottom)

        st77xx_draw_rect(sess->dispdev, left,top,right,bottom, buff+8) ;

        free(buff) ;
        buff = NULL ;

    }

}

bool telnet_ws_projection_sessn_alloc(struct mg_connection *conn) {

    disp_drv_spec_t * dispdrv = default_disp_drv_spec() ;
    if(!dispdrv || !dispdrv->spi_dev) {
        printf("no disp to project\n");
        return false ;
    }

    if(!sess) {
        HMALLOC(sess, sizeof(telws_proj_sess_t)) ;
        memset(sess, 0, sizeof(telws_proj_sess_t)) ;
        sess->jdec.device = sess ;

        // int coreId = xPortGetCoreID()==1? 0: 1 ;
        // core 0
        if (xTaskCreatePinnedToCore(task_jpeg_decode, "task_jpeg_decode", 1*1024, sess, 5, sess->jdec_task, 0) != pdPASS) {
            printf("create task for tjpeg failed\n") ;
            goto fail ;
        }
        
        sess->jdec_que = xQueueCreate(2, sizeof(uint8_t *));
        if(!sess->jdec_que) {
            printf("create queue for tjpeg failed\n") ;
            goto fail ;
        }
        
        sess->jdec_working = xSemaphoreCreateMutex() ;

        // core 1
        if (xTaskCreatePinnedToCore(task_disp, "task_disp", 1.5*1024, sess, 5, sess->disp_task, 1) != pdPASS) {
            printf("create task for display failed\n") ;
            goto fail ;
        }
        
        sess->disp_que = xQueueCreate(2, sizeof(uint8_t *));
        if(!sess->disp_que) {
            printf("create queue for display failed\n") ;
            goto fail ;
        }

    }
    
    if(!sess->jdeca_pool) {
        HMALLOC(sess->jdeca_pool, TJPG_WORK_SIZE)
        if(!sess->jdeca_pool) {
            printf("out of memory?\n") ;
            goto fail ;
        }
    }

    sess->enable = true ;
    sess->dispdev = dispdrv->spi_dev ;

    be_lv_pause() ;

    return true ;

fail:
    telnet_ws_projection_sess_free(sess) ;
    return false;
}

void telnet_ws_projection_sess_free() {

    xSemaphoreTake( sess->jdec_working, portMAX_DELAY ) ;

    if(sess->jdeca_pool) {
        free(sess->jdeca_pool) ;
        sess->jdeca_pool = NULL ;
    }

    sess->enable = false ;

    // 恢复 ui
    be_lv_resume() ;

    xSemaphoreGive( sess->jdec_working ) ;
}

void telnet_ws_response_projection(struct mg_connection *conn, struct mg_ws_message * wm) {

    if(!sess || !sess->jdeca_pool || !sess->enable) {
        return ;
    }
    
    uint8_t * HMALLOC( data, wm->data.len + 4 ) ;
    memcpy(data+4, wm->data.ptr, wm->data.len) ;

    *((uint32_t *)data) = wm->data.len ;

    xQueueSend(sess->jdec_que, &data, portMAX_DELAY);

    mg_ws_send(conn, "ok", sizeof("ok"), WEBSOCKET_OP_TEXT);
    
    vTaskDelay(0) ;
}

