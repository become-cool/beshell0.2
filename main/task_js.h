#ifndef _TASK_JS_H
#define _TASK_JS_H

#include "quickjs-libc.h"

void task_js_main() ;
JSContext * task_current_context() ;

uint8_t task_boot_level() ;
void task_reset(int level, char * script) ;

#endif