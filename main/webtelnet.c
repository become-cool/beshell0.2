#include "webtelnet.h"


httpd_handle_t handle = NULL ;

void be_module_telweb_init() {
    
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_open_sockets = MAX_OPEN_SOCKETS;
    config.server_port = WEBTELNET_PORT ;

    /* Use the URI wildcard matching function in order to
     * allow the same handler to respond to multiple different
     * target URIs which match the wildcard scheme */
    config.uri_match_fn = httpd_uri_match_wildcard;

    httpd_start(&handle, &config) ;
}

httpd_handle_t webtelnet_handle()  {
    return handle ;
}