#ifndef __H_MODULE_HTTP_ROUTE__
#define __H_MODULE_HTTP_ROUTE__

#include "http_struct.h"


JSValue js_http_server_route(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;
JSValue js_http_server_route_ws(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) ;


#endif