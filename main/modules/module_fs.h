#ifndef _MODULE_FS_H
#define _MODULE_FS_H

#include "quickjs-libc.h"

#ifndef SIMULATION
#include "esp_vfs.h"

bool be_module_fs_init() ;
#endif


void module_fs_set_vfs_path_prefix(char * path) ;
char * js_arg_to_vfspath(JSContext *ctx, JSValueConst argv) ;
char * vfspath_to_fs(const char * path) ;

#define JS2VSFPath(path, arg)                                                       \
    char * path = js_arg_to_vfspath(ctx, arg) ;                                     \
    if(!path) {                                                                     \
        JS_ThrowReferenceError(ctx, "cound not malloc for path, memory low ?");     \
        return JS_EXCEPTION ;                                                       \
    }

#define STAT_PATH(path)                                                             \
    struct stat statbuf;                                                            \
    if(stat(path,&statbuf)<0) {                                                     \
        JS_ThrowReferenceError(ctx, "Failed to stat file %s", path);                \
        free(path) ;                                                                \
        return JS_EXCEPTION ;                                                       \
    }

#define CHECK_ARGV0_NOT_DIR(path)                                                   \
    STAT_PATH(path)                                                                 \
    if(S_ISDIR(statbuf.st_mode)) {                                                  \
        JS_ThrowReferenceError(ctx, "Path is a directory %s", path);                \
        free(path) ;                                                                \
        return JS_EXCEPTION ;                                                       \
    }

#define PATH_PREFIX "/fs"

// 递归创建目录
#ifdef WIN32
int mkdir_p(char* file_path) ;
#else
int mkdir_p(char* file_path, mode_t mode) ;
#endif

void be_module_fs_require(JSContext *ctx) ;

#endif
