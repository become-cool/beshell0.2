#include "telnet_protocal.h"
#include "utils.h"
#include <string.h>

PkgCmdProcess telnet_prot_on_package = 0 ;


uint8_t checksum(struct telnet_prot_buffer * buff) {
    uint8_t sum = 0 ;
    for(uint16_t i=0; i<PKG_HEADER_LEN + buff->bytes[PKGPOS_DATALEN]; i++) {
        sum^= buff->bytes[i] ;
    }
    return sum ;
}

void telnet_prot_push_char(struct telnet_prot_buffer * buff, uint8_t byte, const void * ctx) {
    // 包头
    if(buff->writepos == PKGPOS_HEAD1) {
        // printf("writepos=%d,byte=%d\n",buff->writepos, byte) ; 
        if(byte==PKG_HEAD1) {
            buff->bytes[buff->writepos ++] = byte ;
        }
        return ;
    }
    else if(buff->writepos == PKGPOS_HEAD2) {
        // printf("writepos=%d,byte=%d\n",buff->writepos, byte) ; 
        if(byte==PKG_HEAD2) {
            buff->bytes[buff->writepos ++] = byte ;
        }
        return ;
    }
    else if( buff->writepos< PKGPOS_DATALEN) {
        buff->bytes[buff->writepos ++] = byte ;
        return ;
    }

    // 数据长度
    else if( buff->writepos == PKGPOS_DATALEN){
        // printf("datalen=%d\n",byte) ;
        // 大于最大data长度, 丢弃
        if(byte>PKGLEN_MAX_DATA) {
            buff->writepos = 0 ;
            return ;
        }
        buff->bytes[buff->writepos ++] = byte ;
        // dn(buff->bytes[PKGPOS_DATALEN])
        return ;
    }

    // 数据区
    else if( buff->writepos < (PKG_HEADER_LEN + buff->bytes[PKGPOS_DATALEN]) ) {
        // printf("%d->%d\n",buff->writepos,byte) ;
        buff->bytes[buff->writepos ++] = byte ;
        return ;
    }

    // 校验位
    else if( buff->writepos == (PKG_HEADER_LEN + buff->bytes[PKGPOS_DATALEN]) ) {
        // 重置
        buff->writepos = 0 ;
        // 完成
        if( checksum(buff)==byte && telnet_prot_on_package!=NULL ) {
            buff->bytes[ PKG_HEADER_LEN + buff->bytes[PKGPOS_DATALEN] ] = 0 ;
            telnet_prot_on_package(
                buff->bytes[PKGPOS_ID] ,
                buff->bytes[PKGPOS_REMAIN] ,
                buff->bytes[PKGPOS_CMD] ,
                buff->bytes + PKG_HEADER_LEN ,
                buff->bytes[PKGPOS_DATALEN] ,
                ctx
            ) ;
        }
        else {
        }
    }

    // 异常,丢弃数据
    else {
        // printf("invalid writepos=%d\n", buff->writepos) ;
        buff->writepos = 0 ;
    }
}

void telnet_prot_push_bytes(struct telnet_prot_buffer * buff, uint8_t * dat, uint8_t length, const void * ctx) {
    for(uint8_t i=0; i<length; i++) {
        telnet_prot_push_char( buff, *(dat++), ctx ) ;
    }
}


uint8_t telnet_prot_pack(uint8_t * pkg, uint8_t pkgId, uint8_t remain, uint8_t cmd, uint8_t * dat, uint8_t datalen) {
    pkg[0] = PKG_HEAD1 ;
    pkg[1] = PKG_HEAD2 ;
    pkg[2] = pkgId ;
    pkg[3] = remain ;
    pkg[4] = cmd ;


    pkg[5] = datalen ;

    
    uint8_t i ;
    for(i=0;i<datalen;i++) {
        pkg[6+i] = *(dat+i) ;
    }
    uint16_t pkglen = PKGLEN_WITHOUT_DATA + datalen ;
    // 算校验和
    uint8_t sum = 0 ;
    for(i=0; i<=pkglen-2; i++) {
        sum^= pkg[i] ;
    }
    pkg[pkglen-1] = sum ;

    return pkglen ;
}