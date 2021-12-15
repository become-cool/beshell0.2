#ifndef __H_REPL__
#define __H_REPL__

#include "quickjs-libc.h"

void be_module_repl_init() ;

void be_module_repl_loop(JSContext * ctx) ;

#endif