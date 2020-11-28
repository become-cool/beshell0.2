#include "module_wifi.h"
#include "logging.h"
#include "eventloop.h"
#include "task_js.h"
#include "utils.h"
#include "telnet.h"

#include <lwip/apps/sntp.h>
#include <esp_err.h>
#include <esp_event_loop.h>
#include <esp_log.h>
#include <esp_wifi.h>
#include <lwip/dns.h>
#include <lwip/sockets.h>
#include <esp_netif.h>

LOG_TAG("wifi");

#define DEFAULT_SCAN_LIST_SIZE 16


bool sta_connect_pending = false ;
bool _sta_connected = false ;
bool _ap_connected = false ;

JSValue __connect_callback = NULL ;
JSValue __disconnect_callback = NULL ;

uint32_t __disconnect_reason = 0 ;

esp_netif_t * netif_ap = NULL ;

esp_netif_t * netif_sta = NULL ;
esp_netif_ip_info_t sta_ip_info ;



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


// typedef enum {
//     SYSTEM_EVENT_WIFI_READY = 0,           /*!< ESP32 WiFi ready */
//     SYSTEM_EVENT_SCAN_DONE = 1,                /*!< ESP32 finish scanning AP */
//     SYSTEM_EVENT_STA_START = 2,                /*!< ESP32 station start */
//     SYSTEM_EVENT_STA_STOP = 3,                 /*!< ESP32 station stop */
//     SYSTEM_EVENT_STA_CONNECTED =4,            /*!< ESP32 station connected to AP */
//     SYSTEM_EVENT_STA_DISCONNECTED =5,         /*!< ESP32 station disconnected from AP */
//     SYSTEM_EVENT_STA_AUTHMODE_CHANGE =6,      /*!< the auth mode of AP connected by ESP32 station changed */
//     SYSTEM_EVENT_STA_GOT_IP=7,               /*!< ESP32 station got IP from connected AP */
//     SYSTEM_EVENT_STA_LOST_IP =8,              /*!< ESP32 station lost IP and the IP is reset to 0 */
//     SYSTEM_EVENT_STA_WPS_ER_SUCCESS =9,       /*!< ESP32 station wps succeeds in enrollee mode */
//     SYSTEM_EVENT_STA_WPS_ER_FAILED = 10,        /*!< ESP32 station wps fails in enrollee mode */
//     SYSTEM_EVENT_STA_WPS_ER_TIMEOUT =11,       /*!< ESP32 station wps timeout in enrollee mode */
//     SYSTEM_EVENT_STA_WPS_ER_PIN =12,           /*!< ESP32 station wps pin code in enrollee mode */
//     SYSTEM_EVENT_STA_WPS_ER_PBC_OVERLAP =13,   /*!< ESP32 station wps overlap in enrollee mode */
//     SYSTEM_EVENT_AP_START =14,                 /*!< ESP32 soft-AP start */
//     SYSTEM_EVENT_AP_STOP =15,                  /*!< ESP32 soft-AP stop */
//     SYSTEM_EVENT_AP_STACONNECTED =16,          /*!< a station connected to ESP32 soft-AP */
//     SYSTEM_EVENT_AP_STADISCONNECTED = 17,       /*!< a station disconnected from ESP32 soft-AP */
//     SYSTEM_EVENT_AP_STAIPASSIGNED = 18,         /*!< ESP32 soft-AP assign an IP to a connected station */
//     SYSTEM_EVENT_AP_PROBEREQRECVED =19,        /*!< Receive probe request packet in soft-AP interface */
//     SYSTEM_EVENT_GOT_IP6,                  /*!< ESP32 station or ap or ethernet interface v6IP addr is preferred */
//     SYSTEM_EVENT_ETH_START,                /*!< ESP32 ethernet start */
//     SYSTEM_EVENT_ETH_STOP,                 /*!< ESP32 ethernet stop */
//     SYSTEM_EVENT_ETH_CONNECTED,            /*!< ESP32 ethernet phy link up */
//     SYSTEM_EVENT_ETH_DISCONNECTED,         /*!< ESP32 ethernet phy link down */
//     SYSTEM_EVENT_ETH_GOT_IP,               /*!< ESP32 ethernet got IP from connected AP */
//     SYSTEM_EVENT_MAX                       /*!< Number of members in this enum */
// } 
static void esp32_wifi_eventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
	// printf("event_base: %d, event_id: %d, _sta_connected=%d, sta_connect_pending=%d, has callback=%d\n", (int32_t)event_base, event_id, _sta_connected, sta_connect_pending, __connect_callback?1:0);

	// Your event handling code here...
	switch(event_id) {
		
		case SYSTEM_EVENT_STA_CONNECTED: {

			_sta_connected = true ;
			sta_connect_pending = false ;

            if(__connect_callback) {

                JSValueConst * argv = malloc(sizeof(JSValue)) ;  // 由 eventloop_out() 函数 free
                argv[0] = JS_NULL ;
                eventloop_push_with_argv(task_current_context(), __connect_callback, 1, argv) ;
                JS_FreeValue(task_current_context(), __connect_callback) ;

                __connect_callback = NULL ;
            }

			break;
		}

		case SYSTEM_EVENT_STA_DISCONNECTED: {
			
			_sta_connected = false ;
			// printf("_sta_connected=false, pending=%d\n", sta_connect_pending) ;
			// printf("%d\n",((wifi_event_sta_disconnected_t*) event_data)->reason) ;

            __disconnect_reason = ((wifi_event_sta_disconnected_t*) event_data)->reason ;

			// connect() 失败
			if(sta_connect_pending) {

				sta_connect_pending = false ;
                
                if(__connect_callback) {
                    JSContext * ctx = task_current_context() ;
                    JSValueConst * argv = malloc(sizeof(JSValue)) ;  // 由 eventloop_out() 函数 free
                    argv[0] = JS_NewObject(ctx) ;
                    JS_SetPropertyStr(ctx, argv[0], "errno", JS_NewInt32(ctx, __disconnect_reason) ) ;

                    eventloop_push_with_argv(ctx, __connect_callback, 1, argv) ;

                    JS_FreeValue(ctx, __connect_callback) ;
                    __connect_callback = NULL ;
                }
			}

            if(__disconnect_callback) {
                JSContext * ctx = task_current_context() ;
                eventloop_push_with_argv(ctx, __disconnect_callback, 0, NULL) ;
                JS_FreeValue(ctx, __disconnect_callback) ;
                __disconnect_callback = NULL ;
            }

			break;
		}

        case SYSTEM_EVENT_AP_START:
            _ap_connected = true ;
            // printf("SYSTEM_EVENT_AP_START\n") ;
            break;

        case SYSTEM_EVENT_AP_STOP:
            // printf("SYSTEM_EVENT_AP_STOP\n") ;
            _ap_connected = false ;
            break;


		case SYSTEM_EVENT_SCAN_DONE: {
			break;
		} 

		default: {
			break;
		}
	}
}

