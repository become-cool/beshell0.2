#include "module_wifi.h"
#include "logging.h"

#include <lwip/apps/sntp.h>
#include <esp_err.h>
#include <esp_event_loop.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <lwip/dns.h>
#include <lwip/sockets.h>

LOG_TAG("wifi");

#define DEFAULT_SCAN_LIST_SIZE 16


bool sta_connect_pending = false ;
bool wifi_state_connected = false ;



/**
 * Convert a Wifi reason code to a string representation.
 */
/*
static char *wifiReasonToString(uint8_t reason) {
  switch(reason) {
  case WIFI_REASON_4WAY_HANDSHAKE_TIMEOUT:
    return "4WAY_HANDSHAKE_TIMEOUT";
  case WIFI_REASON_802_1X_AUTH_FAILED:
    return "802_1X_AUTH_FAILED";
  case WIFI_REASON_AKMP_INVALID:
    return "AKMP_INVALID";
  case WIFI_REASON_ASSOC_EXPIRE:
    return "ASSOC_EXPIRE";
  case WIFI_REASON_ASSOC_FAIL:
    return "ASSOC_FAIL";
  case WIFI_REASON_ASSOC_LEAVE:
    return "ASSOC_LEAVE";
  case WIFI_REASON_ASSOC_NOT_AUTHED:
    return "ASSOC_NOT_AUTHED";
  case WIFI_REASON_ASSOC_TOOMANY:
    return "ASSOC_TOOMANY";
  case WIFI_REASON_AUTH_EXPIRE:
    return "AUTH_EXPIRE";
  case WIFI_REASON_AUTH_FAIL:
    return "AUTH_FAIL";
  case WIFI_REASON_AUTH_LEAVE:
    return "AUTH_LEAVE";
  case WIFI_REASON_BEACON_TIMEOUT:
    return "BEACON_TIMEOUT";
  case WIFI_REASON_CIPHER_SUITE_REJECTED:
    return "CIPHER_SUITE_REJECTED";
  case WIFI_REASON_DISASSOC_PWRCAP_BAD:
    return "DISASSOC_PWRCAP_BAD";
  case WIFI_REASON_DISASSOC_SUPCHAN_BAD:
    return "DISASSOC_SUPCHAN_BAD";
  case WIFI_REASON_GROUP_CIPHER_INVALID:
    return "GROUP_CIPHER_INVALID";
  case WIFI_REASON_GROUP_KEY_UPDATE_TIMEOUT:
    return "GROUP_KEY_UPDATE_TIMEOUT";
  case WIFI_REASON_HANDSHAKE_TIMEOUT:
    return "HANDSHAKE_TIMEOUT";
  case WIFI_REASON_IE_INVALID:
    return "IE_INVALID";
  case WIFI_REASON_IE_IN_4WAY_DIFFERS:
    return "IE_IN_4WAY_DIFFERS";
  case WIFI_REASON_INVALID_RSN_IE_CAP:
    return "INVALID_RSN_IE_CAP";
  case WIFI_REASON_MIC_FAILURE:
    return "MIC_FAILURE";
  case WIFI_REASON_NOT_ASSOCED:
    return "NOT_ASSOCED";
  case WIFI_REASON_NOT_AUTHED:
    return "NOT_AUTHED";
  case WIFI_REASON_NO_AP_FOUND:
    return "NO_AP_FOUND";
  case WIFI_REASON_PAIRWISE_CIPHER_INVALID:
    return "PAIRWISE_CIPHER_INVALID";
  case WIFI_REASON_UNSPECIFIED:
    return "UNSPECIFIED";
  case WIFI_REASON_UNSUPP_RSN_IE_VERSION:
    return "REASON_UNSUPP_RSN_IE_VERSION";
  }
  return "Unknown reason";
}
*/

