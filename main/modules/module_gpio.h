#ifndef _MODULE_GPIO_H
#define _MODULE_GPIO_H

#include "quickjs-libc.h"

void require_module_gpio(JSContext *ctx) ;

void gpio_init() ;
void gpio_on_before_reset(JSContext *ctx) ;

#endif