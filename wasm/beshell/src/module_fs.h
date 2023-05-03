#ifndef _MODULE_FS_H
#define _MODULE_FS_H

#include "quickjs-libc.h"

void module_fs_set_vfs_path_prefix(char * path) ;
char * vfspath_to_fs(const char * path) ;
char * js_arg_to_vfspath(JSContext *ctx, JSValueConst argv) ;


#endif