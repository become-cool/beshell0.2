#if !defined(_TELNET_H)
#define _TELNET_H
#include <sys/types.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "telnet_protocal.h"

#include "quickjs-libc.h"

void be_module_telnet_init() ;

void be_module_telnet_loop(JSContext *ctx) ;

void be_module_telnet_reset(JSContext *ctx) ;

void telnet_set_input_function(JSContext * ctx, JSValue func) ;

void telnet_send_pkg(char reqid, char cmd, char * data, uint16_t datalen) ;
void telnet_send_pkg_str(char reqid, char cmd, char * data) ;
void telnet_send_ready() ;

void be_module_telnet_require(JSContext *ctx) ;

uint8_t echo_pkgid() ;

#define echo(sth) 		telnet_send_pkg_str(echo_pkgid(), CMD_OUTPUT, sth)
#define echon(sth, len) telnet_send_pkg(echo_pkgid(), CMD_OUTPUT, sth, len)

#define echof(...) 													\
	{																\
		char * str = NULL;											\
		int len = asprintf(&str, __VA_ARGS__) ;						\
        if(len>0)								    				\
            telnet_send_pkg(echo_pkgid(), CMD_OUTPUT, str, len) ;	\
		if(len>-1) {												\
			free(str) ;												\
		}															\
	}
	
void echo_error(JSContext * ctx) ;

#endif