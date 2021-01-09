#include "module_utils.h"
#include "module_utils.h"
#include "logging.h"
#include "esp_system.h"
#include <string.h>
#include <time.h>
#include "eventloop.h"
#include "utils.h"
#include "telnet.h"
#include "module_fs.h"
#include "soc/soc.h"

#include "untar.h"
#include "libb64/cdecode.h"
#include "libb64/cencode.h"

LOG_TAG("util");

JSValue js_util_free_stacks(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    (void) argc ;
    (void) argv ;
    return JS_NewInt32(ctx, esp_get_free_heap_size()) ;
}

JSValue js_util_uuid(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

    uint8_t mac_addr[6] = {0};
    char mac_addr_str[13] = {0};
    esp_efuse_mac_get_default(mac_addr);

    sprintf(mac_addr_str, "%02x%02x%02x%02x%02x%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]) ;
    mac_addr_str[12] = 0 ;
    // printf(mac_addr_str, "[%02x%02x%02x%02x%02x%02x]\n", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]) ;

    return JS_NewStringLen(ctx, mac_addr_str, 12) ;
}


typedef struct {
	char *          levelString;
	esp_log_level_t level;
} level_t;
static level_t levels[] = {
	{"none",    ESP_LOG_NONE},
	{"error",   ESP_LOG_ERROR},
	{"warn",    ESP_LOG_WARN},
	{"info",    ESP_LOG_INFO},
	{"debug",   ESP_LOG_DEBUG},
	{"verbose", ESP_LOG_VERBOSE},
	{NULL, 0}
};

JSValue js_util_set_log_level(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    if(argc<2) {
        JS_ThrowReferenceError(ctx, "Missing param");
        return JS_EXCEPTION ;
    }
	char * tagToChange = (char *)JS_ToCString(ctx, argv[0]);
	char * levelString = (char *)JS_ToCString(ctx, argv[1]);

	// printf("Setting a new log level to be tag: \"%s\", level: \"%s\"\n", tagToChange, levelString);

	level_t *pLevels = levels;
	while(pLevels->levelString != NULL) {
		if (strcmp(pLevels->levelString, levelString) == 0) {
			break;
		}
		pLevels++;
	}
	if (pLevels->levelString != NULL) {
		esp_log_level_set(tagToChange, pLevels->level);
	    return JS_UNDEFINED ;
	}
    else {
        JS_ThrowReferenceError(ctx, "missing param path");
        return JS_EXCEPTION ;
    }
}

JSValue js_util_time(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	struct timespec tm = {0, 0};
	clock_gettime(CLOCK_REALTIME, &tm);
	return JS_NewInt64(ctx, tm.tv_nsec/1000/1000 + tm.tv_sec*1000) ;
}

JSValue js_util_var_ptr(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param");
        return JS_EXCEPTION ;
	}
	return JS_NewInt32(ctx, (uint32_t)argv[0]) ;
}
JSValue js_util_ptr_refcount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param");
        return JS_EXCEPTION ;
	}
	uint32_t ptr ;
	if(JS_ToUint32(ctx, &ptr, argv[0]) ) {
        JS_ThrowReferenceError(ctx, "Invalid param type");
        return JS_EXCEPTION ;
	}
	return JS_NewInt32(ctx, JS_VALUE_HAS_REF_COUNT((JSValue) ptr)? VAR_REFCNT((JSValue) ptr): 0 ) ;
}
JSValue js_util_var_refcount(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param");
        return JS_EXCEPTION ;
	}
	return JS_NewInt32(ctx, JS_VALUE_HAS_REF_COUNT(argv[0])? VAR_REFCNT(argv[0]): 0) ;
}


JSValue __js_util_set_timer(JSContext *ctx, int argc, JSValueConst *argv, bool repeat){
    if(argc<2) {
        JS_ThrowReferenceError(ctx, "Missing param");
        return JS_EXCEPTION ;
    }
	uint32_t interval ;
	if(JS_ToUint32(ctx, &interval, argv[1]) ) {
        JS_ThrowReferenceError(ctx, "Invalid param type");
        return JS_EXCEPTION ;
	}
	uint32_t addr = eventloop_push(ctx, (JSValue)argv[0], interval, repeat) ;
	return JS_NewInt32(ctx, addr) ;
}

JSValue js_util_set_timeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	return __js_util_set_timer(ctx, argc, argv, false) ;
}
JSValue js_util_set_interval(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	return __js_util_set_timer(ctx, argc, argv, true) ;
}

