#include "telnet_protocal.h"
#include "telnet_protocal_0519.h"
#include "module_fs.h"
#include "utils.h"
#include "be_list.h"
#include <string.h>
#include <math.h>
#include <sys/stat.h>


TelnetProtFuncSend  telnet_prot_func_pkg_send = NULL ;
TelnetProtFuncReset telnet_prot_func_reset = NULL ;

static uint8_t send_buff [270] ;
static int protocal_version = 518 ;

static be_list_t * lst_pendings = NULL ;

static uint8_t checksum(struct telnet_prot_buffer * buff) {
    uint8_t sum = 0 ;
    for(uint16_t i=0; i<PKG_HEADER_LEN + buff->bytes[PKGPOS_DATALEN]; i++) {
        sum^= buff->bytes[i] ;
    }
    return sum ;
}

int telnet_prot_push_char(struct telnet_prot_buffer * buff, uint8_t byte, const void * ctx) {
    // 包头
    if(buff->writepos == PKGPOS_HEAD1) {
        // printf("writepos=%d,byte=%d\n",buff->writepos, byte) ; 
        if(byte==PKG_HEAD1) {
            buff->bytes[buff->writepos ++] = byte ;
        }
        return 0 ;
    }
    else if(buff->writepos == PKGPOS_HEAD2) {
        // printf("writepos=%d,byte=%d\n",buff->writepos, byte) ; 
        if(byte==PKG_HEAD2) {
            buff->bytes[buff->writepos ++] = byte ;
        	return 0 ;
        }
		// 升级协议到  0519
		else if(byte == 19) {
			buff->writepos = 0 ;
			return 519;
		}
		// 无效内容
		else {
			buff->writepos = 0 ;
			return 0;
		}
    }
    else if( buff->writepos< PKGPOS_DATALEN) {
        buff->bytes[buff->writepos ++] = byte ;
        return 0 ;
    }

    // 数据长度
    else if( buff->writepos == PKGPOS_DATALEN){
        // printf("datalen=%d\n",byte) ;
        // 大于最大data长度, 丢弃
        if(byte>PKGLEN_MAX_DATA) {
            buff->writepos = 0 ;
            return 0;
        }
        buff->bytes[buff->writepos ++] = byte ;
        // dn(buff->bytes[PKGPOS_DATALEN])
        return 0;
    }

    // 数据区
    else if( buff->writepos < (PKG_HEADER_LEN + buff->bytes[PKGPOS_DATALEN]) ) {
        // printf("%d->%d\n",buff->writepos,byte) ;
        buff->bytes[buff->writepos ++] = byte ;
        return 0;
    }

    // 校验位
    else if( buff->writepos == (PKG_HEADER_LEN + buff->bytes[PKGPOS_DATALEN]) ) {
        // 重置
        buff->writepos = 0 ;
        // 完成
        if( checksum(buff)==byte ) {
            buff->bytes[ PKG_HEADER_LEN + buff->bytes[PKGPOS_DATALEN] ] = 0 ;
            // on_pkg_receive(
            //     buff->bytes[PKGPOS_ID] ,
            //     buff->bytes[PKGPOS_REMAIN] ,
            //     buff->bytes[PKGPOS_CMD] ,
            //     buff->bytes + PKG_HEADER_LEN ,
            //     buff->bytes[PKGPOS_DATALEN] ,
            //     ctx
            // ) ;
        }
        else {
        }
    }

    // 异常,丢弃数据
    else {
        // printf("invalid writepos=%d\n", buff->writepos) ;
        buff->writepos = 0 ;
    }

	return 0 ;
}

