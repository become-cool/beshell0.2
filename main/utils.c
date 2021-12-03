#include "utils.h"
#include <time.h>
#include <stdarg.h>
#include <string.h>

uint64_t gettime() {
	struct timespec tm = {0, 0};
	clock_gettime(CLOCK_REALTIME, &tm);
	return tm.tv_nsec/1000/1000 + tm.tv_sec*1000 ;
}


char * mallocf(char * format, ...) {

	char * str = NULL ;

    va_list argptr;
    va_start(argptr, format);

	size_t len = vsnprintf(NULL, 0, format, argptr) ;
	if(len) {
		str = malloc(len+1) ;
		if(str) {
			vsnprintf(str, len+1, format,argptr ) ;
		}
	}

	va_end(argptr);
	return str ;
}

void freeArrayBuffer(JSRuntime *rt, void *opaque, void *ptr) {
    free(ptr) ;
}

JSValue js_get_prop(JSContext *ctx, JSValue obj, int depth, ...) {
    va_list argptr;
    va_start(argptr, depth);

	char * name ;
	for(int i=0;i<depth-1;i++) {
		name = va_arg(argptr, char *) ;
		obj = JS_GetPropertyStr(ctx,obj,name) ;
		if( !JS_IsObject(obj) ) {
			va_end(argptr);
			return obj ;
		}
		JS_FreeValue(ctx,obj) ;
	}
	name = va_arg(argptr, char *) ;
	va_end(argptr);
	return JS_GetPropertyStr(ctx,obj,name) ;
}

JSValue js_get_glob_prop(JSContext *ctx, int depth, ...) {
	JSValue obj = JS_GetGlobalObject(ctx) ;
	JS_FreeValue(ctx, obj) ;

    va_list argptr;
    va_start(argptr, depth);

	char * name ;
	for(int i=0;i<depth-1;i++) {
		name = va_arg(argptr, char *) ;
		obj = JS_GetPropertyStr(ctx,obj,name) ;
		if( !JS_IsObject(obj) ) {
			va_end(argptr);
			return obj ;
		}
		JS_FreeValue(ctx,obj) ;
	}
	
	name = va_arg(argptr, char *) ;

	va_end(argptr);

	return JS_GetPropertyStr(ctx,obj,name) ;
}