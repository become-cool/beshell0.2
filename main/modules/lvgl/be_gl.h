
#ifndef __H_BE_GL__
#define __H_BE_GL__

#include "lvgl.h"


#define MAX(a,b) ((a>b)? a: b)
#define MIN(a,b) ((a<b)? a: b)
#define ABS(x)  ((x) > 0 ? (x) : -(x))

#define HalfPI 1.5707963267948966
#define PI 3.141592653589793
#define PIHalf 4.71238898038469
#define DoublePI 6.283185307179586

typedef enum {
    BE_GL_PATH_LINE = 0 ,
    BE_GL_PATH_ARC = 1 ,
} be_gl_path_segment_type_t ;

typedef struct _be_gl_path_segment {
    be_gl_path_segment_type_t type ;
    // 添加时确保 y1 < y2
    lv_coord_t x1 ;
    lv_coord_t y1 ;
    lv_coord_t x2 ;
    lv_coord_t y2 ;     
    struct _be_gl_path_segment * next ;

} be_gl_path_segment_t ;



typedef struct {
    be_gl_path_segment_t segment ;
    float k ; // x=(y-b)*k
} be_gl_path_line_t ;


typedef struct {

    be_gl_path_segment_t segment ;
    lv_coord_t y_lower ;
    lv_coord_t y_upper ;
    lv_coord_t cx ;
    lv_coord_t cy ;
    double rad1 ; 
    double rad2 ;
    lv_coord_t radius ;
    bool line_like ;

} be_gl_path_arc_t ;


typedef struct {
    be_gl_path_segment_t * segments ;
    lv_coord_t width ;
    lv_coord_t height ;
    size_t segment_cnt ;
} be_gl_path_t ;


bool be_gl_is_on_arc(double rad_test, double rad_start, double rad_end) ;
double be_gl_coords_to_rad(lv_coord_t x, lv_coord_t y) ;

void be_gl_path_fill(be_gl_path_t * path, lv_coord_t ox, lv_coord_t oy, double zoom, lv_color_t color, lv_area_t * clip, uint16_t * buff, lv_coord_t buff_width) ;
void be_gl_path_stroke(be_gl_path_t * path, lv_coord_t ox, lv_coord_t oy, double zoom, lv_color_t color, lv_area_t * clip, uint16_t * buff, lv_coord_t buff_width) ;

#endif