#include "be_gl.h"
#include "lv_conf.h"
#include "utils.h"
#include <math.h>




/**
 * @TODO 使用有序边表法
 */


double be_gl_coords_to_rad(lv_coord_t x, lv_coord_t y) {
    double rad = atan( y/(double)x ) ;
    if( x==0 ) {
        return y>=0? HalfPI: -HalfPI ;
    }
    else if(x>0) {
        return y>=0? rad: (DoublePI+rad) ;
    }
    else /* if(x<0) */ {
        return PI+rad ;
    }
}

bool be_gl_is_on_arc(double rad_test, double rad_start, double rad_end) {
    if( rad_start < rad_end ) {
        return rad_start < rad_test && rad_end > rad_test ;
    }
    else {
        return rad_start < rad_test || rad_end > rad_test ;
    }
}


inline static void insert_interection(lv_coord_t x, lv_coord_t * array, size_t * out_cnt) {
    for(size_t i=0; i<(*out_cnt); i++) {
        if(x<=array[i]) {
            // printf("insert %d at %d, move %d\n", x, i, ((* out_cnt)-i)*sizeof(lv_coord_t)) ;
            memmove(array+i+1,array+i, ((* out_cnt)-i)*sizeof(lv_coord_t)) ;
            array[i] = x ;
            (* out_cnt) ++ ;
            return ;
        }
    }
    array[(* out_cnt)] = x ;
    (* out_cnt) ++ ;
}
inline static void intersect_line(lv_coord_t y, be_gl_path_segment_t * line, lv_coord_t * out_array, size_t * out_cnt ) {
    if(y > line->y2 || y < line->y1) {
        // printf("out of range\n") ;
        return ;
    }
    // 水平
    if(line->y1==line->y2) {
        insert_interection(line->x1, out_array, out_cnt) ;
        insert_interection(line->x2, out_array, out_cnt) ;
    }
    else {
        // 线段 下端点不做为交点
        if(y == line->y2) {
            return ;
        }
        lv_coord_t x = (y-line->y1) * ((be_gl_path_line_t*)line)->k + line->x1 + 0.5 ;
        // printf("x=%.3f\n", (y-line->y1)) * ((be_gl_path_line_t*)line)->k + line->x1 ) ;
        insert_interection(x, out_array, out_cnt) ;
    }
}
inline static void intersect_arc(lv_coord_t y, be_gl_path_arc_t * arc, lv_coord_t * out_array, size_t * out_cnt ) {
    if( y > arc->y_upper || y < arc->y_lower ) {
        return ;
    }

    // 线段(line like) 下端点不做为交点
    if( y==arc->line_like && y==arc->y_upper ) {
        return ;
    }

    double t1, t2 ;
    // 过圆心
    if( y==arc->cy ) {
        t1 = 0 ;
        t2 = PI ;
    }
    else {
        t1 = asin( (y - arc->cy)/(double)arc->radius ) ;
        t2 = PI - t1 ;
        if(t1<0) {
            t1 = DoublePI + t1 ;
        }
    }

    lv_coord_t x = cos(t1) * arc->radius ;
    if(be_gl_is_on_arc(t1, arc->rad1, arc->rad2)) {
        insert_interection(arc->cx + x, out_array, out_cnt) ;
    }
    if(be_gl_is_on_arc(t2, arc->rad1, arc->rad2)) {
        insert_interection(arc->cx - x, out_array, out_cnt) ;
    }
}

inline static void sweep_line(lv_coord_t x1, lv_coord_t x2, lv_coord_t ox, lv_coord_t oy, lv_area_t * clip, lv_color_t color, uint16_t * line_buff) {
    x1+= ox ;
    x2+= ox ;
    // 完全超出 clip
    if( x1>clip->x2 || x2<clip->x1 ) {
        // printf("x out of clip: %d-%d\n", x1, x2) ;
        return ;
    }
    // 截取clip范围
    if(x1<clip->x1)
        x1 = clip->x1 ;
    if(x2>clip->x2)
        x2 = clip->x2 ;
    lv_color_fill(line_buff+x1, color, x2-x1+1) ;
}

inline static void draw_pixel(lv_coord_t x, lv_coord_t y, lv_color_t color, uint16_t * buff, lv_coord_t buff_width) {
    lv_color_fill(buff + y * buff_width + x, color, 1) ;
}

