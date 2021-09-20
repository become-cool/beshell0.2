#include "telnet.h"
#include "module_fs.h"
#include "driver/uart.h"
#include <stdio.h>
#include <sys/fcntl.h>
#include "esp_vfs.h"
#include "utils.h"
#include "beshell.h"
#include "task_js.h"
#include "esp_vfs_dev.h"
#include "logging.h"
#include <sys/errno.h>

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include <math.h>

LOG_TAG("telnet")

#define TELNET_PORT 8022

uint8_t uart_recv_buff[128];
uint8_t tcp_recv_buff[128];

char send_buff [270] ;

struct telnet_prot_buffer uart_pkg_buff ;
struct telnet_prot_buffer tcp_pkg_buff ;

int _file_pushing_pkgid = -1 ;
char * _file_pushing_path = NULL ;

uint8_t _echo_pkgid = 0 ;
uint8_t echo_pkgid() {
	return _echo_pkgid ++ ;
}

JSValue _func_repl_input = NULL ;
void telnet_set_input_function(JSContext * ctx, JSValue func) {
	if(_func_repl_input) {
		JS_FreeValue(ctx, _func_repl_input) ;
	}
	if(func)
		JS_DupValue(ctx, func) ;
	_func_repl_input = func ;
}

void telnet_on_before_reset(JSContext *ctx) {
	if(_func_repl_input) {
		JS_FreeValue(ctx, _func_repl_input) ;
		_func_repl_input = NULL ;
	}

	_file_pushing_pkgid = -1 ;
	if(_file_pushing_path) {
		free(_file_pushing_path) ;
		_file_pushing_path = NULL ;
	}
}


// --------------------
// UART
uart_config_t uart_config = {
	.baud_rate = 115200,			// 921600
	.data_bits = UART_DATA_8_BITS,
	.parity    = UART_PARITY_DISABLE,
	.stop_bits = UART_STOP_BITS_1,
	.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
	.source_clk = UART_SCLK_APB,
};
int uart_fd;

// --------------------
// TCP
int sock_server = -1 ;
int sock_client = -1 ;

struct sockaddr_in6 source_addr; // Large enough for both IPv4 or IPv6
uint addr_len = sizeof(source_addr);

void on_pkg_receive (uint8_t pkgid, uint8_t remain, uint8_t cmd, uint8_t * data, uint8_t datalen, void * ctx) ;

void telnet_init() {

	telnet_prot_on_package = on_pkg_receive ;

	uart_pkg_buff.writepos = 0 ;
	tcp_pkg_buff.writepos = 0 ;

	// uart telnet ----------
    uart_driver_install(UART_NUM_0, 2*1024, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config);
	if ((uart_fd = open("/dev/uart/0", O_RDWR)) != -1) {
		esp_vfs_dev_uart_use_driver(0);
	}

	
	// tcp telnet ----------
    if(sock_server>-1) {
        echof("telnet startup already.\n") ;
        return ;
    }
    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(TELNET_PORT);
    sock_server = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock_server < 0) {
        echof("Unable to create socket: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }
    // echof("Socket created");

    int err = bind(sock_server, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        echof("Socket unable to bind: errno %d", errno);
        shutdown(sock_server, 0) ;
        close(sock_server);
        return ;
    }
    // echof("Socket bound, port %d\n", TELNET_PORT);

    err = listen(sock_server, 1);
    if (err != 0) {
        echof("Error occurred during listen: errno %d", errno);
        shutdown(sock_server, 0) ;
        close(sock_server);
        return ;
    }

    echof("TCP Telnet start on port %d \n", TELNET_PORT) ;
}


struct timeval telnet_tv = {
	.tv_sec = 0,
	.tv_usec = 0,
};
fd_set telnet_rfds;

void telnet_loop(JSContext *ctx) {

	FD_ZERO(&telnet_rfds);

    int fds = -1 ;
	FD_SET(uart_fd, &telnet_rfds);
	fds = uart_fd ;

    if(sock_server>-1) {
        FD_SET(sock_server, &telnet_rfds);
        if(fds<sock_server)
            fds = sock_server ;
    }
    if(sock_client>-1) {
        FD_SET(sock_client, &telnet_rfds);
        if(fds<sock_client)
            fds = sock_client ;
    }

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
		int len = uart_read_bytes(0, uart_recv_buff, sizeof(uart_recv_buff), 0) ;
		// printf("receive %d\n", len) ;
		if(len<0) {
			printf("read uart error\n") ;
		}
		else {
			telnet_prot_push_bytes(&uart_pkg_buff, uart_recv_buff, len, ctx) ;
		}
	} 
	
	
	// TCP Client 接入
	if(FD_ISSET(sock_server, &telnet_rfds)) {
		// 关闭原有连接
		if(sock_client>-1) {
			echo("Close last tcp client for new one\n") ;
			shutdown(sock_client, 0);
			close(sock_client);
		}

		sock_client = accept(sock_server, (struct sockaddr *)&source_addr, &addr_len);
		if(sock_client<0){
			echof("accept() error: %d.\n", errno) ;
		}
		else {
			echo("TCP client come in\n") ;
		}
	}


	// TCP Client 事件
	if(sock_client>-1 && FD_ISSET(sock_client, &telnet_rfds)) {
		int len = recv(sock_client, tcp_recv_buff, sizeof(tcp_recv_buff) - 1, 0);
		// Client 错误
		if(len<0) {
			char errbuf[32] ;
			esp_err_to_name_r(errno, errbuf, sizeof(errbuf)) ;
			// echof("recv() error: %s.\n", errbuf) ;
		}
		// Client 断开
		else if(len==0) {

			shutdown(sock_client, 0);
			close(sock_client);

			sock_client = -1 ;
			
			echo("TCP client leave\n") ;
		}
		// 接收到数据
		else {
			telnet_prot_push_bytes(&tcp_pkg_buff, tcp_recv_buff, len, ctx) ;
		}
	}
}



