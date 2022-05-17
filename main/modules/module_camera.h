#ifndef __H_MODULE_CAMERA__
#define __H_MODULE_CAMERA__

#include "quickjs-libc.h"

void be_module_camera_init() ;
void be_module_camera_require(JSContext *ctx) ;
void be_module_camera_loop(JSContext *ctx) ;
void be_module_camera_reset(JSContext *ctx) ;

#endif