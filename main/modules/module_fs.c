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
#include <errno.h>

LOG_TAG("fs")

// esp32 vfs 不能挂在根目录，将整个系统挂载到 /fs 下，访问时自动加上
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

JSValue js_fs_stat_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
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
    if(S_ISDIR(statbuf.st_mode)) {
        JS_SetPropertyStr(ctx, obj, "isDir", JS_TRUE) ;
    }
    else {
        JS_SetPropertyStr(ctx, obj, "size", JS_NewInt32(ctx, statbuf.st_size)) ;
        JS_SetPropertyStr(ctx, obj, "isDir", JS_FALSE) ;
    }
    JS_SetPropertyStr(ctx, obj, "atime", JS_NewInt64(ctx, statbuf.st_atime)) ;
    JS_SetPropertyStr(ctx, obj, "mtime", JS_NewInt64(ctx, statbuf.st_mtime)) ;
    JS_SetPropertyStr(ctx, obj, "ctime", JS_NewInt64(ctx, statbuf.st_ctime)) ;

    return obj ;
}

JSValue js_fs_is_dir_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    JS2VSFPath(path, argv[0]) 
    struct stat statbuf;
    if(stat(path,&statbuf)<0) {
        free(path) ;
        return JS_FALSE ;
    }
    free(path) ;
    return S_ISDIR(statbuf.st_mode)? JS_TRUE: JS_FALSE ;
}
JSValue js_fs_is_file_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    JS2VSFPath(path, argv[0]) 
    struct stat statbuf;
    if(stat(path,&statbuf)<0) {
        free(path) ;
        return JS_FALSE ;
    }
    free(path) ;
    return S_ISREG(statbuf.st_mode)? JS_TRUE: JS_FALSE ;
}

JSValue js_fs_exists_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    JS2VSFPath(path, argv[0])
    struct stat statbuf;
    bool exists = stat(path,&statbuf)>=0 ;
    free(path) ;
    return exists? JS_TRUE: JS_FALSE ;
}


bool isDir(const char * path) {
    struct stat statbuf;
    if(stat(path,&statbuf)>=0) {
        return S_ISDIR(statbuf.st_mode)? true: false ;
    }
    return false ;
}

// 递归创建目录
int mkdir_p(char* file_path, mode_t mode) {
    if( !file_path || !(*file_path) )
        return -1 ;
    for (char* p = strchr(file_path + 1, '/'); p; p = strchr(p + 1, '/')) {
        *p = '\0';
        if (mkdir(file_path, mode) == -1) {
            if (errno != EEXIST) {
                *p = '/';
                return -1;
            }
        }
        *p = '/';
    }

    if( !isDir(file_path) ) {
        return mkdir(file_path, mode) ;
    }
    else 
        return 0 ;
}

/**
 * argv:
 *   path string
 *   recursive=false bool
 */
JSValue js_fs_mkdir_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param path");
        return JS_EXCEPTION ;
    }
    JS2VSFPath(path, argv[0]) ;
    
    struct stat statbuf;
    if(stat(path,&statbuf)>=0) {
        free(path) ;
        return S_ISDIR(statbuf.st_mode)? JS_TRUE: JS_FALSE ;
    }

    bool recursive = false ;
    if(argc>1) {
        recursive = JS_ToBool(ctx, argv[1]) ;
    }

    bool ret ;
    if(recursive) {
        ret = mkdir_p(path, ACCESSPERMS)>=0 ;
    }
    else {
        ret = mkdir(path, ACCESSPERMS)>=0 ;
    }
    free(path) ;
    return ret? JS_TRUE: JS_FALSE ;
}

JSValue js_fs_rmdir_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param path");
        return JS_EXCEPTION ;
    }
    JS2VSFPath(path, argv[0]) ;
    bool ret = rmdir(path)>=0 ;
    free(path) ;
    return ret? JS_TRUE: JS_FALSE ;
}

JSValue js_fs_unlink_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param path");
        return JS_EXCEPTION ;
    }
    JS2VSFPath(path, argv[0]) ;
    bool ret = unlink(path)>=0 ;
    free(path) ;
    return ret? JS_TRUE: JS_FALSE ;
}

/**
 * argv:
 *  path string
 *  readlen=-1 int
 *  offset=0 int
 */
JSValue js_fs_read_file_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    
    JS2VSFPath(path, argv[0])
    CHECK_ARGV0_NOT_DIR(path)

    int readlen = -1 ;
    if(argc>1) {
        JS_ToInt32(ctx, &readlen, argv[1]) ;
    }
    int offset = 0 ;
    if(argc>2) {
        JS_ToInt32(ctx, &offset, argv[2]) ;
    }

    if(readlen<0) {
        readlen = statbuf.st_size ;
    }

    if(readlen<1){
        return JS_NewStringLen(ctx, "", 0) ;
    }

    char * buff = malloc(readlen) ;
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

    if(offset>0)
        fseek(fd, offset, SEEK_SET) ;

    int readedBytes = fread(buff, 1, readlen, fd) ;
    // buff[readedBytes] = 0 ;

    fclose(fd) ;

    JSValue str = JS_NewStringLen(ctx, buff, readedBytes) ;
    free(buff) ;

    return str ;
}

/**
 * argv:
 *  path string
 *  data string
 *  append=false int
 */
JSValue js_fs_write_file_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

    CHECK_ARGC(2)
    JS2VSFPath(path, argv[0])

    bool append = false ;

    // 文件已经存在
    struct stat statbuf;
    if(stat(path,&statbuf)>=0) {
        CHECK_ARGV0_NOT_DIR(path)
        // offset
        if(argc>2) {
            append = JS_ToBool(ctx, argv[2]) ;
        }
    }

	int fd = fopen(path, append? "a+": "w");
    if(fd<=0) {
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
        // printf("write(%d): %s\n", length, buff) ;
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

JSValue js_fs_readdir_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
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
    
    JS_SetPropertyStr(ctx, fs, "statSync", JS_NewCFunction(ctx, js_fs_stat_sync, "statSync", 1));
    JS_SetPropertyStr(ctx, fs, "unlinkSync", JS_NewCFunction(ctx, js_fs_unlink_sync, "unlinkSync", 1));
    JS_SetPropertyStr(ctx, fs, "readFileSync", JS_NewCFunction(ctx, js_fs_read_file_sync, "readFileSync", 1));
    JS_SetPropertyStr(ctx, fs, "writeFileSync", JS_NewCFunction(ctx, js_fs_write_file_sync, "writeFileSync", 1));
    JS_SetPropertyStr(ctx, fs, "readdirSync", JS_NewCFunction(ctx, js_fs_readdir_sync, "readdirSync", 1));
    JS_SetPropertyStr(ctx, fs, "mkdirSync", JS_NewCFunction(ctx, js_fs_mkdir_sync, "mkdirSync", 1));
    JS_SetPropertyStr(ctx, fs, "rmdirSync", JS_NewCFunction(ctx, js_fs_rmdir_sync, "rmdirSync", 1));
    JS_SetPropertyStr(ctx, fs, "existsSync", JS_NewCFunction(ctx, js_fs_exists_sync, "existsSync", 1));
    JS_SetPropertyStr(ctx, fs, "isDirSync", JS_NewCFunction(ctx, js_fs_is_dir_sync, "isDirSync", 1));
    JS_SetPropertyStr(ctx, fs, "isFileSync", JS_NewCFunction(ctx, js_fs_is_file_sync, "isFileSync", 1));
    JS_SetPropertyStr(ctx, global, "fs", fs);

    JS_FreeValue(ctx, global);
}