static void ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_id==IP_EVENT_STA_GOT_IP) {
        // printf("ip_event_handler(IP_EVENT_STA_GOT_IP)\n") ;
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        sta_ip_info.ip.addr = event->ip_info.ip.addr ;
        sta_ip_info.netmask.addr = event->ip_info.netmask.addr ;
        sta_ip_info.gw.addr = event->ip_info.gw.addr ;
    }
}

/**
 * ssid
 * password
 * callback
 */
JSValue js_wifi_connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

    esp_err_t errRc;
    esp_netif_dhcpc_start(netif_sta) ;

    JSValue callback = NULL ;
    if(argc>=3 && JS_IsFunction(ctx, argv[2])) {
        callback = argv[3] ;
        // printf("argc=%d, argv[3]is function? %d, ref: %d\n", argc, JS_IsFunction(ctx, callback), VAR_REFCNT(callback)) ;
    }

    __disconnect_reason = 0 ;

    // 已经连接
    if( _sta_connected ) {
        if(callback) {
            MAKE_ARGV1(_cbargv, JS_NULL)
            CALL_FUNC(callback, JS_NULL, 1, _cbargv)
            free(_cbargv) ;
        }
        return JS_UNDEFINED ;
    }

    // 正在连接
    if( sta_connect_pending ) {

        MAKE_ARGV1(_cbargv, JS_NewObject(ctx))            
        JS_SetPropertyStr(ctx, _cbargv[0], "errno", JS_NewInt32(ctx, 300)) ;
        JS_SetPropertyStr(ctx, _cbargv[0], "errname", JS_NewString(ctx, "connecting")) ;

        CALL_FUNC(callback, JS_NULL, 1, _cbargv)
        
        free(_cbargv) ;
		    
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

    // printf("ssid:%s, %d \n", wifi_config.sta.ssid, ssidLen) ;
    // printf("password:%s, %d \n", wifi_config.sta.password, passwordLen) ;

    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );

    sta_connect_pending = true ;
    ESP_ERROR_CHECK(esp_wifi_connect()) ;

    if(__connect_callback) {
        printf("__connect_callback != null???");
        JS_FreeValue(ctx, __connect_callback) ;
    }
    if(callback) {
        JS_DupValue(ctx, callback) ;
        __connect_callback = callback ;
    }

    return JS_UNDEFINED ;
}

