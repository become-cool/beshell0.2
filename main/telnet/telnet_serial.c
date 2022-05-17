
#include "telnet_serial.h"
#include "module_telnet.h"
#include "telnet_protocal.h"
#include "module_fs.h"
#include "driver/uart.h"
#include <stdio.h>
#include <sys/fcntl.h>
#include "esp_vfs.h"
#include "utils.h"
#include "js_main_loop.h"
#include "esp_vfs_dev.h"
#include "logging.h"
#include <sys/errno.h>

#include <sys/types.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <math.h>

LOG_TAG("telnet")

uint8_t recv_buff[128];
char send_buff [270] ;

struct telnet_prot_buffer uart_pkg_buff ;

int _file_pushing_pkgid = -1 ;
char * _file_pushing_path = NULL ;

void be_telnet_serial_reset(JSContext *ctx) {
	_file_pushing_pkgid = -1 ;
	if(_file_pushing_path) {
		free(_file_pushing_path) ;
		_file_pushing_path = NULL ;
	}
}


uart_config_t uart_config = {
	.baud_rate = 115200,			// 921600
	.data_bits = UART_DATA_8_BITS,
	.parity    = UART_PARITY_DISABLE,
	.stop_bits = UART_STOP_BITS_1,
	.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
	.source_clk = UART_SCLK_APB,
};
int uart_fd;



void on_pkg_receive (uint8_t pkgid, uint8_t remain, uint8_t cmd, uint8_t * data, uint8_t datalen, void * ctx) ;

void be_telnet_serial_init() {

	telnet_prot_on_package = on_pkg_receive ;

	uart_pkg_buff.writepos = 0 ;

	// uart telnet ----------
    uart_driver_install(UART_NUM_0, 2*1024, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config);
	if ((uart_fd = open("/dev/uart/0", O_RDWR)) != -1) {
		esp_vfs_dev_uart_use_driver(0);
	}
}


struct timeval telnet_tv = {
	.tv_sec = 0,
	.tv_usec = 0,
};
fd_set telnet_rfds;

void be_telnet_serial_loop(JSContext *ctx) {

	FD_ZERO(&telnet_rfds);

    int fds = -1 ;
	FD_SET(uart_fd, &telnet_rfds);
	fds = uart_fd ;

	int s = select(fds + 1, &telnet_rfds, NULL, NULL, &telnet_tv);
	if (s < 0) {
		LOGE("Select failed: errno %d", errno);
		return;
	} else if (s == 0) {
		// ESP_LOGI(TAG, "Timeout has been reached and nothing has been received");
		return ;
	} 
	
	// UART 接收
	if (FD_ISSET(uart_fd, &telnet_rfds)) {
		int len = uart_read_bytes(0, recv_buff, sizeof(recv_buff), 0) ;
		// printf("receive %d\n", len) ;
		if(len<0) {
			printf("read uart error\n") ;
		}
		else {
			telnet_prot_push_bytes(&uart_pkg_buff, recv_buff, len, ctx) ;
		}
	}
}



void telnet_send_one_pkg(char pkgid, char remain, char cmd, char * data, uint8_t datalen) {

	uint16_t pkglen = PKGLEN_WITHOUT_DATA + datalen ;
	
	// printf("datalen=%d, pkglen=%d\n", datalen, pkglen) ;

	telnet_prot_pack((uint8_t*)send_buff, pkgid, remain, cmd, (uint8_t*)data, datalen) ;

	// 串口输出
	uart_write_bytes(0, send_buff, pkglen);
	// uart_wait_tx_done(0, 10000);
}

void telnet_serial_send_pkg(char pkgid, char cmd, char * data, uint16_t datalen) {
	
	int pkgcnt = (int) ceil( (float)datalen/(float)PKGLEN_MAX_DATA ) ;

	// printf("send>datalen=%d, max=%d, pkgcnt=%d\n", datalen, PKGLEN_MAX_DATA, pkgcnt) ;
	// printf("%s\n",data) ;

	for(int i=pkgcnt-1; i>=1; i--) {
		// printf("%d, len=%d\n", i, PKGLEN_MAX_DATA) ;
		telnet_send_one_pkg(pkgid, i, cmd, data, PKGLEN_MAX_DATA) ;
		data+= PKGLEN_MAX_DATA ;
	}

	// 最后一个包
	telnet_send_one_pkg(pkgid, 0, cmd, data, datalen%PKGLEN_MAX_DATA) ;
}



