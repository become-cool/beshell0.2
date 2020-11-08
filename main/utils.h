#ifndef _UTILS_H
#define _UTILS_H

#include "quickjs-libc.h"


#define VAR_REFCNT(var) ((JSRefCountHeader *)JS_VALUE_GET_PTR(var))->ref_count
#define P_VAR_REFCNT(var) printf(#var" ref count:%d @%d\n", VAR_REFCNT(var), __LINE__) ;

#define THROW_EXCEPTION(msg)                                \
    JS_ThrowReferenceError(ctx, msg);                       \
    return JS_EXCEPTION ;

#define CHECK_ARGC(num)                                     \
    if(argc<num) {                                          \
        THROW_EXCEPTION("Missing param path")               \
    }

#define ARGV_TO_INT(i, var, ctype, api)                     \
	ctype var ;                                             \
	if(api(ctx, &var, argv[i]) ) {                          \
        THROW_EXCEPTION("Invalid param type");              \
	}


#define ARGV_TO_UINT8(i,var)   ARGV_TO_INT(i, var, uint8_t, JS_ToUint32)
#define ARGV_TO_UINT32(i,var)  ARGV_TO_INT(i, va, uint8_t, JS_ToUint32)


#define EVAL_STR(str, filename)                                                     \
    {                                                                               \
    JSValue ret = JS_Eval(ctx, str, strlen(str), filename, JS_EVAL_TYPE_GLOBAL) ;   \
	if(JS_IsException(ret)) {                                                       \
		js_std_dump_error(ctx) ;                                                    \
	}                                                                               \
	JS_FreeValue(ctx, ret) ;                                                        \
    }

#define CALL_FUNC(func, thisobj, argc, argv)                                        \
    {                                                                               \
    JSValue ret = JS_Call(ctx, func, thisobj, argc, argv) ;                         \
    if( JS_IsException(ret) ) {                                                     \
        js_std_dump_error(ctx) ;                                                    \
    }                                                                               \
    JS_FreeValue(ctx, ret) ;                                                        \
    }

#define MAKE_ARGV1(argv, arg1)                                                      \
    JSValueConst * argv = malloc(sizeof(JSValue)) ;                                 \
    argv[0] = arg1 ;

#define MAKE_ARGV2(argv, arg1, arg2)                                                \
    JSValueConst * argv = malloc(sizeof(JSValue)) ;                                 \
    argv[0] = arg1 ;                                                                \
    argv[1] = arg2 ;

#define STR1(R)  #R
#define STR2(R)  STR1(R)



#define dd printf("@%d\n", __LINE__) ;
#define dm(msg) printf("%s: %dKB\n", msg, esp_get_free_heap_size()/1024);

#define dref(var,when) printf( "[%s]" #var " ref:%d\n", when, VAR_REFCNT(var)) ;

uint64_t gettime() ;

#endif