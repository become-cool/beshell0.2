#ifndef __H_MODULE_NVS__
#define __H_MODULE_NVS__

#include "quickjs-libc.h"

void load_boot_level_from_nvs() ;

void be_module_nvs_init() ;
void be_module_nvs_require(JSContext *ctx) ;
void be_module_nvs_loop(JSContext *ctx) ;
void be_module_nvs_reset(JSContext *ctx) ;

#endif