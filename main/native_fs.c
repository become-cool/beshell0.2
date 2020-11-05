#include "native_fs.h"
#include "logging.h"
#include "diskio_wl.h"


LOG_TAG("native_fs")

wl_handle_t fs_root_handle = NULL;

/**
 * Mount & register virtual filesystem.
 */
bool fs_mount_fat(const char * path, const char * label) {
    esp_vfs_fat_mount_config_t conf = {
      .format_if_mount_failed = true,
      .max_files = 4
    };
    esp_err_t err = esp_vfs_fat_spiflash_mount(path, label, &conf, &fs_root_handle);
    if(err){
        printf("Mounting FFat partition failed! Error: %d\n", err);
        return false;
    }

    return true ;
}


const esp_partition_t * fs_check_fat_partition(const char* label) {
    const esp_partition_t* ck_part = esp_partition_find_first(
       ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_FAT, label);
    if (!ck_part) {
        printf("No FAT partition found with label %s\n", label);
        return NULL;
    }
    return ck_part;
}


bool fs_format(char* partitionLabel) {
    esp_err_t result;
    wl_handle_t temp_handle;
    // Attempt to mount to see if there is already data
    const esp_partition_t *ffat_partition = fs_check_fat_partition(partitionLabel);
    if (!ffat_partition) return false;
    result = wl_mount(ffat_partition, &temp_handle);

    if (result == ESP_OK) {
        // Wipe disk- quick just wipes the FAT. Full zeroes the whole disk
        uint32_t wipe_size = 16384;
        wl_erase_range(temp_handle, 0, wipe_size);
        wl_unmount(temp_handle);
    }
    // Now do a mount with format_if_fail (which it will)
    esp_vfs_fat_mount_config_t conf = {
      .format_if_mount_failed = true,
      .max_files = 2
    };
    result = esp_vfs_fat_spiflash_mount("/format_ffat", partitionLabel, &conf, &temp_handle);
    esp_vfs_fat_spiflash_unmount("/format_ffat", temp_handle);
    return result;
}
  
void fs_init() {
    if( !fs_mount_fat("/home", "fatfs") ) {
        printf("Failed to mount fatfs.\n") ;
        return ;
    }

    printf("fs_root_handle=%d\n",fs_root_handle) ;

    printf("fs root has mounted, total: %dKB, free: %dKB\n", (int)fs_total_bytes(), (int)fs_free_bytes()) ;
}

size_t fs_total_bytes() {
    FATFS *fs;
    DWORD free_clust, tot_sect, sect_size;

    BYTE pdrv = ff_diskio_get_pdrv_wl(fs_root_handle);
    char drv[3] = {(char)(48+pdrv), ':', 0};
    if ( f_getfree(drv, &free_clust, &fs) != FR_OK){
        return 0;
    }
    tot_sect = (fs->n_fatent - 2) * fs->csize;
    sect_size = CONFIG_WL_SECTOR_SIZE;
    return tot_sect * sect_size;
}

size_t fs_free_bytes() {
    FATFS *fs;
    DWORD free_clust, free_sect, sect_size;

    BYTE pdrv = ff_diskio_get_pdrv_wl(fs_root_handle);
    char drv[3] = {(char)(48+pdrv), ':', 0};
    if ( f_getfree(drv, &free_clust, &fs) != FR_OK){
        return 0;
    }
    free_sect = free_clust * fs->csize;
    sect_size = CONFIG_WL_SECTOR_SIZE;
    return free_sect * sect_size;
}
