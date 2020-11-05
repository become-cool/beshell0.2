#include "telnet.h"
#include "driver/uart.h"
#include <stdio.h>
#include <sys/fcntl.h>
#include "esp_vfs.h"
#include "esp_vfs_dev.h"
#include "logging.h"
#include <sys/errno.h>

LOG_TAG("telnet")

uart_config_t uart_config = {
	.baud_rate = 115200,
	.data_bits = UART_DATA_8_BITS,
	.parity    = UART_PARITY_DISABLE,
	.stop_bits = UART_STOP_BITS_1,
	.flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
	.source_clk = UART_SCLK_APB,
};
int uart_fd;

char recv_buf[256];

void telnet_init() {
    uart_driver_install(UART_NUM_0, 2*1024, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config);
	
	if ((uart_fd = open("/dev/uart/0", O_RDWR)) == -1) {
		LOGE("Cannot open UART");
		return;
	}


	// We have a driver now installed so set up the read/write functions to use driver also.
	esp_vfs_dev_uart_use_driver(0);

	// printf("uart0 has opened\n")  ;
}

/**
 * Process telnet commands on a separate task.
 */
void telnet_loop(JSContext *ctx) {
	
	int s;
	fd_set rfds;
	struct timeval tv = {
		.tv_sec = 0,
		.tv_usec = 0,
	};

	FD_ZERO(&rfds);
	FD_SET(uart_fd, &rfds);

	s = select(uart_fd + 1, &rfds, NULL, NULL, &tv);

	if (s < 0) {
		LOGE("Select failed: errno %d", errno);
		return;
	} else if (s == 0) {
		// ESP_LOGI(TAG, "Timeout has been reached and nothing has been received");
	} else {
		if (FD_ISSET(uart_fd, &rfds)) {
			int len = read(uart_fd, recv_buf, sizeof(recv_buf)-1) ;
			if ( len > 0) {
				recv_buf[len] = 0 ;
				// printf("js>%s", recv_buf);

				JSValue ret = JS_Eval(ctx, recv_buf, len, ":telnet", JS_EVAL_TYPE_GLOBAL) ;
				// JSValue error = JS_GetException(ctx) ;
				if( JS_IsException(ret) ) {
					js_std_dump_error(ctx) ;
				}
				else {
					const char * retstr = JS_ToCString(ctx, ret);
					printf("%s\n", retstr) ;
					JS_FreeCString(ctx, retstr);
				}

				LOGD("free: %dByte\n", esp_get_free_heap_size()) ;


			} else {
				LOGE("UART read error");
				return;
			}
		} else {
			LOGE("No FD has been set in select()");
			return;
		}
	}

} // newTelnetPartner