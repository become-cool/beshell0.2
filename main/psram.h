#ifndef _PSRAM_H
#define _PSRAM_H

void psram_init() ;


unsigned int getHeapSize(void) ;
unsigned int getFreeHeap(void) ;
unsigned int getPsramSize(void) ;
unsigned int getFreePsram(void) ;

#endif