void be_gl_path_fill(be_gl_path_t * path, lv_coord_t ox, lv_coord_t oy, double zoom, lv_color_t color, lv_area_t * clip, uint16_t * buff, lv_coord_t buff_width) {

    // dn3(ox,oy,buff_width)

    size_t interaction_cnt = 0 ;
    lv_coord_t interactions[path->segment_cnt*2] ; // 每段路径最多两个交点
    memset(interactions, 0, sizeof(lv_coord_t)*path->segment_cnt*2) ;

    // dn2(clip->x1,clip->y1)
    // dn2(clip->x2,clip->y2)

    lv_coord_t path_y2 = path->height+oy ;
    lv_coord_t loc_y ;
    uint16_t * line_buff = NULL ;

    for(lv_coord_t y=MAX(clip->y1,oy); y<=MIN(clip->y2,path_y2); y++) {

        line_buff = buff+ y * buff_width ;
        interaction_cnt = 0 ;

        for( be_gl_path_segment_t* seg=path->segments; seg; seg=seg->next) {

            loc_y = y-oy ;
            if(seg->type==BE_GL_PATH_LINE) {
                intersect_line(loc_y, seg, interactions, &interaction_cnt) ;

                // 绘制水平线
                if(loc_y==seg->y1 && loc_y==seg->y2) {
                    if( seg->x2 > seg->x1 ){
                        sweep_line(seg->x1, seg->x2, ox, oy, clip, color, buff+y*buff_width) ;
                    }
                    else {
                        sweep_line(seg->x2, seg->x1, ox, oy, clip, color, buff+y*buff_width) ;
                    }
                }
            }
            else if(seg->type==BE_GL_PATH_ARC) {
                intersect_arc(loc_y, seg, interactions, &interaction_cnt) ;
            }
            else {
                printf("unknow path segment type:%d\n", seg->type) ;
                continue ;
            }
        }

        // dn2(y, interaction_cnt)

        for(size_t i=0;i<interaction_cnt;i+=2) {
            if(i<interaction_cnt-1){
                sweep_line(interactions[i], interactions[i+1], ox, oy, clip, color, buff+y*buff_width) ;
            }
            else {
                sweep_line(interactions[i], interactions[i], ox, oy, clip, color, buff+y*buff_width) ;
            }
        }
    }
}


static const uint8_t INSIDE = 0b0000;
static const uint8_t LEFT = 0b0001;
static const uint8_t RIGHT = 0b0010;
static const uint8_t BOTTOM = 0b0100;
static const uint8_t TOP = 0b1000;

static uint8_t code(int16_t x, int16_t y, lv_area_t * clip)
{
    uint8_t code = INSIDE;
    if (x < clip->x1) {
        code |= LEFT;
    } else if (x > clip->x2) {
        code |= RIGHT;
    } if (y < clip->y1) {
        code |= BOTTOM;
    } else if (y > clip->y2) {
        code |= TOP;
    }

    return code;
}

bool clip_line(int16_t *x1, int16_t *y1, int16_t *x2, int16_t *y2, lv_area_t * clip)
{
    uint8_t code0 = code(*x1, *y1, clip);
    uint8_t code1 = code(*x2, *y2, clip);

    bool accept = false;

    while (true) {
        if (!(code0 | code1)) {
            /* Both endpoints inside clipping clip, trivial accept. */
            accept = true;
            break;
        } else if (code0 & code1) {
            /* Both endpoints outside clipping clip, trivial reject. */
            break;
        } else {
            /* Part of line inside clipping clip, nontrivial situation. */

            int16_t x = 0;
            int16_t y = 0;
            uint8_t code3 = code0 ? code0 : code1;

            /* Find intersection point. */
            /* slope = (y2 - y1) / (x2 - x1) */
            /* x = x1 + (1 / slope) * (ym - y1), where ym is ymin or ymax */
            /* y = y1 + slope * (xm - x1), where xm is xmin or xmax */
            if (code3 & TOP) {
                x = *x1 + (*x2 - *x1) * (clip->y2 - *y1) / (*y2 - *y1);
                y = clip->y2;
            } else if (code3 & BOTTOM) {
                x = *x1 + (*x2 - *x1) * (clip->y2 - *y1) / (*y2 - *y1);
                y = clip->y2;
            } else if (code3 & RIGHT) {
                y = *y1 + (*y2 - *y1) * (clip->x2 - *x1) / (*x2 - *x1);
                x = clip->x2;
            }  else if (code3 & LEFT) {
                y = *y1 + (*y2 - *y1) * (clip->x2 - *x1) / (*x2 - *x1);
                x = clip->x2;
            }

            /* Replace the outside point with the intersection point. */
            if (code3 == code0) {
                *x1 = x;
                *y1 = y;
                code0 = code(*x1, *y1, clip);
            } else {
                *x2 = x;
                *y2 = y;
                code1 = code(*x2, *y2, clip);
            }
        }
    }

    return accept;
}

