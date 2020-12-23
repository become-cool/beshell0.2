#include "module_wifi.h"
#include "module_socks.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>
#include "logging.h"
#include "utils.h"
#include <sys/errno.h>

#define UDP_POOR_SIZE 16


struct timeval select_tv = {
    .tv_sec = 0,
    .tv_usec = 1,
} ;


int udp_listen_handles[UDP_POOR_SIZE] = {-1} ;
uint16_t udp_listen_ports[UDP_POOR_SIZE] = {0} ;
uint8_t udp_listen_count = 0 ;
fd_set udp_recv_rfds;

JSValue _js_udp_recv_callback = NULL ;

void socks_udp_loop(JSContext *ctx) {

	FD_ZERO(&udp_recv_rfds);
    int fds = -1 ;

    for(int i=0;i<udp_listen_count;i++){
        if(udp_listen_handles[i]<0) {
            continue ;
        }
        FD_SET(udp_listen_handles[i], &udp_recv_rfds);
        if(udp_listen_handles[i]>fds)
            fds = udp_listen_handles[i] ;
    }
    if(fds<0) {
        return ;
    }
    
    int s = select( fds + 1, &udp_recv_rfds, NULL, NULL, &select_tv);
    // 超时
    if(s==0) {
        return ;
    }

    // 失败
    if(s<0){
        printf("udp recv select() error: %d.\n", errno) ;
        return ;
    }

    char rx_buffer[64] ;
    struct sockaddr_in6 source_addr; // Large enough for both IPv4 or IPv6
    socklen_t source_addrlen = sizeof(source_addr);
    
    for(int i=0;i<udp_listen_count;i++) {

        if(!FD_ISSET(udp_listen_handles[i], &udp_recv_rfds)) {
            continue ;
        }

        uint8_t len = recvfrom(udp_listen_handles[i], rx_buffer, sizeof(rx_buffer), 0, (struct sockaddr *)&source_addr, &source_addrlen);
        if (len < 0) { 
            return;
        }
        // rx_buffer[len] = 0 ;
        // printf("data(%d) %s\n", len, rx_buffer) ;


        if(_js_udp_recv_callback!=NULL) {

            JSValueConst * argv = malloc(sizeof(JSValue)*2) ;
            char * data = malloc(len) ;
            memcpy(data, rx_buffer, len) ;
            argv[0] = JS_NewArrayBuffer(ctx, (uint8_t *)data, len, freeArrayBuffer, NULL, false) ;
            argv[1] = JS_NewUint32(ctx, udp_listen_ports[i]) ;
            JS_Call(ctx, _js_udp_recv_callback, JS_UNDEFINED, 2, argv) ;

            JS_FreeValue(ctx, argv[0]) ;
            free(argv) ;
        }
    }
}


JSValue js_udp_listen(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    if(udp_listen_count>=UDP_POOR_SIZE) {
        THROW_EXCEPTION("reach max udp listener")
    }

    CHECK_ARGC(1)
    ARGV_TO_UINT16(0, port)

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0) {
        return JS_FALSE ;
    }
    
    struct sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(port);

    int err = bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr));
    if (err < 0) {
        close(sock) ;
        printf("count not bind() udp port: %d\n", port) ;
        return JS_FALSE ;
    }

    udp_listen_handles[udp_listen_count] = sock ;
    udp_listen_ports[udp_listen_count] = port ;

    udp_listen_count ++ ;

    return JS_TRUE;
}

/**
 * string|null addr
 * number port
 * string|ArrayBuffer data
 */
