#if !defined(_TELNET_H)
#define _TELNET_H
#include <sys/types.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "quickjs-libc.h"

void telnet_init() ;

void telnet_loop(JSContext *ctx) ;

void telnet_on_before_reset(JSContext *ctx) ;

void telnet_set_input_function(JSContext * ctx, JSValue func) ;

void telnet_echo(const char * sth, int len) ;

#define echo(sth) telnet_echo(sth, strlen(sth))

#define echof(...) 									\
	{												\
		char * str = NULL;							\
		int len = asprintf(&str, __VA_ARGS__) ;		\
        if(len>0)								    \
            telnet_echo(str, len) ;				    \
		if(len>-1) {								\
			free(str) ;								\
		}											\
	}
	

#endif