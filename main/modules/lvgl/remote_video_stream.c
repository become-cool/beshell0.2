#include "remote_video_stream.h"
#include "telnet_ws.h"
#include "utils.h"
#include "telnet_protocal_0519.h"



TaskHandle_t task_poll_rvs_handle ;

/**
 * 返回小于0 表示遇到错误
 * 返回读到的字节数
 */
inline static int recvn(int sock, int n, uint8_t * buff) {
    int left = n ;
    for(int readed=0; left>0; left-=readed) {
        readed = recv(sock, buff, left, 0) ;
        if(readed<0) {
            return readed ;
        }
        buff+= readed ;
    }
    return n ;
}

/**
 * 返回小于0 表示遇到错误
 * 遇到指定字节后返回 0
 */
inline static int wait_byte(int sock, uint8_t byte) {
    uint8_t buff ;
    int readed = recvn(sock,1,&buff) ;
    if(readed==1 && byte==buff) {
        return 0 ;
    }
    else {
        return readed ;
    }
}

/**
 * 返回小于0 表示遇到错误
 * 遇到 5 + 19 返回 0
 */
inline static int wait_mark2(int sock) {

    uint8_t buffer[2] ;
    if(recvn(sock, 1, buffer)<0) {
        return -1 ;
    }

    while(1) {
        if(recvn(sock, 1, buffer+1)<0) {
            return -1 ;
        }

        if( buffer[0]==5 && buffer[1]==19 ) {
            return 0 ;
        }

        buffer[0] = buffer[1] ;
    }
}

inline static int recv_pkg_len(int sock) {

    uint8_t byte ;
    int datalen = 0 ;

    #define READ_DATALEN_BYTE(b)                            \
        if(recvn(sock, 1, &byte)<0) {                       \
            return -1 ;                                     \
        }                                                   \
        datalen|= (byte & 0x7F) << (7*b) ;                  \
        if(byte < 0x80) {                                   \
            return datalen ;                                \
        }

    READ_DATALEN_BYTE(0)
    READ_DATALEN_BYTE(1)
    READ_DATALEN_BYTE(2)

    if(recvn(sock, 1, &byte)<0) {
        return -1 ;
    }
    return datalen | ((byte & 0xFF) << 21) ;
}

static int wait_pkg_header(int sock, uint8_t * verifysum) {

    uint8_t buffer[PKG0519_HEADERLEN_WITHOUT_DATALEN - 2] ;

    if(wait_mark2(sock)<0) {
        return -1 ;
    }
    if( recvn(sock, PKG0519_HEADERLEN_WITHOUT_DATALEN - 2, buffer)<0 ){
        return -1 ;
    }
    if(verifysum) {
        *verifysum = buffer[2] ;
    }

    return recv_pkg_len(sock) ;
}

void task_poll_rvs(void * data) {

    telnet_ws_projection_sessn_init() ;

    // #define BUFSIZE 128
    // char buffer[BUFSIZE];

    char * host_ip = "192.168.4.1" ;
    int port = 8018 ;
    int addr_family = 0;
    int ip_protocol = 0;


    int readed = 0 ;
    uint8_t * readptr = NULL ;
    uint8_t vsum = 0 ;
    uint8_t vsum2 = 0 ;
    int pkglen = 0 ;
    uint8_t * body = NULL ;

    while (1) {
        
        struct sockaddr_in dest_addr;
        dest_addr.sin_addr.s_addr = inet_addr(host_ip);
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(port);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
        
        int sock =  socket(addr_family, SOCK_STREAM, ip_protocol);
        if (sock < 0) {
            printf("Unable to create socket: errno %d\n", errno);
            break;
        }
        printf("Socket created, connecting to %s:%d\n", host_ip, port);

        int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in6));
        if (err != 0) {
            printf("Socket unable to connect: errno %d\n", errno);
            break;
        }
        printf("Successfully connected\n");

        while (1) {

            pkglen = wait_pkg_header(sock, &vsum) ;
            if(pkglen<0) {
                break ;
            }

            HMALLOC(body, pkglen) ;   // buffer 由 projection task 释放
            if(!body) {
                printf("out of memory\n") ;
                break ;
            }
            // printf("body:%p\n",body) ;
            
            if(recvn(sock, pkglen, body)<0) {
                break ;
            }

            // vsum2 = telnet_prot0519_checksum(body, pkglen) ;
            // // dn4(body[0],body[1], vsum, vsum2)
            // if(vsum!=vsum2) {
            //     free(body) ;
            //     vTaskDelay(1);
            //     printf("verify sum incorrect\n") ;
            //     continue;
            // }

            post_jpg_rect(0,0,body,pkglen) ;

            vTaskDelay(1);
        }

        if (sock != -1) {
            printf("Shutting down socket and restarting...\n");
            shutdown(sock, 0);
            close(sock);
        }
    }

    telnet_ws_projection_sess_release() ;

    vTaskDelete(NULL);
}

static JSValue js_poll_remote_video_stream(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)

    if(task_poll_rvs_handle) {
        THROW_EXCEPTION("remote video stream has already started")
    }

    xTaskCreatePinnedToCore(&task_poll_rvs, "task_poll_rvs", 1024*3, NULL, 5, &task_poll_rvs_handle, 0) ;

    return JS_UNDEFINED ;
}





void be_remote_video_stream_init() {
}

void be_remote_video_stream_require(JSContext *ctx, JSValue jspkg) {
    JS_SetPropertyStr(ctx, jspkg, "pollRemoteVideoStream", JS_NewCFunction(ctx, js_poll_remote_video_stream, "pollRemoteVideoStream", 1));
}


void be_remote_video_stream_reset(JSContext *ctx) {
}

