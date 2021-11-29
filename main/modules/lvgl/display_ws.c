#include "display_ws.h"
#include "utils.h"
#include "list.h"
#include "webtelnet.h"

#include <esp_log.h>

#define TAG "disp.ws"

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

/*
 * async send function, which we put into the httpd work queue
 */
static void ws_async_send(void *arg) {
    static const char * data = "Async data";
    client_t *resp_arg = arg;
    httpd_handle_t hd = resp_arg->hd;
    int fd = resp_arg->fd;
    httpd_ws_frame_t ws_pkt;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.payload = (uint8_t*)data;
    ws_pkt.len = strlen(data);
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;

    httpd_ws_send_frame_async(hd, fd, &ws_pkt);
    free(resp_arg);
}

// static esp_err_t trigger_async_send(httpd_handle_t handle, client_t * client) {
// }


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





/*
 * This handler echos back the received ws data
 * and triggers an async send if certain message received
 */
static esp_err_t disp_client_handler(httpd_req_t *req) {    

    printf("req->sess_ctx==NULL? %d\n", req->sess_ctx==NULL? 1: 0) ;
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
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;
    esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 128);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "httpd_ws_recv_frame failed with %d", ret);
        return ret;
    }

    ret = httpd_ws_send_frame(req, &ws_pkt);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "httpd_ws_send_frame failed with %d", ret);
    }
    return ret;
}





void vlgl_js_display_ws_init() {
    
    list_init(&disp_clients) ;

    httpd_handle_t server = webtelnet_handle() ;
    if(server) {
        httpd_uri_t ws = {
            .uri        = "/display/dev",
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