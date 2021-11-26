#include "module_http.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>

#include "cutils.h"
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
    httpd_handle_t server ;
    JSValueConst callback ;
    JSContext *ctx ;
    JSValue jsserver ;
    bool ws ;
} server_t;

static JSClassID js_http_server_class_id;


typedef struct _req {
    httpd_req_t * req ;
    server_t * server ;
    JSValue jsreq ;
} req_t;

static JSClassID js_http_req_class_id;



// Http Request ---------------------------------------

static void js_http_req_finalizer(JSRuntime *rt, JSValue val) {    
    req_t * cobj = (req_t*) JS_GetOpaque(val, js_http_req_class_id);
    if(cobj) {
        free(cobj) ;
    }
}


static JSClassDef js_http_req_class = {
    "http.Request",
    .finalizer = js_http_req_finalizer,
}; 


#define REQ_OPAQUE(creq) \
    if(this_val==JS_UNDEFINED || this_val==JS_NULL) {   \
        THROW_EXCEPTION("This function must be called as a method of http.Request object")   \
    }   \
    req_t * creq = (req_t *)JS_GetOpaque(this_val, js_http_req_class_id) ;  \
    if(!creq) { \
        THROW_EXCEPTION("Invalid http.Request object")   \
    }


/**
 * 取得 http request header 
 * 
 * @param {string} header name
 * @return {string}
 */
JSValue js_http_req_get_header(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    REQ_OPAQUE(creq)

    ARGV_TO_STRING_E(0, name, namelen, "http.Request.getHeader() arg name must be a string")


    /* Get header value string length and allocate memory for length + 1,
     * extra byte for null termination */
    size_t val_len = httpd_req_get_hdr_value_len(creq->req, name) ;
    if (val_len < 1) {
        JS_FreeCString(ctx,name) ;
        return JS_NULL ;
    }

    char * val = malloc(val_len+1);
    /* Copy null terminated value string into buffer */
    if (httpd_req_get_hdr_value_str(creq->req, "Host", val, val_len+1) != ESP_OK) {
        JS_FreeCString(ctx,name) ;
        free(val);
        return JS_NULL ;
    }
    
    JSValue jsval = JS_NewStringLen(ctx, val, val_len) ;
    
    free(val);
    JS_FreeCString(ctx,name) ;

    return jsval ;
}

inline JSValue http_send_data(JSContext *ctx, httpd_req_t * req, JSValue jsdata) {
    
    size_t datalen = 0 ;
    const char * data = (char *)JS_GetArrayBuffer(ctx, &datalen, jsdata) ;
    if (data) {
        httpd_resp_send_chunk(req, data, datalen);
        return JS_TRUE ;
    }

    else if(JS_IsString(jsdata)) {
        data = JS_ToCStringLen(ctx, &datalen, jsdata) ;
        httpd_resp_send_chunk(req, data, datalen);
        JS_FreeCString(ctx,data) ;
        return JS_TRUE ;
    }

    else {
        THROW_EXCEPTION("arg 1 must be String/ArrayBuffer")
    }
}

JSValue js_http_req_end(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    
    REQ_OPAQUE(creq)

    if(argc>=1) {
        http_send_data(ctx, creq->req, argv[0]) ;
    }

    httpd_resp_send_chunk(creq->req, NULL, 0);

    return JS_UNDEFINED ;
}



/**
 * 取得 http request header 
 * 
 * @param {string} header name
 * @param {string} header value
 * @return {string}
 */
JSValue js_http_req_write_header(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)
    REQ_OPAQUE(cres)
    ARGV_TO_STRING_E(0, name, namelen, "argv header name must be a string")
    ARGV_TO_STRING_E(1, value, valuelen, "argv header value must be a string")

    JSValue ret = ESP_OK == httpd_resp_set_hdr(cres->req, name, value);

    JS_FreeCString(ctx, name);
    JS_FreeCString(ctx, value);

    return ret ;
}




