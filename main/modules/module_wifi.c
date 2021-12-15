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

LOG_TAG("wifi");

#define DEFAULT_SCAN_LIST_SIZE 16

#define MAX_SSID_CHARLEN        31
#define MAX_PASSWORD_CHARLEN    63


JSValue __event_handle = JS_UNDEFINED ;
JSContext * __event_handle_ctx = NULL ;

esp_netif_t * netif_ap = NULL ;
esp_netif_t * get_netif_ap() {
    return netif_ap ;
}

esp_netif_t * netif_sta = NULL ;
esp_netif_t * get_netif_sta() {
    return netif_sta ;
}

esp_event_handler_instance_t instance_any_id;
esp_event_handler_instance_t instance_got_ip;


bool _sta_ready = false ;
bool _sta_connected = false ;
bool _ap_started = false ;

// typedef enum {
//     WIFI_EVENT_WIFI_READY = 0,           /**< ESP32 WiFi ready */
//     WIFI_EVENT_SCAN_DONE = 1,                /**< ESP32 finish scanning AP */
//     WIFI_EVENT_STA_START = 2,                /**< ESP32 station start */
//     WIFI_EVENT_STA_STOP = 3,                 /**< ESP32 station stop */
//     WIFI_EVENT_STA_CONNECTED = 4,            /**< ESP32 station connected to AP */
//     WIFI_EVENT_STA_DISCONNECTED = 5,         /**< ESP32 station disconnected from AP */
//     WIFI_EVENT_STA_AUTHMODE_CHANGE=6,      /**< the auth mode of AP connected by ESP32 station changed */

//     WIFI_EVENT_STA_WPS_ER_SUCCESS=7,       /**< ESP32 station wps succeeds in enrollee mode */
//     WIFI_EVENT_STA_WPS_ER_FAILED=8,        /**< ESP32 station wps fails in enrollee mode */
//     WIFI_EVENT_STA_WPS_ER_TIMEOUT=9,       /**< ESP32 station wps timeout in enrollee mode */
//     WIFI_EVENT_STA_WPS_ER_PIN=10,           /**< ESP32 station wps pin code in enrollee mode */
//     WIFI_EVENT_STA_WPS_ER_PBC_OVERLAP=11,   /**< ESP32 station wps overlap in enrollee mode */

//     WIFI_EVENT_AP_START = 12,                 /**< ESP32 soft-AP start */
//     WIFI_EVENT_AP_STOP = 13,                  /**< ESP32 soft-AP stop */
//     WIFI_EVENT_AP_STACONNECTED = 14,          /**< a station connected to ESP32 soft-AP */
//     WIFI_EVENT_AP_STADISCONNECTED = 15,       /**< a station disconnected from ESP32 soft-AP */
//     WIFI_EVENT_AP_PROBEREQRECVED=16,        /**< Receive probe request packet in soft-AP interface */

//     WIFI_EVENT_FTM_REPORT=17,               /**< Receive report of FTM procedure */

//     /* Add next events after this only */
//     WIFI_EVENT_STA_BSS_RSSI_LOW=18,         /**< AP's RSSI crossed configured threshold */
//     WIFI_EVENT_ACTION_TX_STATUS=19,         /**< Status indication of Action Tx operation */
//     WIFI_EVENT_ROC_DONE=20,                 /**< Remain-on-Channel operation complete */

//     WIFI_EVENT_STA_BEACON_TIMEOUT=21,       /**< ESP32 station beacon timeout */

//     WIFI_EVENT_MAX,                      /**< Invalid WiFi event ID */
// } wifi_event_t;
static void esp32_wifi_eventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
	// printf("event_base: %d, event_id: %d\n", (int32_t)event_base, event_id);
    
    int eventType = -1 ;
    if(event_base==WIFI_EVENT) {
        eventType = 1 ;
        
        if(event_id == WIFI_EVENT_STA_START) {
            _sta_ready = true ;
        }
        else if(event_id == WIFI_EVENT_STA_STOP) {
            _sta_ready = false ;
        }
        else if(event_id == WIFI_EVENT_STA_CONNECTED) {
            _sta_connected = true ;
        }
        else if(event_id == WIFI_EVENT_STA_DISCONNECTED) {
            _sta_connected = false ;
        }
        else if(event_id == WIFI_EVENT_AP_START) {
            _ap_started = true ;
        }
        else if(event_id == WIFI_EVENT_AP_STOP) {
            _ap_started = false ;
        }
    }
    else if(event_base==IP_EVENT) {
        eventType = 2 ;
    }

    if( !JS_IsUndefined(__event_handle) && __event_handle_ctx!=NULL && JS_IsFunction(__event_handle_ctx, __event_handle) ) {
        MAKE_ARGV3(argv, JS_NewInt32(__event_handle_ctx, eventType), JS_NewInt32(__event_handle_ctx, event_id), JS_UNDEFINED)
        // dis reason
        if(event_base==WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
            argv[2] = JS_NewInt32(__event_handle_ctx, ((wifi_event_sta_disconnected_t*) event_data)->reason) ;
        }

        eventloop_push_with_argv(__event_handle_ctx, __event_handle, 3, argv) ;
    }

}

