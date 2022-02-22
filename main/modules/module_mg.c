#include "module_mg.h"
#include "utils.h"
#include "cutils.h"
#include "stack.h"
#include "module_fs.h"

struct mg_mgr mgr ;

JSValue pkgShadow ;

static JSClassID js_mg_server_class_id ;
static JSClassID js_mg_http_message_class_id ;
static JSClassID js_mg_ws_message_class_id ;
static JSClassID js_mg_http_rspn_class_id ;

typedef struct _server {
    struct mg_connection * conn ;
    JSContext * ctx ;
    JSValue callback ;
} server_t ;

typedef struct _response {
    struct mg_connection * conn ;
    JSValue jsrspn ;
} response_t ;

const char * mg_event_const_to_name(int ev) {
    switch(ev){
        case MG_EV_ERROR: return "error" ;
        case MG_EV_OPEN: return "open" ;
        case MG_EV_POLL: return "poll" ;
        case MG_EV_RESOLVE: return "resolve" ;
        case MG_EV_CONNECT: return "connect" ;
        case MG_EV_ACCEPT: return "accept" ;
        case MG_EV_READ: return "read" ;
        case MG_EV_WRITE: return "write" ;
        case MG_EV_CLOSE: return "close" ;
        case MG_EV_HTTP_MSG: return "http.msg" ;
        case MG_EV_HTTP_CHUNK: return "http.chunk" ;
        case MG_EV_WS_OPEN: return "ws.open" ;
        case MG_EV_WS_MSG: return "ws.msg" ;
        case MG_EV_WS_CTL: return "ws.ctl" ;
        case MG_EV_MQTT_CMD: return "mqtt.cmd" ;
        case MG_EV_MQTT_MSG: return "mqtt.msg" ;
        case MG_EV_MQTT_OPEN: return "mqtt.open" ;
        case MG_EV_SNTP_TIME: return "sntp.time" ;
        case MG_EV_USER: return "user" ;
        default:
            return "unknow" ;
    }
}

int mg_event_name_to_const(const char * evname) {
    if(strcmp(evname,"error")==0) { return MG_EV_ERROR ;}
    else if(strcmp(evname,"open")==0) { return MG_EV_OPEN ;}
    else if(strcmp(evname,"poll")==0) { return MG_EV_POLL ;}
    else if(strcmp(evname,"resolve")==0) { return MG_EV_RESOLVE ;}
    else if(strcmp(evname,"connect")==0) { return MG_EV_CONNECT ;}
    else if(strcmp(evname,"accept")==0) { return MG_EV_ACCEPT ;}
    else if(strcmp(evname,"read")==0) { return MG_EV_READ ;}
    else if(strcmp(evname,"write")==0) { return MG_EV_WRITE ;}
    else if(strcmp(evname,"close")==0) { return MG_EV_CLOSE ;}
    else if(strcmp(evname,"http.msg")==0) { return MG_EV_HTTP_MSG ;}
    else if(strcmp(evname,"http.chunk")==0) { return MG_EV_HTTP_CHUNK ;}
    else if(strcmp(evname,"ws.open")==0) { return MG_EV_WS_OPEN ;}
    else if(strcmp(evname,"ws.msg")==0) { return MG_EV_WS_MSG ;}
    else if(strcmp(evname,"ws.ctl")==0) { return MG_EV_WS_CTL ;}
    else if(strcmp(evname,"mqtt.cmd")==0) { return MG_EV_MQTT_CMD ;}
    else if(strcmp(evname,"mqtt.msg")==0) { return MG_EV_MQTT_MSG ;}
    else if(strcmp(evname,"mqtt.open")==0) { return MG_EV_MQTT_OPEN ;}
    else if(strcmp(evname,"sntp.time")==0) { return MG_EV_SNTP_TIME ;}
    else if(strcmp(evname,"user")==0) { return MG_EV_USER ;}
    else { return -1 ;}
}

bool mg_url_is_listening(const char * url) {
    struct mg_addr addr ;
    memset(&addr, 0, sizeof(struct mg_addr));

    addr.port = mg_htons(mg_url_port(url));
    if (!mg_aton(mg_url_host(url), &addr)) {
        return false ;
    }

    for(struct mg_connection * c = mgr.conns; c != NULL; c = c->next) {
        if(c->peer.ip==addr.ip && c->peer.port==addr.port){
            return false ;
        }
    }

    return true ;
}