/**
 */
JSValue js_http_req_write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    REQ_OPAQUE(cres)    
    return http_send_data(ctx, cres->req, argv[0]) ;
}


static const JSCFunctionListEntry js_http_req_proto_funcs[] = {
    JS_CFUNC_DEF("getHeader", 0, js_http_req_get_header),
    // JS_CGETSET_MAGIC_DEF("path", js_http_req_path, js_http_req_path, 0),
    // JS_CGETSET_MAGIC_DEF("host", js_http_req_host, js_http_req_host, 0),
    JS_CFUNC_DEF("writeHeader", 0, js_http_req_write_header),
    JS_CFUNC_DEF("write", 0, js_http_req_write),
    JS_CFUNC_DEF("end", 0, js_http_req_end),
};



// Http Server ---------------------------------------


static void js_http_server_finalizer(JSRuntime *rt, JSValue val) {
    printf("js_http_server_finalizer(), server obj ref: %d\n", VAR_REFCNT(val)) ;

    server_t * cserver = (server_t*) JS_GetOpaque(val, js_http_server_class_id);
    
    if(cserver->server) {
        httpd_stop(cserver->server) ;
    }

    printf("http server callback func ref: %d\n", VAR_REFCNT(cserver->callback)) ;
    JS_FreeValue(cserver->ctx, cserver->callback) ;

    if(cserver) {
        free(cserver) ;
    }
}

req_t * http_request(httpd_req_t *req) {

    server_t * cserver = (server_t*)req->user_ctx ;
    if(!cserver || !JS_IsFunction(cserver->ctx, cserver->callback)) {
        printf("no callback\n") ;
        return NULL ;
    }

    // req 参数
    req_t * creq = malloc(sizeof(req_t)) ;
    creq->req = req ;
    creq->server = cserver ;
    
    creq->jsreq = JS_NewObjectClass(cserver->ctx, js_http_req_class_id) ;
    JS_SetOpaque(creq->jsreq, creq);

    // req 继承 EventEmitter 属性
    JSValue reqEventHandles = JS_NewObject(cserver->ctx) ;
    JS_SetPropertyStr(cserver->ctx, creq->jsreq, "_handles", reqEventHandles) ;
    
    MAKE_ARGV1(cbargv, creq->jsreq)
    JSValue ret = JS_Call(cserver->ctx, cserver->callback, JS_UNDEFINED, 1, cbargv) ;
    if( JS_IsException(ret) ) {
        js_std_dump_error(cserver->ctx) ;
    }
    JS_FreeValue(cserver->ctx, ret) ;
    free(cbargv) ;

    return creq ;
}


/* Handler to delete a file from the server */
static esp_err_t http_server_handler(httpd_req_t *req) {

    req_t * creq = http_request(req) ;
    if(!creq) {
        return ESP_OK ;
    }

    JS_FreeValue(creq->server->ctx, creq->jsreq) ;

    return ESP_OK ;
}


static esp_err_t http_server_ws_handler(httpd_req_t *req) {

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

        // req_t* creq = (req_t*)req->user_ctx ;
        
        
        // if( !creq ) {
        //     dd
        //     return ESP_OK ;
        // }
        // if( !creq->server) {
        //     dd
        //     return ESP_OK ;
        // }
        // if( !JS_IsFunction(creq->server->ctx, !creq->server->callback)) {
        //     dd
        //     return ESP_OK ;
        // }
        httpd_ws_frame_t ws_pkt;
        ws_pkt.payload = NULL;
        httpd_ws_recv_frame(req, &ws_pkt, 0);

        printf("ws_pkt.len= %d\n", ws_pkt.len) ;
        printf("ws_pkt.final %d\n", ws_pkt.final) ;
        printf("ws_pkt.fragmented %d\n", ws_pkt.fragmented) ;
        printf("ws_pkt.type %d\n", ws_pkt.type) ;
        
        // MAKE_ARGV1(cbargv, creq->jsreq)
        // JSValue ret = JS_Call(creq->server->ctx, creq->server->callback, JS_UNDEFINED, 1, cbargv) ;
        // if( JS_IsException(ret) ) {
        //     js_std_dump_error(creq->server->ctx) ;
        // }
        // JS_FreeValue(creq->server->ctx, ret) ;
        // free(cbargv) ;
    }

    return ESP_OK ;
}

