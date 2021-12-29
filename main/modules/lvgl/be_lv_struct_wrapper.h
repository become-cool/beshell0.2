#ifndef __H_STRUCTS__
#define __H_STRUCTS__

#include "quickjs-libc.h"

extern JSClassID js_lv_draw_rect_dsc_class_id ;
extern JSClassID js_lv_draw_arc_dsc_class_id ;
extern JSClassID js_lv_draw_line_dsc_class_id ;
extern JSClassID js_lv_area_class_id ;

void be_lv_structs_init() ;
void be_lv_structs_require(JSContext *ctx, JSValue lvgl) ;
// void be_structs_loop(JSContext *ctx) ;
// void be_structs_reset(JSContext *ctx) ;

#endif