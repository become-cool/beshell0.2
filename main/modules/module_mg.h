#ifndef __H_MODULE_MG__
#define __H_MODULE_MG__

#include "quickjs-libc.h"

void be_module_mg_init() ;
void be_module_mg_require(JSContext *ctx) ;
void be_module_mg_loop(JSContext *ctx) ;
void be_module_mg_reset(JSContext *ctx) ;

#endif