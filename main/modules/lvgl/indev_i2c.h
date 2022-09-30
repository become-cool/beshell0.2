#ifndef __H_INDEV_I2C__
#define __H_INDEV_I2C__

#include "quickjs-libc.h"

bool indev_nav_read_i2c(indev_driver_spec_t* spec, uint8_t * byte) ;

extern JSClassID js_lv_indev_nav_class_id ;

void be_indev_i2c_init() ;
void be_indev_i2c_require(JSContext *ctx, JSValue pkg, JSValue baseProto) ;
void be_indev_i2c_loop(JSContext *ctx) ;
void be_indev_i2c_reset(JSContext *ctx) ;

#endif