// =================================================
// class Connection

#define THIS_SERVER(var)                                                  \
    server_t * var = JS_GetOpaque(this_val, js_mg_server_class_id) ;    \
    if(!var || !var->conn) {                                                \
        JS_ThrowReferenceError(var->ctx, "mg.Server object has free.");   \
        return JS_EXCEPTION ;                                               \
    }

static JSValue js_mg_server_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    JSValue jsobj = JS_NewObjectClass(ctx, js_mg_server_class_id) ;
    return jsobj ;
}
static void js_mg_server_finalizer(JSRuntime *rt, JSValue this_val){
    // printf("js_mg_server_finalizer()\n") ;
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



// =================================================
// class HttpMessage
#define THIS_HTTP_MSG(var)                                                                  \
    struct mg_http_message * var = JS_GetOpaque(this_val, js_mg_http_message_class_id) ;    \
    if(!var) {                                                                              \
        JS_ThrowReferenceError(ctx, "mg.HttpMessage object has free.");                \
        return JS_EXCEPTION ;                                                               \
    }

static JSValue js_mg_http_message_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    return JS_NewObjectClass(ctx, js_mg_http_message_class_id) ;
}
static void js_mg_http_message_finalizer(JSRuntime *rt, JSValue this_val){
    printf("js_mg_http_message_finalizer()\n") ;
}
static JSClassDef js_mg_http_message_class = {
    "mg.HttpMessage",
    .finalizer = js_mg_http_message_finalizer,
} ;

static JSValue js_mg_http_message_method(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    return JS_NewStringLen(ctx, msg->method.ptr, msg->method.len) ;
}
static JSValue js_mg_http_message_uri(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    return JS_NewStringLen(ctx, msg->uri.ptr, msg->uri.len) ;
}
static JSValue js_mg_http_message_query(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    return JS_NewStringLen(ctx, msg->query.ptr, msg->query.len) ;
}
static JSValue js_mg_http_message_proto(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    return JS_NewStringLen(ctx, msg->proto.ptr, msg->proto.len) ;
}
static JSValue js_mg_http_message_header(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    CHECK_ARGC(1)
    ARGV_TO_STRING_E(0,name, "arg headerName must be a string")
    struct mg_str * val = mg_http_get_header(msg, name) ;
    JS_FreeCString(ctx, name) ;
    return JS_NewStringLen(ctx, val->ptr, val->len) ;
}
static JSValue js_mg_http_message_all_headers(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    JSValue jsheaders = JS_NewObject(ctx) ;
    int max = sizeof(msg->headers) / sizeof(msg->headers[0]);
    for (int i = 0; i < max && msg->headers[i].name.len > 0; i++) {
        JS_SetPropertyStr(
            ctx, jsheaders,
            msg->headers[i].name.ptr ,
            JS_NewStringLen(ctx, msg->headers[i].value.ptr, msg->headers[i].value.len)
        ) ;
    }
    return jsheaders ;
}
static JSValue js_mg_http_message_body(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    return JS_NewStringLen(ctx, msg->body.ptr, msg->body.len) ;
}
static JSValue js_mg_http_message_chunk(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    return JS_NewStringLen(ctx, msg->chunk.ptr, msg->chunk.len) ;
}
static JSValue js_mg_http_message_raw(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    return JS_NewStringLen(ctx, msg->message.ptr, msg->message.len) ;
}
static JSValue js_mg_http_message_rawHead(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_MSG(msg)
    return JS_NewStringLen(ctx, msg->head.ptr, msg->head.len) ;
}

static const JSCFunctionListEntry js_mg_http_message_proto_funcs[] = {
    JS_CFUNC_DEF("method", 0, js_mg_http_message_method),
    JS_CFUNC_DEF("uri", 0, js_mg_http_message_uri),
    JS_CFUNC_DEF("query", 0, js_mg_http_message_query),
    JS_CFUNC_DEF("proto", 0, js_mg_http_message_proto),
    JS_CFUNC_DEF("header", 0, js_mg_http_message_header),
    JS_CFUNC_DEF("allHeaders", 0, js_mg_http_message_all_headers),
    JS_CFUNC_DEF("body", 0, js_mg_http_message_body),
    JS_CFUNC_DEF("chunk", 0, js_mg_http_message_chunk),
    JS_CFUNC_DEF("raw", 0, js_mg_http_message_raw),
    JS_CFUNC_DEF("rawHead", 0, js_mg_http_message_rawHead),
} ;


