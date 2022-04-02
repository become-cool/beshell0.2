#ifndef _MODULE_UTILS_H
#define _MODULE_UTILS_H

#include "quickjs-libc.h"
#include <stdbool.h>

int readPartId() ;
int readPartVersion() ;

void evalScript(JSContext *ctx, const char * path, bool asBin) ;

void be_module_utils_require(JSContext *ctx) ;

#endif