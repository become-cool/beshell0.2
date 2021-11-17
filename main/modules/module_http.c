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



// Http Request ---------------------------------------
typedef struct _req {
    httpd_req_t * req ;
} req_t;

static JSClassID js_http_req_class_id;

static void js_http_req_finalizer(JSRuntime *rt, JSValue val) {    
    req_t * cobj = (req_t*) JS_GetOpaque(val, js_http_req_class_id);
    if(cobj) {
        free(cobj) ;
    }
}


static JSClassDef js_http_req_class = {
    "HttpRequest",
    .finalizer = js_http_req_finalizer,
}; 


#define REQ_OPAQUE(creq) \
    if(this_val==JS_UNDEFINED || this_val==JS_NULL) {   \
        THROW_EXCEPTION("This function must be called as a method of HttpRequest object")   \
    }   \
    req_t * creq = (req_t *)JS_GetOpaque(this_val, js_http_req_class_id) ;  \
    if(!creq) { \
        THROW_EXCEPTION("Invalid HttpRequest object")   \
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

    if(JS_IsString(argv[0])) {
        THROW_EXCEPTION("arg name must be a string")
    }
    ARGV_TO_STRING(0, name, namelen)


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

static const JSCFunctionListEntry js_http_req_proto_funcs[] = {
    JS_CFUNC_DEF("getHeader", 0, js_http_req_get_header),
    JS_CFUNC_DEF("end", 0, js_http_req_end),
};



// Http Response ---------------------------------------
typedef struct _res {
    httpd_req_t * req ;
} res_t;


#define RES_OPAQUE(cres) \
    if(this_val==JS_UNDEFINED || this_val==JS_NULL) {   \
        THROW_EXCEPTION("This function must be called as a method of HttpResonse object")   \
    }   \
    req_t * cres = (res_t *)JS_GetOpaque(this_val, js_http_res_class_id) ;  \
    if(!cres) { \
        THROW_EXCEPTION("Invalid HttpResonse object")   \
    }


static JSClassID js_http_res_class_id;

static void js_http_res_finalizer(JSRuntime *rt, JSValue val) {    
    res_t * cobj = (res_t*) JS_GetOpaque(val, js_http_res_class_id);
    if(cobj) {
        free(cobj) ;
    }
}


static JSClassDef js_http_res_class = {
    "HttpResponse",
    .finalizer = js_http_res_finalizer,
}; 


/**
 * 取得 http request header 
 * 
 * @param {string} header name
 * @param {string} header value
 * @return {string}
 */
JSValue js_http_res_write_header(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)
    return JS_TRUE ;
}




/**
 */
JSValue js_http_res_write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    RES_OPAQUE(cres)
    
    return http_send_data(ctx, cres->req, argv[0]) ;
}

static const JSCFunctionListEntry js_http_res_proto_funcs[] = {
    JS_CFUNC_DEF("writeHeader", 0, js_http_res_write_header),
    JS_CFUNC_DEF("write", 0, js_http_res_write),
};



// Http Server ---------------------------------------

typedef struct _server {
    httpd_handle_t server ;
    JSValueConst callback ;
    // uint16_t port ;
    JSContext *ctx ;
    JSValue jsserver ;
} server_t;

static JSClassID js_http_server_class_id;

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


/* Handler to delete a file from the server */
static esp_err_t http_server_handler(httpd_req_t *req) {
    server_t * cserver = (server_t*) req->user_ctx ;
    if(!cserver || !JS_IsFunction(cserver->ctx, cserver->callback)) {
        return ESP_OK ;
    }

    req_t * creq = malloc(sizeof(req_t)) ;
    creq->req = req ;
    JSValue jsreq = JS_NewObjectClass(cserver->ctx, js_http_req_class_id) ;
    JS_SetOpaque(jsreq, creq);


    res_t * cres = malloc(sizeof(res_t)) ;
    cres->req = req ;
    JSValue jsres = JS_NewObjectClass(cserver->ctx, js_http_res_class_id) ;
    JS_SetOpaque(jsres, cres);


    // eventloop_push_with_argv(task_current_context(), server->callback, 1, cbargv) ;
    
    MAKE_ARGV2(cbargv, jsreq, jsres)
    JSValue ret = JS_Call(cserver->ctx, cserver->callback, JS_UNDEFINED, 2, cbargv) ;
    if( JS_IsException(ret) ) {
        js_std_dump_error(cserver->ctx) ;
    }
    JS_FreeValue(cserver->ctx, ret) ;
    free(cbargv) ;

    JS_FreeValue(cserver->ctx, jsreq) ;
    JS_FreeValue(cserver->ctx, jsres) ;
    
    return ESP_OK ;
}


