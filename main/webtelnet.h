#ifndef __H_WEBTELNET_
#define __H_WEBTELNET_

#include "esp_http_server.h"

#define MAX_OPEN_SOCKETS 7
#define WEBTELNET_PORT 8023

void be_module_telweb_init() ;
httpd_handle_t webtelnet_handle()  ;

#endif