void telnet_send_one_pkg(char pkgid, char remain, char cmd, char * data, uint8_t datalen) {


	uint16_t pkglen = PKGLEN_WITHOUT_DATA + datalen ;
	
	// printf("datalen=%d, pkglen=%d\n", datalen, pkglen) ;

	// char * pkgbuff = malloc(pkglen) ;
	telnet_prot_pack((uint8_t*)send_buff, pkgid, remain, cmd, (uint8_t*)data, datalen) ;

	// 串口输出
	uart_write_bytes(0, send_buff, pkglen);
	// uart_wait_tx_done(0, 10000);

	// tcp socket 输出
	if(sock_client>-1) {
    	send(sock_client, send_buff, pkglen, 0) ;
	}
	// free(pkgbuff) ;
}

void telnet_send_pkg(char pkgid, char cmd, char * data, uint16_t datalen) {
	
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

void telnet_send_pkg_str(char pkgid, char cmd, char * data) {
	telnet_send_pkg(pkgid, cmd, data, strlen(data)) ;
}

/**
 * argv:
 *   pkgid  uint8
 *   cmd    uint8
 *   data   string/array/ArrayBuffer
 */
JSValue js_telnet_send(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
	// printf("js_telnet_send_pack()\n") ;

	CHECK_ARGC(3)
	ARGV_TO_UINT8(0, pkgid)
	ARGV_TO_UINT8(1, cmd)

	// printf("pkgid=%d, cmd=%d\n", pkgid, cmd) ;

	ARGV_TO_STRING(2, data, datalen)
	// printf("send pkg data=%s, datalen=%d\n", data, datalen) ;
	telnet_send_pkg(pkgid, cmd, data, datalen) ;
	JS_FreeCString(ctx, data) ;

	// @todo : array/ArrayBuffer
	return JS_UNDEFINED ;
}
void require_module_telnet(JSContext *ctx) {
    JSValue global = JS_GetGlobalObject(ctx);
    JSValue telnet = JS_NewObject(ctx) ;
    JS_SetPropertyStr(ctx, global, "telnet", telnet);
    JS_SetPropertyStr(ctx, telnet, "send", JS_NewCFunction(ctx, js_telnet_send, "send", 1));
	JS_FreeValue(ctx, global);
}



void js_dump_err(JSContext *ctx, JSValueConst val) {
    const char * str = JS_ToCString(ctx, val);
    if (str) {
		echo(str);
        JS_FreeCString(ctx, str);
    } else {
        printf("[exception]\n");
        echo("[exception]\n");
    }
}
void echo_error(JSContext * ctx) {
    JSValue exception_val = JS_GetException(ctx);
    bool is_error = JS_IsError(ctx, exception_val);
    js_dump_err(ctx, exception_val);
    if (is_error) {
        JSValue val = JS_GetPropertyStr(ctx, exception_val, "stack");
        if (!JS_IsUndefined(val)) {
            js_dump_err(ctx, val);
        }
        JS_FreeValue(ctx, val);
    }
    JS_FreeValue(ctx, exception_val);
}

bool write_file(char pkgid, const char * path, const char * src, size_t len, bool append) {
	int fd = fopen(path, append? "a+": "w");
    if(fd<=0) {

		// printf("%s\n", path) ;

		char * msg = mallocf("Failed to open path %s", path) ;
		if(msg) {
			telnet_send_pkg_str(pkgid, CMD_EXCEPTION, msg) ;
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
			telnet_send_pkg_str(pkgid, CMD_EXCEPTION, msg) ;
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
		telnet_send_pkg_str(pkgid, CMD_RSPN, jsnum) ;
	}

	fclose(fd) ;
	return true ;
}

void on_pkg_receive (uint8_t pkgid, uint8_t remain, uint8_t cmd, uint8_t * data, uint8_t datalen, void * ctx){

	// printf("pack received, pkgid=%d, remain=%d, cmd=%d, datalen=%d\n", pkgid, remain, cmd, datalen) ;

	// JS代码/命令
	if(cmd==CMD_RUN || cmd==CMD_CALL || cmd==CMD_CALL_ASYNC) {
		// *(data+datalen) = '\0' ;
		
		if(_func_repl_input && JS_IsFunction(ctx, _func_repl_input)) {

			JSValueConst * argv = malloc(sizeof(JSValue)*4) ;
			argv[0] = JS_NewInt32(ctx, pkgid) ;
			argv[1] = JS_NewInt32(ctx, remain) ;
			argv[2] = JS_NewInt32(ctx, cmd) ;
			argv[3] = JS_NewStringLen(ctx, (char *)data, datalen) ;

			// printf("%s\n", data) ;

			JSValue ret = JS_Call(ctx, _func_repl_input, JS_NULL, 4, argv) ;
			if( JS_IsException(ret) ) {
				echo_error(ctx) ;
			}

			JS_FreeValue(ctx, ret) ;
			JS_FreeValue(ctx, argv[0]) ;
			JS_FreeValue(ctx, argv[1]) ;
			JS_FreeValue(ctx, argv[2]) ;
			JS_FreeValue(ctx, argv[3]) ;
			free(argv) ;
		}
	}

	/**
	 * 文件操作
	 * 
	 * 第一个包: 路径 + \0 + 文件内容
	 * 后续包: 文件内容
	 */
	else if(cmd==CMD_FILE_PUSH_REQ || cmd==CMD_FILE_APPEND_REQ) {
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
				telnet_send_pkg_str(pkgid, CMD_EXCEPTION, "give me file path") ;
				return ;
			}

			char * raw = (char *)data + pathlen + 1 ;
			size_t rawlen = datalen - pathlen - 1 ;
			// printf("rawlen=%d\n",rawlen) ;

			// 加上实际文件系统 /fs 前缀
			int realpathlen = pathlen + sizeof(PATH_PREFIX) ;
			char * realpath = malloc(realpathlen+1) ;
			if(!realpath) {
				telnet_send_pkg_str(pkgid, CMD_EXCEPTION, "Could not malloc for path, memory low?") ;
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
	}

	else if(cmd==CMD_FILE_PULL_REQ){

		// printf("CMD_FILE_PULL_REQ\n") ;

		int pathlen = strnlen((char *)data, datalen) ;
		if(pathlen==datalen) {
			telnet_send_pkg_str(pkgid, CMD_EXCEPTION, "give me file path") ;
			return ;
		}

		if( pathlen+7 != datalen ){
			telnet_send_pkg_str(pkgid, CMD_EXCEPTION, "missing argv") ;
			return ;
		}

		uint32_t offset = * (uint32_t*)(data + pathlen + 1) ;
		uint16_t bytelen = * (uint16_t*)(data + pathlen + 5) ;

		// printf("offset=%d, bytelen=%d\n", offset, bytelen) ;

		if((bytelen) > PKGLEN_MAX_DATA*255) {
			telnet_send_pkg_str(pkgid, CMD_EXCEPTION, "read length too large") ;
			return ;
		}

		char * realpath = mallocf(PATH_PREFIX"%s", (char *)data) ;
		if(!realpath) {
			telnet_send_pkg_str(pkgid, CMD_EXCEPTION, "failed to malloc for path, memory low?") ;
		}

		if((bytelen)==0) {
			struct stat statbuf;
    		if(stat(realpath,&statbuf)<0 || !S_ISREG(statbuf.st_mode)){
				telnet_send_pkg_str(pkgid, CMD_EXCEPTION, "failed to open file") ;
			}
			bytelen = statbuf.st_size ;
			// printf("file size: %d\n", bytelen) ;
		}
		
		int fd = fopen(realpath, "r");
		free(realpath) ;

		if(fd<0) {
			telnet_send_pkg_str(pkgid, CMD_EXCEPTION, "failed to open file") ;
			return ;
		}

		char * bytes = malloc(bytelen) ;
		if(!bytes) {
			telnet_send_pkg_str(pkgid, CMD_EXCEPTION, "failed to malloc, memory low?") ;
			return ;
		}

		if(offset>0) {
        	fseek(fd, offset, SEEK_SET) ;
		}

		size_t readbytes = fread(bytes, 1, bytelen, fd) ;
		// printf("req size: %d, readed size: %d, errno=%d\n",bytelen,readbytes,errno ) ;
		fclose(fd) ;

		telnet_send_pkg(pkgid, CMD_DATA, bytes, readbytes) ;
		free(bytes) ;
	}


	// 重置
	else if(cmd==CMD_RESET) {
		uint8_t level = -1 ;
		if(datalen==1) {
			level = *data ;
		}
		task_reset(level) ;
	}
	
	else {
		char msg[32] ;
		sprintf(msg, "unknow package cmd value: %d", cmd) ;
		telnet_send_pkg_str(pkgid, CMD_EXCEPTION, msg) ;
	}

}

void telnet_send_ready() {
	char * buff = mallocf("{\"firmware\":\"beshell\",\"version\":\"%s\",\"level\":%d}", BESHELL_VERSION, task_boot_level()) ;
    telnet_send_pkg(0,CMD_READY,buff, strlen(buff)) ;
	free(buff) ;
}