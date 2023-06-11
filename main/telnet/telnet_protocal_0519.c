#include "telnet_protocal_0519.h"
#include "utils.h"
#include <string.h>

// PkgCmdProcess telnet_prot0519_on_package = 0 ;



uint8_t telnet_prot0519_checksum(uint8_t * data, uint32_t len) {
    uint8_t sum = 0 ;
    for(uint16_t i=0; i<len; i++) {
        sum^= data[i] ;
    }
    return sum ;
}


uint8_t telnet_prot0519_pack_header(uint8_t * pkg, uint8_t pkgId, uint8_t remain, uint8_t cmd, uint8_t * dat, uint8_t datalen) {

    pkg[0] = PKG_HEAD1 ;
    pkg[1] = PKG_HEAD2 ;
    pkg[2] = pkgId ;
    pkg[3] = cmd ;


    pkg[4] = datalen ;

    
    uint8_t i ;
    for(i=0;i<datalen;i++) {
        pkg[5+i] = *(dat+i) ;
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


uint8_t telnet_prot0519_pack_data_len(uint32_t len, uint8_t * len_bytes) {
    *(len_bytes+0) = (len>>(7*0)) & 0x7F ;
    *(len_bytes+1) = (len>>(7*1)) & 0x7F ;
    *(len_bytes+2) = (len>>(7*2)) & 0x7F ;
    *(len_bytes+3) = (len>>(7*3)) & 0xFF ;  // 最高位字节可以用满8位

    if( *(len_bytes+3)>0 ) {
        *(len_bytes+0) |= 0x80 ;
        *(len_bytes+1) |= 0x80 ;
        *(len_bytes+2) |= 0x80 ;
        return 4 ;
    }
    else if( *(len_bytes+2)>0 ) {
        *(len_bytes+0) |= 0x80 ;
        *(len_bytes+1) |= 0x80 ;
        return 3 ;
    }
    else if( *(len_bytes+1)>0 ) {
        *(len_bytes+0) |= 0x80 ;
        return 2 ;
    }

    return 1 ;
}