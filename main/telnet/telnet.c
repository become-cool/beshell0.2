#include "telnet.h"
#include "telnet_stdio.h"
#include "telnet_ws.h"
#include "telnet_serial.h"

#include "module_fs.h"
#include "utils.h"
#include "beshell.h"
#include "task_js.h"
#include <math.h>



JSValue _func_repl_input ;
JSValue js_repl_register_handle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    if(!JS_IsFunction(ctx, argv[0])) {
        THROW_EXCEPTION("REPL input function must be a function type")
    }
    
	if(!JS_IsNull(_func_repl_input)) {
		JS_FreeValue(ctx, _func_repl_input) ;
	}

	_func_repl_input = JS_DupValue(ctx, argv[0]) ;

    return JS_UNDEFINED ;
}


uint8_t _echo_pkgid = 0 ;
uint8_t mk_echo_pkgid() {
	return _echo_pkgid ++ ;
}


void telnet_output(uint8_t cmd, int pkgid, const char * data, size_t datalen) {
	// for WebSocket(wifi)
    telnet_ws_output(cmd, pkgid, data, datalen) ;
	
	// for serial(usb) or stdio(simulators)
#ifdef SIMULATION
    printf("%.*s\n", datalen, data) ;
    fflush(stdout) ;
#else
	// ds(data)
	telnet_serial_send_pkg(pkgid, cmd, data, datalen) ;
#endif
}


void telnet_run(JSContext * ctx, uint8_t pkgid, uint8_t remain, uint8_t cmd, uint8_t * data, uint8_t datalen) {
	if(!JS_IsNull(_func_repl_input) && JS_IsFunction(ctx, _func_repl_input)) {
		JSValueConst * argv = malloc(sizeof(JSValue)*4) ;
		argv[0] = JS_NewInt32(ctx, pkgid) ;
		argv[1] = JS_NewInt32(ctx, remain) ;
		argv[2] = JS_NewInt32(ctx, cmd) ;
		argv[3] = JS_NewStringLen(ctx, (char *)data, datalen) ;

		// printf("%s\n", data) ;

		JSValue ret = JS_Call(ctx, _func_repl_input, JS_NULL, 4, argv) ;
		if( JS_IsException(ret) ) {
			echo_error(ctx) ;
		}

		JS_FreeValue(ctx, ret) ;
		JS_FreeValue(ctx, argv[3]) ;
		free(argv) ;
	}
}


void js_dump_err(JSContext *ctx, JSValueConst val) {
    const char * str = JS_ToCString(ctx, val);
    if (str) {
		printf(str);
        JS_FreeCString(ctx, str);
    } else {
        printf("[exception]\n");
    }
    fflush(stdout);
}

void echo_error(JSContext * ctx) {
    JSValue exception_val = JS_GetException(ctx);
    bool is_error = JS_IsError(ctx, exception_val);
    js_dump_err(ctx, exception_val);
    if (is_error) {
        JSValue val = JS_GetPropertyStr(ctx, exception_val, "stack");
        if (!JS_IsUndefined(val)) {
            js_dump_err(ctx, val);
        }
        JS_FreeValue(ctx, val);
    }
    JS_FreeValue(ctx, exception_val);
}



void telnet_send_ready() {
	char * buff = mallocf("{\"firmware\":\"beshell\",\"version\":\"%s\",\"level\":%d}", BESHELL_VERSION, task_boot_level()) ;
    telnet_output(0,CMD_READY,buff, strlen(buff)) ;
	free(buff) ;
}



JSValue js_repl_rspn(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	CHECK_ARGC(3)
	ARGV_TO_UINT8(0, pkgid)
	ARGV_TO_UINT8(1, cmd)
	ARGV_TO_STRING_LEN(2, data, datalen)

    telnet_output(cmd, pkgid, data, datalen) ;

	JS_FreeCString(ctx, data) ;
	
    return JS_UNDEFINED ;
}




void be_telnet_init() {
    _func_repl_input = JS_NULL ;

	be_telnet_ws_init() ;

#ifdef SIMULATION
	be_telnet_stdio_init() ;
#else
	be_telnet_serial_init() ;
#endif

}

void be_telnet_require(JSContext *ctx) {
    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beapi = JS_GetPropertyStr(ctx, global, "beapi") ;


    JSValue telnet = JS_NewObject(ctx) ;
    JS_SetPropertyStr(ctx, beapi, "telnet", telnet);
    JS_SetPropertyStr(ctx, telnet, "rspn", JS_NewCFunction(ctx, js_repl_rspn, "rspn", 1));
    JS_SetPropertyStr(ctx, telnet, "registerHandle", JS_NewCFunction(ctx, js_repl_register_handle, "registerHandle", 1));

	JS_FreeValue(ctx, beapi);
	JS_FreeValue(ctx, global);
	
#ifdef SIMULATION
	be_telnet_stdio_require(ctx) ;
#else
#endif

}
void be_telnet_loop(JSContext *ctx) {
	be_telnet_stdio_loop(ctx) ;
	
#ifdef SIMULATION
	be_telnet_stdio_loop(ctx) ;
#else
#endif
}

void be_telnet_reset(JSContext *ctx) {
    JS_FreeValue(ctx, _func_repl_input) ;
    _func_repl_input = JS_NULL ;

#ifdef SIMULATION
	be_telnet_stdio_reset(ctx) ;
#else
#endif
}