JSValue js_udp_broadcase(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    JSValue ret = JS_UNDEFINED ;
    int sock = -1 ;

    CHECK_ARGC(3)
    // ARGV_TO_STRING(0, addr, addrlen)
    ARGV_TO_UINT16(1, port)

    char * data = NULL ;
    size_t datalen = 0 ;
    bool dataIsString = false ;
    if(JS_IsString(argv[2])) {
        data = JS_ToCStringLen(ctx, &datalen, argv[2]) ;
        dataIsString = true ;
    }
    else {
        data = (char *)JS_GetArrayBuffer(ctx, &datalen, argv[2]) ;
    }

    if(!data || datalen==0) {
        return JS_UNDEFINED ;
    }

    struct sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(port);
    
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (sock < 0) {
        JS_ThrowReferenceError(ctx, "Unable to create socket.") ;
        ret = JS_EXCEPTION ;
        goto end ;
    }
    int err = bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr));
    if (err < 0) {
        JS_ThrowReferenceError(ctx, "Socket unable to bind.") ;
        ret = JS_EXCEPTION ;
        goto end ;
    }
    // Assign multicast TTL (set separately from normal interface TTL)
    uint8_t ttl = IP_MULTICAST_TTL;
    setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(uint8_t));
    if (err < 0) {        
        JS_ThrowReferenceError(ctx, "Failed to set IP_MULTICAST_TTL") ;
        ret = JS_EXCEPTION ;
        goto end ;
    }

    esp_netif_ip_info_t ipinfo;
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);

    // STA network
    bzero(&ipinfo, sizeof(esp_netif_ip_info_t));
    esp_netif_get_ip_info(get_netif_sta(), &ipinfo);
    dest_addr.sin_addr.s_addr = ipinfo.ip.addr | (~ipinfo.netmask.addr) ;

    // printf("sta broadcase addr:%d\n", (uint32_t)dest_addr.sin_addr.s_addr);
    sendto(sock, data, datalen, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    
    // AP network
    bzero(&ipinfo, sizeof(esp_netif_ip_info_t));
    esp_netif_get_ip_info(get_netif_ap(), &ipinfo);
    dest_addr.sin_addr.s_addr = ipinfo.ip.addr | (~ipinfo.netmask.addr) ;

    // printf("ap broadcase addr:%d\n", (uint32_t)dest_addr.sin_addr.s_addr);
    sendto(sock, data, datalen, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

end :
    if(dataIsString)
        JS_FreeCString(ctx,data) ;
    if(sock>-1)
        close(sock);
    return ret ;
}

JSValue js_udp_set_recv_callback(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    if(!JS_IsFunction(ctx, argv[0])) {
        THROW_EXCEPTION("Arg not a function")
    }

    if(_js_udp_recv_callback)
        JS_FreeValue(ctx, _js_udp_recv_callback) ;
    _js_udp_recv_callback = JS_DupValue(ctx, argv[0]) ;
    return JS_UNDEFINED;
}

void require_module_socks(JSContext *ctx) {
    
    JSValue global = JS_GetGlobalObject(ctx);
    JSValue socks = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, global, "socks", socks);
    JSValue udp = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, socks, "udp", udp);

    JS_SetPropertyStr(ctx, udp, "broadcase", JS_NewCFunction(ctx, js_udp_broadcase, "broadcase", 1));
    JS_SetPropertyStr(ctx, udp, "setRecvCallback", JS_NewCFunction(ctx, js_udp_set_recv_callback, "setRecvCallback", 1));
    JS_SetPropertyStr(ctx, udp, "listen", JS_NewCFunction(ctx, js_udp_listen, "listen", 1));

    JS_FreeValue(ctx, global);
}




void socks_on_before_reset(JSContext *ctx) {
    
    // 清空 js 回调函数
    if(_js_udp_recv_callback) {
        JS_FreeValue(ctx, _js_udp_recv_callback) ;
        _js_udp_recv_callback = NULL ;
    }
    
    printf("udp sockets : %d\n", udp_listen_count) ;
    
    for(int i=0;i<UDP_POOR_SIZE;i++) {
        if(udp_listen_handles[i]<0)
            continue ;
            
        printf("close udp socket: %d, %d\n", i, udp_listen_handles[i]) ;

        close(udp_listen_handles[i]) ;

        udp_listen_handles[i] = -1 ;
        udp_listen_ports[i] = 0 ;
    }
    udp_listen_count = 0 ;
}