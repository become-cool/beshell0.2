#ifndef __H_INDEV_I2C__
#define __H_INDEV_I2C__

#include "quickjs-libc.h"

extern JSClassID js_lv_indev_nav_class_id ;

void be_indev_i2c_init() ;
void be_indev_i2c_require(JSContext *ctx, JSValue pkg, JSValue baseProto) ;
void be_indev_i2c_loop(JSContext *ctx) ;
void be_indev_i2c_reset(JSContext *ctx) ;

#endif