#include "module_utils.h"
#include "module_utils.h"
#include "logging.h"
#include "esp_system.h"
#include <string.h>
#include <time.h>
#include "eventloop.h"
#include "utils.h"
#include "module_fs.h"

LOG_TAG("util");

JSValue js_util_free_stacks(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    (void) argc ;
    (void) argv ;
    return JS_NewInt32(ctx, esp_get_free_heap_size()) ;
}

typedef struct {
	char *          levelString;
	esp_log_level_t level;
} level_t;
static level_t levels[] = {
	{"none",    ESP_LOG_NONE},
	{"error",   ESP_LOG_ERROR},
	{"warn",    ESP_LOG_WARN},
	{"info",    ESP_LOG_INFO},
	{"debug",   ESP_LOG_DEBUG},
	{"verbose", ESP_LOG_VERBOSE},
	{NULL, 0}
};

JSValue js_util_set_log_level(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(argc<2) {
        JS_ThrowReferenceError(ctx, "Missing param");
        return JS_EXCEPTION ;
    }
	char * tagToChange = (char *)JS_ToCString(ctx, argv[0]);
	char * levelString = (char *)JS_ToCString(ctx, argv[1]);

	// printf("Setting a new log level to be tag: \"%s\", level: \"%s\"\n", tagToChange, levelString);

	level_t *pLevels = levels;
	while(pLevels->levelString != NULL) {
		if (strcmp(pLevels->levelString, levelString) == 0) {
			break;
		}
		pLevels++;
	}
	if (pLevels->levelString != NULL) {
		esp_log_level_set(tagToChange, pLevels->level);
	    return JS_UNDEFINED ;
	}
    else {
        JS_ThrowReferenceError(ctx, "missing param path");
        return JS_EXCEPTION ;
    }
}

JSValue js_util_time(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	struct timespec tm = {0, 0};
	clock_gettime(CLOCK_REALTIME, &tm);
	return JS_NewInt64(ctx, tm.tv_nsec/1000/1000 + tm.tv_sec*1000) ;
}

JSValue js_util_var_ptr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param");
        return JS_EXCEPTION ;
	}
	return JS_NewInt32(ctx, (uint32_t)argv[0]) ;
}
JSValue js_util_ptr_refcount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param");
        return JS_EXCEPTION ;
	}
	uint32_t ptr ;
	if(JS_ToUint32(ctx, &ptr, argv[0]) ) {
        JS_ThrowReferenceError(ctx, "Invalid param type");
        return JS_EXCEPTION ;
	}
	return JS_NewInt32(ctx, JS_VALUE_HAS_REF_COUNT((JSValue) ptr)? VAR_REFCNT((JSValue) ptr): 0 ) ;
}
JSValue js_util_var_refcount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param");
        return JS_EXCEPTION ;
	}
	return JS_NewInt32(ctx, JS_VALUE_HAS_REF_COUNT(argv[0])? VAR_REFCNT(argv[0]): 0) ;
}


JSValue __js_util_set_timer(JSContext *ctx, int argc, JSValueConst *argv, bool repeat){
    if(argc<2) {
        JS_ThrowReferenceError(ctx, "Missing param");
        return JS_EXCEPTION ;
    }
	uint32_t interval ;
	if(JS_ToUint32(ctx, &interval, argv[1]) ) {
        JS_ThrowReferenceError(ctx, "Invalid param type");
        return JS_EXCEPTION ;
	}
	uint32_t addr = eventloop_push(ctx, (JSValue)argv[0], interval, repeat) ;
	return JS_NewInt32(ctx, addr) ;
}

JSValue js_util_set_timeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	return __js_util_set_timer(ctx, argc, argv, false) ;
}
JSValue js_util_set_interval(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	return __js_util_set_timer(ctx, argc, argv, true) ;
}

JSValue js_util_clear_timeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param");
        return JS_EXCEPTION ;
    }
	uint32_t ptr ;
	if(JS_ToUint32(ctx, &ptr, argv[0]) ) {
        JS_ThrowReferenceError(ctx, "Invalid param type");
        return JS_EXCEPTION ;
	}
	eventloop_remove(ctx, (struct eventloop_callback_t *)ptr) ;
	return JS_UNDEFINED ;
}


