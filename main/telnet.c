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

LOG_TAG("telnet")

#define TELNET_PORT 8022



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
char uart_recv_buf[256];

// --------------------
// TCP
int sock_server = -1 ;
int sock_client = -1 ;

struct sockaddr_in6 source_addr; // Large enough for both IPv4 or IPv6
uint addr_len = sizeof(source_addr);
char tcp_recv_buff[256];


void telnet_init() {

	// uart telnet ----------
    uart_driver_install(UART_NUM_0, 2*1024, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config);
	if ((uart_fd = open("/dev/uart/0", O_RDWR)) != -1) {
		esp_vfs_dev_uart_use_driver(0);
	}

	
	// tcp telnet ----------
    if(sock_server>-1) {
        printf("telnet startup already.\n") ;
        return ;
    }
    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(TELNET_PORT);
    sock_server = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock_server < 0) {
        printf("Unable to create socket: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }
    // printf("Socket created");

    int err = bind(sock_server, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        printf("Socket unable to bind: errno %d", errno);
        shutdown(sock_server, 0) ;
        close(sock_server);
        return ;
    }
    // printf("Socket bound, port %d\n", TELNET_PORT);

    err = listen(sock_server, 1);
    if (err != 0) {
        printf("Error occurred during listen: errno %d", errno);
        shutdown(sock_server, 0) ;
        close(sock_server);
        return ;
    }

    printf("TCP Telnet start on port %d \n\n", TELNET_PORT) ;
}


void eval_input(JSContext *ctx, char * strcode, int len) {
	JSValue ret = NULL ;

	if(_func_repl_input && JS_IsFunction(ctx, _func_repl_input)) {
		JSValue code = JS_NewStringLen(ctx, strcode, len) ;
		CALL_FUNC(_func_repl_input, JS_NULL, 1, &code)
		JS_FreeValue(ctx, code) ;
	} 
	// 默认的解释器
	else {
		ret = JS_Eval(ctx, strcode, len, ":telnet", JS_EVAL_TYPE_GLOBAL) ;
		// JSValue error = JS_GetException(ctx) ;
		if( JS_IsException(ret) ) {
			js_std_dump_error(ctx) ;
		}
		else {
			const char * retstr = JS_ToCString( ctx, ret );
			printf("%s\n", retstr) ;
			JS_FreeCString(ctx, retstr);
		}
	}

	JS_FreeValue(ctx, ret) ;
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
		int len = read(uart_fd, uart_recv_buf, sizeof(uart_recv_buf)-1) ;
		if ( len > 0) {
			uart_recv_buf[len] = 0 ;
			eval_input(ctx, uart_recv_buf, len) ;
		} else {
			LOGE("UART read error");
			return;
		}
	} 
	
	
	// TCP Client 接入
	if(FD_ISSET(sock_server, &telnet_rfds)) {
		// 关闭原有连接
		if(sock_client>-1) {
			
			printf("Close last telnet client for new one\n") ;
			shutdown(sock_client, 0);
			close(sock_client);
		}

		sock_client = accept(sock_server, (struct sockaddr *)&source_addr, &addr_len);
		if(sock_client<0){
			printf("accept() error: %d.\n", errno) ;
		}
		else {
			printf("telnet client come in\n") ;
		}
	}


	// TCP Client 事件
	if(sock_client>-1 && FD_ISSET(sock_client, &telnet_rfds)) {
		int len = recv(sock_client, tcp_recv_buff, sizeof(tcp_recv_buff) - 1, 0);
		// Client 错误
		if(len<0) {
			char errbuf[32] ;
			esp_err_to_name_r(errno, errbuf, sizeof(errbuf)) ;
			printf("recv() error: %s.\n", errbuf) ;
		}
		// Client 断开
		else if(len==0) {
			printf("telnet client leave\n") ;

			shutdown(sock_client, 0);
			close(sock_client);

			sock_client = -1 ;
		}
		// 接收到数据
		else {
			tcp_recv_buff[len] = 0 ;
			printf("tcp recv: %s\n", tcp_recv_buff) ;
			eval_input(ctx, tcp_recv_buff, len) ;
		}
	}

}