void telnet_prot_push_bytes(struct telnet_prot_buffer * buff, uint8_t * dat, size_t length, const void * ctx) {

	for(uint8_t i=0; i<length; i++) {
		telnet_prot_push_char( buff, dat[i], ctx ) ;
	}

	// if(protocal_version == 519) {
	// 	size_t remain = length ;
	// 	if( telnet_prot0519_push_bytes(buff, dat, &remain, ctx) ) {
	// 		// 完成，降级到 0518
	// 		protocal_version = 518 ;
	// 		// 剩余数据重入处理
	// 		if(remain) {
	// 			telnet_prot_push_bytes(buff,dat + (length-remain),remain,ctx) ;
	// 		}
	// 		return ;
	// 	}
	// }

	// else {
	// 	for(uint8_t i=0; i<length; i++) {
	// 		if(telnet_prot_push_char( buff, dat[i], ctx )==519) {
	// 			protocal_version=519 ;
	// 			telnet_prot_push_bytes(buff, dat+i+1, length-i, ctx) ;
	// 			return ;
	// 		}
	// 	}
	// }
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




// static int _file_pushing_pkgid = -1 ;
// static char * _file_pushing_path = NULL ;

void telnet_prot_reset() {
	// _file_pushing_pkgid = -1 ;
	// if(_file_pushing_path) {
	// 	free(_file_pushing_path) ;
	// 	_file_pushing_path = NULL ;
	// }
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

void telnet_proto_process_pkg (telnet_pkg_t * pkg, void * ctx){

	uint8_t * data = pkg->data ;
	size_t datalen = pkg->data_len ;

	// JS代码/命令
	if(pkg->cmd==CMD_RUN || pkg->cmd==CMD_CALL || pkg->cmd==CMD_CALL_ASYNC) {
		telnet_run(ctx, pkg->pkgid, pkg->cmd, data, datalen) ;
	}

	/**
	 * 文件操作
	 * 
	 * 第一个包: 路径 + \0 + 文件内容
	 * 后续包: 文件内容
	 */
	else if(pkg->cmd==CMD_FILE_PUSH_REQ || pkg->cmd==CMD_FILE_APPEND_REQ) {

		int pathlen = strnlen((char *)data, datalen) ;
		// printf("path len: %d\n", pathlen) ;
		if(pathlen==0 || pathlen==datalen) {
			telnet_proto_send_pkg_str(pkg->pkgid, CMD_EXCEPTION, "give me file path") ;
			return ;
		}

		char * raw = (char *)data + pathlen + 1 ;
		size_t rawlen = datalen - pathlen - 1 ;
		// printf("rawlen=%d\n",rawlen) ;

		// 加上实际文件系统 /fs 前缀
		int realpathlen = pathlen + sizeof(PATH_PREFIX) ;
		char * realpath = malloc(realpathlen+1) ;
		if(!realpath) {
			telnet_proto_send_pkg_str(pkg->pkgid, CMD_EXCEPTION, "Could not malloc for path, memory low?") ;
			return ;
		}
		snprintf(realpath, realpathlen+1, PATH_PREFIX"%s", (char *)data) ;
		realpath[realpathlen] = 0 ;
		// printf("[new req] real path: %s\n", realpath) ;

		write_file(pkg->pkgid, realpath, raw, rawlen, pkg->cmd==CMD_FILE_APPEND_REQ ) ;

		free(realpath) ;
	}

	else if(pkg->cmd==CMD_FILE_PULL_REQ){

		// printf("CMD_FILE_PULL_REQ\n") ;

		int pathlen = strnlen((char *)data, datalen) ;
		if(pathlen==datalen) {
			telnet_proto_send_pkg_str(pkg->pkgid, CMD_EXCEPTION, "give me file path") ;
			return ;
		}

		if( pathlen+7 != datalen ){
			telnet_proto_send_pkg_str(pkg->pkgid, CMD_EXCEPTION, "missing argv") ;
			return ;
		}

		uint32_t offset = * (uint32_t*)(data + pathlen + 1) ;
		uint32_t bytelen = * (uint16_t*)(data + pathlen + 5) ;

		// printf("offset=%d, bytelen=%d\n", offset, bytelen) ;
		char * realpath = mallocf(PATH_PREFIX"%s", (char *)data) ;
		if(!realpath) {
			telnet_proto_send_pkg_str(pkg->pkgid, CMD_EXCEPTION, "failed to malloc for path, memory low?") ;
			return ;
		}
		struct stat statbuf;
		if(stat(realpath,&statbuf)<0 || !S_ISREG(statbuf.st_mode)){
			telnet_proto_send_pkg_str(pkg->pkgid, CMD_EXCEPTION, "failed to stat file") ;
			free(realpath) ;
			return ;
		}

		if(offset>=statbuf.st_size){
			telnet_proto_send_pkg_str(pkg->pkgid, CMD_EXCEPTION, "offset larger than file size.") ;
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
			telnet_proto_send_pkg_str(pkg->pkgid, CMD_EXCEPTION, "failed to open file") ;
			return ;
		}

		char * bytes = malloc(PKGLEN_MAX_DATA) ;
		if(!bytes) {
			telnet_proto_send_pkg_str(pkg->pkgid, CMD_EXCEPTION, "failed to malloc, memory low?") ;
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
			telnet_proto_send_one_pkg(pkg->pkgid, i, CMD_DATA, bytes, readed) ;
			remain-= readed ;
			// printf("[%d] sended data %d, remain: %d\n", i, readed, remain) ;
		}

		// 最后一个包
#ifdef PLATFORM_ESP32
		vTaskDelay(1) ;
#endif
		readed = fread(bytes, 1, remain, fd) ;
		telnet_proto_send_one_pkg(pkg->pkgid, 0, CMD_DATA, bytes, readed) ;
		// printf("[0] sended data %d\n", readed) ;

		fclose(fd) ;
		free(bytes) ;
	}


	// 重置
	else if(pkg->cmd==CMD_RESET) {
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
		sprintf(msg, "unknow package cmd value: %d", pkg->cmd) ;
		telnet_proto_send_pkg_str(pkg->pkgid, CMD_EXCEPTION, msg) ;
	}
}



// ----------------------------------------------------------------

static telnet_pkg_t * pkg_uncompleted = NULL ;


inline static int detect_header(uint8_t * bytes, size_t length) {
	for(int i=0;i<length-1;i++) {
		if(bytes[i]==PKG_HEAD1 && bytes[i+1]==PKG_HEAD2) {
			return i ;
		}
	}
	return -1 ;
}


inline static int read_data_length(uint8_t * bytes, size_t length, size_t * datalen) {

	(*datalen) = 0;

	// 第3个字节
    (*datalen)|= bytes[4] & 0x7F ;
    if(bytes[4]<0x80) {
		return 1 ;
    }
	if(length<6) {
		return -1 ;
	}

	// 第3个字节
    (*datalen)|= (bytes[5] & 0x7F) << 7 ;
    if(bytes[5]<0x80) {
		return 2 ;
    }
	if(length<7) {
		return -1 ;
	}

	// 第3个字节
    (*datalen)|= (bytes[6] & 0x7F) << 14 ;
    if(bytes[6]<0x80) {
		return 3 ;
    }
	if(length<8) {
		return -1 ;
	}

	// 第4个字节
    (*datalen)|= (bytes[7] & 0x7F) << 21 ;
    
	return 4 ;
}

inline static bool receive_body(uint8_t * bytes, size_t * length, telnet_pkg_t * pkg) {

	size_t body_unread = pkg->data_len - pkg->data_received ;
	size_t n = (*length) < body_unread? (*length): body_unread ;

	memcpy( pkg->data+pkg->data_received, bytes, n) ;

	(*length)-= n ;
	pkg->data_received+= n ;

	// printf("read body:%d,%d,->%d, @%p\n",(*length),body_unread,n,pkg) ;

	return pkg->data_received == pkg->data_len ;
}

void telnet_proto_free_pkg(telnet_pkg_t * pkg) {
	// printf("free pkg, %d, body:%d @%p\n", pkg->pkgid, pkg->data_len, pkg->data) ;
	if(pkg->data) {
		free(pkg->data) ;
		pkg->data = NULL ;
	}
	free(pkg) ;
	pkg = NULL ;
}


void be_telnet_proto_init(TelnetProtFuncSend sender) {
    if(!lst_pendings) {
        lst_pendings = malloc(sizeof(be_list_t)) ;
        memset(lst_pendings,0,sizeof(be_list_t)) ;
        be_list_init(lst_pendings) ;
    }

	if(sender) {
		telnet_prot_func_pkg_send = sender ;
	}
	
}


void telnet_prot0519_receive(uint8_t * bytes, size_t * length) {

	if(pkg_uncompleted) {
		if(receive_body(bytes,length,pkg_uncompleted)) {
			// printf("-pkg body compiled, body:%d, remain:%d, @%p\n", pkg_uncompleted->data_len,* length,pkg_uncompleted) ;
			be_list_append( lst_pendings, pkg_uncompleted ) ;
			pkg_uncompleted = NULL ;
			return ;
		}
		return ;
	}

	int idx = detect_header(bytes,*length) ;
	if(idx<0) {
		// 最后一个字节 可能是前半个包头
		if(bytes[(*length)-1]==PKG_HEAD1) {
			(*length) = 1 ;
		}
		else {
			(*length) = 0 ;
		}
		return ;
	}

	// 包头以前的数据可清空
	(*length) -= idx ;

	// 后续长度不够，等待后文
    if((*length)<5) {
        return ;
    }

	bytes+= idx ;
	size_t datalen = 0 ;
	int lenBytes = read_data_length(bytes, length, &datalen) ;

	// 长度未接收完，等待后文
	if(lenBytes<-1) {
		return ;
	}
	
	(*length) -= 4 + lenBytes ;

	pkg_uncompleted = malloc(sizeof(telnet_pkg_t)) ;
	if(!pkg_uncompleted) {
		printf("out of memory?\n") ;
		return ;
	}
	// printf("malloc pkg:%p, prev:%p,next:%p\n",pkg_uncompleted,pkg_uncompleted->base.prev,pkg_uncompleted->base.next) ;
	memset(pkg_uncompleted, 0, sizeof(telnet_pkg_t)) ;
	
	// printf("pkg header arrived, id:%d, cmd:%d, %d, len:%d/%d, @%p\n",bytes[2],bytes[3], bytes[4], datalen, lenBytes,pkg_uncompleted) ;

	if(datalen) {
		pkg_uncompleted->data = malloc(datalen) ;
		if(!pkg_uncompleted->data) {
			free(pkg_uncompleted) ;
			pkg_uncompleted = NULL ;
			printf("out of memory?\n") ;
			return ;
		}
	}
	else {
		pkg_uncompleted->data = NULL ;
	}

	pkg_uncompleted->pkgid = bytes[2] ;
	pkg_uncompleted->cmd = bytes[3] ;
	pkg_uncompleted->data_len = datalen ;
	pkg_uncompleted->data_received = 0 ;

	bytes+= 4 + lenBytes ;

	if(datalen) {
		if(!receive_body(bytes,length,pkg_uncompleted)) {
			return ;
		}
	}
	
	// printf("@%d/pkg body compiled, body:%d, remain:%d\n", __LINE__, pkg_uncompleted->data_len, *length) ;
	be_list_append( lst_pendings, pkg_uncompleted ) ;
	pkg_uncompleted = NULL ;
}

inline void be_telnet_proto_loop(void * ctx) {
	if(!lst_pendings->count) {
		return ;
	}
	telnet_pkg_t * pkg = be_list_shift(lst_pendings) ;
	if(pkg) {
		telnet_proto_process_pkg(pkg, ctx) ;
		telnet_proto_free_pkg(pkg) ;
	}
}