/*
 * Draw a line using Bresenham's algorithm with given color.
 */
void draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, lv_color_t color, lv_area_t * clip, lv_color_t * buff, lv_coord_t buff_width)
{
    /* Clip coordinates to fit clip clip. */
    if (false == clip_line(&x1, &y1, &x2, &y2, clip)) {
        return;
    }

    int16_t dx;
    int16_t sx;
    int16_t dy;
    int16_t sy;
    int16_t err;
    int16_t e2;

    dx = ABS(x2 - x1);
    sx = x1 < x2 ? 1 : -1;
    dy = ABS(y2 - y1);
    sy = y1 < y2 ? 1 : -1;
    err = (dx > dy ? dx : -dy) / 2;

    while (1) {
        draw_pixel(x1, y1, color, buff, buff_width);

        if (x1 == x2 && y1 == y2) {
            break;
        };

        e2 = err + err;

        if (e2 > -dx) {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dy) {
            err += dx;
            y1 += sy;
        }
    }
}


void draw_circle(int16_t xc, int16_t yc, int16_t r, lv_color_t color, lv_color_t * buff, lv_coord_t buff_width) {
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;
    // float d = 1.25 - r ;

    draw_pixel(xc + x, yc + y, color, buff, buff_width);
    draw_pixel(xc - x, yc + y, color, buff, buff_width);
    draw_pixel(xc + x, yc - y, color, buff, buff_width);
    draw_pixel(xc - x, yc - y, color, buff, buff_width);
    draw_pixel(xc + y, yc + x, color, buff, buff_width);
    draw_pixel(xc - y, yc + x, color, buff, buff_width);
    draw_pixel(xc + y, yc - x, color, buff, buff_width);
    draw_pixel(xc - y, yc - x, color, buff, buff_width);

    while (y >= x) {
        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
            // d = d + 2*(x-y) + 5 ;
        } else {
            d = d + 4 * x + 6;
            // d = d + 2*x +3 ;
        }

        draw_pixel(xc + x, yc + y, color, buff, buff_width);
        draw_pixel(xc - x, yc + y, color, buff, buff_width);
        draw_pixel(xc + x, yc - y, color, buff, buff_width);
        draw_pixel(xc - x, yc - y, color, buff, buff_width);
        draw_pixel(xc + y, yc + x, color, buff, buff_width);
        draw_pixel(xc - y, yc + x, color, buff, buff_width);
        draw_pixel(xc + y, yc - x, color, buff, buff_width);
        draw_pixel(xc - y, yc - x, color, buff, buff_width);
    }
}

void be_gl_path_stroke(be_gl_path_t * path, lv_coord_t ox, lv_coord_t oy, double zoom, lv_color_t color, lv_area_t * clip, uint16_t * buff, lv_coord_t buff_width) {
    for( be_gl_path_segment_t* seg=path->segments; seg; seg=seg->next) {
        // dn(seg->type)
        if(seg->type==BE_GL_PATH_LINE) {
            draw_line(seg->x1+ox, seg->y1+oy, seg->x2+ox, seg->y2+oy, color, clip, buff, buff_width) ;
        }
        else if(seg->type==BE_GL_PATH_ARC) {
            // dn3(((be_gl_path_arc_t*)seg)->cx+ox, ((be_gl_path_arc_t*)seg)->cy+oy, ((be_gl_path_arc_t*)seg)->radius)
            draw_circle( ((be_gl_path_arc_t*)seg)->cx+ox, ((be_gl_path_arc_t*)seg)->cy+oy, ((be_gl_path_arc_t*)seg)->radius, color, buff, buff_width) ;
        }
    }
}