JSValue js_wifi_sta_ready(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    return JS_NewBool(ctx, _sta_ready) ;
}

JSValue js_wifi_sta_connected(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    return JS_NewBool(ctx, _sta_connected) ;
}

JSValue js_wifi_ap_started(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){
    return JS_NewBool(ctx, _ap_started) ;
}

/**
 * ssid
 * password
 */
JSValue js_wifi_connect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv){

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

    return JS_NewInt32(ctx, esp_wifi_connect()) ;
}

JSValue js_wifi_disconnect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_NewInt32(ctx, esp_wifi_disconnect()) ;
}

JSValue js_wifi_get_ip_info(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    CHECK_ARGC(1)
    ARGV_TO_UINT8(0,type)

    esp_netif_ip_info_t ipinfo;
    bzero(&ipinfo, sizeof(esp_netif_ip_info_t));

    if( type==WIFI_MODE_STA ) {
        esp_netif_get_ip_info(netif_sta, &ipinfo);
    }
    else if(type==WIFI_MODE_AP) {
        esp_netif_get_ip_info(netif_ap, &ipinfo);
    }
    else{
        THROW_EXCEPTION("unknow netif type: %d", type)
    }

    JSValue status = JS_NewObject(ctx) ;

    char ip[16] ;
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

    size_t ssidlen = 0 ;
    char * ssid = JS_ToCStringLen(ctx, &ssidlen, argv[0]) ;
    if(ssidlen>MAX_SSID_CHARLEN) {
      ssidlen = MAX_SSID_CHARLEN ;
      ssid[ssidlen] = 0 ;
    }

    size_t passwordlen = 0 ;
    char * password = JS_ToCStringLen(ctx, &passwordlen, argv[1]) ;
    if(passwordlen>MAX_PASSWORD_CHARLEN) {
        passwordlen = MAX_PASSWORD_CHARLEN ;
        password[passwordlen] = 0 ;
    }
    if(passwordlen>0 && passwordlen<8) {
        THROW_EXCEPTION("wifi password length must be greater than 8")
    }
    
    wifi_mode_t mode;
    if(esp_wifi_get_mode(&mode)!=ESP_OK) {
        THROW_EXCEPTION("esp_wifi_get_mode() failed")
    }
    printf("wifi mode:%d\n", mode) ;
    if(esp_wifi_set_mode( WIFI_MODE_APSTA)!=ESP_OK) {
        THROW_EXCEPTION("esp_wifi_set_mode() failed")
    }

    wifi_config_t wifi_config = {
        .ap = {
            .ssid_len = ssidlen,
            .channel = 1,
            .max_connection = 4,
            .authmode = (passwordlen==0)? WIFI_AUTH_OPEN: WIFI_AUTH_WPA_WPA2_PSK ,
            .ssid_hidden = 0
        },
    };

    memcpy(wifi_config.ap.ssid, ssid, ssidlen) ;
    wifi_config.ap.ssid[ssidlen] = 0 ;
    memcpy(wifi_config.ap.password, password, passwordlen) ;
    wifi_config.ap.password[passwordlen] = 0 ;

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

    return JS_NewInt32(ctx, esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config)) ;
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

    ESP_ERROR_CHECK(esp_wifi_set_mode( mode | WIFI_MODE_STA));

    return JS_UNDEFINED ;
}

JSValue js_wifi_get_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    wifi_mode_t mode_ori;
    esp_wifi_get_mode(&mode_ori) ;
    return JS_NewInt32(ctx, mode_ori);
}
JSValue js_wifi_set_mode(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    ARGV_TO_INT8(0, mode)
    return JS_NewInt32(ctx, esp_wifi_set_mode(mode));
}


JSValue js_wifi_set_ps(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    ARGV_TO_UINT32(0, ps_mode)
    return JS_NewInt32(ctx, esp_wifi_set_ps(ps_mode)) ;
}


JSValue js_wifi_get_config(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    
    CHECK_ARGC(1)
    ARGV_TO_UINT8(0,netif)

    wifi_config_t conf ;

    JSValue jsconf = JS_NewObject(ctx) ;
    if( netif==WIFI_MODE_STA ) {
        esp_err_t err = esp_wifi_get_config(ESP_IF_WIFI_STA, &conf) ;
        if(err!=ESP_OK) {
            THROW_EXCEPTION("esp_wifi_get_config() failed: %d", err)
        }
        JS_SetPropertyStr(ctx, jsconf, "ssid", JS_NewString(ctx, (const char *)conf.sta.ssid)) ;
        JS_SetPropertyStr(ctx, jsconf, "password", JS_NewString(ctx, (const char *)conf.sta.password)) ;
        JS_SetPropertyStr(ctx, jsconf, "channel", JS_NewInt32(ctx, conf.sta.channel)) ;
    }
    else if(netif==WIFI_MODE_AP) {
        esp_err_t err = esp_wifi_get_config(ESP_IF_WIFI_AP, &conf) ;
        if(err!=ESP_OK) {
            THROW_EXCEPTION("esp_wifi_get_config() failed: %d", err)
        }
        JS_SetPropertyStr(ctx, jsconf, "ssid", JS_NewString(ctx, (const char *)conf.ap.ssid)) ;
        JS_SetPropertyStr(ctx, jsconf, "password", JS_NewString(ctx, (const char *)conf.ap.password)) ;
        JS_SetPropertyStr(ctx, jsconf, "channel", JS_NewInt32(ctx, conf.ap.channel)) ;
        JS_SetPropertyStr(ctx, jsconf, "authmode", JS_NewInt32(ctx, conf.ap.authmode)) ;
        JS_SetPropertyStr(ctx, jsconf, "ssid_hidden", JS_NewInt32(ctx, conf.ap.ssid_hidden)) ;
        JS_SetPropertyStr(ctx, jsconf, "max_connection", JS_NewInt32(ctx, conf.ap.max_connection)) ;
    }
    else{
        THROW_EXCEPTION("unknow netif: %d", netif)
    }

    return jsconf ;
}