/**
 * An ESP32 WiFi event handler.
 * The types of events that can be received here are:
 *
 * SYSTEM_EVENT_AP_PROBEREQRECVED
 * SYSTEM_EVENT_AP_STACONNECTED
 * SYSTEM_EVENT_AP_STADISCONNECTED
 * SYSTEM_EVENT_AP_START
 * SYSTEM_EVENT_AP_STOP
 * SYSTEM_EVENT_SCAN_DONE
 * SYSTEM_EVENT_STA_AUTHMODE_CHANGE
 * SYSTEM_EVENT_STA_CONNECTED
 * SYSTEM_EVENT_STA_DISCONNECTED
 * SYSTEM_EVENT_STA_GOT_IP
 * SYSTEM_EVENT_STA_START
 * SYSTEM_EVENT_STA_STOP
 * SYSTEM_EVENT_WIFI_READY
 */
static void esp32_wifi_eventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
	LOGD("event: %d, wifi_state_connected=%d, sta_connect_pending=%d", event_id, wifi_state_connected, sta_connect_pending);

	// Your event handling code here...
	switch(event_id) {

		case SYSTEM_EVENT_STA_GOT_IP: {
			break;
		}
		
		case SYSTEM_EVENT_STA_CONNECTED: {

			wifi_state_connected = true ;
			sta_connect_pending = false ;

			break;
		}

		case SYSTEM_EVENT_STA_DISCONNECTED: {
			
			wifi_state_connected = false ;
			printf("wifi_state_connected=false, pending=%d\n", sta_connect_pending) ;
			printf("%d\n",((wifi_event_sta_disconnected_t*) event_data)->reason) ;

			// connect() 失败
			if(sta_connect_pending) {
				sta_connect_pending = false ;
                
                // todo 
                // callback(error)

				break ;
			}

            // todo 
            // if disconnect

			break;
		}


		case SYSTEM_EVENT_SCAN_DONE: {
			break;
		} 

		default: {
			break;
		}
	}
}

JSValue js_wifi_start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

	esp_err_t errRc;

	LOGD(">> js_wifi_connect");

    tcpip_adapter_dhcpc_start(TCPIP_ADAPTER_IF_STA) ;

	// 已经连接
	if( wifi_state_connected || sta_connect_pending ) {
		return JS_UNDEFINED ;
	}

    wifi_config_t wifi_config = {
        .sta = {
            /* Setting a password implies station will connect to all security modes including WEP/WPA.
             * However these modes are deprecated and not advisable to be used. Incase your Access point
             * doesn't support WPA2, these mode can be enabled by commenting below line */
	     	.threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };

    
    char * ssid = NULL, * password = NULL ;
    size_t ssidLen = 0, passwordLen = 0 ;
    if(argc>0) {
        ssid = JS_ToCStringLen(ctx, &ssidLen, argv[0]);
        if(ssidLen>31)
            ssidLen = 31 ;
    }
    if(argc>1) {
        password = JS_ToCStringLen(ctx, &passwordLen, argv[1]);
        if(passwordLen>63)
            passwordLen = 63 ;
    }
    
	memcpy((char *)wifi_config.sta.ssid, ssid, ssidLen);
    wifi_config.sta.ssid[ssidLen] = 0 ;
    if(passwordLen>0){
	    memcpy((char *)wifi_config.sta.password, password,passwordLen);
    }
    wifi_config.sta.password[passwordLen] = 0 ;

    printf("ssid:%s, %d \n", wifi_config.sta.ssid, ssidLen) ;
    printf("password:%s, %d \n", wifi_config.sta.password, passwordLen) ;

    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );

	sta_connect_pending = true ;
	ESP_ERROR_CHECK(esp_wifi_connect()) ;

    return JS_UNDEFINED ;
}


void wifi_init() {

    ESP_ERROR_CHECK(esp_netif_init());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
	ESP_ERROR_CHECK(esp_wifi_start()) ;

    // hostname
    tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, "BeCore32xxx");

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &esp32_wifi_eventHandler, NULL));
}

void require_module_wifi(JSContext *ctx) {

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue wifi = JS_NewObject(ctx);
    
    JS_SetPropertyStr(ctx, wifi, "connect", JS_NewCFunction(ctx, js_wifi_start, "connect", 1));
    JS_SetPropertyStr(ctx, global, "WiFi", wifi);

    JS_FreeValue(ctx, global);
    // JS_FreeValue(ctx, wifi);
}