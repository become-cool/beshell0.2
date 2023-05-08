#ifdef PLATFORM_WASM

#include "telnet_wasm.h"
#include "utils.h"
#include <string.h>
#include "telnet_protocal.h"
#include <emscripten.h>
#include <math.h>

static uint8_t * telnet_raw_buff = NULL ;
static size_t telnet_raw_buff_len = 0 ;


EMSCRIPTEN_KEEPALIVE bool be_telnet_wasm_send(uint8_t * data, size_t datalen) {

	if(telnet_raw_buff) {
		return false ;
	}

	telnet_raw_buff = malloc(datalen) ;
	if(!telnet_raw_buff) {
		return false ;
	}

	memcpy(telnet_raw_buff, data, datalen) ;
	// printf("%s\n",telnet_raw_buff) ;

	telnet_raw_buff_len = datalen ;

	return true ;
}


static void send_pkg_func(const uint8_t * data, size_t datalen) {
	EM_ASM_ARGS({
		Module.onOutput(Module['HEAPU8'].buffer.slice($0, $0 + $1)); 
	}, data, datalen);
}

void be_telnet_wasm_init() {
	telnet_prot_func_pkg_send = send_pkg_func ;
}


void be_telnet_wasm_loop(JSContext * ctx) {
	if(!telnet_raw_buff) {
		return ;
	}

	// printf("(%d)>>%.*s\n",telnet_raw_buff[PKGPOS_DATALEN],telnet_raw_buff[PKGPOS_DATALEN],telnet_raw_buff + PKG_HEADER_LEN);

	if( telnet_raw_buff_len>=PKGLEN_WITHOUT_DATA && telnet_raw_buff[PKGPOS_DATALEN]+PKGLEN_WITHOUT_DATA==telnet_raw_buff_len ) {
		on_pkg_receive(
			telnet_raw_buff[PKGPOS_ID] ,
			telnet_raw_buff[PKGPOS_REMAIN] ,
			telnet_raw_buff[PKGPOS_CMD] ,
			telnet_raw_buff + PKG_HEADER_LEN ,
			telnet_raw_buff[PKGPOS_DATALEN] ,
			ctx
		) ;

		// telnet_raw_buff[telnet_raw_buff_len-1] = 0 ;
		// eval_code_len(ctx, telnet_raw_buff+PKG_HEADER_LEN, telnet_raw_buff[PKGPOS_DATALEN], "xxxxxx") ;
	}

	else {
		printf("data invalid\n") ;
	}

	free(telnet_raw_buff) ;
	telnet_raw_buff = NULL ;
	telnet_raw_buff_len = 0 ;
}



#endif