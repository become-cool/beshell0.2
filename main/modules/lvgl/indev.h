#ifndef __H_INDEV__
#define __H_INDEV__

#include "quickjs-libc.h"

void be_lv_indev_reset(JSContext * ctx) ;
void be_lv_indev_init() ;
void be_lv_indev_require(JSContext *ctx, JSValue lvgl) ;

#endif