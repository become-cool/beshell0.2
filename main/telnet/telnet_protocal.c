#include "telnet_protocal.h"
#include "module_fs.h"
#include "utils.h"
#include <string.h>
#include <math.h>
#include <sys/stat.h>


TelnetProtFuncSend  telnet_prot_func_pkg_send = NULL ;
TelnetProtFuncReset telnet_prot_func_reset = NULL ;

static uint8_t send_buff [270] ;

void on_pkg_receive (uint8_t pkgid, uint8_t remain, uint8_t cmd, uint8_t * data, uint8_t datalen, void * ctx) ;

static uint8_t checksum(struct telnet_prot_buffer * buff) {
    uint8_t sum = 0 ;
    for(uint16_t i=0; i<PKG_HEADER_LEN + buff->bytes[PKGPOS_DATALEN]; i++) {
        sum^= buff->bytes[i] ;
    }
    return sum ;
}

void telnet_prot_push_char(struct telnet_prot_buffer * buff, uint8_t byte, const void * ctx) {
    // 包头
    if(buff->writepos == PKGPOS_HEAD1) {
        // printf("writepos=%d,byte=%d\n",buff->writepos, byte) ; 
        if(byte==PKG_HEAD1) {
            buff->bytes[buff->writepos ++] = byte ;
        }
        return ;
    }
    else if(buff->writepos == PKGPOS_HEAD2) {
        // printf("writepos=%d,byte=%d\n",buff->writepos, byte) ; 
        if(byte==PKG_HEAD2) {
            buff->bytes[buff->writepos ++] = byte ;
        }
        return ;
    }
    else if( buff->writepos< PKGPOS_DATALEN) {
        buff->bytes[buff->writepos ++] = byte ;
        return ;
    }

    // 数据长度
    else if( buff->writepos == PKGPOS_DATALEN){
        // printf("datalen=%d\n",byte) ;
        // 大于最大data长度, 丢弃
        if(byte>PKGLEN_MAX_DATA) {
            buff->writepos = 0 ;
            return ;
        }
        buff->bytes[buff->writepos ++] = byte ;
        // dn(buff->bytes[PKGPOS_DATALEN])
        return ;
    }

    // 数据区
    else if( buff->writepos < (PKG_HEADER_LEN + buff->bytes[PKGPOS_DATALEN]) ) {
        // printf("%d->%d\n",buff->writepos,byte) ;
        buff->bytes[buff->writepos ++] = byte ;
        return ;
    }

    // 校验位
    else if( buff->writepos == (PKG_HEADER_LEN + buff->bytes[PKGPOS_DATALEN]) ) {
        // 重置
        buff->writepos = 0 ;
        // 完成
        if( checksum(buff)==byte ) {
            buff->bytes[ PKG_HEADER_LEN + buff->bytes[PKGPOS_DATALEN] ] = 0 ;
            on_pkg_receive(
                buff->bytes[PKGPOS_ID] ,
                buff->bytes[PKGPOS_REMAIN] ,
                buff->bytes[PKGPOS_CMD] ,
                buff->bytes + PKG_HEADER_LEN ,
                buff->bytes[PKGPOS_DATALEN] ,
                ctx
            ) ;
        }
        else {
        }
    }

    // 异常,丢弃数据
    else {
        // printf("invalid writepos=%d\n", buff->writepos) ;
        buff->writepos = 0 ;
    }
}

void telnet_prot_push_bytes(struct telnet_prot_buffer * buff, uint8_t * dat, uint8_t length, const void * ctx) {
    for(uint8_t i=0; i<length; i++) {
        telnet_prot_push_char( buff, *(dat++), ctx ) ;
    }
}


uint8_t telnet_prot_pack(uint8_t * pkg, uint8_t pkgId, uint8_t remain, uint8_t cmd, uint8_t * dat, uint8_t datalen) {
    pkg[0] = PKG_HEAD1 ;
    pkg[1] = PKG_HEAD2 ;
    pkg[2] = pkgId ;
    pkg[3] = remain ;
    pkg[4] = cmd ;


    pkg[5] = datalen ;

    
    uint8_t i ;
    for(i=0;i<datalen;i++) {
        pkg[6+i] = *(dat+i) ;
    }
    uint16_t pkglen = PKGLEN_WITHOUT_DATA + datalen ;
    // 算校验和
    uint8_t sum = 0 ;
    for(i=0; i<=pkglen-2; i++) {
        sum^= pkg[i] ;
    }
    pkg[pkglen-1] = sum ;

    return pkglen ;
}




static int _file_pushing_pkgid = -1 ;
static char * _file_pushing_path = NULL ;

void telnet_prot_reset() {
	_file_pushing_pkgid = -1 ;
	if(_file_pushing_path) {
		free(_file_pushing_path) ;
		_file_pushing_path = NULL ;
	}
}




