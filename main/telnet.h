#if !defined(_TELNET_H)
#define _TELNET_H
#include <sys/types.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "quickjs-libc.h"

void telnet_init() ;

void telnet_loop(JSContext *ctx) ;


#endif