#define require_wrapper 												\
	"let module = {exports={}} ;\n"										\
	"(function(exports, require, module, __filename, __dirname) {\n" 	\
	"%s" 																\
	"})(exports); "
JSValue js_util_require_abspath(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	CHECK_ARGC(1)
	char * path = JS_ToCString(ctx, argv[0]) ;


	JS_FreeCString(ctx, path) ;
	return JS_UNDEFINED ;
}

void evalScript(JSContext *ctx, const char * path) {

    struct stat statbuf;
    if(stat(path,&statbuf)<0) {
        LOGE("Path not exists: %s\n", path) ;
        return NULL ;
    }
    if(S_ISDIR(statbuf.st_mode)){
        LOGE("Path is directory: %s\n", path) ;
        return NULL ;
    }
    char * buff = malloc(statbuf.st_size+1) ;
    if(!buff) {
        LOGE("Cound not malloc buff(%d), memory low?\n", (int)statbuf.st_size+1) ;
        return NULL ;
    }
	int fd = fopen(path, "r") ;
    if(fd<0) {
        LOGE("Cound not open path: %s\n", path) ;
        return NULL ;
    }

    int readedBytes = fread(buff, 1, statbuf.st_size, fd) ;
    fclose(fd) ;
    buff[readedBytes] = 0 ;

    JSValue ret = JS_Eval(ctx, buff, readedBytes, ":eval", JS_EVAL_TYPE_GLOBAL) ;
    if( JS_IsException(ret) ) {
        js_std_dump_error(ctx) ;
    }

	JS_FreeValue(ctx, ret) ;
    free(buff) ;
}

JSValue js_fs_eval_script(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    JS2VSFPath(path, argv[0])
    CHECK_NOT_DIR(path)
    evalScript(ctx, path) ;
    free(path) ;
	return JS_UNDEFINED ;
}

#define JSCODE_DEF_PROCESS_OBJECT "global.process= { env:{ LOGNAME: 'become', HOME: '/home/become', PWD: '/home/become' } }"

void require_module_utils(JSContext *ctx) {

    JSValue global = JS_GetGlobalObject(ctx);
    JS_SetPropertyStr(ctx, global, "global", global);

	// utils 
    JSValue utils = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, global, "utils", utils);
    JS_SetPropertyStr(ctx, utils, "freeStacks", JS_NewCFunction(ctx, js_util_free_stacks, "freeStacks", 1));
    JS_SetPropertyStr(ctx, utils, "time", JS_NewCFunction(ctx, js_util_time, "time", 1));
    JS_SetPropertyStr(ctx, utils, "setLogLevel", JS_NewCFunction(ctx, js_util_free_stacks, "setLogLevel", 1));
    JS_SetPropertyStr(ctx, utils, "ptrRefCnt", JS_NewCFunction(ctx, js_util_ptr_refcount, "ptrRefCnt", 1));
    JS_SetPropertyStr(ctx, utils, "varRefCnt", JS_NewCFunction(ctx, js_util_var_refcount, "varRefCnt", 1));
    JS_SetPropertyStr(ctx, utils, "varPtr", JS_NewCFunction(ctx, js_util_var_ptr, "varPtr", 1));

	// process
	EVALSTR(JSCODE_DEF_PROCESS_OBJECT, ":eval")

	// global
    JS_SetPropertyStr(ctx, global, "setTimeout", JS_NewCFunction(ctx, js_util_set_timeout, "setTimeout", 1));
    JS_SetPropertyStr(ctx, global, "setInterval", JS_NewCFunction(ctx, js_util_set_interval, "setInterval", 1));
    JS_SetPropertyStr(ctx, global, "clearTimeout", JS_NewCFunction(ctx, js_util_clear_timeout, "clearTimeout", 1));
    JS_SetPropertyStr(ctx, global, "evalScript", JS_NewCFunction(ctx, js_fs_eval_script, "evalScript", 1));
    
	JS_FreeValue(ctx, global);

	// EVALSTR(JSCODE_EVAL_SCRIPT, ":eval")
}