void telnet_proto_send_one_pkg(char pkgid, char remain, char cmd, char * data, uint8_t datalen) {

	uint16_t pkglen = PKGLEN_WITHOUT_DATA + datalen ;
	
	// printf("datalen=%d, pkglen=%d\n", datalen, pkglen) ;

	telnet_prot_pack((uint8_t*)send_buff, pkgid, remain, cmd, (uint8_t*)data, datalen) ;

	// 输出
	telnet_prot_func_pkg_send(send_buff, pkglen);
	// uart_wait_tx_done(0, 10000);
}

void telnet_proto_send_pkg(char pkgid, char cmd, char * data, uint16_t datalen) {
	
	int pkgcnt = (int) ceil( (float)datalen/(float)PKGLEN_MAX_DATA ) ;

	// printf("send>datalen=%d, max=%d, pkgcnt=%d\n", datalen, PKGLEN_MAX_DATA, pkgcnt) ;
	// printf("%s\n",data) ;

	for(int i=pkgcnt-1; i>=1; i--) {
		// printf("%d, len=%d\n", i, PKGLEN_MAX_DATA) ;
		telnet_proto_send_one_pkg(pkgid, i, cmd, data, PKGLEN_MAX_DATA) ;
		data+= PKGLEN_MAX_DATA ;
	}

	// 最后一个包
	telnet_proto_send_one_pkg(pkgid, 0, cmd, data, datalen%PKGLEN_MAX_DATA) ;
}



void telnet_proto_send_pkg_str(char pkgid, char cmd, char * data) {
	telnet_proto_send_pkg(pkgid, cmd, data, strlen(data)) ;
}



static bool write_file(char pkgid, const char * path, const char * src, size_t len, bool append) {
	int fd = fopen(path, append? "a+": "w");
    if(fd<=0) {
		char * msg = mallocf("Failed to open path %s", path) ;
		if(msg) {
			telnet_proto_send_pkg_str(pkgid, CMD_EXCEPTION, msg) ;
			free(msg) ;
		}
		else {
			printf("memory low ?") ;
		}
        return false ;
    }

	size_t wroteBytes = fwrite(src, 1, len, fd);
	// printf("wroteBytes=%d\n", wroteBytes) ;
	if(wroteBytes<0) {
		char * msg = mallocf("Failed to write file %s", path) ;
		if( msg ) {
			telnet_proto_send_pkg_str(pkgid, CMD_EXCEPTION, msg) ;
			free(msg) ;
		}
		else {
			printf("memory low ?") ;
		}
	}
	else{
		uint8_t _wroteBytes = (uint8_t)(wroteBytes&0xFF) ;
		char jsnum[4] ; // 最大 (255 - 7)
		snprintf(jsnum, sizeof(jsnum), "%d", _wroteBytes) ;
		telnet_proto_send_pkg_str(pkgid, CMD_RSPN, jsnum) ;
	}

	fclose(fd) ;
	return true ;
}