JSValue js_util_clear_timeout(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	if(argc<1) {
        JS_ThrowReferenceError(ctx, "Missing param");
        return JS_EXCEPTION ;
    }
	uint32_t ptr ;
	if(JS_ToUint32(ctx, &ptr, argv[0]) ) {
        JS_ThrowReferenceError(ctx, "Invalid param type");
        return JS_EXCEPTION ;
	}
	eventloop_remove(ctx, (struct eventloop_callback_t *)ptr) ;
	return JS_UNDEFINED ;
}


#define require_wrapper 												\
	"let module = {exports={}} ;\n"										\
	"(function(exports, require, module, __filename, __dirname) {\n" 	\
	"%s" 																\
	"})(exports); "
JSValue js_util_require_abspath(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	CHECK_ARGC(1)
	char * path = JS_ToCString(ctx, argv[0]) ;


	JS_FreeCString(ctx, path) ;
	return JS_UNDEFINED ;
}

void evalScript(JSContext *ctx, const char * path) {

    struct stat statbuf;
    if(stat(path,&statbuf)<0) {
        LOGE("Path not exists: %s\n", path) ;
        return NULL ;
    }
    if(S_ISDIR(statbuf.st_mode)){
        LOGE("Path is directory: %s\n", path) ;
        return NULL ;
    }
    char * buff = malloc(statbuf.st_size+1) ;
    if(!buff) {
        LOGE("Cound not malloc buff(%d), memory low?\n", (int)statbuf.st_size+1) ;
        return NULL ;
    }
	int fd = fopen(path, "r") ;
    if(fd<0) {
        LOGE("Cound not open path: %s\n", path) ;
        return NULL ;
    }

    int readedBytes = fread(buff, 1, statbuf.st_size, fd) ;
    fclose(fd) ;
    buff[readedBytes] = 0 ;

    EVAL_CODE_LEN(buff, readedBytes, path) ;
    free(buff) ;
}

JSValue js_fs_eval_script(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    JS2VSFPath(path, argv[0])
    CHECK_ARGV0_NOT_DIR(path)
    evalScript(ctx, path) ;
    free(path) ;
	return JS_UNDEFINED ;
}

JSValue js_fs_eval_as_file(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)

    size_t len = 0 ;
    char * code = JS_ToCStringLen(ctx, &len, argv[0]) ;
    char * path = JS_ToCString(ctx, argv[1]) ;

    JSValue ret = JS_Eval(ctx, code, len, path, JS_EVAL_TYPE_GLOBAL) ;
	JS_FreeCString(ctx, code) ;
    return ret ;
}


JSValue js_repl_set_input_func(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    if(!JS_IsFunction(ctx, argv[0])) {
        THROW_EXCEPTION("REPL input function must be a function type")
    }
    telnet_set_input_function(ctx, argv[0]) ;
    return JS_UNDEFINED ;
}


JSValue js_utils_part_id(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    int value = REG_READ(0x3FF5A078) ;
    return JS_NewInt32(ctx, value) ;
}

JSValue js_utils_base64_encode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    size_t srclen = 0 ;
    char * src = JS_ToCStringLen(ctx, &srclen, argv[0]) ;
    size_t codelen = base64_encode_expected_len(srclen) ;
    char * code = (char *) malloc(codelen+1);
    if(!code) {
        JS_FreeCString(ctx, src) ;
        THROW_EXCEPTION("Cound not malloc for encode, memory low ?")
    }

    base64_encodestate _state;
    base64_init_encodestate(&_state);
    int len = base64_encode_block((const char *) &src[0], srclen, &code[0], &_state);
    len = base64_encode_blockend((code + len), &_state);
    
    JSValue ret = JS_NewStringLen(ctx, code, codelen) ;

    JS_FreeCString(ctx, src) ;
    free(code);

    return ret ;
}
JSValue js_utils_base64_decode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    size_t codelen = 0 ;
    char * code = JS_ToCStringLen(ctx, &codelen, argv[0]) ;
    size_t srclen = base64_decode_expected_len(codelen) + 1;
    char * src = (char *) malloc(srclen);
    if(!src) {
        JS_FreeCString(ctx, src) ;
        THROW_EXCEPTION("Cound not malloc for decode, memory low ?")
    }
    
    base64_decodestate _state;
    base64_init_encodestate(&_state);

    base64_decode_block((const char *) &code[0], codelen, &src[0], &_state);

    JSValue ret = JS_NewStringLen(ctx, src, srclen) ;

    JS_FreeCString(ctx, code) ;
    free(src);

    return ret ;
}

