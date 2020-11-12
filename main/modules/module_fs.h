#ifndef _MODULE_FS_H
#define _MODULE_FS_H

#include "quickjs-libc.h"
#include "esp_vfs.h"


char * js_arg_to_vfspath(JSContext *ctx, JSValueConst argv) ;

#define JS2VSFPath(path, arg)                                                       \
    char * path = js_arg_to_vfspath(ctx, arg) ;                                     \
    if(!path) {                                                                     \
        JS_ThrowReferenceError(ctx, "cound not malloc for path, memory low ?");     \
        return JS_EXCEPTION ;                                                       \
    }

#define STAT_PATH(path)                                                             \
    struct stat statbuf;                                                            \
    if(stat(path,&statbuf)<0) {                                                     \
        free(path) ;                                                                \
        JS_ThrowReferenceError(ctx, "Failed to stat file.");                        \
        return JS_EXCEPTION ;                                                       \
    }

#define CHECK_ARGV0_NOT_DIR(path)                                                   \
    STAT_PATH(path)                                                                 \
    if(S_ISDIR(statbuf.st_mode)) {                                                  \
        free(path) ;                                                                \
        JS_ThrowReferenceError(ctx, "Path is a directory.");                        \
        return JS_EXCEPTION ;                                                       \
    }

#define PATH_PREFIX "/fs"

bool fs_init() ;
void require_module_fs(JSContext *ctx) ;

#endif