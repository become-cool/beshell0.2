#include "utils.h"
#include "module_serial.h"
#include "driver/spi_master.h"
#include <string.h>

#define DMA_CHAN        2

uint8_t _spi_bus_setup = 0 ;
spi_device_handle_t _spi_handle_pool1[8] = {0,0,0,0,0,0,0,0} ;

int8_t find_free_spi_handle(spi_device_handle_t ** pool) {
    int8_t h = 0 ;
    for(h=0; h<8; h++) {
        if(_spi_handle_pool1[h]==NULL) {
            *pool = & _spi_handle_pool1 ;
            return h ;
        }
    }
    return -1 ;
}

spi_device_handle_t _spi_handle(uint8_t idx) {
    if(idx<8) {
        return _spi_handle_pool1[idx] ;
    }
    return NULL ;
}

/**
 * spi bus num 1-3
 * clk
 * mosi
 * miso
 */
JSValue js_spi_spi_bus_setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

    CHECK_ARGC(4)

    ARGV_TO_UINT8(0, busnum)
    if(busnum<1 || busnum>3) {
        THROW_EXCEPTION("Bus num must be 1-3")
    }
    ARGV_TO_UINT8(1, clkpin)

    int8_t mosipin = -1 ;
    int8_t misopin = -1 ;
    if(!JS_IsUndefined(argv[2])) {
        if( JS_ToUint32(ctx, &mosipin, argv[2])!=0 ){
            THROW_EXCEPTION("MOSI pin invalid.")
        }
    }
    if(!JS_IsUndefined(argv[3])) {
        if( JS_ToUint32(ctx, &misopin, argv[3])!=0 ){
            THROW_EXCEPTION("MISO pin invalid.")
        }
    }

    // pf("mosi=%d, miso=%d, clk=%d", mosipin, misopin, clkpin)

    spi_bus_config_t buscfg = {
        .miso_io_num=misopin,
        .mosi_io_num=mosipin,
        .sclk_io_num=clkpin,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
        .max_transfer_sz=20480
    } ;

    esp_err_t ret = spi_bus_initialize(busnum, &buscfg, DMA_CHAN);
    if(ret==0) {
        _spi_bus_setup|= 1<<(busnum-1) ;
    }

    return JS_NewInt32(ctx, ret) ;
}
JSValue js_spi_bus_free(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, busnum)
    if(busnum<1 || busnum>3) {
        THROW_EXCEPTION("Bus num must be 1-3")
    }
    esp_err_t ret = spi_bus_free(busnum) ;
    if(ret==0) {
        _spi_bus_setup&= ~(1<<(busnum-1)) ;
    }

    return JS_NewInt32(ctx, ret) ;
}

/**
 * spi bus num 1-3
 * cspin pin (-1 表示不使用)
 * freq
 * mode
 */
JSValue js_spi_device_add(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

    spi_device_handle_t handle = NULL ;
    uint8_t spi_handle_index ;
    spi_device_handle_t * spipool ;
    int8_t spiidx = find_free_spi_handle(&spipool) ;
    if(spiidx<0) {
        THROW_EXCEPTION("SPI Handle too much")
    }

    CHECK_ARGC(4)
    ARGV_TO_UINT8(0, bus)
    ARGV_TO_UINT8(1, cspin)
    ARGV_TO_UINT32(2, freq)
    ARGV_TO_UINT8(3, mode)
    
    spi_device_interface_config_t devcfg={
        .clock_speed_hz=freq,
        .mode=mode,
        .spics_io_num=cspin,
        .queue_size=7,                          //We want to be able to queue 7 transactions at a time
        .pre_cb=NULL,
    };
    esp_err_t ret=spi_bus_add_device(bus, &devcfg, &spipool[spiidx]);
    return JS_NewInt32(ctx, ret!=0? -1: spiidx) ;
}
JSValue js_spi_device_remove(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

    return JS_UNDEFINED ;
}

/**
 * 
 * bus (1-3)
 * data {string|ArrayBuffer}
 * offset?
 * length?
 */
JSValue js_spi_send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)
    ARGV_TO_UINT8(0, spiidx)
    spi_device_handle_t handle = _spi_handle(spiidx) ;
    if(handle==NULL) {
        THROW_EXCEPTION("know spi handle")
    }

    int offset = 0 ;
    if(argc>=3) {
        if( JS_ToInt32(ctx, &offset, argv[2])!=0 ) {
            THROW_EXCEPTION("Invalid param type for offset");
        }
    }
    int length = -1 ;
    if(argc>=4) {
        if( JS_ToInt32(ctx, &length, argv[3])!=0 ) {
            THROW_EXCEPTION("Invalid param type for length");
        }
    }

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));

    esp_err_t ret = -1 ;
    if(JS_IsString(argv[1])) {
        t.tx_buffer = JS_ToCStringLen(ctx, &t.length, argv[1]) ;
        if(length>-1 && length<t.length) {
            t.length = length ;
        }
        t.length*= 8 ;
        ret = spi_device_transmit(handle, &t) ;

        JS_FreeCString(ctx, t.tx_buffer) ;
        goto end ;
    }

    t.tx_buffer = JS_GetArrayBuffer(ctx, &t.length, argv[1]) ;
    if(t.tx_buffer) {
        if(length>-1 && length<t.length) {
            t.length = length ;
        }
        t.length*= 8 ;
        ret = spi_device_transmit(handle, &t) ;
        // JS_FreeValue(ctx, argv[1]) ;
    }
    else {
        THROW_EXCEPTION("Invalid data")
    }


end:
    return JS_NewInt32(ctx, ret) ;
}

JSValue js_spi_recv(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

    return JS_UNDEFINED ;
}

void require_module_serial(JSContext *ctx) {
    
    JSValue global = JS_GetGlobalObject(ctx);
    JSValue serialapi = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, global, "serialapi", serialapi);
    
    JS_SetPropertyStr(ctx, serialapi, "spiBusSetup", JS_NewCFunction(ctx, js_spi_spi_bus_setup, "spiBusSetup", 1));
    JS_SetPropertyStr(ctx, serialapi, "spiBusFree", JS_NewCFunction(ctx, js_spi_bus_free, "spiBusFree", 1));
    JS_SetPropertyStr(ctx, serialapi, "spiDeviceAdd", JS_NewCFunction(ctx, js_spi_device_add, "spiDeviceAdd", 1));
    JS_SetPropertyStr(ctx, serialapi, "spiDeviceRemove", JS_NewCFunction(ctx, js_spi_device_remove, "spiDeviceRemove", 1));
    JS_SetPropertyStr(ctx, serialapi, "spiSend", JS_NewCFunction(ctx, js_spi_send, "spiSend", 1));
    JS_SetPropertyStr(ctx, serialapi, "spiRecv", JS_NewCFunction(ctx, js_spi_recv, "spiRecv", 1));

    JS_FreeValue(ctx, global);
}

#define FREE_BUS(busnum)    if(_spi_bus_setup&(1<<(busnum-1))){ spi_bus_free(busnum); }
void serial_on_before_reset(JSContext *ctx) {

    // 回收 SPI 资源
    for(uint8_t h=0; h<8; h++) {
        if(_spi_handle_pool1[h]!=NULL) {
            spi_bus_remove_device(_spi_handle_pool1[h]) ;
            _spi_handle_pool1[h] = NULL ;
        }
    }
    FREE_BUS(1)
    FREE_BUS(2)
    FREE_BUS(3)
    _spi_bus_setup = 0 ;
}