void telnet_serial_send_pkg_str(char pkgid, char cmd, char * data) {
	telnet_serial_send_pkg(pkgid, cmd, data, strlen(data)) ;
}


bool write_file(char pkgid, const char * path, const char * src, size_t len, bool append) {
	int fd = fopen(path, append? "a+": "w");
    if(fd<=0) {
		char * msg = mallocf("Failed to open path %s", path) ;
		if(msg) {
			telnet_serial_send_pkg_str(pkgid, CMD_EXCEPTION, msg) ;
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
			telnet_serial_send_pkg_str(pkgid, CMD_EXCEPTION, msg) ;
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
		telnet_serial_send_pkg_str(pkgid, CMD_RSPN, jsnum) ;
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
				telnet_serial_send_pkg_str(pkgid, CMD_EXCEPTION, "give me file path") ;
				return ;
			}

			char * raw = (char *)data + pathlen + 1 ;
			size_t rawlen = datalen - pathlen - 1 ;
			// printf("rawlen=%d\n",rawlen) ;

			// 加上实际文件系统 /fs 前缀
			int realpathlen = pathlen + sizeof(PATH_PREFIX) ;
			char * realpath = malloc(realpathlen+1) ;
			if(!realpath) {
				telnet_serial_send_pkg_str(pkgid, CMD_EXCEPTION, "Could not malloc for path, memory low?") ;
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
			telnet_serial_send_pkg_str(pkgid, CMD_EXCEPTION, "give me file path") ;
			return ;
		}

		if( pathlen+7 != datalen ){
			telnet_serial_send_pkg_str(pkgid, CMD_EXCEPTION, "missing argv") ;
			return ;
		}

		uint32_t offset = * (uint32_t*)(data + pathlen + 1) ;
		uint32_t bytelen = * (uint16_t*)(data + pathlen + 5) ;

		// printf("offset=%d, bytelen=%d\n", offset, bytelen) ;
		char * realpath = mallocf(PATH_PREFIX"%s", (char *)data) ;
		if(!realpath) {
			telnet_serial_send_pkg_str(pkgid, CMD_EXCEPTION, "failed to malloc for path, memory low?") ;
			return ;
		}
		struct stat statbuf;
		if(stat(realpath,&statbuf)<0 || !S_ISREG(statbuf.st_mode)){
			telnet_serial_send_pkg_str(pkgid, CMD_EXCEPTION, "failed to stat file") ;
			free(realpath) ;
			return ;
		}

		if(offset>=statbuf.st_size){
			telnet_serial_send_pkg_str(pkgid, CMD_EXCEPTION, "offset larger than file size.") ;
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
			telnet_serial_send_pkg_str(pkgid, CMD_EXCEPTION, "failed to open file") ;
			return ;
		}

		char * bytes = malloc(PKGLEN_MAX_DATA) ;
		if(!bytes) {
			telnet_serial_send_pkg_str(pkgid, CMD_EXCEPTION, "failed to malloc, memory low?") ;
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
    		vTaskDelay(1) ;
			readed = fread(bytes, 1, PKGLEN_MAX_DATA, fd) ;
			telnet_send_one_pkg(pkgid, i, CMD_DATA, bytes, readed) ;
			remain-= readed ;
			// printf("[%d] sended data %d, remain: %d\n", i, readed, remain) ;
		}

		// 最后一个包
		vTaskDelay(1) ;
		readed = fread(bytes, 1, remain, fd) ;
		telnet_send_one_pkg(pkgid, 0, CMD_DATA, bytes, readed) ;
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
		task_reset(level, NULL) ;
	}
	
	else {
		char msg[32] ;
		sprintf(msg, "unknow package cmd value: %d", cmd) ;
		telnet_serial_send_pkg_str(pkgid, CMD_EXCEPTION, msg) ;
	}
}