JSValue js_http_server_listen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(1)
    ARGV_TO_UINT16(0, port)

    if(this_val==JS_UNDEFINED || this_val==JS_NULL) {
        THROW_EXCEPTION("This function must be called as a method of HttpServer object")
    }

    server_t * cserver = (server_t *)JS_GetOpaque(this_val, js_http_server_class_id) ;
    if(!cserver) {
        THROW_EXCEPTION("Invalid HttpServer object")
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
        THROW_EXCEPTION("Avalid HttpServer object")
    }

    if(cserver->server) {
        httpd_stop(cserver->server) ;
        cserver->server = NULL ;
    }

    return JS_DupValue(ctx, this_val) ;
}

static JSClassDef js_http_server_class = {
    "HttpServer",
    .finalizer = js_http_server_finalizer,
}; 

static const JSCFunctionListEntry js_http_server_proto_funcs[] = {
    JS_CFUNC_DEF("listen", 0, js_http_server_listen),
    JS_CFUNC_DEF("close", 1, js_http_server_close),
};

JSValue js_http_create_server(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)

    if(!JS_IsFunction(ctx,argv[0])) {
        THROW_EXCEPTION("Arg must be a callback function")
    }

    server_t * cserver = malloc(sizeof(server_t)) ;
    cserver->server = NULL ;
    cserver->callback = JS_DupValue(ctx, argv[0]) ;
    cserver->ctx = ctx ;

    JSValue jsserver = JS_NewObjectClass(ctx, js_http_server_class_id) ;

    // printf("jsserver ref: %d\n", VAR_REFCNT(jsserver)) ;
    // JS_DupValue(ctx,this_val) ;
    // printf("jsserver ref: %d\n", VAR_REFCNT(jsserver)) ;


    JS_SetOpaque(jsserver, cserver);
    cserver->jsserver = jsserver ;

    return jsserver ;
}






void require_module_http(JSContext *ctx) {

    // request
    JS_NewClass(JS_GetRuntime(ctx), js_http_req_class_id, &js_http_req_class);
    JSValue reqProto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, reqProto, js_http_req_proto_funcs, countof(js_http_req_proto_funcs));
    JS_SetClassProto(ctx, js_http_req_class_id, reqProto);

    // response
    JS_NewClass(JS_GetRuntime(ctx), js_http_res_class_id, &js_http_res_class);
    JSValue resProto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, resProto, js_http_res_proto_funcs, countof(js_http_res_proto_funcs));
    JS_SetClassProto(ctx, js_http_res_class_id, resProto);

    // server
    JS_NewClass(JS_GetRuntime(ctx), js_http_server_class_id, &js_http_server_class);
    JSValue httpProto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, httpProto, js_http_server_proto_funcs, countof(js_http_server_proto_funcs));
    JS_SetClassProto(ctx, js_http_server_class_id, httpProto);

    // api
    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beapi = JS_GetPropertyStr(ctx, global, "beapi") ;

    JSValue http = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "http", http);    
    JS_SetPropertyStr(ctx, http, "createServer", JS_NewCFunction(ctx, js_http_create_server, "createServer", 1));

    JS_FreeValue(ctx, global);
    JS_FreeValue(ctx, beapi);
}

void http_init() {
    // class id 全局, 分配一次
    JS_NewClassID(&js_http_req_class_id);
    JS_NewClassID(&js_http_res_class_id);
    JS_NewClassID(&js_http_server_class_id);
}


void http_on_before_reset(JSContext *ctx) {

}