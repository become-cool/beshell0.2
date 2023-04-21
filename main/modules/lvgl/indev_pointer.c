#include "indev.h"
#include "indev_pointer.h"
#include "utils.h"
#include "widgets_gen.h"



#ifndef SIMULATION

#include "tp_spi.h"
#include "xpt2046.h"

// @todo 由用户校正
// #define OFFSET_X 0
#define OFFSET_X 11

#endif



// ------------
// InDevPointer 

JSClassID js_lv_indev_pointer_class_id = 0 ;

static void indev_pointer_read(lv_indev_drv_t *drv, lv_indev_data_t *data) {
    if(!drv->user_data) {
        return ;
    }
    indev_driver_spec_touch_t * driver_spec = (indev_driver_spec_t *) drv->user_data ;
    if(driver_spec->spec.fake) {
        driver_spec->spec.fake = false ;
        data->point.x = driver_spec->spec.data.pointer.x ;
        data->point.y = driver_spec->spec.data.pointer.y ;
        data->state = driver_spec->spec.data.pointer.state ;
        data->continue_reading = false ;
    }
#ifndef SIMULATION
    else if(INDEV_DRIVER_XPT2046 == driver_spec->spec.driver) {
        data->continue_reading = xpt2046_read(drv, data) ;
        // if( data->point.x > OFFSET_X ) {
        //     data->point.x += OFFSET_X ;
        // }
    }
#endif
    else {
        return ;
    }

    data->point.x+= driver_spec->offset_x ;
    data->point.y+= driver_spec->offset_y ;

    if(driver_spec->swap_xy) {
        lv_coord_t y = data->point.x ;
        data->point.x = data->point.y ;
        data->point.y = y ;
    }
    if(driver_spec->inv_x && driver_spec->max_x>0) {
        // if(data->state) {
        //     lv_coord_t x = driver_spec->max_x - data->point.x ;
        //     dn3( data->point.x,  driver_spec->max_x, x)
        // }
        data->point.x = driver_spec->max_x - data->point.x ;
    }
    if(driver_spec->inv_y && driver_spec->max_y>0) {
        data->point.y = driver_spec->max_y - data->point.y ;
    }


    indev_global_cb_proc(data) ;
}

