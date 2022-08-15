#include "mg_server.h"
#include "module_mg.h"
#include "module_fs.h"
#include "telnet_ws.h"
#include "utils.h"
#include "cutils.h"



// =================================================
// class Server


static JSClassID js_mg_server_class_id ;


#define THIS_SERVER(var)                                                    \
    be_http_server_t * var = JS_GetOpaque(this_val, js_mg_server_class_id) ;\
    if(!var || !var->conn) {                                                \
        JS_ThrowReferenceError(var->ctx, "mg.Server object has closed.");     \
        return JS_EXCEPTION ;                                               \
    }

static JSValue js_mg_server_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    JSValue jsobj = JS_NewObjectClass(ctx, js_mg_server_class_id) ;
    return jsobj ;
}
static void js_mg_server_finalizer(JSRuntime *rt, JSValue this_val){

    // @TODO: 改为在 MG_EV_CLOSE 事件里 free(server)
    THIS_SERVER(server)
    free(server) ;

    // @todo
    // 未断开的客户端连接 (chrome等现代浏览器会保持长连接)
}
static JSClassDef js_mg_server_class = {
    "mg.Server",
    .finalizer = js_mg_server_finalizer,
} ;

static JSValue js_mg_server_close(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_SERVER(server)
    server->conn->is_closing = true ;
    return JS_UNDEFINED ;
}

static const JSCFunctionListEntry js_mg_server_proto_funcs[] = {
    JS_CFUNC_DEF("close", 0, js_mg_server_close),
} ;



// enum {
//   MG_EV_ERROR,       // Error                         char *error_message
//   MG_EV_OPEN,        // Client created           NULL
//   MG_EV_POLL,        // mg_mgr_poll iteration        unsigned long *millis
//   MG_EV_RESOLVE,     // Host name is resolved        NULL
//   MG_EV_CONNECT,     // Client established       NULL
//   MG_EV_ACCEPT,      // Client accepted          NULL
//   MG_EV_READ,        // Data received from socket    struct mg_str *
//   MG_EV_WRITE,       // Data written to socket       long *bytes_written
//   MG_EV_CLOSE,       // Client closed            NULL
//   MG_EV_HTTP_MSG,    // HTTP request/response        struct mg_http_message *
//   MG_EV_HTTP_CHUNK,  // HTTP chunk (partial msg)     struct mg_http_message *
//   MG_EV_WS_OPEN,     // Websocket handshake done     struct mg_http_message *
//   MG_EV_WS_MSG,      // Websocket msg, text or bin   struct mg_ws_message *
//   MG_EV_WS_CTL,      // Websocket control msg        struct mg_ws_message *
//   MG_EV_MQTT_CMD,    // MQTT low-level command       struct mg_mqtt_message *
//   MG_EV_MQTT_MSG,    // MQTT PUBLISH received        struct mg_mqtt_message *
//   MG_EV_MQTT_OPEN,   // MQTT CONNACK received        int *connack_status_code
//   MG_EV_SNTP_TIME,   // SNTP time received           struct timeval *
//   MG_EV_USER,        // Starting ID for user events
// };
static void http_server_event_handler(struct mg_connection * conn, int ev, void *ev_data, void *fn_data) {
    if(ev== MG_EV_POLL || !fn_data) {
        return ;
    }

    be_http_server_t * server = (be_http_server_t *)fn_data ;
    if(!JS_IsFunction(server->ctx, server->callback)) {
        printf("callback is not a function， event:%s\n", mg_event_const_to_name(ev)) ;
        return ;
    }

    // printf("event:%s, conn:%p, server:%p, server->conn:%p, \n", mg_event_const_to_name(ev), conn, server, server->conn) ;
    // printf("%s %s \n", ((server->conn==NULL || server->conn==conn)?"server":"client"), mg_event_const_to_name(ev)) ;

    // server connection
    if(server->conn==NULL || server->conn==conn) {
        if(ev==MG_EV_CLOSE) {
            JS_FreeValue(server->ctx, server->callback) ;
            server->callback = JS_NULL ;
            return ;
        }
    }

    // client connections
    else {
        switch(ev) {
            case MG_EV_OPEN: {
                response_t * rspn = malloc(sizeof(response_t)) ;
                rspn->conn = conn ;
                rspn->jsrspn = JS_NewObjectClass(server->ctx, js_mg_http_rspn_class_id) ;
                JS_SetOpaque(rspn->jsrspn, rspn) ;
                conn->userdata = rspn ;
                return ;
            }
            case MG_EV_CLOSE:
                if(conn->userdata) {
                    response_t * rspn = (response_t *)conn->userdata ;
                    JS_SetOpaque(rspn->jsrspn, NULL) ;
                    JS_FreeValue(server->ctx, rspn->jsrspn) ;
                    free(rspn) ;
                    conn->userdata = NULL ;
                }
                return ;

            case MG_EV_HTTP_MSG:
            case MG_EV_HTTP_CHUNK: {
                if(!conn->userdata) {
                    printf("conn->userdata == NULL ??") ;
                    return ;
                }
                response_t * rspn = (response_t *)conn->userdata ;

                JSValue jsreq = JS_NewObjectClass(server->ctx, js_mg_http_message_class_id) ;
                JS_SetOpaque(jsreq, ev_data) ;

                MAKE_ARGV3(cbargv, JS_NewString(server->ctx, mg_event_const_to_name(ev)), jsreq, rspn->jsrspn)
                JSValue ret = JS_Call(server->ctx, server->callback, JS_UNDEFINED, 3, cbargv) ;

                JS_SetOpaque(jsreq, NULL) ;  // mg 事件函数结束后 mg_http_message 对象销毁
                JS_FreeValue(server->ctx, jsreq) ;
                free(cbargv) ;

                if(JS_IsException(ret)) {
                    js_std_dump_error(server->ctx) ;
                }

                return ;
            }
            case MG_EV_WS_OPEN: {
                return ;
            }
            case MG_EV_WS_CTL: 
            case MG_EV_WS_MSG: {

                if(!conn->userdata) {
                    printf("conn->userdata == NULL ??") ;
                    return ;
                }
                response_t * rspn = (response_t *)conn->userdata ;

                struct mg_ws_message * msg = (struct mg_ws_message *)ev_data ;

                JSValue jsmsg = JS_NewObject(server->ctx) ;
                uint8_t op = msg->flags&15 ;
                JS_SetPropertyStr(server->ctx, jsmsg, "type", JS_NewUint32(server->ctx, op)) ;

                if(op == WEBSOCKET_OP_TEXT) {
                    JS_SetPropertyStr(server->ctx, jsmsg, "data", JS_NewStringLen(server->ctx, msg->data.ptr, msg->data.len)) ;
                }
                else if(op == WEBSOCKET_OP_BINARY) {
                    JS_SetPropertyStr(server->ctx, jsmsg, "data", JS_NewArrayBuffer(server->ctx,msg->data.ptr, msg->data.len,NULL,NULL,false)) ;
                }

                MAKE_ARGV3(cbargv, JS_NewString(server->ctx, mg_event_const_to_name(ev)), jsmsg, rspn->jsrspn)
                JSValue ret = JS_Call(server->ctx, server->callback, JS_UNDEFINED, 3, cbargv) ;

                if( JS_IsException(ret) ){
                    js_std_dump_error(server->ctx) ;
                }
                JS_FreeValue(server->ctx, jsmsg) ;
                free(cbargv) ;

                return ;
            }
        }
    }
}


