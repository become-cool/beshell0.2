#include "module_serial_spi.h"
#include "utils.h"
#include <string.h>



static uint8_t _spi_bus_setup = 0 ;
static spi_device_handle_t _spi_handle_pool1[8] = {0,0,0,0,0,0,0,0} ;

static int8_t find_free_spi_handle(spi_device_handle_t ** pool) {
    int8_t h = 0 ;
    for(h=0; h<8; h++) {
        if(_spi_handle_pool1[h]==NULL) {
            *pool = & _spi_handle_pool1 ;
            return h ;
        }
    }
    return -1 ;
}

static spi_device_handle_t _spi_handle(uint8_t idx) {
    if(idx<8) {
        return _spi_handle_pool1[idx] ;
    }
    return NULL ;
}

/**
 * spi bus num 1-3
 * miso
 * mosi
 * clk
 */
static JSValue js_spi_bus_setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

    CHECK_ARGC(4)

    int8_t misopin = -1 ;
    int8_t mosipin = -1 ;
    if(!JS_IsUndefined(argv[1]) && !JS_IsNull(argv[1])) {
        if( JS_ToUint32(ctx, &misopin, argv[1])!=0 ){
            THROW_EXCEPTION("MISO pin invalid.")
        }
    }
    if(!JS_IsUndefined(argv[2]) && !JS_IsNull(argv[2])) {
        if( JS_ToUint32(ctx, &mosipin, argv[2])!=0 ){
            THROW_EXCEPTION("MOSI pin invalid.")
        }
    }

    ARGV_TO_UINT8(0, busnum)
    if(busnum<0 || busnum>3) {
        THROW_EXCEPTION("Bus num must be 1-3")
    }
    ARGV_TO_UINT8(3, clkpin)

    // pf("miso=%d, mosi=%d, clk=%d", misopin, mosipin, clkpin)
    // dn(busnum)

    spi_bus_config_t buscfg = {
        .miso_io_num=misopin,
        .mosi_io_num=mosipin,
        .sclk_io_num=clkpin,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
        .max_transfer_sz=20480
    } ;

    esp_err_t ret = spi_bus_initialize(busnum, &buscfg, SPI_DMA_CH2);
    if(ret==0) {
        _spi_bus_setup|= 1<<busnum ;
    }

    return JS_NewInt32(ctx, ret) ;
}

/**
 * spi bus num 0-2
 */
static JSValue js_spi_bus_free(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, busnum)
    if(busnum<0 || busnum>2) {
        THROW_EXCEPTION("Bus num must be 0-2")
    }
    esp_err_t ret = spi_bus_free(busnum) ;
    if(ret==0) {
        _spi_bus_setup&= ~(1<<busnum) ;
    }

    return JS_NewInt32(ctx, ret) ;
}

/**
 * spi bus num 1-3
 * cspin pin (-1 表示不使用)
 * freq
 * mode
 */
static JSValue js_spi_device_add(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

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
static JSValue js_spi_device_remove(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

    return JS_UNDEFINED ;
}

/**
 * 
 * bus (1-3)
 * data {string|ArrayBuffer}
 * offset?
 * length?
 */
static JSValue js_spi_send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
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

static JSValue js_spi_recv(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

    return JS_UNDEFINED ;
}



// void be_module_serial_spi_init() {
// }

void be_module_serial_spi_require(JSContext *ctx, JSValue pkg) {

    JSValue spi = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, pkg, "spi", spi);
    JS_SetPropertyStr(ctx, spi, "setup", JS_NewCFunction(ctx, js_spi_bus_setup, "setup", 1));
    JS_SetPropertyStr(ctx, spi, "free", JS_NewCFunction(ctx, js_spi_bus_free, "free", 1));
    JS_SetPropertyStr(ctx, spi, "addDevice", JS_NewCFunction(ctx, js_spi_device_add, "addDevice", 1));
    JS_SetPropertyStr(ctx, spi, "removeDevice", JS_NewCFunction(ctx, js_spi_device_remove, "removeDevice", 1));
    JS_SetPropertyStr(ctx, spi, "send", JS_NewCFunction(ctx, js_spi_send, "send", 1));
    JS_SetPropertyStr(ctx, spi, "recv", JS_NewCFunction(ctx, js_spi_recv, "recv", 1));

}

// void be_module_serial_spi_loop(JSContext *ctx) {
// }

void be_module_serial_spi_reset(JSContext *ctx) {
    
    // 回收 SPI 资源
    for(uint8_t h=0; h<8; h++) {
        if(_spi_handle_pool1[h]!=NULL) {
            spi_bus_remove_device(_spi_handle_pool1[h]) ;
            _spi_handle_pool1[h] = NULL ;
        }
    }
    FREE_BUS_SPI(1)
    FREE_BUS_SPI(2)
    FREE_BUS_SPI(3)
    _spi_bus_setup = 0 ;

}

