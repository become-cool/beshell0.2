#include "display_ws.h"
#include "display.h"
#include "utils.h"
#include "list.h"
#include "webtelnet.h"
#include "lv_conf.h"

#include <esp_log.h>

#define TAG "disp.ws"

#include "esp_camera.h"
#include "img_converters.h"

static httpd_handle_t server = NULL;

list_t disp_clients ;

/*
 * Structure holding server handle
 * and internal socket fd in order
 * to use out of request send
 */
typedef struct {
    httpd_handle_t hd;
    int fd;
} client_t ;

typedef struct {
    uint8_t * buff ;
    uint16_t len ;
} jpg_frm_t ;



lv_indev_drv_t ws_indev_drv ;
uint16_t ws_input_x = 0 ;
uint16_t ws_input_y = 0 ;
bool ws_input_pressed = false ;
bool ws_input_update = false ;

bool ws_driver_input_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
    if(!ws_input_update) {
        return false ;
    }
    if(ws_input_pressed) {
        data->state = LV_INDEV_STATE_PRESSED ;
        data->point.x = ws_input_x ;
        data->point.y = ws_input_y ;
    }
    else {
        data->state = LV_INDEV_STATE_RELEASED ;
    }
    ws_input_update = false ;
    return true ;
}



void free_socket_ctx(void * ctx) {
    printf("xxxxxxxx free_socket_ctx()\n") ;
    if(!ctx) {
        printf("free_socket_ctx() ctx == NULL !") ;
        return ;
    }
    list_remove(&disp_clients, ctx) ;
    free(ctx) ;

    list_is_empty(&disp_clients) ;
    printf("client cnt:%d \n", list_count(&disp_clients)) ;
}


static size_t jpg_encode_stream(void * arg, size_t index, const void* data, size_t len) {

    jpg_frm_t* jpgfrm = (jpg_frm_t*) arg ;

    if(len>0) {
        memcpy(jpgfrm->buff+index, data, len) ;
        jpgfrm->len+= len ;
    }
    return len ;
}

void disp_virtual_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) {
    size_t size = (area->x2-area->x1+1) * (area->y2-area->y1+1) * sizeof(lv_color_t) ;

    if(list_is_empty(&disp_clients)) {
        lv_disp_flush_ready(disp) ;
        return ;
    }

    uint8_t * buff = malloc(size+8) ;
    if(!buff) {
        printf("malloc(%d) failed, out of memory?\n", size+8) ;
        lv_disp_flush_ready(disp) ;
        return ;
    }

    uint16_t * coord = (uint16_t *)buff ;
    (*coord) = area->x1 ;
    coord++ ;
    (*coord) = area->y1 ;
    coord++ ;
    (*coord) = area->x2 ;
    coord++ ;
    (*coord) = area->y2 ;

    jpg_frm_t jpgfrm ;
    jpgfrm.buff = buff+8 ;
    jpgfrm.len = 0 ;

    if( fmt2jpg_cb((uint8_t *)color_p, size, area->x2-area->x1+1, area->y2-area->y1+1, PIXFORMAT_RGB565, 100, jpg_encode_stream, &jpgfrm) ) {

        httpd_ws_frame_t ws_pkt;
        memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
        ws_pkt.payload = buff;
        ws_pkt.len = jpgfrm.len+8;
        ws_pkt.type = HTTPD_WS_TYPE_BINARY;

        FOREACH_LIST(&disp_clients, item) {
            httpd_ws_send_frame_async(((client_t *)item->data)->hd, ((client_t *)item->data)->fd, &ws_pkt) ;
        }
    }

    lv_disp_flush_ready(disp) ;
    free (buff) ;
}


/*
 * This handler echos back the received ws data
 * and triggers an async send if certain message received
 */
static esp_err_t disp_client_handler(httpd_req_t *req) {    

    if(!req->sess_ctx) {

        client_t * client = malloc(sizeof(client_t));
        client->hd = req->handle;
        client->fd = httpd_req_to_sockfd(req);
        list_append(&disp_clients, client) ;

        req->sess_ctx = (void *) client ;
        req->free_ctx = free_socket_ctx ;
    }

    uint8_t buf[128] = { 0 };
    httpd_ws_frame_t ws_pkt;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.payload = buf;
    ws_pkt.type = HTTPD_WS_TYPE_BINARY;
    esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 128);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "httpd_ws_recv_frame failed with %d", ret);
        return ret;
    }

    if( ws_pkt.type!=HTTPD_WS_TYPE_BINARY ) {
        return ret ;
    }

    // printf("ws_pkt.len=%d, cmd:%d\n", ws_pkt.len, ws_pkt.payload[0]) ;
    // printf("ws_pkt.payload=%s\n", (char*)ws_pkt.payload) ;

    
    if( ws_pkt.payload[0]==WS_DISP_CMD_REFRESH && ws_pkt.len==1 ){
        lv_disp_t * disp = lv_disp_get_default();
        if(disp) {
            lv_area_t area ;
            memset(&area, 0, sizeof(lv_area_t));
            area.x2 = disp->driver->hor_res-1 ;
            area.y2 = disp->driver->ver_res-1 ;
            
            // printf("_lv_inv_area() (%d,%d)->(%d,%d)", area.x1,area.y1,area.x2,area.y2) ;
            _lv_inv_area(disp, &area) ;
        }
        else {
            printf("there is no default display\n") ;
        }
    }
    else if( ws_pkt.payload[0]==WS_DISP_CMD_RELEASE && ws_pkt.len==1 ){
        ws_input_pressed = false ;
        ws_input_update = true ;
        // printf("ws input release\n") ;
    }
    else if( ws_pkt.payload[0]==WS_DISP_CMD_PRESS && ws_pkt.len==5 ){
        ws_input_pressed = true ;
        uint16_t * data = (uint16_t *)(ws_pkt.payload+1) ;
        ws_input_x = *data ;
        ws_input_y = *(data+1) ;
        ws_input_update = true ;

        // printf("ws input press  %d, %d\n",ws_input_x,ws_input_y) ;
    }
    
    return ESP_OK;
}


void vlgl_js_display_ws_init() {

    // lv_indev_drv_init(&ws_indev_drv);
    // ws_indev_drv.read_cb = touch_driver_read;
    // ws_indev_drv.type = LV_INDEV_TYPE_POINTER;
    // lv_indev_t *indev = lv_indev_drv_register(&ws_indev_drv);
    // if(!indev) {
    //     printf("Cound create virtual indev\n") ;
    // }



    
    list_init(&disp_clients) ;

    httpd_handle_t server = webtelnet_handle() ;
    if(server) {
        httpd_uri_t ws = {
            .uri        = "/display",
            .method     = HTTP_GET,
            .handler    = disp_client_handler,
            .user_ctx   = NULL,
            .is_websocket = true
        };
        if( httpd_register_uri_handler(server, &ws)!=ESP_OK ){
            printf("cound not register_uri\n") ;
            return ;
        }
    }
}