JSValue be_http_server_new(JSContext *ctx, struct mg_connection * conn, JSValue callback) {

    be_http_server_t * server = malloc(sizeof(be_http_server_t)) ;
    server->ctx  = ctx ;
    server->callback = JS_DupValue(ctx,callback) ;

    server->conn = conn ;
    conn->userdata = server ;

    JSValue jsobj = JS_NewObjectClass(ctx, js_mg_server_class_id) ;
    JS_SetOpaque(jsobj, server) ;
    
    return jsobj ;
}

static JSValue js_mg_mgr_http_listen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(2)
    if( !JS_IsFunction(ctx, argv[1]) ) {
        THROW_EXCEPTION("arg callback must be a function")
    }
    ARGV_TO_STRING_E(0, addr, "arg addr must be a string") ;

    if(!mg_url_is_listening(addr)) {
        JS_ThrowReferenceError(ctx, "addr %s has listened", addr) ;
        JS_FreeCString(ctx, addr) ;
        return JS_EXCEPTION ;
    }

    struct mg_connection * conn = mg_http_listen(be_module_mg_mgr(), addr, http_server_event_handler, NULL) ;
    if(conn==NULL) {
        THROW_EXCEPTION_FREE({
            JS_FreeCString(ctx, addr) ;
        }, "could not listen addr: %s", addr)
    }
    
    JS_FreeCString(ctx, addr) ;
    return be_http_server_new(ctx, conn, argv[1]) ;
}



void be_module_mg_server_init() {
    JS_NewClassID(&js_mg_server_class_id);
}
void be_module_mg_server_require(JSContext *ctx, JSValue mg, JSValue pkgShadow) {
    QJS_DEF_CLASS(mg_server, "Server", "mg.Server", JS_UNDEFINED, pkgShadow)
    
    JS_SetPropertyStr(ctx, mg, "httpListen", JS_NewCFunction(ctx, js_mg_mgr_http_listen, "httpListen", 1));
}