#include "module_fs.h"
#include "logging.h"
#include "diskio_wl.h"
#include "vfs_fat_internal.h"
#include <fcntl.h>
#include "esp_littlefs.h"
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "utils.h"

LOG_TAG("fs")

// esp32 vfs 不能挂在根目录，将整个系统挂载到 /fs 下，访问时自动加上
#define PATH_PREFIX "/fs"

char * js_arg_to_vfspath(JSContext *ctx, JSValueConst argv) {
    char * jspath = JS_ToCString(ctx, argv) ;
    int len = strlen(jspath) ;
    len = sizeof(PATH_PREFIX) + len ;
    char * path = malloc(len+1) ;
    if(!path) {
        JS_FreeCString(ctx, jspath) ;
        return NULL ;
    }
    memcpy(path, PATH_PREFIX, sizeof(PATH_PREFIX)) ;
    strcpy(path+3, jspath) ;
    JS_FreeCString(ctx, jspath) ;
    return path ;
}
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

#define CHECK_NOT_DIR(path)                                                         \
    STAT_PATH(path)                                                                 \
    if(S_ISDIR(statbuf.st_mode)) {                                                  \
        free(path) ;                                                                \
        JS_ThrowReferenceError(ctx, "Path is a directory.");                        \
        return JS_EXCEPTION ;                                                       \
    }



JSValue js_fs_statSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)
    JS2VSFPath(path, argv[0]) ;
    struct stat statbuf;
    if(stat(path,&statbuf)<0) {
        free(path) ;
        return JS_NULL ;
    }
    free(path) ;
    
    JSValue obj = JS_NewObject(ctx) ;
    JS_SetPropertyStr(ctx, obj, "dev", JS_NewInt32(ctx, statbuf.st_dev)) ;
    JS_SetPropertyStr(ctx, obj, "ino", JS_NewInt32(ctx, statbuf.st_ino)) ;
    JS_SetPropertyStr(ctx, obj, "mode", JS_NewInt32(ctx, statbuf.st_mode)) ;
    JS_SetPropertyStr(ctx, obj, "size", JS_NewInt32(ctx, statbuf.st_size)) ;
    JS_SetPropertyStr(ctx, obj, "atime", JS_NewInt64(ctx, statbuf.st_atime)) ;
    JS_SetPropertyStr(ctx, obj, "mtime", JS_NewInt64(ctx, statbuf.st_mtime)) ;
    JS_SetPropertyStr(ctx, obj, "ctime", JS_NewInt64(ctx, statbuf.st_ctime)) ;
    JS_SetPropertyStr(ctx, obj, "isDir", S_ISDIR(statbuf.st_mode)? JS_TRUE: JS_FALSE) ;

    return obj ;
}

JSValue js_fs_idDirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param path");
        return JS_EXCEPTION ;
    }
    JS2VSFPath(path, argv[0]) ;
    STAT_PATH(path)
    free(path) ;
    return S_ISDIR(statbuf.st_mode)? JS_TRUE: JS_FALSE ;
}


JSValue js_fs_mkdirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param path");
        return JS_EXCEPTION ;
    }
    JS2VSFPath(path, argv[0]) ;
    bool ret = mkdir(path, ACCESSPERMS)>=0 ;
    free(path) ;
    return ret? JS_TRUE: JS_FALSE ;
}
JSValue js_fs_rmdirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param path");
        return JS_EXCEPTION ;
    }
    JS2VSFPath(path, argv[0]) ;
    bool ret = rmdir(path)>=0 ;
    free(path) ;
    return ret? JS_TRUE: JS_FALSE ;
}

JSValue js_fs_unlinkSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param path");
        return JS_EXCEPTION ;
    }
    JS2VSFPath(path, argv[0]) ;
    bool ret = unlink(path)>=0 ;
    free(path) ;
    return ret? JS_TRUE: JS_FALSE ;
}

