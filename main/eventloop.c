#include "eventloop.h"
#include <time.h>
#include "utils.h"

struct eventloop_callback_t * _callback_stack_top = NULL ;

// struct eventloop_callback_t * eventloop_header() {
//     return _callback_stack_top ;
// }

struct eventloop_callback_t * eventloop_push(JSContext *ctx, JSValue func, int interval, bool repeat) {

    struct eventloop_callback_t * newcb = malloc(sizeof(struct eventloop_callback_t)) ;
    newcb->func = JS_DupValue(ctx, func) ;
    newcb->interval = interval ;
    newcb->repeat = repeat ;
    newcb->next = _callback_stack_top ;
    newcb->deadline = gettime() + interval ;
    newcb->argc = 0 ;
    newcb->argv = NULL ;

    _callback_stack_top = newcb ;
    
    return newcb ;
}



struct eventloop_callback_t * eventloop_push_with_argv(JSContext *ctx, JSValue func, int argc, JSValueConst *argv) {
    struct eventloop_callback_t * newcb = malloc(sizeof(struct eventloop_callback_t)) ;
    newcb->func = JS_DupValue(ctx, func) ;
    newcb->interval = 0 ;
    newcb->repeat = 0 ;
    newcb->next = _callback_stack_top ;
    newcb->deadline = 0 ;
    newcb->argc = argc ;
    newcb->argv = argv ;

    for(int i=0;i<argc; i++) {
        if(JS_VALUE_HAS_REF_COUNT(argv[i])){
            JS_DupValue(ctx, argv[i]) ;
        }
    }

    _callback_stack_top = newcb ;
    return newcb ;
}

void eventloop_out(JSContext *ctx, struct eventloop_callback_t * prev, struct eventloop_callback_t * current) {

    // 回收 argv
    for(int i=0; i<current->argc; i++) {
        if(JS_VALUE_HAS_REF_COUNT(current->argv[i]))
            JS_FreeValue(ctx, current->argv[i]) ;
    }
    free(current->argv) ;
    current->argv = NULL ;

    // 回收 func
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

        JSValue ret = JS_Call(ctx, current->func, JS_UNDEFINED, 0, NULL) ;
        if( JS_IsException(ret) ) {
            js_std_dump_error(ctx) ;
        }
        JS_FreeValue(ctx, ret) ;

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

void eventloop_on_before_reset(JSContext *ctx){
    // 清空未决事件表
    for(
        struct eventloop_callback_t * prev = NULL, * current = _callback_stack_top;
        current!=NULL ;
        prev=current, current=current->next
    ) {
        eventloop_out(ctx, prev, current) ;
    }
    _callback_stack_top = NULL ;
}