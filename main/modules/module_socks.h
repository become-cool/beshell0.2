#ifndef _MODULE_SOCKS_H
#define _MODULE_SOCKS_H

#include "quickjs-libc.h"

void socks_init() ;
void socks_udp_loop(JSContext *ctx) ;
void socks_on_before_reset(JSContext *ctx) ;

void require_module_socks(JSContext *ctx) ;

#endif