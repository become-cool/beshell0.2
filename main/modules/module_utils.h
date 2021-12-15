#ifndef _MODULE_UTILS_H
#define _MODULE_UTILS_H

#include "quickjs-libc.h"

void evalScript(JSContext *ctx, const char * path) ;

void be_module_utils_require(JSContext *ctx) ;

#endif