#include "module_fs.h"
#include "logging.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include "utils.h"
#include <errno.h>
#include <stdbool.h>

#define PARTITION_LABEL_ROOT "fsroot"
#define PARTITION_LABEL_HOME "fshome"

#ifndef SIMULATION
#include "diskio_wl.h"
#include "esp_littlefs.h"
#include "vfs_fat_internal.h"
#include <esp_heap_caps.h>
#include "rawfs.h"
LOG_TAG("fs")
#endif

char * vfs_path_prefix = PATH_PREFIX ;
void module_fs_set_vfs_path_prefix(char * path) {
    vfs_path_prefix = path ;
}

// 返回值 由调用者 free
char * vfspath_to_fs(const char * path) {
    size_t pathlen = strlen(path) ;
    size_t prefixlen = strlen(vfs_path_prefix) ;
    char * fullpath = (void *)malloc(prefixlen+pathlen+1) ;
    if(fullpath) {
        strcpy(fullpath, vfs_path_prefix) ;
        strcpy(fullpath+prefixlen, path) ;
    }
    return fullpath ;
}

// esp32 vfs 不能挂在根目录，将整个系统挂载到 /fs 下，访问时自动加上
char * js_arg_to_vfspath(JSContext *ctx, JSValueConst argv) {
    char * jspath = JS_ToCString(ctx, argv) ;
    const char * path = vfspath_to_fs(jspath) ;
    // char * path = mallocf(PATH_PREFIX"%s", jspath) ;
    JS_FreeCString(ctx, jspath) ;
    return path ;
}


/**
 * 同步返回文件状态
 * 
 * @param {string} path 文件路径
 * @return {object} {dev, ino, mode, size, isDir, atime, mtime, ctime}
 */
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
        JS_SetPropertyStr(ctx, obj, "isDir", JS_FALSE) ;
    }
    JS_SetPropertyStr(ctx, obj, "size", JS_NewInt32(ctx, statbuf.st_size)) ;
    JS_SetPropertyStr(ctx, obj, "atime", JS_NewInt64(ctx, statbuf.st_atime)) ;
    JS_SetPropertyStr(ctx, obj, "mtime", JS_NewInt64(ctx, statbuf.st_mtime)) ;
    JS_SetPropertyStr(ctx, obj, "ctime", JS_NewInt64(ctx, statbuf.st_ctime)) ;

    return obj ;
}

/**
 * 同步返回路径是否是一个存在的目录
 * 
 * @param {string} path
 * @return {bool}
 */
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


/**
 * 同步返回路径是否是一个存在的文件
 * 
 * @param {string} path
 * @return {bool}
 */
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

/**
 * 同步返回路径是否存在
 * 
 * @param {string} path
 * @return {bool}
 */
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
bool isFile(const char * path) {
    struct stat statbuf;
    if(stat(path,&statbuf)>=0) {
        return S_ISREG(statbuf.st_mode)? true: false ;
    }
    return false ;
}

