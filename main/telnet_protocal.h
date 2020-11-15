#ifndef __TELNET_PROTOCAL
#define __TELNET_PROTOCAL

#include <stdbool.h>
#include <stdint.h>

#define PKGLEN_WITHOUT_DATA         7
#define PKG_HEADER_LEN              6
#define PKGLEN_MAX_DATA             248
#define BUFFLEN_RECEIVE             260     // > PKGLEN_MAX_DATA + PKGLEN_WITHOUT_DATA

// 05 18
#define PKG_HEAD1                   5
#define PKG_HEAD2                   18

#define PKGPOS_HEAD1                0
#define PKGPOS_HEAD2                1
#define PKGPOS_ID                   2
#define PKGPOS_REMAIN               3
#define PKGPOS_CMD                  4
#define PKGPOS_DATALEN              5

#define CMD_RUN	                	1		// 执行js代码，无返回值
#define CMD_CALL	                2		// 执行js代码，有返回值
#define CMD_CALL_ASYNC              3		// 执行js代码，有异步返回值
#define CMD_RSPN	                4		// CMD_CALL 的返回
#define CMD_EXCEPTION	            5		// CMD_RUN/CMD_CALL 的异常
#define CMD_CALLBACK                6		// 执行本地js代码(远程发起)
#define CMD_OUTPUT	                7		// 远程输出
#define CMD_DATA	                8		// 数据包(用于 CMD_FILE_PULL_REQ 的回应)

#define CMD_FILE_PUSH_REQ			10		// 发送文件. DATA区格式：路径(0结尾字符串) + 文件内容
#define CMD_FILE_APPEND_REQ			11		// 发送文件后续内容. DATA区格式和 CMD_FILE_APPEND 相同
#define CMD_FILE_PULL_REQ			12		// 拉取文件. DATA区格式：路径(0结尾字符串) + Offset(uint32) + MaxSize(uint16)
#define CMD_RESET			        30		// 重置命令. DATA区格式：boot_level (uint8)
#define CMD_READY           		31		// 系统准备就绪事件


typedef void (*PkgCmdProcess)(uint8_t pkgId, uint8_t remain, uint8_t pkgCmd, const uint8_t * pkgDat, uint8_t pkgDatLen, const void * ctx);
extern PkgCmdProcess telnet_prot_on_package ;

struct telnet_prot_buffer {
    uint8_t bytes[BUFFLEN_RECEIVE] ;
    uint16_t writepos ;
} ;


void telnet_prot_push_char(struct telnet_prot_buffer * buff, uint8_t byte, const void * ctx) ;
void telnet_prot_push_bytes(struct telnet_prot_buffer * buff, uint8_t * dat, uint8_t length, const void * ctx) ;
uint8_t telnet_prot_pack(uint8_t * pkg, uint8_t pkgId, uint8_t remain, uint8_t cmd, uint8_t * dat, uint8_t datalen) ;


#endif