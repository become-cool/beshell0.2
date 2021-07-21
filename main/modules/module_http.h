#ifndef _MODULE_HTTP_H
#define _MODULE_HTTP_H

#include "quickjs-libc.h"


void require_module_http(JSContext *ctx) ;

void http_init() ;
void http_on_before_reset(JSContext *ctx) ;

#endif