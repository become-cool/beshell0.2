#include "module_http.h"
#include "telnet_ws.h"

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
#include "task_js.h"
#include "eventloop.h"

#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "esp_littlefs.h"

#define MAX_OPEN_SOCKETS 8

static JSClassID js_http_server_class_id;


list_t lst_server ;

// Http Server ---------------------------------------

inline server_t * this_http_server(JSValue this_val) {
    return (server_t *)JS_GetOpaque(this_val, js_http_server_class_id) ;
}

static void js_http_server_finalizer(JSRuntime *rt, JSValue val) {
    printf("js_http_server_finalizer(), server obj ref: %d\n", VAR_REFCNT(val)) ;

    server_t * cserver = (server_t*) JS_GetOpaque(val, js_http_server_class_id);
    if(!cserver) {
        printf("Opaque is NULL!\n") ;
        return ;
    }
    
    http_server_close(cserver) ;

    // 清理 route
    FOREACH_LIST(&cserver->lst_route, item) {
        server_route_t * route = ((server_route_t*)item->data) ;
        JS_FreeValue(route->ctx, route->callback) ;
        free(item->data) ;
        list_remove(&cserver->lst_route, item->data) ;
    }

    if(cserver) {
        list_remove(&lst_server, cserver) ;
        free(cserver) ;
    }
}


JSValue js_http_server_listen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    SERVER_THIS(cserver)

    CHECK_ARGC(1)
    ARGV_TO_UINT16(0, port)
    
    esp_err_t err = http_server_listen(cserver, port) ;
    if(err!=ESP_OK) {
        THROW_EXCEPTION(ctx,"could not listen on port %d, error code: %d", port, err) ;
    }

    return JS_TRUE ;
}

JSValue js_http_server_close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    SERVER_THIS(cserver)
    esp_err_t err = http_server_close(cserver) ;
    if(err!=ESP_OK) {
        THROW_EXCEPTION("faild to close http server, error code:%d", (uint32_t)err)
    }
    return JS_DupValue(ctx, this_val) ;
}



static JSClassDef js_http_server_class = {
    "http.Server",
    .finalizer = js_http_server_finalizer,
}; 

static const JSCFunctionListEntry js_http_server_proto_funcs[] = {
    JS_CFUNC_DEF("listen", 0, js_http_server_listen),
    JS_CFUNC_DEF("close", 1, js_http_server_close),
    JS_CFUNC_DEF("route", 1, js_http_server_route),
    JS_CFUNC_DEF("routeWebsocket", 1, js_http_server_route_ws),
};



server_t * http_server_new(JSContext * owner, httpd_handle_t handle) {
    server_t * cserver = malloc(sizeof(server_t)) ;
    cserver->owner = owner ;

    cserver->jsserver = JS_NewObjectClass(owner, js_http_server_class_id) ;
    JS_SetOpaque(cserver->jsserver, cserver);

    if(handle) {
        cserver->handle = handle ;
        JS_DupValue(owner, cserver->jsserver) ;
    }
    else {
        cserver->handle = NULL ;
    }

    list_init(&cserver->lst_route) ;

    list_append(&lst_server, (void*)cserver) ;

    return cserver ;
}

esp_err_t http_server_listen(server_t * cserver, uint16_t port) {
    
    if(!cserver || cserver->handle){
        return false ;
    }

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_open_sockets = MAX_OPEN_SOCKETS;
    config.server_port = port ;
    config.ctrl_port += port ;

    /* Use the URI wildcard matching function in order to
     * allow the same handler to respond to multiple different
     * target URIs which match the wildcard scheme */
    config.uri_match_fn = httpd_uri_match_wildcard;

    esp_err_t err = httpd_start(&cserver->handle, &config) ;

    if (err==ESP_OK) {
        JS_DupValue(cserver->owner, cserver->jsserver) ;
    }
    else{
        printf("error :%d\n", err) ;
        IS_NULL(cserver->handle)
    }
    return err ;
}

esp_err_t http_server_close(server_t * cserver) {
    if(!cserver->handle)
        return ESP_OK ;
    esp_err_t err = httpd_stop(cserver->handle) ;
    if( err != ESP_OK ) {
        return false ;
    }
    cserver->handle = NULL ;
    if(cserver->jsserver){
        JS_FreeValue(cserver->owner, cserver->jsserver) ;
    }
    return err ;
}

JSValue js_http_create_server(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    server_t * cserver = http_server_new(ctx, NULL) ;

    if( argc>0 ) {
        int32_t port ;
        if(JS_ToInt32(ctx, &port, argv[0])!=0 || port<1){
            JS_ThrowReferenceError(ctx,"port must be a uint") ;
            goto err ;
        }
        esp_err_t err = http_server_listen(cserver, port) ;
        if(err!=ESP_OK) {
            JS_ThrowReferenceError(ctx,"could not listen on port %d, error code: %d", port, err) ;
            goto err ;
        }
    }

    return cserver->jsserver ;
err:
    dd
    JS_FreeValue(ctx, cserver->jsserver) ;
    return JS_EXCEPTION ;
}


void require_module_http(JSContext *ctx) {
    
    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beapi = JS_GetPropertyStr(ctx, global, "beapi") ;
    JSValue http = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "http", http);    

    JSValue EventEmitter = JS_GetPropertyStr(ctx, beapi, "EventEmitter") ;
    JSValue EventEmitterProto = JS_GetPropertyStr(ctx, EventEmitter, "prototype") ;

    JS_SetPropertyStr(ctx, http, "createServer", JS_NewCFunction(ctx, js_http_create_server, "createServer", 1));

    // server
    JS_NewClass(JS_GetRuntime(ctx), js_http_server_class_id, &js_http_server_class);
    JSValue httpProto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, httpProto, js_http_server_proto_funcs, countof(js_http_server_proto_funcs));
    JS_SetClassProto(ctx, js_http_server_class_id, httpProto);
    JS_SetPropertyStr(ctx, httpProto, "__proto__", EventEmitterProto);  // 继承自 EventEmitter

    // web telnet
    // server_t * webtelnet = http_server_new(ctx, webtelnet_handle()) ;
    // JS_SetPropertyStr(ctx, http, "telnet", webtelnet->jsserver);

    require_module_http_req(ctx, EventEmitterProto) ;

    JS_FreeValue(ctx, global);
    JS_FreeValue(ctx, beapi);
    JS_FreeValue(ctx, EventEmitter);
    JS_FreeValue(ctx, EventEmitterProto);
}

void be_module_http_init() {
    // class id 全局, 分配一次
    JS_NewClassID(&js_http_server_class_id);

    list_init(&lst_server) ;

    http_req_init() ;
}


void be_module_http_reset(JSContext *ctx) {

}