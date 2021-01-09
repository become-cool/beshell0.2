#ifndef _UTILS_H
#define _UTILS_H

#include "quickjs-libc.h"
// #include "telnet.h"

#define VAR_REFCNT(var) ((JSRefCountHeader *)JS_VALUE_GET_PTR(var))->ref_count
#define P_VAR_REFCNT(var) printf(#var" ref count:%d @%d\n", VAR_REFCNT(var), __LINE__) ;

#define THROW_EXCEPTION(msg)                                \
    JS_ThrowReferenceError(ctx, msg);                       \
    return JS_EXCEPTION ;

#define THROW_FORMAT(format, ...)                           \
    {                                                       \
        char * msg = mallocf(__VA_ARGS__) ;                 \
        if( msg ) {                                         \
            JS_ThrowReferenceError(ctx, msg);               \
            free(msg) ;                                     \
        }                                                   \
        else {                                              \
            JS_ThrowReferenceError(ctx, format);            \
        }                                                   \
        return JS_EXCEPTION ;                               \
    }

#define CHECK_ARGC(num)                                     \
    if(argc<num) {                                          \
        THROW_EXCEPTION("Missing param")               \
    }

#define ARGV_TO_INT(i, var, ctype, api)                     \
	ctype var ;                                             \
	if(api(ctx, &var, argv[i]) ) {                          \
        THROW_EXCEPTION("Invalid param type");              \
	}


#define  ARGV_TO_UINT8(i,var)   ARGV_TO_INT(i, var, uint8_t,  JS_ToUint32)
#define ARGV_TO_UINT16(i,var)   ARGV_TO_INT(i, var, uint16_t, JS_ToUint32)
#define ARGV_TO_UINT32(i,var)   ARGV_TO_INT(i, var, uint32_t, JS_ToUint32)
#define ARGV_TO_DOUBLE(i,var)                               \
    double var ;                                            \
    JS_ToFloat64(ctx, &var, argv[i]) ;
#define ARGV_TO_STRING(i, var, len)                         \
    size_t len = 0 ;                                        \
    char * var = JS_ToCStringLen(ctx, &len, argv[i]) ;
#define ARGV_TO_ARRAYBUFFER(i, var, varlen)                                         \
    size_t varlen = 0;                                                              \
    char * var = (char *)JS_GetArrayBuffer(ctx, &varlen, argv[i]) ;                 \
    if(!var) {                                                                      \
        THROW_EXCEPTION("argv is not a ArrayBuffer")                                \
    }


#define EVAL_CODE_LEN(str, len, filename)                                           \
    {                                                                               \
    JSValue ret = JS_Eval(ctx, str, len, filename, JS_EVAL_TYPE_GLOBAL) ;           \
	if(JS_IsException(ret)) {                                                       \
		echo_error(ctx) ;                                                           \
	}                                                                               \
	JS_FreeValue(ctx, ret) ;                                                        \
    }

#define EVAL_CODE(str, filename) EVAL_CODE_LEN(str, strlen(str), filename)

#define CALL_FUNC(func, thisobj, argc, argv)                                        \
    {                                                                               \
    JSValue ret = JS_Call(ctx, func, thisobj, argc, argv) ;                         \
    if( JS_IsException(ret) ) {                                                     \
        echo_error(ctx) ;                                                           \
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



#define pf(...) printf(__VA_ARGS__) ;printf("\n") ;
#define dd printf("@%d\n", __LINE__) ;
#define df(msg) printf("@%d %s\n", __LINE__, msg) ;
#define dm(msg) printf("%s: %dKB\n", msg, esp_get_free_heap_size()/1024);
#define dn(v)   printf(#v"=%d\n", v) ;
#define dn2(v1,v2)   printf(#v1"=%d, "#v2"=\n", v1, v2) ;

#define dref(var,when) printf( "[%s]" #var " ref:%d\n", when, VAR_REFCNT(var)) ;

uint64_t gettime() ;

char * mallocf(char * format, ...) ;

void freeArrayBuffer(JSRuntime *rt, void *opaque, void *ptr) ;

#endif