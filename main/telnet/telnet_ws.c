#include "telnet_ws.h"
#include "module_telnet.h"
#include "module_metadata.h"

#ifndef SIMULATION
#include "driver_camera.h"
#include "esp_camera.h"
#endif

#include "img_converters.h"

#include "beshell.h"
#include "js_main_loop.h"
#include "display.h"
#include "indev.h"
#include "utils.h"
#include "stack.h"
#include "module_mg.h"
#include "module_lvgl.h"
#include "module_fs.h"


#include "json.h"

#define WS_DISP_CMD_REFRESH 1
#define WS_DISP_CMD_DRAW 2
#define WS_DISP_CMD_PRESS 22
#define WS_DISP_CMD_RELEASE 23
#define WS_DISP_CMD_QUALITY 30

#define WS_DISP_BUFF_RAW 1
#define WS_DISP_BUFF_JPEG 2

#define TELNET_WS_ADDR "ws://0.0.0.0:8022"

#define CROS_RSPN_HEADERS                                       \
        "Access-Control-Allow-Origin: *\r\n"                    \
        "Access-Control-Request-Method: GET, POST, OPTIONS\r\n" \
        "Access-Control-Request-Headers: content-type\r\n"      \
        "Content-Type: application/octet-stream\r\n"


static char * fs_root = NULL;
struct mg_connection * conn ;


struct mg_connection * client ;

uint8_t jpeg_quality = 20 ;


typedef enum {
    WS_REPL = 1 ,
    WS_DISP ,
} WS_TYPE ;

typedef struct {
    stack_base_t stack_item;
    struct mg_connection *conn;
    WS_TYPE type ;   // 1: repl, 2: display
    bool active ;
    lv_disp_t * disp ;

} ws_client_t;

ws_client_t *lst_clients = NULL;

ws_client_t *lst_clients_search_by_conn(ws_client_t *lst, struct mg_connection *conn) {
    for (ws_client_t *item = lst; item != NULL; item = (ws_client_t *)((stack_base_t *)item)->_next) {
        if (item->conn == conn)
            return item;
    }
    return NULL;
}


struct mg_connection * repl_client = NULL ;

void telnet_ws_set_repl_client(struct mg_connection * conn) {
    if(repl_client) {
        if(repl_client==conn) {
            return ;
        }
        // @todo tell them
        repl_client->is_closing = 1 ;
    }
    repl_client = conn ;
}


