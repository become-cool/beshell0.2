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
    REQ_OPAQUE(creq)
    ARGV_TO_STRING_E(0, name, namelen, "argv header name must be a string")
    ARGV_TO_STRING_E(1, value, valuelen, "argv header value must be a string")

    JSValue ret = ESP_OK == httpd_resp_set_hdr(creq->req, name, value);

    JS_FreeCString(ctx, name);
    JS_FreeCString(ctx, value);

    return ret ;
}



JSValue js_http_req_write(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    REQ_OPAQUE(creq)    
    return http_send_data(ctx, creq->req, argv[0]) ;
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

    return JS_UNDEFINED ;
}


static const JSCFunctionListEntry js_http_req_proto_funcs[] = {
    JS_CFUNC_DEF("getHeader", 0, js_http_req_get_header),
    // JS_CGETSET_MAGIC_DEF("path", js_http_req_path, js_http_req_path, 0),
    // JS_CGETSET_MAGIC_DEF("host", js_http_req_host, js_http_req_host, 0),
    JS_CFUNC_DEF("writeHeader", 0, js_http_req_write_header),
    JS_CFUNC_DEF("write", 0, js_http_req_write),
    JS_CFUNC_DEF("end", 0, js_http_req_end),
    JS_CFUNC_DEF("rspnFile", 0, js_http_req_rspn_file),
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