JSValue js_http_server_listen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(1)
    ARGV_TO_UINT16(0, port)

    if(this_val==JS_UNDEFINED || this_val==JS_NULL) {
        THROW_EXCEPTION("This function must be called as a method of http.Server object")
    }

    server_t * cserver = (server_t *)JS_GetOpaque(this_val, js_http_server_class_id) ;
    if(!cserver) {
        THROW_EXCEPTION("Invalid http.Server object")
    }
    
    if(argc>=2) {
        if(!JS_IsFunction(ctx, argv[1])) {
            THROW_EXCEPTION("arg 2 must be a callback function")
        }
    }

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_open_sockets = MAX_OPEN_SOCKETS;
    config.server_port = port ;

    /* Use the URI wildcard matching function in order to
     * allow the same handler to respond to multiple different
     * target URIs which match the wildcard scheme */
    config.uri_match_fn = httpd_uri_match_wildcard;

    if (httpd_start(&cserver->server, &config)!=ESP_OK) {
        // THROW_EXCEPTION("Failed to start file server!")
        if(argc>=2) {
            JSValue msg = JS_NewString(ctx, "can not start http server");
            MAKE_ARGV2(cbargv, msg, this_val) ;
            JS_Call(ctx, argv[1], JS_UNDEFINED, 2, cbargv) ;
            free(cbargv) ;
            JS_FreeValue(ctx, msg) ;
        }
        return JS_DupValue(ctx, this_val) ;
    }

    // websocket
    if(cserver->ws) {
        printf("listen as websocket\n") ;
        httpd_uri_t ws_header = {
            .uri        = "/*",
            .method     = HTTP_GET,
            .handler    = http_server_ws_handler,
            .is_websocket = true ,
            // .handle_ws_control_frames = true ,
            .user_ctx  = cserver
        };
        httpd_register_uri_handler(cserver->server, &ws_header);
    }

    // http
    else {
        httpd_uri_t get_handler = {
            .uri       = "/*",
            .method    = HTTP_GET,
            .handler   = http_server_handler,
            .user_ctx  = cserver
        };
        httpd_register_uri_handler(cserver->server, &get_handler);

        httpd_uri_t post_handler = {
            .uri       = "/*",
            .method    = HTTP_POST,
            .handler   = http_server_handler,
            .user_ctx  = cserver
        };
        httpd_register_uri_handler(cserver->server, &post_handler);

        httpd_uri_t delete_handler = {
            .uri       = "/*",
            .method    = HTTP_DELETE,
            .handler   = http_server_handler,
            .user_ctx  = cserver
        };
        httpd_register_uri_handler(cserver->server, &delete_handler);
    }

    if(argc>=2) {
        MAKE_ARGV2(cbargv, JS_NULL, this_val) ;
        JS_Call(ctx, argv[1], JS_UNDEFINED, 2, cbargv) ;
        free(cbargv) ;
    }

    // printf("server ref: %d\n", VAR_REFCNT(this_val)) ;

    return JS_DupValue(ctx, this_val) ;
}
JSValue js_http_server_close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    
    server_t * cserver = (server_t *)JS_GetOpaque(this_val, js_http_server_class_id) ;
    if(!cserver) {
        THROW_EXCEPTION("Avalid http.Server object")
    }

    if(cserver->server) {
        httpd_stop(cserver->server) ;
        cserver->server = NULL ;
    }

    return JS_DupValue(ctx, this_val) ;
}


