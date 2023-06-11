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

static be_list_t * lst_pendings = NULL ;

static uint8_t checksum(uint8_t * data, size_t len) {
    uint8_t sum = 0 ;
    for(uint16_t i=0; i<len; i++) {
        sum^= data[i] ;
    }
    return sum ;
}


static uint8_t telnet_prot_pack_data_len(uint32_t len, uint8_t * len_bytes) {
    *(len_bytes+0) = (len>>(7*0)) & 0x7F ;
    *(len_bytes+1) = (len>>(7*1)) & 0x7F ;
    *(len_bytes+2) = (len>>(7*2)) & 0x7F ;
    *(len_bytes+3) = (len>>(7*3)) & 0xFF ;  // 最高位字节可以用满8位

    if( *(len_bytes+3)>0 ) {
        *(len_bytes+0) |= 0x80 ;
        *(len_bytes+1) |= 0x80 ;
        *(len_bytes+2) |= 0x80 ;
        return 4 ;
    }
    else if( *(len_bytes+2)>0 ) {
        *(len_bytes+0) |= 0x80 ;
        *(len_bytes+1) |= 0x80 ;
        return 3 ;
    }
    else if( *(len_bytes+1)>0 ) {
        *(len_bytes+0) |= 0x80 ;
        return 2 ;
    }
    return 1 ;
}

uint8_t * telnet_prot_pack(uint8_t pkgId, uint8_t cmd, uint8_t * dat, size_t datalen, size_t * pkglen) {

	uint8_t buff_datalen[4] ;
	uint8_t datalen_bytes = telnet_prot_pack_data_len(datalen, buff_datalen) ;

	*pkglen = 4+datalen_bytes+datalen+1 ;

	uint8_t * pkg = malloc(*pkglen) ;
	if(!pkg) {
		printf("output of memory?\n") ;
		return NULL ;
	}

    pkg[0] = PKG_HEAD1 ;
    pkg[1] = PKG_HEAD2 ;
    pkg[2] = pkgId ;
    pkg[3] = cmd ;

	memcpy(pkg+4, buff_datalen, datalen_bytes) ;

	memcpy(pkg+4+datalen_bytes, dat, datalen) ;

	// 校验和
	pkg[(*pkglen)-1] = checksum(pkg, (*pkglen)-1) ;

    return pkg ;
}

void telnet_prot_reset() {
	// _file_pushing_pkgid = -1 ;
	// if(_file_pushing_path) {
	// 	free(_file_pushing_path) ;
	// 	_file_pushing_path = NULL ;
	// }
}




void telnet_proto_send_pkg(uint8_t pkgid, uint8_t cmd, uint8_t * data, size_t datalen) {
	size_t pkglen = 0 ;
	uint8_t * pkg = telnet_prot_pack(pkgid, cmd, (uint8_t*)data, datalen, &pkglen) ;
	// 输出
	if(pkg) {
		telnet_prot_func_pkg_send(pkg, pkglen);
		free(pkg) ;
	}
}


static void telnet_proto_send_pkg_str(uint8_t pkgid, uint8_t cmd, char * data) {
	telnet_proto_send_pkg(pkgid, cmd, (uint8_t *)data, strlen(data)) ;
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
		
		int fd = fopen(realpath, "r");
		free(realpath) ;

		if(fd<0) {
			telnet_proto_send_pkg_str(pkg->pkgid, CMD_EXCEPTION, "failed to open file") ;
			return ;
		}

		uint8_t * bytes = malloc(bytelen) ;
		if(!bytes) {
			fclose(fd) ;
			telnet_proto_send_pkg_str(pkg->pkgid, CMD_EXCEPTION, "failed to malloc, memory low?") ;
			return ;
		}

		if(offset>0) {
        	fseek(fd, offset, SEEK_SET) ;
		}

		size_t readed = fread(bytes, 1, bytelen, fd) ;

		telnet_proto_send_pkg(pkg->pkgid, CMD_DATA, bytes, readed) ;

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

	// 第1个字节
    (*datalen)|= bytes[4] & 0x7F ;
    if(bytes[4]<0x80) {
		return 1 ;
    }
	if(length<6) {
		return -1 ;
	}

	// 第2个字节
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