static JSValue js_lv_indev_pointer_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){

    if(lv_disp_get_default()==NULL) {
        THROW_EXCEPTION("There is no default display.")
    }
    if(_indev_id>=255){
        THROW_EXCEPTION("max indev count 255") ;
    }

    CHECK_ARGC(1)
    ARGV_TO_STRING(0, driver)

    indev_driver_spec_touch_t * driver_spec = malloc(sizeof(indev_driver_spec_touch_t)) ;
    memset(driver_spec, 0, sizeof(indev_driver_spec_touch_t)) ;

    if(strcmp(driver, "fake")==0) {
        driver_spec->spec.driver = INDEV_DRIVER_FAKE ;
    }
#ifndef SIMULATION
    else if(strcmp(driver, "XPT2046")==0) {
        printf("XPT2046\n")  ;
        if(argc<3) {
            free(driver_spec) ;
            JS_FreeCString(ctx, driver) ;
            THROW_EXCEPTION("Missing argv")
        }

        ARGV_TO_UINT8(2, cs)
        ARGV_TO_UINT8(1, busnum)

        driver_spec->spec.driver = INDEV_DRIVER_XPT2046 ;

        esp_err_t res = tp_spi_add_device(busnum, cs, &driver_spec->spec.conf.spi.handle) ;
        if(res!=ESP_OK) {
            free(driver_spec) ;
            JS_FreeCString(ctx, driver) ;
            THROW_EXCEPTION("spi_add_device failed: %d",res)
        }
        xpt2046_init() ;
    }
#endif
    else {
        free(driver_spec) ;
        JS_FreeCString(ctx, driver) ;
        THROW_EXCEPTION("unknow InDevPointer driver")
    }
    
    driver_spec->spec.id = _indev_id ++ ;
    driver_spec->spec.fake = false ;
    driver_spec->spec.data.pointer.x = 0 ;
    driver_spec->spec.data.pointer.y = 0 ;
    driver_spec->spec.data.pointer.state = LV_INDEV_STATE_RELEASED ;

    lv_indev_drv_t * indev_drv = malloc(sizeof(lv_indev_drv_t)) ;
    lv_indev_drv_init(indev_drv);
    indev_drv->type = LV_INDEV_TYPE_POINTER;
    indev_drv->read_cb = indev_pointer_read ;
    indev_drv->user_data = driver_spec ;

    lv_indev_t * indev = lv_indev_drv_register(indev_drv) ;
    indev->driver->gesture_limit = 30 ;

    lv_task_handler() ;

    JSValue jsobj = JS_NewObjectClass(ctx, js_lv_indev_pointer_class_id) ;
    JS_SetOpaque(jsobj, indev) ;
    JS_SetPropertyStr(ctx, jsobj, "driver", JS_NewString(ctx, driver)) ;

    // 调用 lv_indev_remove() 后解除这个引用
    JS_DupValue(ctx, jsobj) ;
    
    JS_FreeCString(ctx, driver) ;
    return jsobj ;
}
static void js_lv_indev_pointer_finalizer(JSRuntime *rt, JSValue this_val){
    // printf("js_lv_indev_pointer_finalizer()\n") ;
    lv_indev_t * thisobj = (lv_indev_t *)JS_GetOpaque(this_val, js_lv_indev_pointer_class_id) ;
    if(thisobj) {
        lv_indev_remove(thisobj) ;
        if(thisobj->driver) {
            if(thisobj->driver->user_data) {

                indev_driver_spec_t * pointer = (indev_driver_spec_t *) thisobj->driver->user_data ;
                #ifndef SIMULATION
                if(pointer->driver == INDEV_DRIVER_XPT2046) {
                    spi_bus_remove_device(& pointer->conf.spi.handle) ;
                }
                #endif

                free(thisobj->driver->user_data) ;
                thisobj->driver->user_data = NULL ;
                return ;
            }
            free(thisobj->driver) ;
            thisobj->driver = NULL ;
        }
        free(thisobj) ;
    }


}
static JSClassDef js_lv_indev_pointer_class = {
    "InDevPointer",
    .finalizer = js_lv_indev_pointer_finalizer,
} ;


static JSValue js_lv_indev_pointer_set(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    THIS_OBJ("lv.InDevPointer", "set", thisobj, lv_indev_t)
    CHECK_ARGC(3)
    ARGV_TO_INT32(0, x)
    ARGV_TO_INT32(1, y)
    bool state = JS_ToBool(ctx, argv[2]) ;

    indev_driver_spec_t * driver_spec = (indev_driver_spec_t *)thisobj->driver->user_data ;
    if(!driver_spec) {
        THROW_EXCEPTION("invalid indev")
    }
    driver_spec->fake = true ;
    driver_spec->data.pointer.x = x ;
    driver_spec->data.pointer.y = y ;
    driver_spec->data.pointer.state = state? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;

    return JS_UNDEFINED ;
}