const uint8_t gamma8_table[256] = {
          0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
          0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
          2,  2,  2,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
          5,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9, 10, 10, 10,
         11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17,
         18, 18, 19, 19, 20, 20, 21, 21, 22, 23, 23, 24, 24, 25, 26, 26,
         27, 28, 28, 29, 30, 30, 31, 32, 32, 33, 34, 35, 35, 36, 37, 38,
         38, 39, 40, 41, 42, 42, 43, 44, 45, 46, 47, 48, 49, 49, 50, 51,
         52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
         69, 70, 71, 72, 73, 74, 75, 76, 78, 79, 80, 81, 82, 84, 85, 86,
         87, 89, 90, 91, 92, 94, 95, 96, 98, 99,100,102,103,104,106,107,
        109,110,112,113,114,116,117,119,120,122,123,125,126,128,130,131,
        133,134,136,138,139,141,143,144,146,148,149,151,153,154,156,158,
        160,161,163,165,167,169,170,172,174,176,178,180,182,183,185,187,
        189,191,193,195,197,199,201,203,205,207,209,211,213,215,218,220,
        222,224,226,228,230,233,235,237,239,241,244,246,248,250,253,255,
};


JSValue js_utils_gamma8_correct(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0, val)
    return JS_NewUint32(ctx, gamma8_table[val]) ;
}


int untar_dist_fd = NULL;
int untar_entry_header_cb(header_translated_t *proper, int entry_index, void *context_data) {
    if(proper->type == T_NORMAL) {
        char * distpath = mallocf("%s/%s", (char *)context_data, proper->filename) ;
        printf(distpath) ;
        printf("\n") ;
        if((untar_dist_fd = fopen(distpath, "wb")) == NULL) {
            printf("Could not open [%s] for write.\n", distpath);
            free(distpath) ;
            return -1;
        }
        free(distpath) ;
    }
    else if(proper->type == T_DIRECTORY) {
        pf("dir:%s", proper->filename)
        char * distpath = mallocf("%s/%s", (char *)context_data, proper->filename) ;
        if( mkdir_p(distpath, ACCESSPERMS) !=0 ){
            free(distpath) ;
            return -1 ;
        }
        free(distpath) ;
    } else {
        untar_dist_fd = NULL ;
    }

    return 0 ;
}
int untar_entry_data_cb(header_translated_t *proper, int entry_index, void *context_data, unsigned char *block, int length) {
    if(untar_dist_fd != NULL)
        fwrite(block, length, 1, untar_dist_fd);
    return 0 ;
}
int untar_entry_end_cb(header_translated_t *proper, int entry_index, void *context_data) {
    if(untar_dist_fd != NULL) {
        fclose(untar_dist_fd);
        untar_dist_fd = NULL;
    }
    return 0 ;
}

JSValue js_fs_untar(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    CHECK_ARGC(2)

    char * srcpath = js_arg_to_vfspath(ctx, argv[0])  ;
    char * distpath = js_arg_to_vfspath(ctx, argv[1])  ;

    if( mkdir_p(distpath, ACCESSPERMS) != 0 ){
        THROW_EXCEPTION("dist path invalid")
    }

    // untar
    entry_callbacks_t entry_callbacks = { untar_entry_header_cb, untar_entry_data_cb, untar_entry_end_cb };
    int res = read_tar(srcpath, &entry_callbacks, (void *)distpath)==0 ;

    free(srcpath) ;
    free(distpath) ;

    return res==0? JS_TRUE: JS_FALSE ;
}

JSValue js_string_bytes(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_STRING(0, str, len)

    JS_FreeCString(ctx, str) ;
    return JS_NewUint32(ctx, len) ;
}

