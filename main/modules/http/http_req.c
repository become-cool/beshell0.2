#include "http_req.h"
#include "module_http.h"
#include "module_fs.h"
#include "utils.h"
#include "cutils.h"

#define CHUNK_SIZE 1024

static JSClassID js_http_req_class_id;


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

static JSValue req_get_header(JSContext *ctx, httpd_req_t * req, const char * name) {

    size_t val_len = httpd_req_get_hdr_value_len(req, name) ;
    if (val_len < 1) {
        return JS_NULL ;
    }

    char * val = malloc(val_len+1);
    /* Copy null terminated value string into buffer */
    if (httpd_req_get_hdr_value_str(req, "Host", val, val_len+1) != ESP_OK) {
        free(val);
        return JS_NULL ;
    }
    
    JSValue jsval = JS_NewStringLen(ctx, val, val_len) ;
    free(val);

    return jsval ;
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

    ARGV_TO_STRING_LEN_E(0, name, namelen, "http.Request.getHeader() arg name must be a string")

    JSValue jsval = req_get_header(ctx, creq->req, name) ;
    JS_FreeCString(ctx,name) ;

    return jsval ;
}


JSValue js_http_req_get_method(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    REQ_OPAQUE(creq) 
    switch(creq->req->method){
        case HTTP_DELETE:
            return JS_NewString(ctx, "DELETE") ;
        case HTTP_GET:
            return JS_NewString(ctx, "GET") ;
        case HTTP_HEAD:
            return JS_NewString(ctx, "HEAD") ;
        case HTTP_POST:
            return JS_NewString(ctx, "POST") ;
        case HTTP_PUT:
            return JS_NewString(ctx, "PUT") ;
        case HTTP_MOVE:
            return JS_NewString(ctx, "MOVE") ;
        default:
            return JS_NewString(ctx, "unknow") ;
    }   
}
JSValue js_http_req_get_host(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    REQ_OPAQUE(creq)    
    return req_get_header(ctx, creq->req, "Host") ; 
}
JSValue js_http_req_get_path(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    REQ_OPAQUE(creq)    
    return JS_NewString(ctx, creq->req->uri);
}
JSValue js_http_req_get_query(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

    REQ_OPAQUE(creq)
    size_t len = httpd_req_get_url_query_len(creq->req) ;
    if(len<1) {
        return JS_NULL ;
    }
    char * query = malloc(len+1) ;
    if( httpd_req_get_url_query_str(creq->req, query, len+1)!=ESP_OK ) {
        free(query) ;
        return JS_NULL ;
    }

    JSValue jsquery = JS_NewStringLen(ctx, query, len) ;
    free(query) ;

    return jsquery ;
}

inline JSValue http_send_data(JSContext *ctx, httpd_req_t * req, JSValue jsdata) {
    
    size_t datalen = 0 ;
    const char * data = (char *)JS_GetArrayBuffer(ctx, &datalen, jsdata) ;
    if (data) {
        httpd_resp_send_chunk(req, data, datalen);
        return TRUE ;
    }

    else if(JS_IsString(jsdata)) {
        data = JS_ToCStringLen(ctx, &datalen, jsdata) ;
        httpd_resp_send_chunk(req, data, datalen);
        JS_FreeCString(ctx,data) ;
        return TRUE ;
    }

    else {
        THROW_EXCEPTION("arg 1 must be String/ArrayBuffer")
    }
}

JSValue js_http_req_end(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    
    REQ_OPAQUE(creq)

    if(argc>=1) {
        if( JS_IsException(http_send_data(ctx, creq->req, argv[0])) ) {
            return JS_EXCEPTION ;
        }
    }

    httpd_resp_send_chunk(creq->req, NULL, 0);

    return JS_DupValue(ctx, this_val);
}


JSValue js_http_req_rspn_status(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

    CHECK_ARGC(1)
    ARGV_TO_STRING_LEN_E(0, code, codelen, "http response status must be a string, eg. \"200\"")
    REQ_OPAQUE(creq)

    httpd_resp_set_status(creq->req, code) ;

    return JS_DupValue(ctx, this_val);
}

/**
 * 取得 http request header 
 * 
 * @param {string} header name
 * @param {string} header value
 * @return {string}
 */
JSValue js_http_req_rspn_header(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)
    REQ_OPAQUE(creq)
    ARGV_TO_STRING_LEN_E(0, name, namelen, "argv header name must be a string")
    ARGV_TO_STRING_LEN_E(1, value, valuelen, "argv header value must be a string")

    httpd_resp_set_hdr(creq->req, name, value) ;

    JS_FreeCString(ctx, name);
    JS_FreeCString(ctx, value);

    return JS_DupValue(ctx, this_val);
}



JSValue js_http_req_rspn(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    REQ_OPAQUE(creq)    
    if( JS_IsException(http_send_data(ctx, creq->req, argv[0])) ) {
        return JS_EXCEPTION ;
    }
    return JS_DupValue(ctx, this_val);
}


