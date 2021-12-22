#include "http_lws.h"
#include "display.h"
#include "utils.h"
#include "mongoose.h"
#include "stack.h"
#include "module_mg.h"
#include "module_lvgl.h"


#define WS_DISP_CMD_REFRESH 1
#define WS_DISP_CMD_DRAW 2
#define WS_DISP_CMD_PRESS 22
#define WS_DISP_CMD_RELEASE 23

#define WS_DISP_BUFF_RAW 1
#define WS_DISP_BUFF_JPEG 2


// uint16_t ws_input_x = 0 ;
// uint16_t ws_input_y = 0 ;
// bool ws_input_pressed = false ;
// bool ws_input_update = false ;

void ws_driver_input_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
    // if(!ws_input_update) {
    //     return ;
    // }
    if(indev_input_pressed) {
        data->state = LV_INDEV_STATE_PRESSED ;
        data->point.x = indev_input_x ;
        data->point.y = indev_input_y ;
    }
    else {
        data->state = LV_INDEV_STATE_RELEASED ;
    }
    // ws_input_update = false ;
}


typedef struct
{
    stack_base_t stack_item;
    struct mg_connection *conn;
    void *jsobj;

} ws_disp_client_t;

ws_disp_client_t *lst_clients_search_by_conn(ws_disp_client_t *lst, struct mg_connection *conn) {
    for (ws_disp_client_t *item = lst; item != NULL; item = (ws_disp_client_t *)((stack_base_t *)item)->_next) {
        if (item->conn == conn)
            return item;
    }
    return NULL;
}

ws_disp_client_t *lst_clients = NULL;

static const char *s_listen_on = "ws://0.0.0.0:8022";
static const char *s_web_root = ".";


void ws_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {

    if(stack_count(lst_clients)==0) {
        lv_disp_flush_ready(disp) ;
        return ;
    }

    size_t size = (area->x2-area->x1+1) * (area->y2-area->y1+1) * sizeof(lv_color_t) ;

    uint8_t * buff = ((uint8_t *)color_p) - 10 ;
    buff[0] = WS_DISP_CMD_DRAW ;
    buff[1] = WS_DISP_BUFF_RAW ;
    uint16_t * coord = (uint16_t *)(buff + 2);
    (*coord) = area->x1 ;
    coord++ ;
    (*coord) = area->y1 ;
    coord++ ;
    (*coord) = area->x2 ;
    coord++ ;
    (*coord) = area->y2 ;

    size+= 8 ;
    STACK_FOREACH(lst_clients, client, ws_disp_client_t) {
        mg_ws_send(client->conn, buff, size, WEBSOCKET_OP_BINARY);
    }

    lv_disp_flush_ready(disp) ;
}

// This RESTful server implements the following endpoints:
//   /websocket - upgrade to Websocket, and implement websocket echo server
//   /api/rest - respond with JSON string {"result": 123}
//   any other URI serves static files from s_web_root
static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG)
    {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        if (mg_http_match_uri(hm, "/display"))
        {
            mg_ws_upgrade(c, hm, NULL);

            if (lst_clients_search_by_conn(lst_clients, c) == NULL)
            {
                ws_disp_client_t *client = malloc(sizeof(ws_disp_client_t));
                memset(client, 0, sizeof(ws_disp_client_t));
                client->conn = c;
                stack_unshift(&lst_clients, client);

                printf("new ws connect, lst_clients count: %d\n", stack_count(lst_clients));
            }

            else
            {
                printf("connect exists\n");
            }
        }
        else
        {
            // Serve static files
            struct mg_http_serve_opts opts = {.root_dir = s_web_root};
            mg_http_serve_dir(c, ev_data, &opts);
        }
    }
    else if (ev == MG_EV_WS_MSG)
    {

        struct mg_ws_message * wm = (struct mg_ws_message *) ev_data;
        // printf("%d(%d)\n", wm->data.ptr[0], wm->data.len) ;

        if (wm->data.ptr[0] == WS_DISP_CMD_REFRESH && wm->data.len == 1) {

            // printf("WS_DISP_CMD_REFRESH\n") ;
            
            lv_disp_t *disp = lv_disp_get_default();
            if (disp)
            {
                lv_area_t area;
                memset(&area, 0, sizeof(lv_area_t));
                area.x2 = disp->driver->hor_res - 1;
                area.y2 = disp->driver->ver_res - 1;

                // printf("_lv_inv_area() (%d,%d)->(%d,%d)", area.x1,area.y1,area.x2,area.y2) ;
                _lv_inv_area(disp, &area);
            }
            else
            {
                printf("there is no default display\n");
            }
        }

        else if (wm->data.ptr[0] == WS_DISP_CMD_RELEASE && wm->data.len == 1)
        {
            // ws_input_pressed = false;
            // ws_input_update = true;

            indev_input_pressed = false ;
        }
        else if (wm->data.ptr[0] == WS_DISP_CMD_PRESS && wm->data.len == 5)
        {
            // ws_input_pressed = true;
            uint16_t *data = (uint16_t *)(wm->data.ptr + 1);
            // ws_input_x = *data;
            // ws_input_y = *(data + 1);

            
            indev_input_x = *data ;
            indev_input_y = *(data + 1);
            indev_input_pressed = true ;

            // ws_input_update = true;

            // printf("ws input press  %d, %d\n",ws_input_x,ws_input_y) ;
        }
    }
    else if (ev == MG_EV_CLOSE)
    {
        printf("MG_EV_CLOSE\n");

        ws_disp_client_t *client = lst_clients_search_by_conn(lst_clients, c);
        if (!client)
        {
            printf("unknow client close ?\n");
        }
        else
        {
            stack_remove(&lst_clients, client);
        }
        printf("lst_clients count: %d\n", stack_count(lst_clients));
    }
    (void)fn_data;
}

struct mg_mgr mgr;

void be_module_httplws_init() {
    mg_http_listen(be_module_mg_mgr(), s_listen_on, fn, NULL); // Create HTTP listener
}

void be_module_httplws_loop(JSContext *ctx) {
}