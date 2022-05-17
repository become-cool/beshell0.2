#ifndef _MODULE_SERIAL_H
#define _MODULE_SERIAL_H

#include "quickjs-libc.h"
#include "driver/spi_master.h"
#include "driver/i2c.h"

#define I2C_IS_SETUP(busnum) (_i2c_bus_setup & (1<<(busnum)))

#define ARGV_I2C_BUSNUM(i, var)                     \
    ARGV_TO_UINT8(i, var)                           \
    if(var<0 || var>1) {                            \
        THROW_EXCEPTION("Bus num must be 0-1")      \
    }

#define I2C_BEGIN(addr, act)                                        \
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();                   \
	i2c_master_start(cmd);                                          \
    i2c_master_write_byte(cmd, (addr << 1) | I2C_MASTER_##act, true);
    
#define I2C_BEGIN_READ(addr)    I2C_BEGIN(addr, READ)
#define I2C_BEGIN_WRITE(addr)   I2C_BEGIN(addr, WRITE)

#define I2C_RECV(buffer, len)                                       \
    if(len>1) {                                                     \
	    i2c_master_read(cmd, buffer, len-1, I2C_MASTER_ACK);        \
	    i2c_master_read(cmd, (buffer)+len-1, 1, I2C_MASTER_NACK);   \
    }                                                               \
    else {                                                          \
	    i2c_master_read(cmd, buffer, 1, I2C_MASTER_NACK);           \
    }

#define I2C_COMMIT(bus)                                                     \
	i2c_master_stop(cmd);                                                   \
	esp_err_t res=i2c_master_cmd_begin(bus, cmd, 10/portTICK_PERIOD_MS) ;   \
	i2c_cmd_link_delete(cmd);

#define I2C_READ_INT(var, type, size)                                   \
    CHECK_ARGC(3)                                                       \
    ARGV_I2C_BUSNUM(0, busnum)                                          \
    if(!I2C_IS_SETUP(busnum))                                           \
        return JS_NULL ;                                                \
    ARGV_TO_UINT8(1, addr)                                              \
    ARGV_TO_UINT8(2, reg)                                               \
    type var = 0 ;                                                      \
    if( i2c_read(busnum, addr, reg, (uint8_t*)&var, size)!=ESP_OK ) {   \
        return JS_NULL ;                                                \
    }

#define FREE_BUS_SPI(busnum)    if(_spi_bus_setup&(1<<(busnum))){ dn(busnum); spi_bus_free(busnum); }
#define FREE_BUS_I2C(busnum)    if(_i2c_bus_setup&(1<<(busnum))){ dn(busnum); i2c_driver_delete(busnum); }


void be_module_serial_require(JSContext *ctx) ;

void be_module_serial_reset(JSContext *ctx) ;

#endif