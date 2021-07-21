#include "module_http.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>

#include "esp_log.h"
#include "telnet.h"
#include "task_js.h"
#include "eventloop.h"

#include "esp_http_server.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "esp_littlefs.h"

#define MAX_OPEN_SOCKETS 7

typedef struct _server {
    uint8_t idx ;
    httpd_handle_t * server ;
    JSValueConst callback ;
} server_t;

server_t server_pool[8] ;
#define SERVER_POOL_SIZE (sizeof(server_pool)/sizeof(server_t))

/* Handler to delete a file from the server */
static esp_err_t http_server_handler(httpd_req_t *req) {

    server_t * server = (server_t*) req->user_ctx ;

    MAKE_ARGV1(cbargv, JS_NewUint32(task_current_context(), server->idx))
    eventloop_push_with_argv(task_current_context(), server->callback, 1, cbargv) ;

    httpd_resp_send_chunk(req, "hoho", 4);
    httpd_resp_send_chunk(req, NULL, 0);
    
    return ESP_OK ;
}

/**
 * port = 80
 */
JSValue js_http_server_start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)
    ARGV_TO_UINT8(0, port)
    if(!JS_IsFunction(ctx, argv[1])) {
        THROW_EXCEPTION("argv 2 must be a function")
    }

    int8_t h = -1 ;
    for(int8_t i=0;i<SERVER_POOL_SIZE; i++) {
        if(!server_pool[i].server) {
            h = i ;
            break ;
        }
    }
    if(h==-1) {
        THROW_EXCEPTION("Too many http servers")
    }
    
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_open_sockets = MAX_OPEN_SOCKETS;
    config.server_port = port ;
    
    /* Use the URI wildcard matching function in order to
     * allow the same handler to respond to multiple different
     * target URIs which match the wildcard scheme */
    config.uri_match_fn = httpd_uri_match_wildcard;
    
    JS_DupValue(ctx, argv[1]) ;
    server_pool[h].callback = argv[1] ;

    if (httpd_start(&server_pool[h].server, &config)!=ESP_OK) {
        THROW_EXCEPTION("Failed to start file server!")
    }

    
    httpd_uri_t get_handler = {
        .uri       = "/*",
        .method    = HTTP_GET,
        .handler   = http_server_handler,
        .user_ctx  = & server_pool[h]
    };
    httpd_register_uri_handler(server_pool[h].server, &get_handler);
    
    httpd_uri_t post_handler = {
        .uri       = "/*",
        .method    = HTTP_POST,
        .handler   = http_server_handler,
        .user_ctx  = & server_pool[h]
    };
    httpd_register_uri_handler(server_pool[h].server, &post_handler);
    
    httpd_uri_t delete_handler = {
        .uri       = "/*",
        .method    = HTTP_DELETE,
        .handler   = http_server_handler,
        .user_ctx  = & server_pool[h]
    };
    httpd_register_uri_handler(server_pool[h].server, &delete_handler);



    return JS_NewUint32(ctx, h) ;
}

#define ARGV_AS_SERVER_IDX(i, var)                                              \
            ARGV_TO_INT8(i, var)                                                \
            if(i<0 || i>=SERVER_POOL_SIZE || server_pool[var].server==NULL) {   \
                THROW_EXCEPTION("argv is not a valid http server handle.")      \
            }

/**
 * serverHandle
 */
JSValue js_http_server_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    ARGV_AS_SERVER_IDX(0, h)

    if(httpd_stop(server_pool[h].server)!=ESP_OK) {
        return JS_FALSE ;
    }

    JS_FreeValue(ctx, server_pool[h].callback) ;

    server_pool[h].callback = NULL ;
    server_pool[h].server = NULL ;

    return JS_TRUE ;
}
// httpd_stop

/**
 * serverHandle
 */
JSValue js_http_resp_header(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(3)
    return JS_UNDEFINED ;
}
/**
 * serverHandle
 */
JSValue js_http_resp_send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)

    return JS_UNDEFINED ;
}
/**
 * serverHandle
 */
JSValue js_http_resp_end(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    ARGV_TO_UINT32(0, handle)
    return JS_UNDEFINED ;
}


void require_module_http(JSContext *ctx) {

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beapi = JS_GetPropertyStr(ctx, global, "beapi") ;

    JSValue http = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "http", http);

    JS_SetPropertyStr(ctx, http, "serverStart", JS_NewCFunction(ctx, js_http_server_start, "serverStart", 1));
    JS_SetPropertyStr(ctx, http, "serverStop", JS_NewCFunction(ctx, js_http_server_stop, "serverStop", 1));
    JS_SetPropertyStr(ctx, http, "respSendHeader", JS_NewCFunction(ctx, js_http_resp_header, "respSendHeader", 1));
    JS_SetPropertyStr(ctx, http, "respSendBody", JS_NewCFunction(ctx, js_http_resp_send, "respSendBody", 1));
    JS_SetPropertyStr(ctx, http, "respEnd", JS_NewCFunction(ctx, js_http_resp_end, "respEnd", 1));

    JS_FreeValue(ctx, global);
    JS_FreeValue(ctx, beapi);
}

void http_init() {
    for(int8_t i=0;i<SERVER_POOL_SIZE; i++) {
        server_pool[i].idx = i ;
        server_pool[i].server = NULL ;
    }
}


void http_on_before_reset(JSContext *ctx) {
    for(int8_t i=0;i<SERVER_POOL_SIZE; i++) {
        if(server_pool[i].server==NULL)
            continue ;
        echof("stop http server %d\n", i) ;
        
        JS_FreeValue(ctx, server_pool[i].callback) ;
        httpd_stop(server_pool[i].server) ;

        server_pool[i].server = NULL ;
        server_pool[i].callback = NULL ;
    }
}