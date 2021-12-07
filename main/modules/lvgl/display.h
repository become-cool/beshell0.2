#ifndef _VLGL_JS_DISPLAY
#define _VLGL_JS_DISPLAY

#include "quickjs-libc.h"


#define DMA_BUFF_LEN 320*20*2
#define DMA_BUFF_AUX_SIZE 16        // 附加数据，记录 area(x1,y1,x2,y2)，放在 DMA_BUFF 前面


void vlgl_js_display_init() ;

uint8_t * display_dma_buff() ;

void require_vlgl_js_display(JSContext *ctx, JSValue lvgl) ;

#endif