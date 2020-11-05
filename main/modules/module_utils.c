#include "module_utils.h"
#include "logging.h"
#include "esp_system.h"
#include <string.h>

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
    printf("%d\n", argc) ;
    if(argc<2) {
        dd
        return JS_UNDEFINED ;
    }
	char * tagToChange = (char *)JS_ToCString(ctx, argv[0]);
	char * levelString = (char *)JS_ToCString(ctx, argv[1]);

	printf("Setting a new log level to be tag: \"%s\", level: \"%s\"\n", tagToChange, levelString);

	level_t *pLevels = levels;
	while(pLevels->levelString != NULL) {
		if (strcmp(pLevels->levelString, levelString) == 0) {
			break;
		}
		pLevels++;
	}
	if (pLevels->levelString != NULL) {
        dd
		esp_log_level_set(tagToChange, pLevels->level);
	    return JS_UNDEFINED ;
	}
    else {
        dd
        JS_ThrowReferenceError(ctx, "missing param path");
        return JS_EXCEPTION ;
    }
}


void require_module_utils(JSContext *ctx) {
    JSValue global = JS_GetGlobalObject(ctx);
    JSValue utils = JS_NewObject(ctx);
    
    JS_SetPropertyStr(ctx, utils, "freeStacks", JS_NewCFunction(ctx, js_util_free_stacks, "freeStacks", 1));
    JS_SetPropertyStr(ctx, utils, "setLogLevel", JS_NewCFunction(ctx, js_util_free_stacks, "setLogLevel", 1));
    JS_SetPropertyStr(ctx, global, "utils", utils);

    JS_FreeValue(ctx, global);
}