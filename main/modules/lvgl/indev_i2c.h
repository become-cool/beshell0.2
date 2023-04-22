#ifndef __H_INDEV_I2C__
#define __H_INDEV_I2C__

#include "quickjs-libc.h"
#include "list.h"
#include "indev.h"


bool indev_nav_read_i2c(indev_driver_spec_t* spec, uint8_t * byte) ;
uint8_t indev_nav_take_value(indev_driver_spec_t * spec) ;

void be_indev_i2c_init() ;
void be_indev_i2c_require(JSContext *ctx, JSValue pkg, JSValue baseProto) ;
void be_indev_i2c_reset(JSContext *ctx) ;
void be_indev_i2c_loop(JSContext *ctx) ;

#endif