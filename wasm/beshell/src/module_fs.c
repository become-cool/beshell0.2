#include "module_fs.h"
#include <string.h>

void module_fs_set_vfs_path_prefix(char * path) {
}
char * vfspath_to_fs(const char * path) {
    char * _path = malloc(strlen(path)+1) ;
    strcpy(_path, path) ;
    return _path ;
}


// esp32 vfs 不能挂在根目录，将整个系统挂载到 /fs 下，访问时自动加上
// 返回值 由调用者 free
char * js_arg_to_vfspath(JSContext *ctx, JSValueConst argv) {
    char * jspath = JS_ToCString(ctx, argv) ;
    const char * path = vfspath_to_fs(jspath) ;
    // char * path = mallocf(PATH_PREFIX"%s", jspath) ;
    JS_FreeCString(ctx, jspath) ;
    return path ;
}
