#ifndef __H_MODULE_DRIVERS__
#define __H_MODULE_DRIVERS__

#include "quickjs-libc.h"

void be_module_drivers_init() ;
void be_module_drivers_require(JSContext *ctx) ;
void be_module_drivers_loop(JSContext *ctx) ;
void be_module_drivers_reset(JSContext *ctx) ;

#endif