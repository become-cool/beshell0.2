#ifndef __H_MODULE_MG__
#define __H_MODULE_MG__

#include "quickjs-libc.h"
#include <stdbool.h>

bool mg_url_is_listening(const char * url) ;

const char * mg_event_const_to_name(int ev) ;
int mg_event_name_to_const(const char * evname) ;

void be_module_mg_init() ;
void be_module_mg_require(JSContext *ctx) ;
void be_module_mg_loop(JSContext *ctx) ;
void be_module_mg_reset(JSContext *ctx) ;

#endif