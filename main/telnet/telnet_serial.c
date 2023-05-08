
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

LOG_TAG("telnet")

uint8_t recv_buff[128];

struct telnet_prot_buffer uart_pkg_buff ;


void be_telnet_serial_reset(JSContext *ctx) {
	telnet_prot_reset() ;
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


static void send_pkg_func(uint8_t * data, size_t datalen) {
	uart_write_bytes(0, data, datalen);
}

void be_telnet_serial_init() {

	telnet_prot_func_pkg_send = send_pkg_func ;
	telnet_prot_func_reset = task_reset ;

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