JSValue js_http_ctor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    if(!JS_IsFunction(ctx,argv[0])) {
        THROW_EXCEPTION("arg callback must be a function")
    }
    // options
    if(argc>1) {
        if(!JS_IsObject(argv[1])) {
            THROW_EXCEPTION("arg options must be a object")
        }
    }

    JSValue proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    JSValue obj = JS_UNDEFINED ;
    if (JS_IsException(proto))
        goto fail;
    obj = JS_NewObjectProtoClass(ctx, proto, js_http_server_class_id);
    JS_FreeValue(ctx, proto);
    if (JS_IsException(obj))
        goto fail;

    // EventEmitter 属性
    JSValue _handles = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, obj, "_handles", _handles) ;

    server_t * cserver = malloc(sizeof(server_t)) ;
    cserver->server = NULL ;
    cserver->callback = JS_DupValue(ctx, argv[0]) ;
    cserver->ctx = ctx ;
    cserver->ws = false ;

    if(argc>1) {
        JSValue opt_ws = JS_GetPropertyStr(ctx, argv[1], "ws") ;
        if( JS_ToBool(ctx, opt_ws) ) {
            printf("cserver->ws = true ;\n") ;
            cserver->ws = true ;
        }
        JS_FreeValue(ctx, opt_ws);
    }

    JS_SetOpaque(obj, cserver);
    return obj;

 fail:
    JS_FreeValue(ctx, obj);
    return JS_EXCEPTION;
}



static JSClassDef js_http_server_class = {
    "http.Server",
    .finalizer = js_http_server_finalizer,
}; 

static const JSCFunctionListEntry js_http_server_proto_funcs[] = {
    JS_CFUNC_DEF("listen", 0, js_http_server_listen),
    JS_CFUNC_DEF("close", 1, js_http_server_close),
};



void require_module_http(JSContext *ctx) {
    
    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beapi = JS_GetPropertyStr(ctx, global, "beapi") ;
    JSValue http = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "http", http);    

    JSValue EventEmitter = JS_GetPropertyStr(ctx, beapi, "EventEmitter") ;
    JSValue EventEmitterProto = JS_GetPropertyStr(ctx, EventEmitter, "prototype") ;

    // request
    JS_NewClass(JS_GetRuntime(ctx), js_http_req_class_id, &js_http_req_class);
    JSValue reqProto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, reqProto, js_http_req_proto_funcs, countof(js_http_req_proto_funcs));
    JS_SetClassProto(ctx, js_http_req_class_id, reqProto);
    JS_SetPropertyStr(ctx, reqProto, "__proto__", EventEmitterProto);  // 继承自 EventEmitter

    // server
    JS_NewClass(JS_GetRuntime(ctx), js_http_server_class_id, &js_http_server_class);
    JSValue httpProto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, httpProto, js_http_server_proto_funcs, countof(js_http_server_proto_funcs));
    JSValue serverCtor = JS_NewCFunction2(ctx, js_http_ctor, "http.Server", 1, JS_CFUNC_constructor, 0) ;
    JS_SetConstructor(ctx, serverCtor, httpProto) ;
    JS_SetClassProto(ctx, js_http_server_class_id, httpProto);
    JS_SetPropertyStr(ctx, httpProto, "__proto__", EventEmitterProto);  // 继承自 EventEmitter
    JS_SetPropertyStr(ctx, http, "Server", serverCtor);

    JS_FreeValue(ctx, global);
    JS_FreeValue(ctx, beapi);
    JS_FreeValue(ctx, EventEmitter);
    JS_FreeValue(ctx, EventEmitterProto);
}

void http_init() {
    // class id 全局, 分配一次
    JS_NewClassID(&js_http_req_class_id);
    JS_NewClassID(&js_http_server_class_id);
}


void http_on_before_reset(JSContext *ctx) {

}