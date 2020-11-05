#ifndef _NATIVE_FS_H
#define _NATIVE_FS_H

#include "esp_vfs.h"
#include "vfs_fat_internal.h"

void fs_init() ;
size_t fs_free_bytes() ;
size_t fs_total_bytes() ;
bool fs_format(char* partitionLabel) ;
const esp_partition_t * fs_check_fat_partition(const char* label) ;
bool fs_mount_fat(const char * path, const char * label) ;

#endif