JSValue js_fs_readFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    
    JS2VSFPath(path, argv[0])
    CHECK_NOT_DIR(path)

    char * buff = malloc(statbuf.st_size) ;
    if(!buff) {
        free(path) ;
        JS_ThrowReferenceError(ctx, "Failed to malloc buff");
        return JS_EXCEPTION ;
    }

	int fd = fopen(path, "r");
    free(path) ;

    if(fd<0) {
        free(buff) ;
        JS_ThrowReferenceError(ctx, "Failed to open file (%d).", fd);
        return JS_EXCEPTION ;
    }

    int readedBytes = fread(buff, 1, statbuf.st_size, fd) ;
    // buff[readedBytes] = 0 ;

    fclose(fd) ;

    JSValue str = JS_NewStringLen(ctx, buff, readedBytes) ;
    free(buff) ;

    return str ;
}

JSValue js_fs_writeFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)
    
    JS2VSFPath(path, argv[0])
    CHECK_NOT_DIR(path)


	int fd = fopen(path, "w");
    if(fd<0) {
        free(path) ;
        JS_ThrowReferenceError(ctx, "Failed to open file");
        return JS_EXCEPTION ;
    }

    char * buff = NULL ;
	uint32_t length;
    bool buffIsString = JS_IsString(argv[1]) ;

    if(buffIsString) {
        buff = JS_ToCString(ctx, argv[1]) ;
        length = strlen(buff) ;
    }

    else if(JS_IsObject(argv[1]) /* @todo: 检查 arraybuffer */) {
        // @todo
    }
    else {
        free(path) ;
        fclose(fd) ;
        JS_ThrowReferenceError(ctx, "Invalid param type of path.");
        return JS_EXCEPTION ;
    }

    int wroteBytes = 0 ;
    if(buff) {
        printf("write(%d): %s\n", length, buff) ;
	    wroteBytes = fwrite(buff, 1, length, fd);
    }

    fclose(fd) ;
    free(path) ;
    if(buffIsString) {
        JS_FreeCString(ctx, buff) ;
    }
    else {
        // @todo
        // free buff
    }

    return JS_NewInt32(ctx, wroteBytes) ;
}

JSValue js_fs_readdirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)

    JS2VSFPath(path, argv[0]) ;
    DIR* dir = opendir(path);
    free(path) ;

    if(!dir) {
        JS_ThrowReferenceError(ctx, "Cound not open dir");
        return JS_EXCEPTION ;
    }
    
    struct dirent *dirEnt;
    JSValue array = JS_NewArray(ctx) ;

    int idx = 0 ;
    while((dirEnt = readdir(dir))) {
        JS_SetPropertyUint32(ctx, array, idx++, JS_NewString(ctx, dirEnt->d_name)) ;
    }
    closedir(dir);

    return array ;
}

bool fs_init() {
    const esp_vfs_littlefs_conf_t conf_etc = {
        .base_path = "/fs",
        .partition_label = "fs",
        .format_if_mount_failed = true
    };
    if(esp_vfs_littlefs_register(&conf_etc)==ESP_OK){
        printf("fs has mounted\n") ;
        return true ;
    }
    else {
        printf("Failed to mount fs.\n") ;
        return false ;
    }
}


void require_module_fs(JSContext *ctx) {

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue fs = JS_NewObject(ctx);
    
    JS_SetPropertyStr(ctx, fs, "statSync", JS_NewCFunction(ctx, js_fs_statSync, "statSync", 1));
    JS_SetPropertyStr(ctx, fs, "unlinkSync", JS_NewCFunction(ctx, js_fs_unlinkSync, "unlinkSync", 1));
    JS_SetPropertyStr(ctx, fs, "readFileSync", JS_NewCFunction(ctx, js_fs_readFileSync, "readFileSync", 1));
    JS_SetPropertyStr(ctx, fs, "writeFileSync", JS_NewCFunction(ctx, js_fs_writeFileSync, "writeFileSync", 1));
    JS_SetPropertyStr(ctx, fs, "readdirSync", JS_NewCFunction(ctx, js_fs_readdirSync, "readdirSync", 1));
    JS_SetPropertyStr(ctx, fs, "mkdirSync", JS_NewCFunction(ctx, js_fs_mkdirSync, "mkdirSync", 1));
    JS_SetPropertyStr(ctx, fs, "rmdirSync", JS_NewCFunction(ctx, js_fs_rmdirSync, "rmdirSync", 1));
    JS_SetPropertyStr(ctx, global, "fs", fs);

    JS_FreeValue(ctx, global);
}