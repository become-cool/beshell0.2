#include "http_route.h"
#include "module_http.h"
#include "utils.h"
#include "cutils.h"





/* Handler to delete a file from the server */
static esp_err_t http_server_handler(httpd_req_t *req) {

    req_t * creq = http_request(req) ;
    if(!creq) {
        return ESP_OK ;
    }

    JS_FreeValue(creq->server->owner, creq->jsreq) ;

    return ESP_OK ;
}

static void free_socket_ctx(void * ctx) {
    printf("free_socket_ctx()\n") ;
}

static esp_err_t http_server_ws_handler(httpd_req_t *req) {

    
    printf("req->sess_ctx==NULL? %d\n", req->sess_ctx==NULL? 1: 0) ;
    
    // 建立连接
    if(!req->sess_ctx) {

        // client_t * client = malloc(sizeof(client_t));
        // client->hd = req->handle;
        // client->fd = httpd_req_to_sockfd(req);
        // list_append(&disp_clients, client) ;

        // req->sess_ctx = (void *) client ;
        // req->free_ctx = free_socket_ctx ;
    }




    return ESP_OK ;

    printf("ws msg come in, method=%d\n", req->method) ;

    if(req->method==HTTP_GET) {

        req_t * creq = http_request(req) ;
        if(!creq) {
            return ESP_OK ;
        }

        // 替换 user_ctx
        req->user_ctx = (void*)creq ; 
    }

    // websocket frame (websocket 建立握手连接后，首次进入 method==HTTP_GET)
    else {

        printf("path: %s\n", req->uri) ;

        httpd_ws_frame_t ws_pkt;
        ws_pkt.payload = NULL;
        httpd_ws_recv_frame(req, &ws_pkt, 0);

        printf("ws_pkt.len= %d\n", ws_pkt.len) ;
        printf("ws_pkt.final %d\n", ws_pkt.final) ;
        printf("ws_pkt.fragmented %d\n", ws_pkt.fragmented) ;
        printf("ws_pkt.type %d\n", ws_pkt.type) ;
        
        // MAKE_ARGV1(cbargv, creq->jsreq)
        // JSValue ret = JS_Call(creq->server->owner, creq->server->callback, JS_UNDEFINED, 1, cbargv) ;
        // if( JS_IsException(ret) ) {
        //     js_std_dump_error(creq->server->owner) ;
        // }
        // JS_FreeValue(creq->server->owner, ret) ;
        // free(cbargv) ;
    }

    return ESP_OK ;
}


JSValue js_http_server_route(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    SERVER_THIS(cserver)

    CHECK_ARGC(2)
    ARGV_TO_STRING_LEN_E(0, uri, urilen, "argv callback must be a function")
    if( !JS_IsFunction(ctx, argv[1]) ) {
        THROW_EXCEPTION("argv callback must be a function")
    }

    server_route_t * route = malloc(sizeof(server_route_t)) ;
    route->ctx = ctx ;
    route->server = cserver ;    
    route->callback = argv[1] ;

    bool suc = false ;

    httpd_uri_t get_handler = {
        .uri       = uri,
        .method    = HTTP_GET,
        .handler   = http_server_handler,
        .user_ctx  = route
    };
    if( ESP_OK == httpd_register_uri_handler(cserver->handle, &get_handler) ){
        suc = true ;
    }

    httpd_uri_t post_handler = {
        .uri       = uri,
        .method    = HTTP_POST,
        .handler   = http_server_handler,
        .user_ctx  = route
    };
    if( ESP_OK == httpd_register_uri_handler(cserver->handle, &post_handler) ){
        suc = true ;
    }

    httpd_uri_t delete_handler = {
        .uri       = uri,
        .method    = HTTP_DELETE,
        .handler   = http_server_handler,
        .user_ctx  = route
    };
    if( ESP_OK == httpd_register_uri_handler(cserver->handle, &delete_handler) ){
        suc = true ;
    }

    JS_FreeCString(ctx, uri) ;

    if(!suc) {
        free(route) ;
        THROW_EXCEPTION("register route faided.") ;
    }

    JS_DupValue(ctx, route->callback) ;
    list_append(&cserver->lst_route, route) ;

    return JS_DupValue(ctx, this_val) ;
}

JSValue js_http_server_route_ws(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    SERVER_THIS(cserver)

    CHECK_ARGC(2)
    ARGV_TO_STRING_LEN_E(0, uri, urilen, "argv callback must be a function")
    if( !JS_IsFunction(ctx, argv[1]) ) {
        THROW_EXCEPTION("argv callback must be a function")
    }
    
    server_route_t * route = malloc(sizeof(server_route_t)) ;

    httpd_uri_t ws_uri = {
        .uri        = uri,
        .method     = HTTP_GET,
        .handler    = http_server_ws_handler,
        .is_websocket = true ,
        .user_ctx  = route
    };
    if(httpd_register_uri_handler(cserver->handle, &ws_uri)!=ESP_OK) {
        free(route) ;
        JS_FreeCString(ctx, uri) ;
        THROW_EXCEPTION("register route faided.") ;
    }

    JS_FreeCString(ctx, uri) ;

    route->callback =JS_DupValue(ctx, argv[1]) ;
    route->server = cserver ;
    list_append(&cserver->lst_route, route) ;

    return JS_DupValue(ctx, this_val) ;
}