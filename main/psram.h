#ifndef _PSRAM_H
#define _PSRAM_H

void psram_init() ;
inline void echo_DMA(const char * where) ;

unsigned int getHeapTotal(void) ;
unsigned int getHeapUsed(void) ;
unsigned int getHeapFree(void) ;

unsigned int getPsramTotal(void) ;
unsigned int getPsramUsed(void) ;
unsigned int getPsramFree(void) ;

unsigned int getDMATotal(void) ;
unsigned int getDMAUsed(void) ;
unsigned int getDMAFree(void) ;
#endif