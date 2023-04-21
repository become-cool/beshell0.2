#ifndef __H_INDEV_POINTER__
#define __H_INDEV_POINTER__

#include "quickjs-libc.h"

void be_indev_pointer_init() ;
void be_indev_pointer_require(JSContext *ctx, JSValue pkg, JSValue baseProto) ;
void be_indev_pointer_loop(JSContext *ctx) ;
void be_indev_pointer_reset(JSContext *ctx) ;

#endif