// 递归创建目录
#ifdef WIN32
int mkdir_p(char* file_path) {
#else
int mkdir_p(char* file_path, mode_t mode) {
#endif
    if( !file_path || !(*file_path) )
        return -1 ;
    char* p = strchr(file_path + 1, '/') ;
    while (p!=NULL) {
        *p = '\0';
#ifdef WIN32
        if (mkdir(file_path) == -1) {
#else
        if (mkdir(file_path, mode) == -1) {
#endif
            if (errno != EEXIST) {
                *p = '/';
                return -1;
            }
        }
        *p = '/';
        p = strchr(p + 1, '/') ;
    }

    if( !isDir(file_path) ) {
#ifdef WIN32
        return mkdir(file_path) ;
#else
        return mkdir(file_path, mode) ;
#endif
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
#ifdef WIN32
        ret = mkdir_p(path)>=0 ;
#else
        ret = mkdir_p(path, ACCESSPERMS)>=0 ;
#endif
    }
    else {
#ifdef WIN32
        ret = mkdir(path)>=0 ;
#else
        ret = mkdir(path, ACCESSPERMS)>=0 ;
#endif
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
        return JS_NewArrayBuffer(ctx, NULL, 0, freeArrayBuffer, NULL, false) ;
    }

#ifndef SIMULATION
    char * buff = heap_caps_malloc( readlen, MALLOC_CAP_SPIRAM);
#else
    char * buff = malloc(readlen) ;
#endif

    if(!buff) {
        free(path) ;
        THROW_EXCEPTION("Failed to malloc buff");
    }

	FILE * fd = fopen(path, "r");
    free(path) ;

    if(NULL==fd) {
        free(buff) ;
        THROW_EXCEPTION("Failed to open file (%d).", errno);
    }
    if(offset>0) {
        fseek(fd, offset, SEEK_SET) ;
    }

    int readedBytes = fread(buff, 1, readlen, fd) ;
    fclose(fd) ;

    return JS_NewArrayBuffer(ctx, (uint8_t *)buff, readedBytes, freeArrayBuffer, NULL, false) ;
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

	FILE * fd = fopen(path, append? "a+": "w");
    if(NULL==fd) {
        JS_ThrowReferenceError(ctx, "Failed to open file %s", path);
        free(path) ;
        return JS_EXCEPTION ;
    }

    char * buff = NULL ;
	uint32_t length;
    bool buffIsString = JS_IsString(argv[1]) ;

    if(buffIsString) {
        buff = JS_ToCString(ctx, argv[1]) ;
        length = strlen(buff) ;
    }

    else {
        buff = (char *)JS_GetArrayBuffer(ctx, &length, argv[1]) ;
        if(!buff || !length) {
            free(path) ;
            fclose(fd) ;
            JS_ThrowReferenceError(ctx, "arg data is invalid type or empty");
            return JS_EXCEPTION ;
        }
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

/**
 * argv:
 *  path string
 *  detail=false
 */
JSValue js_fs_readdir_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)

    JS2VSFPath(path, argv[0]) ;
    DIR* dir = opendir(path);

    bool detail = false ;
    if(argc>1) {
        detail = JS_ToBool(ctx, argv[1]) ;
    }

    if(!dir) {
        JS_ThrowReferenceError(ctx, "Cound not open dir %s", path);
        free(path) ;
        return JS_EXCEPTION ;
    }
    
    struct dirent *dirEnt;
    JSValue ret = JS_NewArray(ctx) ;

    int idx = 0 ;
    struct stat statbuf;
    while((dirEnt = readdir(dir))) {
        if(detail) {

            JSValue item = JS_NewObject(ctx) ;
            JS_SetPropertyUint32(ctx, ret, idx++, item) ;
            JS_SetPropertyStr(ctx, item, "name", JS_NewString(ctx, dirEnt->d_name) ) ;

            char * childpath = mallocf("%s/%s",path,dirEnt->d_name) ;
            if(!childpath){
                // pf("could not malloc for path %s/%s ?", path, dirEnt->d_name)
                THROW_EXCEPTION("could not malloc for path %s/%s ?", path, dirEnt->d_name)
                break ;
            }
            if(stat(childpath,&statbuf)!=0) {
                JS_SetPropertyStr(ctx, item, "type", JS_NewString(ctx, "unknow") ) ;
            }
            else {
                if(S_ISDIR(statbuf.st_mode)) {
                    JS_SetPropertyStr(ctx, item, "type", JS_NewString(ctx, "dir") ) ;
                }
                else {
                    JS_SetPropertyStr(ctx, item, "type", JS_NewString(ctx, "file") ) ;
                    JS_SetPropertyStr(ctx, item, "size", JS_NewUint32(ctx, statbuf.st_size) ) ;
                }
            }
            
            free(childpath) ;
        } else {
            JS_SetPropertyUint32(ctx, ret, idx++, JS_NewString(ctx, dirEnt->d_name)) ;
        }
    }

#ifndef SIMULATION
    // esp32 vfs 读取目录时，忽略了挂载点，将已知挂载点补充上
    if(strcmp(path,"/fs/")==0 || strcmp(path,"/fs")==0) {
        if(detail) {
            JSValue item = JS_NewObject(ctx) ;
            JS_SetPropertyStr(ctx, item, "name", JS_NewString(ctx, "home") ) ;
            JS_SetPropertyStr(ctx, item, "type", JS_NewString(ctx, "dir") ) ;
            JS_SetPropertyUint32(ctx, ret, idx++, item) ;
        }
        else {
            JS_SetPropertyUint32(ctx, ret, idx++, JS_NewString(ctx, "home")) ;

        }
    }
#endif

    free(path) ;
    closedir(dir);
    

    return ret ;
}

bool rm(const char * path, bool recursive) {

    struct stat statbuf;

    // 文件不存在
    if(stat(path,&statbuf)<0) {
        return true ;
    }

    // 直接删除文件
    if(S_ISREG(statbuf.st_mode)) {
        // printf("unlink(%s)\n",path) ;
        return unlink(path)>-1 ;
    }

    // 目录
    else if(S_ISDIR(statbuf.st_mode)) {
        // 递归删除文件/子目录
        if(recursive) {
            DIR* dir = opendir(path);
            if(dir) {
                struct dirent *dirEnt;
                while((dirEnt = readdir(dir))) {
                    const char * childpath = mallocf("%s/%s",path, dirEnt->d_name) ;
                    if(strcmp(dirEnt->d_name, ".")==0 || strcmp(dirEnt->d_name, "..")==0) { 
                        continue ;
                    }
                    // ds(childpath)
                    if(childpath) {                        
                        rm(childpath, recursive) ;
                        free(childpath) ;
                    }
                }
            }
        }

        // printf("rmdir(%s)\n",path) ;
        return rmdir(path) > -1 ;
    }

    // unknow type? soft link ?
    return false ;
}

JSValue js_fs_rm_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    JS2VSFPath(path, argv[0]) ;

    bool recursive = false ;
    if(argc>1) {
        recursive = JS_ToBool(ctx, argv[1]) ;
    }

    bool ret = rm(path, recursive) ;
    free(path) ;

    return ret? JS_TRUE: JS_FALSE ;
}
static JSValue js_fs_rename_sync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)
    JS2VSFPath(oldpath, argv[0]) ;
    JS2VSFPath(newpath, argv[1]) ;

    int ret = rename(oldpath, newpath) ;

    free(oldpath) ;
    free(newpath) ;

    return JS_NewInt32(ctx, ret) ;
}