// =================================================
// class HttpResponse
#define THIS_HTTP_RSPN(var)                                                 \
    response_t * var = JS_GetOpaque(this_val, js_mg_http_rspn_class_id) ;   \
    if(!var) {                                                              \
        JS_ThrowReferenceError(ctx, "mg.HttpResponse object has free.");    \
        return JS_EXCEPTION ;                                               \
    }

#define NOT_WS_FUNC(methodName)                                                     \
    if(rspn->conn->is_websocket) {                                                  \
        THROW_EXCEPTION(methodName"() do not invoke with a websocket connection.")  \
    }

#define MUST_BE_WS_FUNC(methodName)                                                     \
    if(!rspn->conn->is_websocket) {                                                  \
        THROW_EXCEPTION(methodName"() must be invoked with a websocket connection.")  \
    }

static JSValue js_mg_http_rspn_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    return JS_NewObjectClass(ctx, js_mg_http_rspn_class_id) ;
}
static void js_mg_http_rspn_finalizer(JSRuntime *rt, JSValue this_val){
    printf("js_mg_http_rspn_finalizer()\n") ;
}
static JSClassDef js_mg_http_rspn_class = {
    "mg.HttpResponse",
    .finalizer = js_mg_http_rspn_finalizer,
} ;

static JSValue js_mg_rspn_reply(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_HTTP_RSPN(rspn)
    NOT_WS_FUNC("mg.HttpResponse.replay")
    
    if( JS_IsString(argv[0]) ) {
        const char * str = JS_ToCString(ctx, argv[0]) ;
        mg_http_reply(rspn->conn, 200, "", str) ;
        JS_FreeCString(ctx, str) ;
    }

    else {
        THROW_EXCEPTION("invalid type of arg body")
    }

    return JS_UNDEFINED ;
}


static JSValue js_mg_rspn_serve_dir(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    THIS_HTTP_RSPN(rspn)
    NOT_WS_FUNC("mg.HttpResponse.replay")

    if(!qjs_instanceof(ctx, argv[0], js_mg_http_message_class_id)){
        THROW_EXCEPTION("arg req must be an instance of mg.HttpMessage")
    }
    struct mg_http_message * msg = JS_GetOpaque(argv[0], js_mg_http_message_class_id) ;

    ARGV_TO_STRING_E(1, _path, "arg path must be a string")
    char * path = vfspath_to_fs(_path) ;

    struct mg_http_serve_opts opts = {.root_dir = path};
    mg_http_serve_dir(rspn->conn, msg, &opts);

    JS_FreeCString(ctx, _path) ;
    free(path) ;

    return JS_UNDEFINED ;
}

static JSValue js_mg_http_rspn_upgrade(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_HTTP_RSPN(rspn)
    CHECK_ARGC(1)
    NOT_WS_FUNC("mg.HttpResponse.upgrade")
    if( !qjs_instanceof(ctx, argv[0], js_mg_http_message_class_id) ){
        THROW_EXCEPTION("arg rspn must be a mg.HttpMessage")
    }
    struct mg_http_message * msg = JS_GetOpaque(argv[0], js_mg_http_message_class_id) ;
    if(!msg) {
        JS_ThrowReferenceError(ctx, "mg.HttpMessage object has free.");
        return JS_EXCEPTION ;
    }
    mg_ws_upgrade(rspn->conn, msg, NULL);
    return JS_UNDEFINED ;
}

static JSValue js_mg_rspn_ws_send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    THIS_HTTP_RSPN(rspn)
    MUST_BE_WS_FUNC("mg.HttpResponse.wsSend")

    if( JS_IsString(argv[0]) ){
        ARGV_TO_STRING_LEN(0, str, len)
        mg_ws_send(rspn->conn, str, len, WEBSOCKET_OP_TEXT) ;
        JS_FreeCString(ctx, str) ;
    }
    
    // if( JS_IsArray(argv[0]) ){

    // }
    // else {

    // }

    return JS_UNDEFINED ;
}

static const JSCFunctionListEntry js_mg_http_rspn_proto_funcs[] = {
    JS_CFUNC_DEF("reply", 0, js_mg_rspn_reply),
    JS_CFUNC_DEF("serveDir", 0, js_mg_rspn_serve_dir),
    JS_CFUNC_DEF("upgrade", 0, js_mg_http_rspn_upgrade),
    JS_CFUNC_DEF("wsSend", 0, js_mg_rspn_ws_send),
} ;



