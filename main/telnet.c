#include "telnet.h"
#include "driver/uart.h"
#include <stdio.h>
#include <sys/fcntl.h>
#include "esp_vfs.h"
#include "utils.h"
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

uint8_t uart_recv_buf[256];
uint8_t tcp_recv_buff[256];

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
}



// --------------------
// UART
uart_config_t uart_config = {
	.baud_rate = 115200,
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

void telnet_init() {

	pPkgCmdProcess = on_pkg_receive ;

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
		int len = uart_read_bytes(0, uart_recv_buf, sizeof(uart_recv_buf), 0) ;
		if(len<0) {
			printf("read uart error\n") ;
		}
		else {
			WriteToBuffer(uart_recv_buf, len) ;
			PACKAGE_LOOP(ctx)
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
			echof("recv() error: %s.\n", errbuf) ;
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
			WriteToBuffer(tcp_recv_buff, len) ;
			PACKAGE_LOOP(ctx)
		}
	}
}


char * send_buff [270] ;

void telnet_send_one_pkg(char pkgid, char remain, char cmd, char * data, uint8_t datalen) {


	uint16_t pkglen = PKGLEN_WITHOUT_DATA + datalen ;
	
	// printf("datalen=%d, pkglen=%d\n", datalen, pkglen) ;

	// char * pkgbuff = malloc(pkglen) ;
	Pack((uint8_t*)send_buff, pkgid, remain, cmd, (uint8_t*)data, datalen) ;

	// 串口输出
	uart_write_bytes(0, send_buff, pkglen);
	uart_wait_tx_done(0, 10000);

	
	// tcp socket 输出
	if(sock_client>-1) {
    	send(sock_client, send_buff, pkglen, 0) ;
	}
	// free(pkgbuff) ;
}

void telnet_send_pkg(char pkgid, char cmd, char * data, uint16_t datalen) {
	
	int pkgcnt = (int) ceil( (float)datalen/(float)PKGLEN_MAX_DATA ) ;

	// printf("datalen=%d, max=%d, pkgcnt=%d\n", datalen, PKGLEN_MAX_DATA, pkgcnt) ;
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

	if( JS_IsString(argv[2]) ) {
		ARGV_TO_STRING(2, data, datalen)
		// printf("data=%s, datalen=%d\n", data, datalen) ;
		telnet_send_pkg(pkgid, cmd, data, datalen) ;
		JS_FreeCString(ctx, data) ;
	}

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

void on_pkg_receive (char pkgid, char remain, char cmd, char datalen, void * ctx){
	// echof("pack received, pkgid=%d, cmd=%d, datalen=%d\n", pkgid, cmd, datalen) ;

	uint8_t * databuff = malloc(datalen+1) ;
	if(!databuff) {
		echo("Drop a package, memory low.") ;
		return ;
	}
	MemCpy(databuff, 6, datalen) ;

	// JS代码/命令
	if(cmd==CMD_RUN || cmd==CMD_CALL || cmd==CMD_CALL_ASYNC) {
		*(databuff+datalen) = '\0' ;
		
		if(_func_repl_input && JS_IsFunction(ctx, _func_repl_input)) {

			JSValueConst * argv = malloc(sizeof(JSValue)*3) ;
			argv[0] = JS_NewInt32(ctx, pkgid) ;
			argv[1] = JS_NewInt32(ctx, remain) ;
			argv[2] = JS_NewInt32(ctx, cmd) ;
			argv[3] = JS_NewStringLen(ctx, (char *)databuff, datalen) ;

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

	// 文件操作
	else if(cmd==CMD_FILE_PUSH_REQ || cmd==CMD_FILE_APPEND_REQ) {
		
	}
	else if(cmd==CMD_FILE_PULL_REQ) {
		
	}

	free(databuff) ;
}