JSValue js_wifi_disconnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    JSValue callback = NULL ;
    if(argc>=1 && JS_IsFunction(ctx, argv[0])) {
        callback = argv[0] ;
    }

    if( !_sta_connected && !sta_connect_pending) {
        if(callback) {
            JSValue ret = JS_Call(ctx, callback, JS_NULL, 0, NULL) ;
            if( JS_IsException(ret) ){
                js_std_dump_error(ctx) ;
            }
            JS_FreeValue(ctx, ret) ;
        }
    }

    else {
        ESP_ERROR_CHECK(esp_wifi_disconnect()) ;

        if(__disconnect_callback) {
            JS_FreeValue(ctx, __connect_callback) ;
        }
        if(callback) {
            JS_DupValue(ctx, callback) ;
            __disconnect_callback = callback ;
        }
    }

    return JS_UNDEFINED ;
}

JSValue js_wifi_get_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    wifi_mode_t mode;
    ESP_ERROR_CHECK(esp_wifi_get_mode(&mode));
    switch(mode) {
    case WIFI_MODE_NULL:
        return JS_NewString(ctx,"none") ;
        break;
    case WIFI_MODE_STA:
        return JS_NewString(ctx,"sta") ;
        break;
    case WIFI_MODE_AP:
        return JS_NewString(ctx,"ap") ;
        break;
    case WIFI_MODE_APSTA:
        return JS_NewString(ctx,"apsta") ;
        break;
    default:
        return JS_NewString(ctx,"unknow") ;
        break;
    }
}

JSValue js_wifi_get_status(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    JSValue status = JS_NewObject(ctx) ;
    char ip[16] ;
    wifi_config_t config;

    if(_sta_connected){
        JS_SetPropertyStr(ctx, status, "status", JS_NewString(ctx,"connected")) ;
    }
    else if(sta_connect_pending) {
        JS_SetPropertyStr(ctx, status, "status", JS_NewString(ctx,"connecting")) ;
    }
    else {
        JS_SetPropertyStr(ctx, status, "status", JS_NewString(ctx,"disconnected")) ;
        JS_SetPropertyStr(ctx, status, "reason",JS_NewInt32(ctx,__disconnect_reason)) ;
    }
    
    sprintf(ip,IPSTR, IP2STR((&sta_ip_info.ip))) ;
    JS_SetPropertyStr(ctx, status, "ip", JS_NewString(ctx, ip)) ;
    sprintf(ip,IPSTR, IP2STR((&sta_ip_info.netmask))) ;
    JS_SetPropertyStr(ctx, status, "netmask", JS_NewString(ctx, ip)) ;
    sprintf(ip,IPSTR, IP2STR((&sta_ip_info.gw))) ;
    JS_SetPropertyStr(ctx, status, "gw", JS_NewString(ctx, ip)) ;

    esp_wifi_get_config(WIFI_IF_STA, &config);
    JS_SetPropertyStr(ctx, status, "ssid", JS_NewString(ctx,(const char *)config.sta.ssid)) ;
    JS_SetPropertyStr(ctx, status, "password", JS_NewString(ctx,(const char *)config.sta.password)) ;

    return status ;
}


JSValue js_wifi_get_ap_status(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
  
    char ip[16] ;
    wifi_config_t config;
    esp_wifi_get_config(WIFI_IF_AP, &config);
    JSValue status = JS_NewObject(ctx) ;

    esp_wifi_get_config(WIFI_IF_AP, &config);
    JS_SetPropertyStr(ctx, status, "ssid", JS_NewString(ctx,(const char *)config.sta.ssid)) ;
    JS_SetPropertyStr(ctx, status, "password", JS_NewString(ctx,(const char *)config.sta.password)) ;
    JS_SetPropertyStr(ctx, status, "started", _ap_connected? JS_TRUE: JS_FALSE) ;

    esp_netif_ip_info_t ipinfo;
    bzero(&ipinfo, sizeof(esp_netif_ip_info_t));

    esp_netif_get_ip_info(netif_ap, &ipinfo);

    sprintf(ip,IPSTR, IP2STR((&ipinfo.ip))) ;
    JS_SetPropertyStr(ctx, status, "ip", JS_NewString(ctx, ip)) ;

    sprintf(ip,IPSTR, IP2STR((&ipinfo.netmask))) ;
    JS_SetPropertyStr(ctx, status, "netmask", JS_NewString(ctx, ip)) ;

    sprintf(ip,IPSTR, IP2STR((&ipinfo.gw))) ;
    JS_SetPropertyStr(ctx, status, "gw", JS_NewString(ctx, ip)) ;

    return status ;

}


JSValue js_wifi_set_hostname(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(1)

    char * hostname = JS_ToCString(ctx, argv[0]) ;
    esp_netif_set_hostname(netif_sta, hostname);
    esp_netif_set_hostname(netif_ap, hostname);

    JS_FreeCString(ctx, hostname) ;

    return JS_UNDEFINED ;
}

