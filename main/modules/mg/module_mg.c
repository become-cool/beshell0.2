#include "module_mg.h"
#include "utils.h"
#include "cutils.h"
#include "stack.h"
#include "module_fs.h"
#include "mg_client.h"
#include "mg_server.h"


struct mg_mgr mgr ;

JSValue pkgShadow ;


JSClassID js_mg_http_message_class_id ;
JSClassID js_mg_ws_message_class_id ;
JSClassID js_mg_http_rspn_class_id ;



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



static void sntp_callback(mg_req_t * req, int64_t time) {
    MAKE_ARGV1( argv, JS_NewInt64(req->ctx, time) )
    JS_Call(req->ctx, req->callback, JS_UNDEFINED, 1, argv) ;
    free(argv) ;
}

static void sntp_cb(struct mg_connection *c, int ev, void *evd, void *fnd) {
    if (ev == MG_EV_POLL) {
        mg_req_t * req = (mg_req_t *)fnd ;
        req->poll_times++ ;
        if(++req->poll_times > 3000 ){
            sntp_callback((mg_req_t *)fnd, -1) ;
            c->is_closing = 1 ;
        }
    } else if (ev == MG_EV_CONNECT) {
        if (c->is_resolving) {
            sntp_callback((mg_req_t *)fnd, -2) ;
            c->is_closing = 1 ;
        }
        else {
            uint8_t buf[48] = {0};
            buf[0] = (0 << 6) | (4 << 3) | 3;
            mg_send(c, buf, sizeof(buf));
        }
    } else if (ev == MG_EV_READ) {
        struct timeval tv = {0, 0};
        if (mg_sntp_parse(c->recv.buf, c->recv.len, &tv) == 0) {
            mg_req_t * req = (mg_req_t *)fnd ;
            int64_t ms = tv.tv_sec ;
            ms*= 1000 ;
            sntp_callback((mg_req_t *)fnd, ms) ;
        }
        c->recv.len = 0;  // Clear receive buffer
        c->is_closing = 1 ;
    } else if (ev==MG_EV_ERROR) {
        mg_req_t * req = (mg_req_t *)fnd ;
        sntp_callback((mg_req_t *)fnd, -3) ;
        c->is_closing = 1 ;
    } else if (ev == MG_EV_CLOSE) {
        mg_req_t * req = (mg_req_t *)fnd ;
        JS_FreeValue(req->ctx, req->callback) ;
        free(req) ;
        req = NULL ;
        fnd = NULL ;
    }
    (void) evd;
}

static JSValue js_mg_sntp_request(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(2)

    ARGV_TO_STRING_E(0, url, "arg url must be a string")
    if(!JS_IsFunction(ctx, argv[1])) {
        THROW_EXCEPTION("arg callback must be a function")
    }

    mg_req_t * req = malloc(sizeof(mg_req_t)) ;
    if(!req) {
        THROW_EXCEPTION("out of memory?")
    }
    req->ctx = ctx ;
    req->callback = JS_DupValue(ctx,argv[1]) ;
    req->poll_times = 0 ;    

    struct mg_connection * conn = mg_sntp_connect(&mgr, url, sntp_cb, req) ;
    if(!conn) {
        sntp_callback(req, -4) ;
        free(req) ;
        JS_FreeValue(ctx,argv[1]) ;
    }
    else {
        // mg 内部机制 有 sntp 1小时访问一次的全局限制
        // 取消 mg 的 sntp 实现，在 sntp_cb() 函数中接管
        conn->pfn = NULL ;
    }

    if(url) {
        JS_FreeCString(ctx, url) ;
    }

    return JS_UNDEFINED ;
}

static JSValue js_mg_conn_peer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(1)
    ARGV_TO_UINT16(0, idx)

    struct mg_connection * conn = mgr.conns ;
    uint16_t i = idx ;
    for(; conn && i--; conn=conn->next)
    {}
    if(!conn) {
        THROW_EXCEPTION("conn idx not exist: %d", idx) ;
    }

    char addr[30] ;
    mg_straddr(conn,addr,sizeof(addr)) ;

    return JS_NewString(ctx, addr) ;
}

static JSValue js_mg_conn_cnt(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    int cnt = 0 ;
    for(struct mg_connection * conn = mgr.conns ; conn; conn=conn->next) {
        cnt ++ ;
    }
    return JS_NewUint32(ctx, cnt) ;
}


static JSValue js_mg_get_dns(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_NewString(ctx,mgr.dns4.url) ;
}

static JSValue js_mg_parse_url(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_STRING_E(0,url,"arg url must be a string")

    JSValue obj = JS_NewObject(ctx) ;

    struct mg_str host = mg_url_host(url) ;
    JS_SetPropertyStr(ctx, obj, "host", JS_NewStringLen(ctx,host.ptr,host.len)) ;
    JS_SetPropertyStr(ctx, obj, "port", JS_NewUint32(ctx,mg_url_port(url))) ;
    JS_SetPropertyStr(ctx, obj, "uri", JS_NewString(ctx,mg_url_uri(url))) ;

    JS_FreeCString(ctx,url);

    return obj ;
}

static JSValue js_mg_set_log(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_STRING_E(0, log, "arg loglevel must be a string")

    mg_log_set(log) ;

    JS_FreeCString(ctx, log) ;
    return JS_UNDEFINED ;
}

void be_module_mg_init() {
    mg_mgr_init(&mgr) ;
    // mgr.dns4.url = "udp://1.1.1.1:53";

    JS_NewClassID(&js_mg_http_message_class_id);
    JS_NewClassID(&js_mg_http_rspn_class_id);

    pkgShadow = JS_UNDEFINED ;

    mg_log_set("1") ;

    be_module_mg_client_init() ;
    be_module_mg_server_init() ;
}

void be_module_mg_require(JSContext *ctx) {
    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue mg = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "mg", mg);

    pkgShadow = JS_NewObject(ctx);

    QJS_DEF_CLASS(mg_http_message, "HttpMessage", "mg.HttpMessage", JS_UNDEFINED, pkgShadow)
    QJS_DEF_CLASS(mg_http_rspn, "HttpResponse", "mg.HttpResponse", JS_UNDEFINED, pkgShadow)

    JS_SetPropertyStr(ctx, mg, "sntpRequest", JS_NewCFunction(ctx, js_mg_sntp_request, "sntpRequest", 1));
    JS_SetPropertyStr(ctx, mg, "connCnt", JS_NewCFunction(ctx, js_mg_conn_cnt, "connCnt", 1));
    JS_SetPropertyStr(ctx, mg, "connPeer", JS_NewCFunction(ctx, js_mg_conn_peer, "connPeer", 1));
    JS_SetPropertyStr(ctx, mg, "getDNS", JS_NewCFunction(ctx, js_mg_get_dns, "getDNS", 1));
    JS_SetPropertyStr(ctx, mg, "setLog", JS_NewCFunction(ctx, js_mg_set_log, "setLog", 1));
    JS_SetPropertyStr(ctx, mg, "parseUrl", JS_NewCFunction(ctx, js_mg_parse_url, "parseUrl", 1));

    be_module_mg_client_require(ctx, mg, pkgShadow) ;
    be_module_mg_server_require(ctx, mg, pkgShadow) ;
    
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
    // 关闭所有的 be_http_server_t.conn
}


struct mg_mgr * be_module_mg_mgr() {
    return & mgr ;
}