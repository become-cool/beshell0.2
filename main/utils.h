#ifndef _UTILS_H
#define _UTILS_H

#include "quickjs-libc.h"
#include <stdbool.h>

#ifdef SIMULATION

void echo_error(JSContext *) ;

#endif


#define JS_IsFalse(ctx, var) (JS_IsBool(var) && !JS_ToBool(ctx, var))
#define JS_IsTrue(ctx, var) (JS_IsBool(var) && JS_ToBool(ctx, var))

#define VAR_REFCNT(var) ((JSRefCountHeader *)JS_VALUE_GET_PTR(var))->ref_count
#define P_VAR_REFCNT(var) printf(#var" ref count:%d @%d\n", VAR_REFCNT(var), __LINE__) ;

#define THROW_EXCEPTION(...)                                \
    JS_ThrowReferenceError(ctx, __VA_ARGS__);               \
    return JS_EXCEPTION ;

#define CHECK_ARGC(num)                                     \
    if(argc<num) {                                          \
        THROW_EXCEPTION("Missing param")                    \
    }

#define ARGV_TO_INT(i, var, ctype, api)                     \
	ctype var ;                                             \
	if(api(ctx, &var, argv[i]) ) {                          \
        THROW_EXCEPTION("Invalid param type");              \
	}

#define  ARGV_TO_UINT8(i,var)   ARGV_TO_INT(i, var, uint8_t,  JS_ToUint32)
#define   ARGV_TO_INT8(i,var)   ARGV_TO_INT(i, var, int8_t,   JS_ToInt32)
#define ARGV_TO_UINT16(i,var)   ARGV_TO_INT(i, var, uint16_t, JS_ToUint32)
#define ARGV_TO_INT16(i,var)    ARGV_TO_INT(i, var, int16_t,  JS_ToInt32)
#define ARGV_TO_UINT32(i,var)   ARGV_TO_INT(i, var, uint32_t, JS_ToUint32)
#define ARGV_TO_INT32(i,var)    ARGV_TO_INT(i, var, int32_t,  JS_ToInt32)
#define ARGV_TO_INT64(i,var)    ARGV_TO_INT(i, var, int64_t,  JS_ToInt64)
#define ARGV_TO_DOUBLE(i,var)                               \
    double var ;                                            \
    JS_ToFloat64(ctx, &var, argv[i]) ;

#define ARGV_TO_STRING_LEN(i, var, len)                     \
    size_t len = 0 ;                                        \
    char * var = JS_ToCStringLen(ctx, &len, argv[i]) ;
#define ARGV_TO_STRING_LEN_E(i, var, len, msg)              \
    if(!JS_IsString(argv[i])) {                             \
        THROW_EXCEPTION(msg)                                \
    }                                                       \
    ARGV_TO_STRING_LEN(i, var, len)

#define ARGV_TO_STRING(i, var)                              \
    char * var = JS_ToCString(ctx, argv[i]) ;
#define ARGV_TO_STRING_E(i, var, msg)                       \
    if(!JS_IsString(argv[i])) {                             \
        THROW_EXCEPTION(msg)                                \
    }                                                       \
    ARGV_TO_STRING(i, var)

#define ARGV_TO_ARRAYBUFFER(i, var, varlen)                                         \
    size_t varlen = 0;                                                              \
    uint8_t * var = (uint8_t *)JS_GetArrayBuffer(ctx, &varlen, argv[i]) ;           \
    if(!var) {                                                                      \
        THROW_EXCEPTION("argv is not a ArrayBuffer")                                \
    }

#define EVAL_CODE_LEN(str, len, filename)                                           \
    {                                                                               \
    JSValue ret = JS_Eval(ctx, str, len, filename, JS_EVAL_TYPE_GLOBAL|JS_EVAL_FLAG_STRIP) ;           \
	if(JS_IsException(ret)) {                                                       \
		echo_error(ctx) ;                                                           \
	}                                                                               \
	JS_FreeValue(ctx, ret) ;                                                        \
    }

void eval_code_len(JSContext *ctx, const char * str,size_t len,const char * filename) ;

#define EVAL_CODE(str, filename) eval_code_len(ctx, str, strlen(str), filename) ;

#define CALLBACK(func, thisobj, argc, argv)                                         \
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
    JSValueConst * argv = malloc(sizeof(JSValue)*2) ;                               \
    argv[0] = arg1 ;                                                                \
    argv[1] = arg2 ;