static JSValue js_fs_info(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(1)
    ARGV_TO_STRING(0, jslabel)

    size_t total = 0 ;
    size_t used = 0 ;

#ifndef SIMULATION
    if(strcmp(jslabel,"/home")==0){
        if( esp_littlefs_info(PARTITION_LABEL_HOME, &total, &used) != ESP_OK ) {
            THROW_EXCEPTION("esp_littlefs_info() bad")
        }
    }
    else if(strcmp(jslabel,"/")==0) {
        vfs_rawfs_t * fs = be_rawfs_root() ;
        if(fs) {
            used = fs->size ;
            total = fs->partition_size ;
        }
    }
    else {
        JS_ThrowReferenceError(ctx, "unknow mount point: %s", jslabel) ;
        JS_FreeCString(ctx, jslabel) ;
        return JS_EXCEPTION ;
    }
#endif
    JS_FreeCString(ctx, jslabel) ;

    JSValue obj = JS_NewObject(ctx) ;
    JS_SetPropertyStr(ctx, obj, "total", JS_NewUint32(ctx, total));
    JS_SetPropertyStr(ctx, obj, "used", JS_NewUint32(ctx, used));

    return obj ;
}

#ifndef SIMULATION
void extend_partition() {
    // 检查 unextend 文件
    struct stat statbuf;
    if(stat("/fs/unextend",&statbuf)<0) {
        return ;
    }

    // 分区大小
    size_t partition_size = 0 ;
    printf("partition size ... \n") ;
    esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, NULL);
    for (; it != NULL; it = esp_partition_next(it)) {
        const esp_partition_t *part = esp_partition_get(it);
        printf("partition '%s' at offset 0x%x with size 0x%x\n", part->label, part->address, part->size);
        if(strcmp(part->label, PARTITION_LABEL_ROOT)==0) {
            partition_size = part->size ;
            break ;
        }
    }
    esp_partition_iterator_release(it);
    if(partition_size==0) {
        printf("unknow partition size\n") ;
        return ;
    }

    // 分区内的文件系统大小
    size_t fstotal = 0 ;
    size_t fsused = 0 ;
    if( esp_littlefs_info(PARTITION_LABEL_ROOT, &fstotal, &fsused) != ESP_OK ) {
        printf("esp_littlefs_info() bad\n") ;
        return ;
    }

    if(fstotal>=partition_size) {
        return ;
    }

    printf("extend partition from %d to %d ...\n", fstotal, partition_size) ;

    lfs_t * lfs = esp_littlefs_hanlde(PARTITION_LABEL_ROOT) ;

    // 删除 unextend 文件
    // return unlink(path)>-1 ;
}