JSValue js_wifi_register_event_handle(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    if( !JS_IsFunction(ctx, argv[0]) ){
        THROW_EXCEPTION("wifi event handle must be a function")
    }
    JS_DupValue(ctx, argv[0]) ;
    __event_handle = argv[0] ;
    __event_handle_ctx = ctx ;
    return JS_UNDEFINED ;
}

JSValue js_wifi_start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    esp_err_t err = esp_wifi_start() ;
    // printf("esp_wifi_start(): %d\n", err) ;
    return JS_NewInt32(ctx, err);
}
JSValue js_wifi_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return JS_NewInt32(ctx, esp_wifi_stop());
}

void be_module_wifi_init() {

    ESP_ERROR_CHECK(esp_netif_init());

    netif_sta = esp_netif_create_default_wifi_sta();
    netif_ap = esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &esp32_wifi_eventHandler, NULL, &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &esp32_wifi_eventHandler, NULL, &instance_got_ip));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));

    printf("be_module_wifi_init()\n") ;
}

void be_module_wifi_require(JSContext *ctx) {

    JSValue global = JS_GetGlobalObject(ctx);
    JSValue beapi = JS_GetPropertyStr(ctx, global, "beapi") ;

    JSValue wifi = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, beapi, "wifi", wifi);

    JS_SetPropertyStr(ctx, wifi, "start", JS_NewCFunction(ctx, js_wifi_start, "start", 1));
    JS_SetPropertyStr(ctx, wifi, "stop", JS_NewCFunction(ctx, js_wifi_stop, "stop", 1));
    JS_SetPropertyStr(ctx, wifi, "setPS", JS_NewCFunction(ctx, js_wifi_set_ps, "setPS", 1));
    JS_SetPropertyStr(ctx, wifi, "setMode", JS_NewCFunction(ctx, js_wifi_set_mode, "setMode", 1));
    JS_SetPropertyStr(ctx, wifi, "getMode", JS_NewCFunction(ctx, js_wifi_get_mode, "getMode", 1));
    JS_SetPropertyStr(ctx, wifi, "getConfig", JS_NewCFunction(ctx, js_wifi_get_config, "getConfig", 1));
    JS_SetPropertyStr(ctx, wifi, "connect", JS_NewCFunction(ctx, js_wifi_connect, "connect", 1));
    JS_SetPropertyStr(ctx, wifi, "disconnect", JS_NewCFunction(ctx, js_wifi_disconnect, "disconnect", 1));
    JS_SetPropertyStr(ctx, wifi, "startAP", JS_NewCFunction(ctx, js_wifi_start_ap, "startAP", 1));
    JS_SetPropertyStr(ctx, wifi, "stopAP", JS_NewCFunction(ctx, js_wifi_stop_ap, "stopAP", 1));
    JS_SetPropertyStr(ctx, wifi, "getIpInfo", JS_NewCFunction(ctx, js_wifi_get_ip_info, "getIpInfo", 1));
    JS_SetPropertyStr(ctx, wifi, "setHostname", JS_NewCFunction(ctx, js_wifi_set_hostname, "setHostname", 1));
    JS_SetPropertyStr(ctx, wifi, "registerEventHandle", JS_NewCFunction(ctx, js_wifi_register_event_handle, "registerEventHandle", 1));
    
    JS_SetPropertyStr(ctx, wifi, "staReady", JS_NewCFunction(ctx, js_wifi_sta_ready, "staReady", 1));
    JS_SetPropertyStr(ctx, wifi, "staConnected", JS_NewCFunction(ctx, js_wifi_sta_connected, "staConnected", 1));
    JS_SetPropertyStr(ctx, wifi, "apStarted", JS_NewCFunction(ctx, js_wifi_ap_started, "apStarted", 1));

    JS_FreeValue(ctx, global);
    JS_FreeValue(ctx, beapi);
}



void be_module_wifi_reset(JSContext *ctx) {
    JS_FreeValue(ctx, __event_handle);
    __event_handle  = JS_UNDEFINED ;
    __event_handle_ctx = NULL ;
}
