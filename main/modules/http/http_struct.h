#ifndef __H_MODULE_HTTP_STRUCT___
#define __H_MODULE_HTTP_STRUCT___

#include "quickjs-libc.h"
#include "esp_http_server.h"
#include "list.h"


typedef struct {
    httpd_handle_t handle ;
    JSContext * owner ;
    JSValue jsserver ;
    list_t lst_route ;
} server_t;


typedef struct {
    server_t * server ;
    JSContext * ctx ;
    JSValue callback ;
} server_route_t ;


typedef struct {
    httpd_req_t * req ;
    server_t * server ;
    JSValue jsreq ;
} req_t;


#endif