// enum {
//   MG_EV_ERROR,       // Error                         char *error_message
//   MG_EV_OPEN,        // Connection created           NULL
//   MG_EV_POLL,        // mg_mgr_poll iteration        unsigned long *millis
//   MG_EV_RESOLVE,     // Host name is resolved        NULL
//   MG_EV_CONNECT,     // Connection established       NULL
//   MG_EV_ACCEPT,      // Connection accepted          NULL
//   MG_EV_READ,        // Data received from socket    struct mg_str *
//   MG_EV_WRITE,       // Data written to socket       long *bytes_written
//   MG_EV_CLOSE,       // Connection closed            NULL
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
static void http_event_handler(struct mg_connection * conn, int ev, void *ev_data, void *fn_data) {
    if(ev== MG_EV_POLL || !fn_data) {
        return ;
    }

    server_t * server = (server_t *)fn_data ;
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
                dd
                JS_FreeValue(server->ctx, jsmsg) ;
                free(cbargv) ;

                return ;
            }
        }
    }
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

    server_t * server = malloc(sizeof(server_t)) ;
    server->ctx  = ctx ;
    server->callback = argv[1] ;
    server->conn = NULL ;

    server->conn = mg_http_listen(&mgr, addr, http_event_handler, server) ;
    JS_FreeCString(ctx, addr) ;

    if(server->conn==NULL) {
        free(server) ;
        THROW_EXCEPTION(ctx,"could not listen addr: %s", addr)
    }

    JS_DupValue(ctx,argv[1]) ;

    JSValue jsobj = JS_NewObjectClass(ctx, js_mg_server_class_id) ;
    JS_SetOpaque(jsobj, server) ;
    
    return jsobj ;
}


static void sntp_event_handler(struct mg_connection * conn, int ev, void *ev_data, void *fn_data) {
    if(ev==MG_EV_POLL)
        return ;
    ds( mg_event_const_to_name(ev) ) ;
    if (ev == MG_EV_SNTP_TIME) {
        // Time received
        struct timeval *tv = (struct timeval *)ev_data;
    }
}

static JSValue js_mg_sntp_connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    // CHECK_ARGC(0)

    mg_sntp_connect(&mgr, "udp://pool.ntp.org:123" /* connect to time.google.com */, sntp_event_handler, NULL);

    return JS_UNDEFINED ;
}



void be_module_mg_init() {
    mg_mgr_init(&mgr) ;

    JS_NewClassID(&js_mg_server_class_id);
    JS_NewClassID(&js_mg_http_message_class_id);
    JS_NewClassID(&js_mg_http_rspn_class_id);

    pkgShadow = JS_UNDEFINED ;
}

void be_module_mg_require(JSContext *ctx) {
    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue mg = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "mg", mg);

    pkgShadow = JS_NewObject(ctx);

    QJS_DEF_CLASS(mg_server, "Server", "mg.Server", JS_UNDEFINED, pkgShadow)
    QJS_DEF_CLASS(mg_http_message, "HttpMessage", "mg.HttpMessage", JS_UNDEFINED, pkgShadow)
    QJS_DEF_CLASS(mg_http_rspn, "HttpResponse", "mg.HttpResponse", JS_UNDEFINED, pkgShadow)

    JS_SetPropertyStr(ctx, mg, "httpListen", JS_NewCFunction(ctx, js_mg_mgr_http_listen, "httpListen", 1));
    JS_SetPropertyStr(ctx, mg, "sntpConnect", JS_NewCFunction(ctx, js_mg_sntp_connect, "sntpConnect", 1));
    
    // JS_FreeValue(ctx, mg);
    JS_FreeValue(ctx, beapi);
}

void be_module_mg_loop(JSContext *ctx) {
    mgr.userdata = ctx ;
    mg_mgr_poll(&mgr, 10);
}

void be_module_mg_reset(JSContext *ctx) {
    JS_FreeValue(ctx, pkgShadow) ;
    // assert(VAR_REFCNT(pkgShadow)==0) ;

    // @todo
    // 关闭所有的 server_t.conn
}


struct mg_mgr * be_module_mg_mgr() {
    return & mgr ;
}