void on_pkg_receive (uint8_t pkgid, uint8_t remain, uint8_t cmd, uint8_t * data, uint8_t datalen, void * ctx){

	// printf("pack received, pkgid=%d, remain=%d, cmd=%d, datalen=%d\n", pkgid, remain, cmd, datalen) ;

	// JS代码/命令
	if(cmd==CMD_RUN || cmd==CMD_CALL || cmd==CMD_CALL_ASYNC) {
		// printf("run cmd\n") ;
		// *(data+datalen) = '\0' ;
		telnet_run(ctx, pkgid, remain, cmd, data, datalen) ;
	}

	/**
	 * 文件操作
	 * 
	 * 第一个包: 路径 + \0 + 文件内容
	 * 后续包: 文件内容
	 */
	else if(cmd==CMD_FILE_PUSH_REQ || cmd==CMD_FILE_APPEND_REQ) {

		// printf("file push: %d ? %d\n", _file_pushing_pkgid, pkgid) ;

		if(_file_pushing_pkgid>-1 && _file_pushing_pkgid!=pkgid) {
			_file_pushing_pkgid = -1 ;
			if(_file_pushing_path) {
				free(_file_pushing_path) ;
				_file_pushing_path = NULL ;
			}
		}

		// 新请求
		if(_file_pushing_pkgid<0) {

			// printf("new write\n") ;

			if(_file_pushing_path) {
				free(_file_pushing_path) ;
			}

			int pathlen = strnlen((char *)data, datalen) ;
			// printf("path len: %d\n", pathlen) ;
			if(pathlen==0 || pathlen==datalen) {
				telnet_proto_send_pkg_str(pkgid, CMD_EXCEPTION, "give me file path") ;
				return ;
			}

			char * raw = (char *)data + pathlen + 1 ;
			size_t rawlen = datalen - pathlen - 1 ;
			// printf("rawlen=%d\n",rawlen) ;

			// 加上实际文件系统 /fs 前缀
			int realpathlen = pathlen + sizeof(PATH_PREFIX) ;
			char * realpath = malloc(realpathlen+1) ;
			if(!realpath) {
				telnet_proto_send_pkg_str(pkgid, CMD_EXCEPTION, "Could not malloc for path, memory low?") ;
				return ;
			}
			snprintf(realpath, realpathlen+1, PATH_PREFIX"%s", (char *)data) ;
			realpath[realpathlen] = 0 ;
			// printf("[new req] real path: %s\n", realpath) ;

			write_file(pkgid, realpath, raw, rawlen, cmd==CMD_FILE_APPEND_REQ ) ;

			if(remain>0) {
				_file_pushing_pkgid = pkgid ;
				_file_pushing_path = malloc(realpathlen+1) ;
				strcpy(_file_pushing_path, realpath) ;
				// printf("_file_pushing_path=%s]]]\n",_file_pushing_path) ;
			}

			free(realpath) ;
		}

		// 后续包
		else {
			// printf("append write(%d)\n", _file_pushing_pkgid) ;
			// printf("remain=%d, path=%s, datalen=%d\n",remain,_file_pushing_path,datalen) ;

			write_file(pkgid, (char *)_file_pushing_path, (char *)data, datalen, true) ;
			// 最后一个包
			if(remain==0) {
				_file_pushing_pkgid = -1 ;
				if(_file_pushing_path) {
					free(_file_pushing_path) ;
					_file_pushing_path = NULL ;
				}
			}
		}

		// printf("file push done\n") ;
	}

	else if(cmd==CMD_FILE_PULL_REQ){

		// printf("CMD_FILE_PULL_REQ\n") ;

		int pathlen = strnlen((char *)data, datalen) ;
		if(pathlen==datalen) {
			telnet_proto_send_pkg_str(pkgid, CMD_EXCEPTION, "give me file path") ;
			return ;
		}

		if( pathlen+7 != datalen ){
			telnet_proto_send_pkg_str(pkgid, CMD_EXCEPTION, "missing argv") ;
			return ;
		}

		uint32_t offset = * (uint32_t*)(data + pathlen + 1) ;
		uint32_t bytelen = * (uint16_t*)(data + pathlen + 5) ;

		// printf("offset=%d, bytelen=%d\n", offset, bytelen) ;
		char * realpath = mallocf(PATH_PREFIX"%s", (char *)data) ;
		if(!realpath) {
			telnet_proto_send_pkg_str(pkgid, CMD_EXCEPTION, "failed to malloc for path, memory low?") ;
			return ;
		}
		struct stat statbuf;
		if(stat(realpath,&statbuf)<0 || !S_ISREG(statbuf.st_mode)){
			telnet_proto_send_pkg_str(pkgid, CMD_EXCEPTION, "failed to stat file") ;
			free(realpath) ;
			return ;
		}

		if(offset>=statbuf.st_size){
			telnet_proto_send_pkg_str(pkgid, CMD_EXCEPTION, "offset larger than file size.") ;
			free(realpath) ;
			return ;
		}

		if( bytelen==0 || bytelen+offset>statbuf.st_size ) {
			bytelen = statbuf.st_size - offset ;
		}

		if((bytelen) > PKGLEN_MAX_DATA*255) {
			bytelen = PKGLEN_MAX_DATA*255 ;
		}
		
		int fd = fopen(realpath, "r");
		free(realpath) ;

		if(fd<0) {
			telnet_proto_send_pkg_str(pkgid, CMD_EXCEPTION, "failed to open file") ;
			return ;
		}

		char * bytes = malloc(PKGLEN_MAX_DATA) ;
		if(!bytes) {
			telnet_proto_send_pkg_str(pkgid, CMD_EXCEPTION, "failed to malloc, memory low?") ;
			return ;
		}

		if(offset>0) {
        	fseek(fd, offset, SEEK_SET) ;
		}


		int pkgcnt = (int) ceil( (float)bytelen/(float)PKGLEN_MAX_DATA ) ;
		size_t readed = 0 ;
		size_t remain = bytelen ;

		// printf("send file>total len=%d, pkgcnt=%d\n", datalen, pkgcnt) ;

		for(int i=pkgcnt-1; i>=1; i--) {
#ifdef PLATFORM_ESP32
    		vTaskDelay(1) ;
#endif
			readed = fread(bytes, 1, PKGLEN_MAX_DATA, fd) ;
			telnet_proto_send_one_pkg(pkgid, i, CMD_DATA, bytes, readed) ;
			remain-= readed ;
			// printf("[%d] sended data %d, remain: %d\n", i, readed, remain) ;
		}

		// 最后一个包
#ifdef PLATFORM_ESP32
		vTaskDelay(1) ;
#endif
		readed = fread(bytes, 1, remain, fd) ;
		telnet_proto_send_one_pkg(pkgid, 0, CMD_DATA, bytes, readed) ;
		// printf("[0] sended data %d\n", readed) ;

		fclose(fd) ;
		free(bytes) ;
	}


	// 重置
	else if(cmd==CMD_RESET) {
		uint8_t level = -1 ;
		if(datalen==1) {
			level = *data ;
		}
		if(telnet_prot_func_reset) {
            telnet_prot_func_reset(level) ;
        }
	}
	
	else {
		char msg[32] ;
		sprintf(msg, "unknow package cmd value: %d", cmd) ;
		telnet_proto_send_pkg_str(pkgid, CMD_EXCEPTION, msg) ;
	}
}
