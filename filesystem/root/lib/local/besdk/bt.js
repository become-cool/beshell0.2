



    // JS_SetPropertyStr(ctx, bt, "controllerMemRelease", JS_NewCFunction(ctx, js_bt_controller_mem_release, "controllerMemRelease", 1));
    // JS_SetPropertyStr(ctx, bt, "controllerInitAsDefault", JS_NewCFunction(ctx, js_bt_controller_init_as_default, "controllerInitAsDefault", 1));
    // JS_SetPropertyStr(ctx, bt, "controllerDeinit", JS_NewCFunction(ctx, js_bt_controller_deinit, "controllerDeinit", 1));
    // JS_SetPropertyStr(ctx, bt, "controllerEnable", JS_NewCFunction(ctx, js_bt_controller_enable, "controllerEnable", 1));
    // JS_SetPropertyStr(ctx, bt, "controllerDisable", JS_NewCFunction(ctx, js_bt_controller_disable, "controllerDisable", 1));
    // JS_SetPropertyStr(ctx, bt, "controllerStatus", JS_NewCFunction(ctx, js_bt_controller_get_status, "controllerStatus", 1));
    
    // JS_SetPropertyStr(ctx, bt, "bluedroidInit", JS_NewCFunction(ctx, js_bt_bluedroid_init, "bluedroidInit", 1));
    // JS_SetPropertyStr(ctx, bt, "bluedroidDeinit", JS_NewCFunction(ctx, js_bt_bluedroid_deinit, "bluedroidDeinit", 1));
    // JS_SetPropertyStr(ctx, bt, "bluedroidEnable", JS_NewCFunction(ctx, js_bt_bluedroid_enable, "bluedroidEnable", 1));
    // JS_SetPropertyStr(ctx, bt, "bluedroidDisable", JS_NewCFunction(ctx, js_bt_bluedroid_disable, "bluedroidDisable", 1));
    // JS_SetPropertyStr(ctx, bt, "bluedroidStatus", JS_NewCFunction(ctx, js_bt_bluedroid_get_status, "bluedroidStatus", 1));


exports.start = function (mode) {

    beapi.bt.controllerMemRelease(3)

    beapi.bt.controllerInitAsDefault(mode)
    beapi.bt.controllerEnable()

    beapi.bt.bluedroidInit()
    beapi.bt.bluedroidEnable()


    
}


