#include "module_driver.h"
#include "driver_camera.h"

#include "module_fs.h"
#include "utils.h"

#include "esp_system.h"
#include "driver/sdspi_host.h"
#include "driver/sdmmc_host.h"
#include "vfs_fat_internal.h"
#include "sdmmc_cmd.h"
#include "diskio_sdmmc.h"
#include "diskio_impl.h"

    

#define TAG "driver"


static JSValue js_driver_mount_sd(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    
    CHECK_ARGC(3)

    ARGV_TO_UINT8(0, spi)
    ARGV_TO_UINT8(1, cs)

    JS2VSFPath(mntPath, argv[2]) ;

    esp_err_t ret;
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = cs;
    slot_config.host_id = spi;

    sdmmc_card_t* card;
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = spi ;

    ret = esp_vfs_fat_sdspi_mount(mntPath, &host, &slot_config, &mount_config, &card);
    free(mntPath) ;
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            THROW_EXCEPTION("Failed to mount the card .\n")
        } else {
            THROW_EXCEPTION("Failed to initialize the card (%s).\n",esp_err_to_name(ret))
        }
    }

    // sdmmc_card_print_info(stdout, card);

    return JS_TRUE ;
}


void be_module_driver_init() {
    be_module_driver_camera_init() ;
}

void be_module_driver_require(JSContext *ctx) {

    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue driver = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "driver", driver);

    JS_SetPropertyStr(ctx, driver, "mountSD", JS_NewCFunction(ctx, js_driver_mount_sd, "mountSD", 1));

    be_module_driver_camera_require(ctx, driver) ;    

    JS_FreeValue(ctx, beapi);
}

void be_module_driver_loop(JSContext *ctx) {
}

void be_module_driver_reset(JSContext *ctx) {
    be_module_driver_camera_reset(ctx) ;   
}

