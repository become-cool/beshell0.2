#include "module_fs.h"
#include "logging.h"
#include "esp_vfs.h"
#include "diskio_wl.h"
#include "vfs_fat_internal.h"
#include <fcntl.h>

#include "esp_littlefs.h"

LOG_TAG("fs")

JSValue js_fs_unlinkSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    return JS_UNDEFINED ;
}

JSValue js_fs_readFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    return JS_UNDEFINED ;
}

JSValue js_fs_writeFileSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(argc<2) {
        JS_ThrowReferenceError(ctx, "Missing param path");
        return JS_EXCEPTION ;
    }
    
    char * path = JS_ToCString(ctx, argv[0]) ;
	int fd = fopen(path, "w");
    if(fd<0) {
        JS_FreeCString(ctx, path) ;
        JS_ThrowReferenceError(ctx, "Failed to open file: %s", path);
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

    }
    else {
        JS_FreeCString(ctx, path) ;
        JS_ThrowReferenceError(ctx, "Invalid param type of path.");
        return JS_EXCEPTION ;
    }

    int rc = 0 ;
    if(buff) {
	    rc = write(fd, buff, length);
    }

    JS_FreeCString(ctx, path) ;
    if(buffIsString) {
        JS_FreeCString(ctx, buff) ;
    }
    else {
        // @todo
        // free buff
    }

    return JS_NewInt32(ctx, rc) ;
}

JSValue js_fs_readdirSync(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param path");
        return JS_EXCEPTION ;
    }

    char * path = JS_ToCString(ctx, argv[0]) ;
    
    DIR* dir = opendir(path);
    JS_FreeCString(ctx, path) ;

    if(!dir) {
        JS_ThrowReferenceError(ctx, "Cound not open dir: %s", path);
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

void fs_init() {
    const esp_vfs_littlefs_conf_t conf_etc = {
        .base_path = "/etc",
        .partition_label = "etc",
        .format_if_mount_failed = true
    };
    if(esp_vfs_littlefs_register(&conf_etc)==ESP_OK){
        printf("/etc has mounted\n") ;
    }
    else {
        printf("Failed to mount /etc\n") ;
    }

    const esp_vfs_littlefs_conf_t conf_home = {
        .base_path = "/home",
        .partition_label = "home",
        .format_if_mount_failed = true
    };
    if(esp_vfs_littlefs_register(&conf_home)==ESP_OK){
        printf("/home has mounted\n") ;
    }
    else {
        printf("Failed to mount /home\n") ;
    }

}


void require_module_fs(JSContext *ctx) {

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue fs = JS_NewObject(ctx);
    
    JS_SetPropertyStr(ctx, fs, "unlinkSync", JS_NewCFunction(ctx, js_fs_unlinkSync, "unlinkSync", 1));
    JS_SetPropertyStr(ctx, fs, "readFileSync", JS_NewCFunction(ctx, js_fs_readFileSync, "readFileSync", 1));
    JS_SetPropertyStr(ctx, fs, "writeFileSync", JS_NewCFunction(ctx, js_fs_writeFileSync, "writeFileSync", 1));
    JS_SetPropertyStr(ctx, fs, "readdirSync", JS_NewCFunction(ctx, js_fs_readdirSync, "readdirSync", 1));
    JS_SetPropertyStr(ctx, global, "fs", fs);

    JS_FreeValue(ctx, global);
}