void ws_disp_flush(lv_disp_drv_t *dispdrv, const lv_area_t *area, lv_color_t *color_p) {

    if(stack_count(lst_clients)==0) {
        lv_disp_flush_ready(dispdrv) ;
        return ;
    }

    size_t size = (area->x2-area->x1+1) * (area->y2-area->y1+1) * sizeof(lv_color_t) ;

    uint64_t t0 = gettime() ;

    //todo: allocate proper buffer for holding JPEG data
    //this should be enough for CIF frame size
    size_t jpglen = size*2;
    uint8_t * buff = (uint8_t *)malloc(jpglen+10);
    if(buff == NULL) {
        printf("JPG buffer malloc failed");
        return ;
    }

    buff[0] = WS_DISP_CMD_DRAW ;
    buff[1] = WS_DISP_BUFF_JPEG ;
    uint16_t * coord = (uint16_t *)(buff + 2);
    (*coord) = area->x1 ;
    coord++ ;
    (*coord) = area->y1 ;
    coord++ ;
    (*coord) = area->x2 ;
    coord++ ;
    (*coord) = area->y2 ;

    // dn2(area->x1,area->y1)

    uint8_t * jpgbuff = buff + 10 ;

    fmt2jpg((uint8_t *)color_p, size, area->x2-area->x1+1, area->y2-area->y1+1, PIXFORMAT_RGB565, jpeg_quality, jpgbuff, &jpglen);

    STACK_FOREACH(lst_clients, client, ws_client_t) {
        if(client->type==WS_DISP && client->active && client->disp->driver==dispdrv) {
            mg_ws_send(client->conn, (char *)buff, jpglen+10, WEBSOCKET_OP_BINARY);
        }
    }

    free(buff) ;

    lv_disp_flush_ready(dispdrv) ;
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

    if( type==WS_REPL ) {
        telnet_ws_set_repl_client(c) ;
    }
    else if ( type==WS_DISP && lst_clients_search_by_conn(lst_clients, c)==NULL ) {

        char strid[4] ;
        int idlen = mg_http_get_var(&hm->query,"id",strid,sizeof(strid)) ;
        if(idlen<=0) {

            // @TODO : 返回错误给客户端

            printf("missing display id\n") ;
            c->is_closing = 1 ;
            return;
        }
        strid[idlen] = 0 ;
        int id = atoi(strid) ;
        if(id<0 || id>255) {
            // @TODO : 返回错误给客户端
            printf("invalid display id: %d\n", id) ;
            c->is_closing = 1 ;
            return;
        }

        lv_disp_t * disp = find_disp_by_id(id) ;
        if(!disp) {
            // @TODO : 返回错误给客户端
            printf("display id not exists: %d\n", id) ;
            c->is_closing = 1 ;
            return;
        }

        ws_client_t *client = malloc(sizeof(ws_client_t));
        memset(client, 0, sizeof(ws_client_t));
        client->conn = c;
        client->type = WS_DISP;
        client->disp = disp ;
        client->active = true ;
        stack_unshift(&lst_clients, client);

        printf("new ws connect, lst_clients count: %d\n", stack_count(lst_clients));
    }
}

