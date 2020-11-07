#ifndef _MODULE_UTILS_H
#define _MODULE_UTILS_H

#include "quickjs-libc.h"

void evalScript(JSContext *ctx, const char * path) ;

void require_module_utils(JSContext *ctx) ;

#endif