JSValue js_pack(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    size_t totalsize = 0 ;
    char * str ;
    size_t num = 0 ;
    for(int i=0;i<argc; i++) {
        if(JS_IsString(argv[i])) {
            str = JS_ToCStringLen(ctx, &num, argv[i]) ;
            JS_FreeCString(ctx, str) ;
            str = NULL ;
            totalsize+= num + 1 ;
        }
        else if(JS_IsNumber(argv[i])) {
            totalsize+= 4 ;
        }
        else {
            THROW_EXCEPTION("Invalid data type of package")
        }
    }

    char * pkgbuff = malloc(totalsize) ;
    JSValue pkg = JS_NewArrayBuffer(ctx, (uint8_t *)pkgbuff, totalsize, freeArrayBuffer, NULL, false) ;

    size_t offset = 0 ;
    for(int i=0;i<argc; i++) {
        if(JS_IsString(argv[i])) {
            str = JS_ToCStringLen(ctx, &num, argv[i]) ;
            if(num>255)
                num = 255 ;
            * (pkgbuff+offset) = num ;
            offset+= 1 ;
            memcpy(pkgbuff+offset, str, num) ;
            offset+= num ;
            JS_FreeCString(ctx, str) ;
        }
        else if(JS_IsNumber(argv[i])) {
            if(JS_ToInt32(ctx, (int32_t*)&num, argv[i])){
                // dd
            }
            memcpy(pkgbuff+offset, (void *)&num, 4) ;
            offset+= 4 ;
        }
    }

    return pkg ;
}

JSValue js_pack_int32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(3)
    ARGV_TO_ARRAYBUFFER(0, buff, bufflen)
    ARGV_TO_UINT16(1, offset)
    if(offset+4>bufflen) {
        THROW_EXCEPTION("out of buffer length")
    }
    ARGV_TO_UINT32(2, num)
    memcpy(buff+offset, (void *)&num, 4) ;
    return JS_NewUint32(ctx, offset+4) ;
}
JSValue js_unpack_int32(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_ARRAYBUFFER(0, buff, bufflen)
    ARGV_TO_UINT16(1, offset)
    if(offset+4>bufflen) {
        THROW_EXCEPTION("out of buffer length")
    }
    return JS_NewInt32(ctx, * (int32_t *)(buff+offset)) ;
}
JSValue js_pack_string(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(3)
    ARGV_TO_ARRAYBUFFER(0, buff, bufflen)
    ARGV_TO_UINT16(1, offset)
    ARGV_TO_STRING(2, str, strlen)

    if(strlen>255) {
        strlen = 255 ;
    }
    if(offset+strlen+1>bufflen) {
        strlen = bufflen - offset - 1 ;
    }

    *(buff+offset) = strlen ;

    if(strlen>0) {
        memcpy(buff+offset+1, str, strlen) ;
    }

    JS_FreeCString(ctx, str) ;
    return JS_NewUint32(ctx, offset+strlen+1) ;
}
JSValue js_unpack_string(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_ARRAYBUFFER(0, buff, bufflen)
    ARGV_TO_UINT16(1, offset)
    uint8_t strlen = *(buff+offset) ;
    if(offset+strlen+1>bufflen) {
        strlen = bufflen - offset - 1 ;
    }
    return JS_NewStringLen(ctx, buff+offset+1, strlen) ;
}

JSValue js_write_string_to_ArrayBuffer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)
    ARGV_TO_ARRAYBUFFER(0, buff, bufflen)
    ARGV_TO_STRING(1, str, strlen)
    size_t offset = 0 ;
    if(argc>=3) {
        if( JS_ToUint32(ctx, &offset, argv[2]) ) {
            JS_FreeCString(ctx, str) ;
            THROW_EXCEPTION("argv offset is not a number")
        }
    }
    if(offset+strlen>bufflen) {
        JS_FreeCString(ctx, str) ;
        THROW_EXCEPTION("out of buffer length")
    }
    memcpy(buff+offset, str, strlen) ;

    JS_FreeCString(ctx, str) ;
    return JS_UNDEFINED ;
}

JSValue js_read_string_from_ArrayBuffer(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_ARRAYBUFFER(0, buff, bufflen)

    size_t offset = 0 ;
    if(argc>=2) {
        if( JS_ToUint32(ctx, &offset, argv[1]) ) {
            THROW_EXCEPTION("argv offset is not a number")
        }
    }
    size_t strlen = 0 ;
    if(argc>=3) {
        if( JS_ToUint32(ctx, &strlen, argv[2]) ) {
            THROW_EXCEPTION("argv length is not a number")
        }
    }
    else {
        strlen = bufflen - offset ;
    }

    if(offset+strlen>bufflen) {
        THROW_EXCEPTION("out of buffer length")
    }

    return JS_NewStringLen(ctx, buff+offset, strlen) ;
}

