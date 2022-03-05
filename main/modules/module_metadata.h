#ifndef __H_MODULE_METADATA__
#define __H_MODULE_METADATA__

#include "quickjs-libc.h"

void module_metadata_init() ;
void module_metadata_require(JSContext *ctx) ;
void module_metadata_loop(JSContext *ctx) ;
void module_metadata_reset(JSContext *ctx) ;

#endif