#ifndef __H_PROTOCOL
#define __H_PROTOCOL

#include <stdbool.h>
#include <stdint.h>

#define PKGLEN_WITHOUT_DATA         7
#define PKGLEN_MAX_DATA             248
#define BUFFLEN_RECEIVE             260     // > PKGLEN_MAX_DATA + PKGLEN_WITHOUT_DATA

// 05 18
#define PKG_HEAD1                   5
#define PKG_HEAD2                   18

#define CMD_RUN	                	1		// 执行js代码，无返回值
#define CMD_CALL	                2		// 执行js代码，有返回值
#define CMD_CALL_ASYNC              3		// 执行js代码，有异步返回值
#define CMD_RSPN	                4		// CMD_CALL 的返回
#define CMD_EXCEPTION	            5		// CMD_RUN/CMD_CALL 的异常
#define CMD_CALLBACK                6		// 执行本地js代码(远程发起)
#define CMD_OUTPUT	                7		// 远程输出

#define CMD_FILE_PUSH_REQ			10		// 发送文件. DATA区格式：路径(0结尾字符串) + 文件内容
#define CMD_FILE_APPEND_REQ			11		// 发送文件后续内容. DATA区格式和 CMD_FILE_APPEND 相同
#define CMD_FILE_PULL_REQ			12		// 拉取文件. DATA区格式：路径(0结尾字符串) + Offset(uint32) + MaxSize(uint8)


extern bool bHasUnread ;


typedef void (*PkgCmdRespone)(uint8_t * pkg, uint8_t len) ;
typedef void (*PkgCmdProcess)(uint8_t pkgId, uint8_t remain, uint8_t pkgCmd, uint8_t pkgDatLen, void * ctx);

extern PkgCmdProcess pPkgCmdProcess ;


void ClearBuffer() ;

void WriteCharToBuffer(uint8_t byte)  ;
void WriteToBuffer(uint8_t * dat, uint8_t length)  ;

bool ParsePackage() ;

#define PACKAGE_LOOP(ctx) while(ParsePackage(ctx)) ;

uint8_t ReadData(uint8_t offset) ;
void MemCpy(uint8_t * bufTo, uint8_t offset, uint8_t length) ;

uint8_t Pack(uint8_t * pkg, uint8_t pkgId, uint8_t remain, uint8_t cmd, uint8_t * data, uint8_t datalen) ;

#endif
