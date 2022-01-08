#ifndef __H_BE_GL__
#define __H_BE_GL__

#include "quickjs-libc.h"

void be_gl_init() ;
void be_gl_require(JSContext *ctx, JSValue lvgl) ;
void be_gl_loop(JSContext *ctx) ;
void be_gl_reset(JSContext *ctx) ;

#endif