bool be_module_fs_init() {
    // const esp_vfs_littlefs_conf_t conf_root = {
    //     .base_path = "/fs",
    //     .partition_label = PARTITION_LABEL_ROOT,
    //     .format_if_mount_failed = true,
    //     .block_size = 4096
    // };
    // if(esp_vfs_littlefs_register(&conf_root)!=ESP_OK){
    //     printf("Failed to mount fs.\n") ;
    //     return false ;
    // }
    
    const esp_vfs_littlefs_conf_t conf_home = {
        .base_path = "/fs/home",
        .partition_label = PARTITION_LABEL_HOME,
        .format_if_mount_failed = true ,
        .block_size = 4096
    };
    if(esp_vfs_littlefs_register(&conf_home)!=ESP_OK){
        printf("Failed to mount fs.\n") ;
        return false ;
    }

    // extend_partition() ;
    
    return true ;
}
#endif

void be_module_fs_require(JSContext *ctx) {

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beapi = JS_GetPropertyStr(ctx, global, "beapi") ;
    
    JSValue fs = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "fs", fs);
    
    JS_SetPropertyStr(ctx, fs, "statSync", JS_NewCFunction(ctx, js_fs_stat_sync, "statSync", 1));
    JS_SetPropertyStr(ctx, fs, "unlinkSync", JS_NewCFunction(ctx, js_fs_unlink_sync, "unlinkSync", 1));
    JS_SetPropertyStr(ctx, fs, "readFileSync", JS_NewCFunction(ctx, js_fs_read_file_sync, "readFileSync", 1));
    JS_SetPropertyStr(ctx, fs, "writeFileSync", JS_NewCFunction(ctx, js_fs_write_file_sync, "writeFileSync", 1));
    JS_SetPropertyStr(ctx, fs, "readdirSync", JS_NewCFunction(ctx, js_fs_readdir_sync, "readdirSync", 1));
    JS_SetPropertyStr(ctx, fs, "mkdirSync", JS_NewCFunction(ctx, js_fs_mkdir_sync, "mkdirSync", 1));
    JS_SetPropertyStr(ctx, fs, "rmdirSync", JS_NewCFunction(ctx, js_fs_rmdir_sync, "rmdirSync", 1));
    JS_SetPropertyStr(ctx, fs, "rmSync", JS_NewCFunction(ctx, js_fs_rm_sync, "rmSync", 1));
    JS_SetPropertyStr(ctx, fs, "existsSync", JS_NewCFunction(ctx, js_fs_exists_sync, "existsSync", 1));
    JS_SetPropertyStr(ctx, fs, "isDirSync", JS_NewCFunction(ctx, js_fs_is_dir_sync, "isDirSync", 1));
    JS_SetPropertyStr(ctx, fs, "isFileSync", JS_NewCFunction(ctx, js_fs_is_file_sync, "isFileSync", 1));
    JS_SetPropertyStr(ctx, fs, "renameSync", JS_NewCFunction(ctx, js_fs_rename_sync, "renameSync", 1));
    JS_SetPropertyStr(ctx, fs, "info", JS_NewCFunction(ctx, js_fs_info, "info", 1));

    JS_FreeValue(ctx, global);
    JS_FreeValue(ctx, beapi);
}
