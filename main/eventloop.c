#include "eventloop.h"
#include <time.h>
#include "utils.h"

struct eventloop_callback_t * _callback_stack_top = NULL ;

struct eventloop_callback_t * eventloop_header() {
    return _callback_stack_top ;
}

struct eventloop_callback_t * eventloop_push(JSContext *ctx, JSValue func, int interval, bool repeat) {

    struct eventloop_callback_t * newcb = malloc(sizeof(struct eventloop_callback_t)) ;
    newcb->func = JS_DupValue(ctx, func) ;
    newcb->interval = interval ;
    newcb->repeat = repeat ;
    newcb->next = _callback_stack_top ;
    newcb->deadline = gettime() + interval ;

    _callback_stack_top = newcb ;
    
    return newcb ;
}

void eventloop_out(JSContext *ctx, struct eventloop_callback_t * prev, struct eventloop_callback_t * current) {

    JS_FreeValue(ctx, current->func) ;

    if(prev) {
        prev->next = current->next ;
    }
    else {
        _callback_stack_top = current->next ;
    }

    free(current) ;
}

void eventloop_punp(JSContext *ctx) {
    if(!_callback_stack_top) {
        return ;
    }

    uint64_t now = gettime() ;
    
    struct eventloop_callback_t * prev = NULL, * current = _callback_stack_top;
    while( current!=NULL ) {
        if(current->deadline > now) {
            goto next ;
        }

        if( JS_IsException(JS_Call(ctx, current->func, JS_UNDEFINED, 0, NULL)) ) {
            js_std_dump_error(ctx) ;
        }

        // 一次性任务
        if(!current->repeat) {
            struct eventloop_callback_t * _current = current;
            current = current->next ;
            eventloop_out(ctx, prev, _current) ;
            continue ;
        }

        // 连续重复任务
        if(current->interval==0) {
            current->deadline = now ;
        }
        // 间隔重复
        else {
            uint64_t dur = now - current->deadline ;
            current->deadline+= (dur/current->interval) * current->interval ;
            if( dur%current->interval>0 ){
                current->deadline+= current->interval ;
            }
        }
            
    next:
        prev=current ;
        current=current->next ;
    }
}

void eventloop_remove(JSContext *ctx, struct eventloop_callback_t * item) {
    for(
        struct eventloop_callback_t * prev = NULL, * current = _callback_stack_top;
        current!=NULL ;
        prev=current, current=current->next
    ) {
        if(current == item) {
            eventloop_out(ctx, prev, current) ;
            return ;
        }
    }
}