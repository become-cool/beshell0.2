#include "telnet_ws.h"
#include "telnet.h"

#include "beshell.h"
#include "task_js.h"
#include "display.h"
#include "utils.h"
#include "mongoose.h"
#include "stack.h"
#include "module_mg.h"
#include "module_lvgl.h"
#include "module_fs.h"

#include "json.h"

#define WS_DISP_CMD_REFRESH 1
#define WS_DISP_CMD_DRAW 2
#define WS_DISP_CMD_PRESS 22
#define WS_DISP_CMD_RELEASE 23

#define WS_DISP_BUFF_RAW 1
#define WS_DISP_BUFF_JPEG 2





char * fs_root = NULL;

void indev_global_cb_proc(lv_indev_data_t *data) ;

void ws_driver_input_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
    // if(!ws_input_update) {
    //     return ;
    // }
    if(!be_lv_fake_indev(data)) {
        if(indev_input_pressed) {
            data->state = LV_INDEV_STATE_PRESSED ;
            data->point.x = indev_input_x ;
            data->point.y = indev_input_y ;
        }
        else {
            data->state = LV_INDEV_STATE_RELEASED ;
        }
    }
    // ws_input_update = false ;

    indev_global_cb_proc(data) ;
}


typedef enum {
    WS_REPL = 1 ,
    WS_DISP ,
} WS_TYPE ;

typedef struct {
    stack_base_t stack_item;
    struct mg_connection *conn;
    void *jsobj;
    WS_TYPE type ;   // 1: repl, 2: display
} ws_client_t;

ws_client_t *lst_clients = NULL;

ws_client_t *lst_clients_search_by_conn(ws_client_t *lst, struct mg_connection *conn) {
    for (ws_client_t *item = lst; item != NULL; item = (ws_client_t *)((stack_base_t *)item)->_next) {
        if (item->conn == conn)
            return item;
    }
    return NULL;
}


static const char *s_listen_on = "ws://0.0.0.0:8022";


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
    STACK_FOREACH(lst_clients, client, ws_client_t) {
        mg_ws_send(client->conn, (char *)buff, size, WEBSOCKET_OP_BINARY);
    }

    lv_disp_flush_ready(disp) ;
}

static void listdir(struct mg_connection *c, const char * path) {

    size_t size = 1024 ;
    char * buffer = malloc(size) ;

    char * childpath = NULL ;
    char * childstr = NULL ;
                
    DIR* dir = opendir(path);

    if(!dir) {
        mg_http_reply(c, 500, "", "%s", "Open Dir Failed");
        return ;
    }

    
    JSON json;
    json_init(&json) ;

    JSON_ARRAY(json);
    
    struct dirent * dirEnt;    

    int idx = 0 ;
    struct stat statbuf;
    while((dirEnt = readdir(dir))) {

        if(strcmp(dirEnt->d_name, ".")==0 || strcmp(dirEnt->d_name, "..")==0) {
            continue ;
        }

        childpath = mallocf("%s/%s",path,dirEnt->d_name) ;
        if(!childpath){
            mg_http_reply(c, 500, "Content-Type: application/json\r\n", "{error: \"%s\"}", "Out Of Memory");
            free(buffer) ;
            return ;
        }

        if(stat(childpath,&statbuf)!=0) {
            mg_http_reply(c, 500, "Content-Type: application/json\r\n", "{error: \"Stat File Failed: %s\"}", dirEnt->d_name) ;
            free(childpath) ;
            free(buffer) ;
            json_free(&json) ;
            return ;
        }

        
        JSON_OBJECT(json);

        JSON_KEY(json, "name", 4);
        JSON_STRING(json, dirEnt->d_name, strlen(dirEnt->d_name));

        JSON_KEY(json, "type", 4);
        if(S_ISREG(statbuf.st_mode)) {
            JSON_STRING(json, "file", 4);
            
            JSON_KEY(json, "size", 4);
            JSON_NUMBER(json, statbuf.st_size);
        }
        else if(S_ISDIR(statbuf.st_mode)) {
            JSON_STRING(json, "dir", 3);
        }
        else {
            JSON_STRING(json, "unknow", 6);
        }

        JSON_OBJECT_POP(json);

        free(childpath) ;
        free(childstr) ;
    }

    JSON_ARRAY_POP(json);

    mg_http_reply(c, 200, "Content-Type: application/json\r\n", "%s", json.buf) ;

    closedir(dir);

    free(buffer) ;
    json_free(&json) ;
}

static void upgrade_ws(struct mg_connection *c, struct mg_http_message *hm, WS_TYPE type) {

    mg_ws_upgrade(c, hm, NULL);

    if (lst_clients_search_by_conn(lst_clients, c) == NULL) {

        ws_client_t *client = malloc(sizeof(ws_client_t));
        memset(client, 0, sizeof(ws_client_t));
        client->conn = c;
        client->type = type;
        stack_unshift(&lst_clients, client);

        // printf("new ws connect, lst_clients count: %d\n", stack_count(lst_clients));
    }

    else
    {
        printf("connect exists\n");
    }
}