static void response_fs(struct mg_connection *c, struct mg_http_message *hm, const char * path) {

    // 判断 路径不存在
    struct stat statbuf;
    if(stat(path,&statbuf)<0) {
        mg_http_reply(c, 404,  "Content-Type: application/json\r\n", "{error: \"%s\"}", "Not Found");
        return ;
    }

    if(mg_vcmp(&hm->method, "GET")==0) {
        // 文件
        if(S_ISREG(statbuf.st_mode)) {
            struct mg_http_serve_opts opts = {.mime_types = "application/octet-stream"};
            mg_http_serve_file(c, hm, path, &opts);  // Send file

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
    // 上传文件 CROS 浏览器跨域时的预检请求
    else if(mg_vcmp(&hm->method, "OPTIONS")==0){
        printf("CROS\n") ;
        mg_http_reply(c, 204,  CROS_RSPN_HEADERS
        , "", "");
    }
    else if(mg_vcmp(&hm->method, "POST")==0) {
        // printf("post path=%s, body len:%d\n", path, hm->body.len); ;

        // 检查目录是否存在
        if(!S_ISDIR(statbuf.st_mode)) {
            mg_http_reply(c, 500, "", "{error: \"%s\"}", "arg path is not a dir") ;
            return ;
        }

        struct mg_http_part part;
        for(int pos=0; (pos=mg_http_next_multipart(hm->body, 0, &part));) {
            if( mg_vcmp(&part.name, "file")==0 ) {

                const char * fullpath = mallocf("%s/%.*s", path, part.filename.len, part.filename.ptr) ;
                // ds(fullpath)
                // dn(part.body.len)
                
                FILE * fd = fopen(fullpath, "w");
                if(NULL==fd) {
                    free(fullpath) ;
                    mg_http_reply(c, 500, "", "{error: \"%s\"}", "can not open file") ;
                    return ;
                }
                free(fullpath) ;

	            size_t wroteBytes = fwrite(part.body.ptr, 1, part.body.len, fd);
                fclose(fd) ;

                if(wroteBytes==part.body.len) {
                    mg_http_reply(c, 200, CROS_RSPN_HEADERS, "%s", "ok") ;
                }
                else {
                    mg_http_reply(c, 200, CROS_RSPN_HEADERS, "{error: \"%s\"}", "some error occur") ;
                }
                return ;
            }
        }

        mg_http_reply(c, 403, "", "{error: \"%s\"}", "missing file") ;
    }
    // 删除文件/目录
    else if(mg_vcmp(&hm->method, "DELETE")==0) {
        // dd
    }
    // 无效请求
    else {
        mg_http_reply(c, 500,  "Content-Type: application/json\r\n", "{error: \"%s\"}", "Unknow Method");
    }
}


// www.msftconnecttest.com
// ipv6.msftconnecttest.com
// http://captive.apple.com/hotspotdetect.html
// http://connectivitycheck.android.com/generate_204
// ****/generate_204
static bool telnet_ws_is_captive_portal_request(struct mg_connection *c, struct mg_http_message *hm) {

    struct mg_str * host = mg_http_get_header(hm, "Host") ;
    if(mg_strcmp(*host, mg_str("captive.apple.com"))==0) {
        if(mg_http_match_uri(hm,"/hotspotdetect.html")) {
            return true ;
        }
    }
    else if(mg_http_match_uri(hm,"/generate_204")) {
        return true ;
    }
    else if(mg_strcmp(*host, mg_str("www.msftconnecttest.com"))==0) {
        return true ;
    }

    return false ;
}

bool telnet_ws_response_http(struct mg_connection *c, struct mg_http_message *hm) {
    if(!hm) {
        mg_http_reply(c, 500, "", "%s", "Invalid mg_http_message");
        return true ;
    }

    if (mg_http_match_uri(hm, "/repl")) {
        upgrade_ws(c, hm, WS_REPL) ;
        return true ;
    }
    else if( telnet_ws_is_captive_portal_request(c,hm) ) {
        mg_http_reply(c, 302, "Location: http://192.168.4.1:8022\r\n", "Redirection") ;
        return true ;
    }
    else if(mg_http_match_uri(hm, "/") || mg_http_match_uri(hm, "/telweb")) {
        char * extra_headers = mallocf(
                "Content-Encoding: gzip\r\n"
                "Content-Type: text/html\r\n" 
                "BECOME-PartID: %d(v%d)\r\n"
                , readPartId()
                , readPartVersion()
        ) ;
        struct mg_http_serve_opts opts = {
                .mime_types = "text/html",
                .extra_headers = extra_headers
        };
#ifndef SIMULATION
        mg_http_serve_file(c, hm, "/fs/lib/local/telweb/index.html.gz", &opts);  // Send file
#else
        mg_http_serve_file(c, hm, "../filesystem/root/lib/local/telweb/index.html.gz", &opts);  // Send file
#endif

        free(extra_headers) ;
        return true ;
    }
    else if(mg_http_match_uri(hm, "/favicon.ico")) {

        struct mg_http_serve_opts opts = {
                .mime_types = "image/vnd.microsoft.icon",
                .extra_headers = "Content-Type: image/vnd.microsoft.icon\r\n"
        };

        // content-type: image/vnd.microsoft.icon
#ifndef SIMULATION
        mg_http_serve_file(c, hm, "/fs/lib/local/telweb/favicon.ico", &opts);  // Send file
#else
        mg_http_serve_file(c, hm, "../filesystem/root/lib/local/telweb/favicon.ico", &opts);  // Send file
#endif
        return true ;
    }
    // /fs
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
        response_fs(c, hm, path) ;

        free(path) ;
        return true ;
    }

    else if (mg_http_match_uri(hm, "/display")) {
        upgrade_ws(c, hm, WS_DISP) ;
        return true ;
    }

    return false ;
}

bool telnet_ws_response_ws(struct mg_connection *c, struct mg_ws_message * wm) {

    ws_client_t *client = lst_clients_search_by_conn(lst_clients, c);

    // for /display
    if( client && client->type == WS_DISP) {
        // printf("%d(%d)\n", wm->data.ptr[0], wm->data.len) ;

        if (wm->data.ptr[0] == WS_DISP_CMD_REFRESH && wm->data.len == 1) {
            if (client->disp) {
                lv_area_t area;
                memset(&area, 0, sizeof(lv_area_t));
                area.x2 = client->disp->driver->hor_res - 1;
                area.y2 = client->disp->driver->ver_res - 1;

                // printf("_lv_inv_area() (%d,%d)->(%d,%d)", area.x1,area.y1,area.x2,area.y2) ;
                bool active = client->active ;
                client->active = true ;
                _lv_inv_area(client->disp, &area);
                lv_task_handler();
                client->active = active ;
            }
            else {
                printf("there is no default display\n");
            }
        }

        else if (wm->data.ptr[0] == WS_DISP_CMD_RELEASE) {
            if(wm->data.len == 2) {
                indev_driver_spec_t * indev_spec = find_indev_spec_by_id( ((uint8_t *)wm->data.ptr)[1] ) ;
                if(indev_spec) {
                    indev_spec->data.pointer.state = LV_INDEV_STATE_RELEASED ;
                    indev_spec->fake = true ;
                }
                else {
                    printf("invalid indev id: %d\n", ((uint8_t *)wm->data.ptr)[1] ) ;
                }
            }
            else {
                printf("invalid indev frame data len: %d\n", wm->data.len ) ;
            }
        }
        else if (wm->data.ptr[0] == WS_DISP_CMD_PRESS) {
            if(wm->data.len == 6) {
                indev_driver_spec_t * indev_spec = find_indev_spec_by_id( ((uint8_t *)wm->data.ptr)[1] ) ;
                if(indev_spec) {
                    uint16_t *data = (uint16_t *)(wm->data.ptr + 2);
                    indev_spec->data.pointer.x = *data ;
                    indev_spec->data.pointer.y = *(data + 1);
                    indev_spec->data.pointer.state = LV_INDEV_STATE_PRESSED ;
                    indev_spec->fake = true ;
                }
                else {
                    printf("invalid indev id: %d\n", ((uint8_t *)wm->data.ptr)[1] ) ;
                }
            }
            else {
                printf("invalid indev frame data len: %d\n", wm->data.len ) ;
            }
        }
        else if (wm->data.ptr[0] == WS_DISP_CMD_QUALITY) {
            jpeg_quality = wm->data.ptr[1] ;
            
            lv_area_t area;
            memset(&area, 0, sizeof(lv_area_t));
            area.x2 = client->disp->driver->hor_res - 1;
            area.y2 = client->disp->driver->ver_res - 1;
            _lv_inv_area(client->disp, &area);
        }

        else {
            printf("invalid cmd type: %d\n", wm->data.ptr[0]) ;
            return false ;
        }
    }

    // for /repl
    else if(c==repl_client) {
        JSContext * ctx = (JSContext *)be_module_mg_mgr()->userdata ;
        if(ctx) {
            uint8_t cmd = wm->data.ptr[0] ;
            uint8_t pkgid = wm->data.ptr[1] ;
            char * code = wm->data.ptr + 2 ;
            size_t codelen = wm->data.len - 2 ;

            telnet_run(ctx, pkgid, 0, cmd, (uint8_t *) code, codelen) ;
        }
    }

    else {
        
        printf("invalid client\n") ;

        return false ;
    }

    return true ;
}

// 返回 true 表示已经处理完必，否则返回 false
bool telnet_ws_response(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    (void)fn_data;
    
#ifndef SIMULATION
    // camera
    if(be_module_driver_camera_response(c,ev,ev_data)) {
        return true ;
    }
#endif

    if (ev == MG_EV_HTTP_MSG) {
        return telnet_ws_response_http(c,ev_data) ;
    }
    else if (ev == MG_EV_WS_MSG) {
        return telnet_ws_response_ws(c,ev_data) ;
    }
    else if (ev == MG_EV_CLOSE) {
        ws_client_t *client = lst_clients_search_by_conn(lst_clients, c);
        if (!client) {
            // printf("unknow client close ?\n");
        }
        else {
            stack_remove(&lst_clients, client);
        }

        if(repl_client == c) {
            repl_client = NULL ;
        }

        return false ;
        // printf("lst_clients count: %d\n", stack_count(lst_clients));
    }
    
    else {
        return true ;
    }
}



void telnet_ws_output(uint8_t cmd, int pkgid, const char * data, size_t datalen) {


    size_t pkglen = datalen+2 ;
    char * pkg = malloc(pkglen) ;
    pkg[0] = cmd ;
    pkg[1] = pkgid ;
    memcpy(pkg+2, data, datalen) ;
    
    // printf("lst_clients count: %d\n", stack_count(lst_clients));
    // STACK_FOREACH(lst_clients, client, ws_client_t) {
    //     mg_ws_send(client->conn, pkg, pkglen, WEBSOCKET_OP_BINARY);
    // }

    if(repl_client) {
        mg_ws_send(repl_client, pkg, pkglen, WEBSOCKET_OP_BINARY);
    }

	free(pkg) ;
}



// DNS answer section. We response with IP 1.2.3.4 - you can change it
//  in the last 4 bytes of this array
uint8_t answer[] = {
    0xc0, 0x0c,          // Point to the name in the DNS question
    0,    1,             // 2 bytes - record type, A
    0,    1,             // 2 bytes - address class, INET
    0,    0,    0, 120,  // 4 bytes - TTL
    0,    4,             // 2 bytes - address length
    192,    168,    4, 1     // 4 bytes - IP address
};

static void captive_dns(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
  if (ev == MG_EV_OPEN) {
    c->is_hexdumping = 1;
  } else if (ev == MG_EV_READ) {
    struct mg_dns_rr rr;  // Parse first question, offset 12 is header size
    size_t n = mg_dns_parse_rr(c->recv.buf, c->recv.len, 12, true, &rr);
    // MG_INFO(("DNS request parsed, result=%d", (int) n));
    if (n > 0) {
      char buf[512];
      struct mg_dns_header *h = (struct mg_dns_header *) buf;
      memset(buf, 0, sizeof(buf));  // Clear the whole datagram
      h->txnid = ((struct mg_dns_header *) c->recv.buf)->txnid;  // Copy tnxid
      h->num_questions = mg_htons(1);  // We use only the 1st question
      h->num_answers = mg_htons(1);    // And only one answer
      h->flags = mg_htons(0x8400);     // Authoritative response
      memcpy(buf + sizeof(*h), c->recv.buf + sizeof(*h), n);  // Copy question
      memcpy(buf + sizeof(*h) + n, answer, sizeof(answer));   // And answer
      mg_send(c, buf, 12 + n + sizeof(answer));               // And send it!
    }
    mg_iobuf_del(&c->recv, 0, c->recv.len);
  }
  (void) fn_data;
  (void) ev_data;
}



void be_telnet_ws_init() {
    fs_root = mallocf("/fs/=%s", vfs_path_prefix) ;
    // conn = mg_http_listen(be_module_mg_mgr(), TELNET_WS_ADDR, telnet_ws_response, NULL);
    // printf("telnet websocket addr: %s\n", TELNET_WS_ADDR) ;

    // Captive Portal
    mg_listen(be_module_mg_mgr(), "udp://0.0.0.0:53", captive_dns, NULL);

    repl_client = NULL ;
}
void be_telnet_ws_require(JSContext * ctx, JSValue telnet) {
    // JSValue server = be_http_server_new(ctx, conn, JS_UNDEFINED) ;
    // JS_SetPropertyStr(ctx, telnet, "ws", server) ;
}
