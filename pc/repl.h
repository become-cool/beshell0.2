#ifndef __H_REPL__
#define __H_REPL__

#include "quickjs-libc.h"

void repl_init() ;

void repl_loop(JSContext * ctx) ;

#endif