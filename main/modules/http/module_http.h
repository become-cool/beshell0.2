#ifndef _MODULE_HTTP_H
#define _MODULE_HTTP_H

#include "http_struct.h"
#include "http_req.h"
#include "http_route.h"


#define SERVER_THIS(cvar)                                                                   \
    if(this_val==JS_UNDEFINED || this_val==JS_NULL) {                                       \
        THROW_EXCEPTION("This function must be called as a method of http.Server object")   \
    }                                                                                       \
    server_t * cvar = this_http_server(this_val) ;                                          \
    if(!cvar) {                                                                             \
        THROW_EXCEPTION("Invalid http.Server object")                                       \
    }


server_t * http_server_new(JSContext * owner, httpd_handle_t handle) ;
esp_err_t http_server_listen(server_t * cserver, uint16_t port) ;
esp_err_t http_server_close(server_t * cserver) ;

void require_module_http(JSContext *ctx) ;

void http_init() ;
void http_on_before_reset(JSContext *ctx) ;

server_t * this_http_server(JSValue this_val) ;

#endif