void require_module_utils(JSContext *ctx) {

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beshellapi = JS_GetPropertyStr(ctx, global, "beshellapi") ;
    JS_SetPropertyStr(ctx, beshellapi, "_repl_set_input_func", JS_NewCFunction(ctx, js_repl_set_input_func, "_repl_set_input_func", 1));

	// utils 
    JSValue utils = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beshellapi, "utils", utils);
    JS_SetPropertyStr(ctx, utils, "freeStacks", JS_NewCFunction(ctx, js_util_free_stacks, "freeStacks", 1));
    JS_SetPropertyStr(ctx, utils, "time", JS_NewCFunction(ctx, js_util_time, "time", 1));
    JS_SetPropertyStr(ctx, utils, "setLogLevel", JS_NewCFunction(ctx, js_util_set_log_level, "setLogLevel", 1));
    JS_SetPropertyStr(ctx, utils, "partId", JS_NewCFunction(ctx, js_utils_part_id, "partId", 1));
    JS_SetPropertyStr(ctx, utils, "uuid", JS_NewCFunction(ctx, js_util_uuid, "uuid", 1));
    JS_SetPropertyStr(ctx, utils, "ptrRefCnt", JS_NewCFunction(ctx, js_util_ptr_refcount, "ptrRefCnt", 1));
    JS_SetPropertyStr(ctx, utils, "varRefCnt", JS_NewCFunction(ctx, js_util_var_refcount, "varRefCnt", 1));
    JS_SetPropertyStr(ctx, utils, "varPtr", JS_NewCFunction(ctx, js_util_var_ptr, "varPtr", 1));
    JS_SetPropertyStr(ctx, utils, "base64Encode", JS_NewCFunction(ctx, js_utils_base64_encode, "base64Encode", 1));
    JS_SetPropertyStr(ctx, utils, "base64Decode", JS_NewCFunction(ctx, js_utils_base64_decode, "base64Decode", 1));
    JS_SetPropertyStr(ctx, utils, "gamma8Correct", JS_NewCFunction(ctx, js_utils_gamma8_correct, "gamma8Correct", 1));
    JS_SetPropertyStr(ctx, utils, "untar", JS_NewCFunction(ctx, js_fs_untar, "untar", 1));
    JS_SetPropertyStr(ctx, utils, "stringBytes", JS_NewCFunction(ctx, js_string_bytes, "stringBytes", 1));
    JS_SetPropertyStr(ctx, utils, "pack", JS_NewCFunction(ctx, js_pack, "pack", 1));
    JS_SetPropertyStr(ctx, utils, "packInt32", JS_NewCFunction(ctx, js_pack_int32, "packInt32", 1));
    JS_SetPropertyStr(ctx, utils, "unpackInt32", JS_NewCFunction(ctx, js_unpack_int32, "unpackInt32", 1));
    JS_SetPropertyStr(ctx, utils, "packString", JS_NewCFunction(ctx, js_pack_string, "packString", 1));
    JS_SetPropertyStr(ctx, utils, "unpackString", JS_NewCFunction(ctx, js_unpack_string, "unpackString", 1));
    JS_SetPropertyStr(ctx, utils, "writeStringToArrayBuffer", JS_NewCFunction(ctx, js_write_string_to_ArrayBuffer, "writeStringToArrayBuffer", 1));
    JS_SetPropertyStr(ctx, utils, "readStringFromArrayBuffer", JS_NewCFunction(ctx, js_read_string_from_ArrayBuffer, "readStringFromArrayBuffer", 1));

	// global
    JS_SetPropertyStr(ctx, global, "setTimeout", JS_NewCFunction(ctx, js_util_set_timeout, "setTimeout", 1));
    JS_SetPropertyStr(ctx, global, "setInterval", JS_NewCFunction(ctx, js_util_set_interval, "setInterval", 1));
    JS_SetPropertyStr(ctx, global, "clearTimeout", JS_NewCFunction(ctx, js_util_clear_timeout, "clearTimeout", 1));
    JS_SetPropertyStr(ctx, global, "evalScript", JS_NewCFunction(ctx, js_fs_eval_script, "evalScript", 1));
    JS_SetPropertyStr(ctx, global, "evalAsFile", JS_NewCFunction(ctx, js_fs_eval_as_file, "evalAsFile", 1));

    
	JS_FreeValue(ctx, beshellapi);
	JS_FreeValue(ctx, global);
}