static void fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *)ev_data;
        if (mg_http_match_uri(hm, "/repl")) {
            upgrade_ws(c, hm, WS_REPL) ;
        }
        else if (mg_http_match_uri(hm, "/display")) {
            upgrade_ws(c, hm, WS_DISP) ;
        }
        else if( hm->uri.len>=3 && hm->uri.ptr[0]=='/' && hm->uri.ptr[1]=='f' && hm->uri.ptr[2]=='s'
            && (hm->uri.len==3 || hm->uri.ptr[3]=='/')
        ) {

#ifndef SIMULATION
            char * path = malloc(hm->uri.len+1) ;
            memcpy(path,hm->uri.ptr,hm->uri.len) ;
            path[hm->uri.len] = '\0' ;
#else
            char * path ;
            if(hm->uri.len==3) {
                path = mallocf("%s", vfs_path_prefix) ;
            }
            else {
                char end = hm->uri.ptr[hm->uri.len] ;
                char * uri = hm->uri.ptr ;
                uri[hm->uri.len] = '\0' ;

                path = mallocf("%s%s", vfs_path_prefix, uri+4) ;

                uri[hm->uri.len] = end ;
            }

#endif
            if(mg_vcmp(&hm->method, "GET")==0) {
                struct stat statbuf;
                // 不存在
                if(stat(path,&statbuf)<0) {
                    mg_http_reply(c, 404,  "Content-Type: application/json\r\n", "{error: \"%s\"}", "Not Found");
                }
                // 文件
                else if(S_ISREG(statbuf.st_mode)) {
                    mg_http_reply(c, 200, "", "file:%s", path);
                }
                // 目录
                else if(S_ISDIR(statbuf.st_mode)) {
                    listdir(c, path) ;
                }
                // unknow
                else {
                    mg_http_reply(c, 500,  "Content-Type: application/json\r\n", "{error: \"%s\"}", "Unknow File Type");
                }
            }
            else if(mg_vcmp(&hm->method, "POST")==0) {
                dd
            }
            else if(mg_vcmp(&hm->method, "DELETE")==0) {
                dd
            }
            else {
                mg_http_reply(c, 500,  "Content-Type: application/json\r\n", "{error: \"%s\"}", "Unknow Method");
            }

            free(path) ;
        }
        else {
            mg_http_reply(c, 403, "", "%s", "Invalid Path");
        }
    }
    else if (ev == MG_EV_WS_MSG) {
        
        ws_client_t *client = lst_clients_search_by_conn(lst_clients, c);
        struct mg_ws_message * wm = (struct mg_ws_message *) ev_data;

        // for /display
        if(client->type == WS_DISP) {
            // printf("%d(%d)\n", wm->data.ptr[0], wm->data.len) ;

            if (wm->data.ptr[0] == WS_DISP_CMD_REFRESH && wm->data.len == 1) {

                // printf("WS_DISP_CMD_REFRESH\n") ;
                
                lv_disp_t *disp = lv_disp_get_default();
                if (disp) {
                    lv_area_t area;
                    memset(&area, 0, sizeof(lv_area_t));
                    area.x2 = disp->driver->hor_res - 1;
                    area.y2 = disp->driver->ver_res - 1;

                    // printf("_lv_inv_area() (%d,%d)->(%d,%d)", area.x1,area.y1,area.x2,area.y2) ;
                    _lv_inv_area(disp, &area);
                }
                else {
                    printf("there is no default display\n");
                }
            }

            else if (wm->data.ptr[0] == WS_DISP_CMD_RELEASE && wm->data.len == 1) {
                indev_input_pressed = false ;
            }
            else if (wm->data.ptr[0] == WS_DISP_CMD_PRESS && wm->data.len == 5) {
                uint16_t *data = (uint16_t *)(wm->data.ptr + 1);
    
                indev_input_x = *data ;
                indev_input_y = *(data + 1);
                indev_input_pressed = true ;
            }
        }

        // for /repl
        else if(client->type == WS_REPL) {
            JSContext * ctx = (JSContext *)be_module_mg_mgr()->userdata ;
            if(ctx) {
                uint8_t cmd = wm->data.ptr[0] ;
                uint8_t pkgid = wm->data.ptr[1] ;
                char * code = wm->data.ptr + 2 ;
                size_t codelen = wm->data.len - 2 ;

                telnet_run(ctx, pkgid, 0, cmd, (uint8_t *) code, codelen) ;
            }
        }
    }
    else if (ev == MG_EV_CLOSE) {
        printf("MG_EV_CLOSE\n");

        ws_client_t *client = lst_clients_search_by_conn(lst_clients, c);
        if (!client) {
            printf("unknow client close ?\n");
        }
        else {
            stack_remove(&lst_clients, client);
        }
        // printf("lst_clients count: %d\n", stack_count(lst_clients));
    }
    (void)fn_data;
}


void telnet_ws_output(uint8_t cmd, int pkgid, const char * data, size_t datalen) {

    size_t pkglen = datalen+2 ;
    char * pkg = malloc(pkglen) ;
    pkg[0] = cmd ;
    pkg[1] = pkgid ;
    memcpy(pkg+2, data, datalen) ;
    
    // printf("lst_clients count: %d\n", stack_count(lst_clients));
    STACK_FOREACH(lst_clients, client, ws_client_t) {
        mg_ws_send(client->conn, pkg, pkglen, WEBSOCKET_OP_BINARY);
    }

	free(pkg) ;
}


void be_telnet_ws_init() {
    fs_root = mallocf("/fs/=%s", vfs_path_prefix) ;
    mg_http_listen(be_module_mg_mgr(), s_listen_on, fn, NULL); // Create HTTP listener
}