JSValue js_wifi_start_ap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)

    wifi_config_t wifi_config ;

    size_t ssidlen = 0 ;
    char * ssid = JS_ToCStringLen(ctx, &ssidlen, argv[0]) ;
    if(ssidlen>sizeof(wifi_config.ap.ssid)) {
      ssidlen = sizeof(wifi_config.ap.ssid) - 1 ;
      ssid[ssidlen] = 0 ;
    }

    size_t passwordlen = 0 ;
    char * password = JS_ToCStringLen(ctx, &passwordlen, argv[1]) ;
    if(passwordlen>sizeof(wifi_config.ap.password)) {
        passwordlen = sizeof(wifi_config.ap.password) ;
        password[passwordlen] = 0 ;
    }

    wifi_config.ap.ssid_len = ssidlen ;
    memcpy(wifi_config.ap.ssid, ssid, ssidlen) ;
    wifi_config.ap.ssid[ssidlen] = 0 ;
    memcpy(wifi_config.ap.password, password, passwordlen) ;
    wifi_config.ap.password[passwordlen] = 0 ;

    wifi_config.ap.channel = 0 ;
    wifi_config.ap.max_connection = 4 ;
    wifi_config.ap.authmode = passwordlen==0? WIFI_AUTH_OPEN: WIFI_AUTH_WPA2_PSK ;
    
    wifi_mode_t mode;
    ESP_ERROR_CHECK(esp_wifi_get_mode(&mode));
    mode|= WIFI_MODE_AP ;
    pf("esp_wifi_set_mode(%d)", mode)
    ESP_ERROR_CHECK(esp_wifi_set_mode(mode));

    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));

    // DHCP Server
    // esp_netif_ip_info_t info;
    // bzero(&info, sizeof(esp_netif_ip_info_t)) ;
    // inet_pton(AF_INET, "192.168.4.1", &info.ip.addr);
    // inet_pton(AF_INET, "192.168.4.1", &info.gw.addr);
    // inet_pton(AF_INET, "255.255.255.0", &info.netmask.addr);
    // esp_netif_dhcps_stop(netif_ap);
    // esp_netif_set_ip_info(netif_ap, &info);
    // esp_netif_dhcps_start(netif_ap);

    // esp_wifi_set_protocol() ;


    JS_FreeCString(ctx, ssid) ;
    JS_FreeCString(ctx, password) ;

    return JS_UNDEFINED ;
}




JSValue js_wifi_stop_ap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
        
    wifi_mode_t mode;
    ESP_ERROR_CHECK(esp_wifi_get_mode(&mode));
    if(mode==WIFI_MODE_AP) {
        mode = WIFI_MODE_NULL ;
    }
    else if(mode==WIFI_MODE_APSTA) {
        mode = WIFI_MODE_STA ;
    }
    else {
        return JS_UNDEFINED ;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode( mode | WIFI_MODE_AP));

    return JS_UNDEFINED ;
}

void wifi_init() {

    ESP_ERROR_CHECK(esp_netif_init());

    netif_sta = esp_netif_create_default_wifi_sta();
    netif_ap = esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_start()) ;

    bzero(&sta_ip_info, sizeof(esp_netif_ip_info_t));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &esp32_wifi_eventHandler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &ip_event_handler, NULL));
}

void require_module_wifi(JSContext *ctx) {

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue wifi = JS_NewObject(ctx);
    
    JS_SetPropertyStr(ctx, wifi, "connect", JS_NewCFunction(ctx, js_wifi_connect, "connect", 1));
    JS_SetPropertyStr(ctx, wifi, "disconnect", JS_NewCFunction(ctx, js_wifi_disconnect, "disconnect", 1));
    JS_SetPropertyStr(ctx, wifi, "getMode", JS_NewCFunction(ctx, js_wifi_get_mode, "getMode", 1));
    JS_SetPropertyStr(ctx, wifi, "getStatus", JS_NewCFunction(ctx, js_wifi_get_status, "getStatus", 1));
    JS_SetPropertyStr(ctx, wifi, "getAPStatus", JS_NewCFunction(ctx, js_wifi_get_ap_status, "getAPStatus", 1));
    JS_SetPropertyStr(ctx, wifi, "setHostname", JS_NewCFunction(ctx, js_wifi_set_hostname, "setHostname", 1));
    JS_SetPropertyStr(ctx, wifi, "startAP", JS_NewCFunction(ctx, js_wifi_start_ap, "startAP", 1));
    JS_SetPropertyStr(ctx, wifi, "stopAP", JS_NewCFunction(ctx, js_wifi_stop_ap, "stopAP", 1));
    JS_SetPropertyStr(ctx, global, "WiFi", wifi);
    JS_FreeValue(ctx, global);
}