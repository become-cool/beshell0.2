#ifndef _EVETNTLOOP_H
#define _EVETNTLOOP_H

#define bool int8_t

#include "quickjs-libc.h"

typedef struct eventloop_callback_t {
    JSValue func ;
    uint64_t deadline ;
    uint32_t interval ;
    bool repeat ;
    int argc ;
    JSValueConst * argv ;
    struct eventloop_callback_t * next ;
}  ;

inline struct eventloop_callback_t * eventloop_header() ;

struct eventloop_callback_t * eventloop_push(JSContext *ctx, JSValue func, int interval, bool repeat) ;
struct eventloop_callback_t * eventloop_push_with_argv(JSContext *ctx, JSValue func, int argc, JSValueConst *argv) ;

void eventloop_pump(JSContext *ctx) ;

void eventloop_remove(JSContext *ctx, struct eventloop_callback_t * item) ;

void eventloop_on_before_reset(JSContext *ctx) ;

#endif