#define MAKE_ARGV3(argv, arg1, arg2, arg3)                                          \
    JSValueConst * argv = malloc(sizeof(JSValue)*3) ;                               \
    argv[0] = arg1 ;                                                                \
    argv[1] = arg2 ;                                                                \
    argv[2] = arg3 ;

#define MAKE_ARGV4(argv, arg1, arg2, arg3, arg4)                                    \
    JSValueConst * argv = malloc(sizeof(JSValue)*4) ;                               \
    argv[0] = arg1 ;                                                                \
    argv[1] = arg2 ;                                                                \
    argv[2] = arg3 ;                                                                \
    argv[3] = arg4 ;

#define STR1(R)  #R
#define STR2(R)  STR1(R)



#define pf(...) printf(__VA_ARGS__) ;printf("\n") ;
#define dd printf("@%d\n", __LINE__) ;
#define df(msg) printf("@%d %s\n", __LINE__, msg) ;
#define dm(msg) printf("%s: %dKB\n", msg, esp_get_free_heap_size()/1024);
#define dp(p)   printf(#p"@%p\n", p) ;
#define ds(s)   printf(#s"=%s\n", s) ;
#define dn(v)   printf(#v"=%d @%d\n", v, __LINE__) ;
#define dn64(v)   printf(#v"=%lld\n", v) ;
#define dn2(v1,v2)   printf(#v1"=%d, "#v2"=%d\n", v1, v2) ;

#define YES_OR_NO(exp) printf(#exp"? %s\n", (exp)? "yes": "no") ;
#define IS_NULL(exp) YES_OR_NO((exp)==NULL)

#define dref(var,when) printf( "[%s]" #var " ref:%d\n", when, VAR_REFCNT(var)) ;

uint64_t gettime() ;

char * mallocf(char * format, ...) ;

void freeArrayBuffer(JSRuntime *rt, void *opaque, void *ptr) ;

#define GET_INT_PROP(obj, propName, cvar, error_goto)                               \
    int32_t cvar = 0 ;                                                              \
    {                                                                               \
        JSValue jsvar = JS_GetPropertyStr(ctx, obj, propName) ;                     \
        if( JS_IsUndefined(jsvar) ) {                                               \
            JS_ThrowReferenceError(ctx, "property %s not exists", propName) ;       \
            goto error_goto ;                                                       \
        }                                                                           \
        if( !JS_IsNumber(jsvar) ) {                                                 \
            JS_FreeValue(ctx, jsvar) ;                                              \
            JS_ThrowReferenceError(ctx, "property %s is not a number", propName) ;  \
            goto error_goto ;                                                       \
        }                                                                           \
        JS_ToInt32(ctx, &cvar, jsvar) ;                                             \
    }

#define GET_INT_PROP_DEFAULT(obj, propName, cvar, default, error_goto)                  \
    int32_t cvar = 0 ;                                                                  \
    {                                                                                   \
        JSValue jsvar = JS_GetPropertyStr(ctx, obj, propName) ;                         \
        if( jsvar==JS_UNDEFINED ) {                                                     \
            cvar = default ;                                                            \
        }                                                                               \
        else {                                                                          \
            if( !JS_IsNumber(jsvar) ) {                                                 \
                JS_FreeValue(ctx, jsvar) ;                                              \
                JS_ThrowReferenceError(ctx, "property %s is not a number", propName) ;  \
                goto error_goto ;                                                       \
            }                                                                           \
            JS_ToInt32(ctx, &cvar, jsvar) ;                                             \
        }                                                                               \
    }

#endif

JSValue js_get_prop(JSContext *ctx, JSValue obj, int depth, ...)  ;
JSValue js_get_glob_prop(JSContext *ctx, int depth, ...)  ;

JSValue qjs_def_class(
        JSContext *ctx,
        const char * className,
        JSClassID js_class_id,
        JSClassDef * js_class_p,
        const char * cotrName,
        JSCFunction cotr,
        const JSCFunctionListEntry* funclst,
        uint16_t funcs,
        JSValue parentProto ,
        JSValue pkg
) ;

bool qjs_instanceof(JSContext *ctx, JSValue obj, JSClassID clz_id) ;

#define QJS_DEF_CLASS(typeName, clzName, fullClzName, parentProto, pkgVar)                          \
    qjs_def_class(ctx, clzName, js_##typeName##_class_id, &js_##typeName##_class     \
                , fullClzName, js_##typeName##_constructor, js_##typeName##_proto_funcs, countof(js_##typeName##_proto_funcs), parentProto, pkgVar) ;