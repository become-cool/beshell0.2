#ifndef _MODULE_WIFI_H
#define _MODULE_WIFI_H

#include "quickjs-libc.h"
#include <esp_netif.h>

esp_netif_t * get_netif_ap() ;
esp_netif_t * get_netif_sta() ;

void wifi_init() ;

void require_module_wifi(JSContext *ctx) ;

#endif