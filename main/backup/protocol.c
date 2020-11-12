#include "utils.h"
#include "protocol.h"
#include <string.h>

// 循环接收缓存区
uint8_t buffer_receive[BUFFLEN_RECEIVE] ;  

uint16_t idx_write = 0 ;
uint16_t idx_read = 0 ;

bool bHasUnread = false ;

PkgCmdProcess pPkgCmdProcess = 0 ;

// 将一个指针(读或写)向前移动
#define FORWARD(idx, length) \
    idx+= length ;           \
    if(idx>=BUFFLEN_RECEIVE) {       \
        idx-= BUFFLEN_RECEIVE ;      \
    }

// 未读取长度
#define UNREAD_SIZE     ((idx_write>=idx_read)? (idx_write - idx_read): (BUFFLEN_RECEIVE - idx_read + idx_write)) 


void WriteCharToBuffer(uint8_t byte) {
    buffer_receive[idx_write] = byte ;
    FORWARD(idx_write, 1)
    // 覆盖未读区
    if(idx_read==idx_write) {
        FORWARD(idx_read, 1)
    }
    
    bHasUnread = true ;
}

void WriteToBuffer(uint8_t * dat, uint8_t length)  {
    // printf("in buffer %d\n", length) ;
    for(uint8_t i=0; i<length; i++) {
        WriteCharToBuffer(*dat++) ;
    }
}

uint8_t ReadByte(uint8_t offset) {
    uint16_t idx = offset + idx_read ;
    if(idx>=BUFFLEN_RECEIVE) {
        idx = idx % BUFFLEN_RECEIVE ;
    }
    return buffer_receive[idx] ;
}

uint8_t ReadData(uint8_t offset) {
    return ReadByte(5+offset) ;
}

uint8_t Checksum(uint16_t from, uint16_t to) {
    uint8_t sum = 0 ;
    uint16_t i ;
    for(i=from; i<=to; i++) {
        sum^= ReadByte(i) ;
    }
    return sum ;
}

bool ParsePackage(void * ctx) {
    uint8_t pkgdatlen ;
    uint16_t pkglen ;
		
    // 定位到第一个包头（丢弃包头前的无效数据）
    uint16_t unread = UNREAD_SIZE ;
    uint16_t i = 0;
    for(;i<unread-1;i++) {
        if(ReadByte(0)==PKG_HEAD1 && ReadByte(1)==PKG_HEAD2) {
            break ;
        }
        FORWARD(idx_read, 1)
    }
    // 低于最小包长度
    unread = UNREAD_SIZE ;
    if(unread<PKGLEN_WITHOUT_DATA) {
        printf("unread=%d, min=%d\n", unread, PKGLEN_WITHOUT_DATA) ;
        return false ;
    }

    pkgdatlen = ReadByte(5) ;
    pkglen = pkgdatlen + PKGLEN_WITHOUT_DATA ;
    // printf("pkgdatlen=%d, pkglen=%d, unread=%d\n", pkgdatlen, pkglen, unread) ;
    
    // 低于实际包长
    if(unread<pkglen) {
        printf("unread=%d, pkglen=%d\n", unread, pkglen) ;
        return false ;
    }

    // 检查校验位
    if( Checksum(0, pkgdatlen+5) != ReadByte(6+pkgdatlen) ){
        FORWARD(idx_read, 2)    // 跳过当前包头
        return true ;           // 返回 true ，使外层循环重入
    }

    pPkgCmdProcess(ReadByte(2), ReadByte(3), ReadByte(4), pkgdatlen, ctx) ;

    FORWARD(idx_read, pkglen)

    return true ;
}

void ClearBuffer() {
    idx_write = 0 ;
    idx_read = 0 ;
}



uint8_t Pack(uint8_t * pkg, uint8_t pkgId, uint8_t remain, uint8_t cmd, uint8_t * dat, uint8_t datalen) {
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

void MemCpy(uint8_t * bufTo, uint16_t offset, uint16_t length) {
    uint16_t end ;
    offset+= idx_read ;
    if(offset>BUFFLEN_RECEIVE)
        offset-= BUFFLEN_RECEIVE ;
    
    end = offset + length ; // 有效内容不含end
    if(end>BUFFLEN_RECEIVE) {
        uint16_t over = end-BUFFLEN_RECEIVE ;
        memcpy(bufTo, buffer_receive+offset, length-over) ;
        memcpy(bufTo+length-over, buffer_receive, over) ;
    }
    else {
        memcpy(bufTo, buffer_receive+offset, length) ;
    }
}