static JSValue js_lv_indev_point_prop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, int magic) {
    int propId = magic & 0xF ;

    THIS_OBJ("lv.InDevPointer", "set", thisobj, lv_indev_t)
    indev_driver_spec_touch_t * spec = (indev_driver_spec_t *)thisobj->driver->user_data ;

    // setter
    if( magic & 0x10 ) {
        CHECK_ARGC(1)
        switch(propId) {
            case 0x01 :
                JS_ToUint32(ctx, &spec->max_x, argv[0]) ;
                break ;
            case 0x02 :
                JS_ToUint32(ctx, &spec->max_y, argv[0]) ;
                break ;
            case 0x03 :
                JS_ToUint32(ctx, &spec->offset_x, argv[0]) ;
                break ;
            case 0x04 :
                JS_ToUint32(ctx, &spec->offset_y, argv[0]) ;
                break ;
            case 0x05 :
                spec->swap_xy = JS_ToBool(ctx, argv[0]) ;
                break ;
            case 0x06 :
                spec->inv_x = JS_ToBool(ctx, argv[0]) ;
                break ;
            case 0x07 :
                spec->inv_y = JS_ToBool(ctx, argv[0]) ;
                break ;
        }
    }

    // getter
    else {
        switch(propId) {
            case 0x01 :
                return JS_NewUint32(ctx, spec->max_x) ;
            case 0x02 :
                return JS_NewUint32(ctx, spec->max_y) ;
            case 0x03 :
                return JS_NewUint32(ctx, spec->offset_x) ;
            case 0x04 :
                return JS_NewUint32(ctx, spec->offset_y) ;
            case 0x05 :
                return JS_NewBool(ctx, spec->swap_xy) ;
            case 0x06 :
                return JS_NewBool(ctx, spec->inv_x) ;
            case 0x07 :
                return JS_NewBool(ctx, spec->inv_y) ;
        }
    }

    return JS_UNDEFINED ;
}




static const JSCFunctionListEntry js_lv_indev_pointer_proto_funcs[] = {
    JS_CFUNC_DEF("set", 0, js_lv_indev_pointer_set),
    // JS_CFUNC_DEF("tick", 0, js_lv_indev_tick),
    // JS_CFUNC_DEF("setGroup", 0, js_lv_indev_set_group),
    // JS_CFUNC_DEF("id", 0, js_lv_indev_id),

    JS_CFUNC_MAGIC_DEF("maxX", 0, js_lv_indev_point_prop, 0x01),
    JS_CFUNC_MAGIC_DEF("maxY", 0, js_lv_indev_point_prop, 0x02),
    JS_CFUNC_MAGIC_DEF("offsetX", 0, js_lv_indev_point_prop, 0x03),
    JS_CFUNC_MAGIC_DEF("offsetY", 0, js_lv_indev_point_prop, 0x04),
    JS_CFUNC_MAGIC_DEF("swapXY", 0, js_lv_indev_point_prop, 0x05),
    JS_CFUNC_MAGIC_DEF("invX", 0, js_lv_indev_point_prop, 0x06),
    JS_CFUNC_MAGIC_DEF("invY", 0, js_lv_indev_point_prop, 0x07),

    JS_CFUNC_MAGIC_DEF("setMaxX", 0, js_lv_indev_point_prop, 0x11),
    JS_CFUNC_MAGIC_DEF("setMaxY", 0, js_lv_indev_point_prop, 0x12),
    JS_CFUNC_MAGIC_DEF("setOffsetX", 0, js_lv_indev_point_prop, 0x13),
    JS_CFUNC_MAGIC_DEF("setOffsetY", 0, js_lv_indev_point_prop, 0x14),
    JS_CFUNC_MAGIC_DEF("setSwapXY", 0, js_lv_indev_point_prop, 0x15),
    JS_CFUNC_MAGIC_DEF("setInvX", 0, js_lv_indev_point_prop, 0x16),
    JS_CFUNC_MAGIC_DEF("setInvY", 0, js_lv_indev_point_prop, 0x17),
    
} ;




void be_indev_pointer_init() {
    JS_NewClassID(&js_lv_indev_pointer_class_id);
}

void be_indev_pointer_require(JSContext *ctx, JSValue lvgl, JSValue baseProto) {
    QJS_DEF_CLASS(lv_indev_pointer, "InDevPointer", "lv.InDevPointer", baseProto, lvgl)
}

void be_indev_pointer_loop(JSContext *ctx) {
}

void be_indev_pointer_reset(JSContext *ctx) {
}