JSValue js_http_req_rspn_file(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    if( !JS_IsString(argv[0]) ) {
        THROW_EXCEPTION("arg path must be a string")
    }
    REQ_OPAQUE(creq)    

    JS2VSFPath(path, argv[0]) 
    CHECK_ARGV0_NOT_DIR(path)

	int fd = fopen(path, "r");
    free(path) ;
    if(fd<0) {
        JS_ThrowReferenceError(ctx, "Failed to open file (%d).", fd);
        return JS_EXCEPTION ;
    }


    char * buff = malloc(CHUNK_SIZE) ;
    if(!buff) {
        free(path) ;
        JS_ThrowReferenceError(ctx, "Failed to malloc buff");
        return JS_EXCEPTION ;
    }

    size_t remain = statbuf.st_size ;

    char * contentlength = mallocf("%d", remain) ;
    httpd_resp_set_hdr(creq->req, "Content-Length", contentlength);

    size_t readed = 0 ;
    while(remain>0) {
        readed = fread(buff, 1, CHUNK_SIZE, fd) ;
        remain-= readed ;
        httpd_resp_send_chunk(creq->req, buff, readed);
    }

    httpd_resp_send_chunk(creq->req, NULL, 0) ;

    fclose(fd) ;
    free(buff) ;
    free(contentlength) ;

    return JS_DupValue(ctx, this_val);
}



static JSValue js_http_req_getter(JSContext *ctx, JSValueConst this_val, JSValueConst val, int magic){
    switch(magic) {
        case 0: 
            return js_http_req_get_method(ctx,this_val,0,NULL) ;
        case 1: 
            return js_http_req_get_host(ctx,this_val,0,NULL) ;
        case 2: 
            return js_http_req_get_path(ctx,this_val,0,NULL) ;
        case 3: 
            return js_http_req_get_query(ctx,this_val,0,NULL) ;
        default:
            return JS_UNDEFINED ;
    }
}

static const JSCFunctionListEntry js_http_req_proto_funcs[] = {
    JS_CFUNC_DEF("getHeader", 0, js_http_req_get_header),
    JS_CFUNC_DEF("getMethod", 0, js_http_req_get_method),
    JS_CFUNC_DEF("getHost", 0, js_http_req_get_host),
    JS_CFUNC_DEF("getPath", 0, js_http_req_get_path),
    JS_CFUNC_DEF("getQuery", 0, js_http_req_get_query),
    JS_CFUNC_DEF("rspnStatus", 0, js_http_req_rspn_status),
    JS_CFUNC_DEF("rspnHeader", 0, js_http_req_rspn_header),
    JS_CFUNC_DEF("rspn", 0, js_http_req_rspn),
    JS_CFUNC_DEF("end", 0, js_http_req_end),
    JS_CFUNC_DEF("rspnFile", 0, js_http_req_rspn_file),

    JS_CGETSET_MAGIC_DEF("method", js_http_req_getter, NULL, 0) ,
    JS_CGETSET_MAGIC_DEF("host", js_http_req_getter, NULL, 1) ,
    JS_CGETSET_MAGIC_DEF("path", js_http_req_getter, NULL, 2) ,
    JS_CGETSET_MAGIC_DEF("query", js_http_req_getter, NULL, 3) ,
};


req_t * http_request(httpd_req_t *req) {

    server_route_t * route = (server_route_t*)req->user_ctx ;
    if(!route || !route->server ) {
        printf("route or route->server is NULL\n") ;
        return NULL ;
    }
    if( !JS_IsFunction(route->ctx, route->callback) ) {
        printf("callback is not a function\n") ;
        return NULL ;
    }

    // req 参数
    req_t * creq = malloc(sizeof(req_t)) ;
    creq->req = req ;
    creq->server = route->server ;
    
    creq->jsreq = JS_NewObjectClass(route->ctx, js_http_req_class_id) ;
    JS_SetOpaque(creq->jsreq, creq);

    // req 继承 EventEmitter 属性
    JSValue reqEventHandles = JS_NewObject(route->ctx) ;
    JS_SetPropertyStr(route->ctx, creq->jsreq, "_handles", reqEventHandles) ;
    
    MAKE_ARGV1(cbargv, creq->jsreq)
    JSValue ret = JS_Call(route->ctx, route->callback, JS_UNDEFINED, 1, cbargv) ;
    if( JS_IsException(ret) ) {
        js_std_dump_error(route->ctx) ;
    }
    JS_FreeValue(route->ctx, ret) ;
    free(cbargv) ;

    return creq ;
}


void http_req_init() {
    // class id 全局, 分配一次
    JS_NewClassID(&js_http_req_class_id);
}


void require_module_http_req(JSContext *ctx, JSValue EventEmitterProto) {
    

    // request
    JS_NewClass(JS_GetRuntime(ctx), js_http_req_class_id, &js_http_req_class);
    JSValue reqProto = JS_NewObject(ctx);
    JS_SetPropertyFunctionList(ctx, reqProto, js_http_req_proto_funcs, countof(js_http_req_proto_funcs));
    JS_SetClassProto(ctx, js_http_req_class_id, reqProto);
    JS_SetPropertyStr(ctx, reqProto, "__proto__", EventEmitterProto);  // 继承自 EventEmitter

}
