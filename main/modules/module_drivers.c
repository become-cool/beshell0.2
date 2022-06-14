#include "module_drivers.h"
#include "utils.h"

#include "esp_system.h"
#include "driver/sdspi_host.h"
#include "driver/sdmmc_host.h"
#include "vfs_fat_internal.h"
#include "sdmmc_cmd.h"
#include "diskio_sdmmc.h"
#include "diskio_impl.h"




//Deprecated, make use of new sdspi_host_init_device
esp_err_t _sdspi_host_init_slot(int slot, const sdspi_slot_config_t* slot_config)
{
    esp_err_t ret = ESP_OK;

    sdspi_dev_handle_t sdspi_handle;
    sdspi_device_config_t dev_config = {
        .host_id = slot,
        .gpio_cs = slot_config->gpio_cs,
        .gpio_cd = slot_config->gpio_cd,
        .gpio_wp = slot_config->gpio_wp,
        .gpio_int = slot_config->gpio_int,
    };
    ret =  sdspi_host_init_device(&dev_config, &sdspi_handle);
    if (ret != ESP_OK) {
        goto cleanup;
    }
    if (sdspi_handle != (int)slot) {
        printf("The deprecated sdspi_host_init_slot should be called before all other devices on the specified bus.\n");
        sdspi_host_remove_device(sdspi_handle);
        ret = ESP_ERR_INVALID_STATE;
        goto cleanup;
    }
    return ESP_OK;
cleanup:
    spi_bus_free(slot);
    return ret;
}


static esp_err_t mount_to_vfs_fat(const esp_vfs_fat_mount_config_t *mount_config, sdmmc_card_t *card, uint8_t pdrv, const char *base_path) {
    FATFS* fs = NULL;
    esp_err_t err;
    ff_diskio_register_sdmmc(pdrv, card);
    // ESP_LOGD(TAG, "using pdrv=%i", pdrv);
    char drv[3] = {(char)('0' + pdrv), ':', 0};

    // connect FATFS to VFS
    err = esp_vfs_fat_register(base_path, drv, mount_config->max_files, &fs);
    if (err == ESP_ERR_INVALID_STATE) {
        // it's okay, already registered with VFS
    } else if (err != ESP_OK) {
        // ESP_LOGD(TAG, "esp_vfs_fat_register failed 0x(%x)", err);
        goto fail;
    }

    // Try to mount partition
    FRESULT res = f_mount(fs, drv, 1);
    if (res != FR_OK) {
        goto fail;
    }
    return ESP_OK;

fail:
    if (fs) {
        f_mount(NULL, drv, 0);
    }
    esp_vfs_fat_unregister_path(base_path);
    ff_diskio_unregister(pdrv);
    return err;
}


static void call_host_deinit(const sdmmc_host_t *host_config)
{
    if (host_config->flags & SDMMC_HOST_FLAG_DEINIT_ARG) {
        host_config->deinit_p(host_config->slot);
    } else {
        host_config->deinit();
    }
}

// esp_err_t _esp_vfs_fat_sdmmc_mount(const char* base_path,
//                                   const sdmmc_host_t* host_config,
//                                   const void* slot_config,
//                                   const esp_vfs_fat_mount_config_t* mount_config,
//                                   sdmmc_card_t** out_card)
// {
    
//     esp_err_t err;
//     sdmmc_card_t* card = NULL;
//     BYTE pdrv = 0;
//     char* dup_path = NULL;
//     bool host_inited = false;

//     // err = mount_prepare_mem(base_path, &pdrv, &dup_path, &card);
//     // if (err != ESP_OK) {
//     //     ESP_LOGE(TAG, "mount_prepare failed");
//     //     return err;
//     // }


//         err = _sdspi_host_init_slot(host_config->slot, slot_config);
//         // CHECK_EXECUTE_RESULT(err, "slot init failed");

//         //Set `host_inited` to true to indicate that host_config->deinit() needs
//         //to be called to revert `init_sdspi_host_deprecated`; set `card_handle`
//         //to -1 to indicate that no other deinit is required.
//         host_inited = true;

//     // probe and initialize card
//     err = sdmmc_card_init(host_config, card);
//     // CHECK_EXECUTE_RESULT(err, "sdmmc_card_init failed");

//     err = mount_to_vfs_fat(mount_config, card, pdrv, dup_path);
//     // CHECK_EXECUTE_RESULT(err, "mount_to_vfs failed");

//     // if (out_card != NULL) {
//     //     *out_card = card;
//     // }
//     // if (s_card == NULL) {
//     //     //store the ctx locally to be back-compatible
//     //     s_card = card;
//     //     s_pdrv = pdrv;
//     //     s_base_path = dup_path;
//     // } else {
//     //     free(dup_path);
//     // }
//     return ESP_OK;
// // cleanup:
// //     if (host_inited) {
// //         call_host_deinit(host_config);
// //     }
// //     free(card);
// //     free(dup_path);
// //     return err;
// }


static JSValue js_driver_mount_sd(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)

    ARGV_TO_UINT8(0, spi)
    ARGV_TO_UINT8(1, cs)

    

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    // host.slot = 2 ; // SPI2(VSPI)
    host.slot = 1 ; // SPI1(HSPI)
    host.max_freq_khz = 20000 ;
    // host

    sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
    slot_config.gpio_miso = 12;
    slot_config.gpio_mosi = 13;
    slot_config.gpio_sck  = 14;
    slot_config.gpio_cs   = cs;

    // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = true,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    // Note: esp_vfs_fat_sdmmc_mount is an all-in-one convenience function.
    // Please check its source code and implement error recovery when developing
    // production applications.
    esp_err_t err ;
    sdmmc_card_t * card = (sdmmc_card_t*) malloc(sizeof(sdmmc_card_t)) ;

    BYTE pdrv = FF_DRV_NOT_USED ;
    if (ff_diskio_get_drive(&pdrv) != ESP_OK || pdrv == FF_DRV_NOT_USED) {
        THROW_EXCEPTION("the maximum count of volumes is already mounted") ;
    }
    
    _sdspi_host_init_slot(host.slot, &slot_config);
    
    err = sdmmc_card_init(&host, card);
    if(err!=ESP_OK) {
        printf("sdmmc_card_init() %d\n", err) ;
        return JS_FALSE ;
    }
    // CHECK_EXECUTE_RESULT(err, "sdmmc_card_init failed");

    err = mount_to_vfs_fat(&mount_config, card, pdrv, "/fs/sd");
    if(err!=ESP_OK) {
        printf("mount_to_vfs_fat() %d\n", err) ;
        return JS_FALSE ;
    }


    sdmmc_card_print_info(stdout, card);

    return JS_UNDEFINED ;
}

void be_module_drivers_init() {
}

void be_module_drivers_require(JSContext *ctx) {

    JSValue beapi = js_get_glob_prop(ctx, 1, "beapi") ;
    JSValue drivers = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "drivers", drivers);

    JS_SetPropertyStr(ctx, drivers, "mountSD", JS_NewCFunction(ctx, js_driver_mount_sd, "mountSD", 1));
    

    JS_FreeValue(ctx, beapi);
}

void be_module_drivers_loop(JSContext *ctx) {
}

void be_module_drivers_reset(JSContext *ctx) {
}

