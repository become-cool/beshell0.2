#ifndef __H_MODULE_HTTP_REQ__
#define __H_MODULE_HTTP_REQ__

#include "http_struct.h"


void http_req_init() ;
void require_module_http_req(JSContext *ctx, JSValue EventEmitterProto) ;

req_t * http_request(httpd_req_t *req)  ;

#endif