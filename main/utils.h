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


#define dd printf("@%d\n", __LINE__)
#define dm(msg) printf("%s: %dKB\n", msg, esp_get_free_heap_size()/1024);

uint64_t gettime() ;

#endif