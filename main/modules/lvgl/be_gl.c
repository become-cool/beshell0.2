#include "be_gl.h"
#include "lvgl.h"
#include "lv_conf.h"
#include "utils.h"
#include "cutils.h"
#include "widgets.h"
#include "widgets_gen.h"
#include "be_lv_struct_wrapper.h"
#include <math.h>


#define JSARRAY_TO_POINT(arr, jsvar, cvar, tmp)     \
    jsvar = JS_GetPropertyUint32(ctx, arr, 0) ;     \
    if(JS_ToInt32(ctx, &tmp, jsvar)!=0) {           \
        THROW_EXCEPTION("x is not a valid number")  \
    }                                               \
    cvar.x = tmp ;                                  \
    JS_FreeValue(ctx, jsvar) ;                      \
    jsvar = JS_GetPropertyUint32(ctx, arr, 1) ;     \
    if(JS_ToInt32(ctx, &tmp, jsvar)!=0) {           \
        THROW_EXCEPTION("y is not a valid number")  \
    }                                               \
    cvar.y = tmp ;                                  \
    JS_FreeValue(ctx, jsvar) ;


static JSValue js_lv_draw_line(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(4)
    if(!JS_IsArray(ctx, argv[0])) {
        THROW_EXCEPTION("arg start must be a array")
    }
    if(!JS_IsArray(ctx, argv[1])) {
        THROW_EXCEPTION("arg end must be a array")
    }
    
    JSValue val ;
    lv_point_t start, end ;
    int32_t tmp ;

    JSARRAY_TO_POINT(argv[0], val, start, tmp)
    JSARRAY_TO_POINT(argv[1], val, end, tmp)

    lv_area_t * clip = JS_GetOpaque(argv[2],js_lv_area_class_id) ;
    if(!clip) {
        THROW_EXCEPTION("arg rect must a lv.Area object")
    }
    lv_draw_line_dsc_t * dsc = JS_GetOpaque(argv[3],js_lv_draw_line_dsc_class_id) ;
    if(!dsc) {
        THROW_EXCEPTION("arg dsc must a lv.DrawRectDsc object")
    }
    lv_draw_line(&start, &end, clip, dsc) ;
    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(3)
    lv_area_t * rect = JS_GetOpaque(argv[0],js_lv_area_class_id) ;
    if(!rect) {
        THROW_EXCEPTION("arg rect must a lv.Area object")
    }
    lv_area_t * clip = JS_GetOpaque(argv[1],js_lv_area_class_id) ;
    if(!clip) {
        THROW_EXCEPTION("arg clip must a lv.Area object")
    }
    lv_draw_rect_dsc_t * dsc = JS_GetOpaque(argv[2],js_lv_draw_rect_dsc_class_id) ;
    if(!dsc) {
        THROW_EXCEPTION("arg dsc must a lv.DrawRectDsc object")
    }
    lv_draw_rect(rect, clip, dsc) ;
    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_arc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(7)
    ARGV_TO_INT16(0, x)
    ARGV_TO_INT16(1, y)
    ARGV_TO_UINT16(2, radius)
    ARGV_TO_UINT16(3, startAngle)
    ARGV_TO_UINT16(4, endAngle)
    lv_area_t * clip = JS_GetOpaque(argv[5],js_lv_area_class_id) ;
    if(!clip) {
        THROW_EXCEPTION("arg rect must a lv.Area object")
    }
    lv_draw_rect_dsc_t * dsc = JS_GetOpaque(argv[6],js_lv_draw_arc_dsc_class_id) ;
    if(!dsc) {
        THROW_EXCEPTION("arg dsc must a lv.DrawRectDsc object")
    }
    lv_draw_arc(x, y, radius, startAngle, endAngle, clip, dsc) ;
    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_polygon(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(3)

    if(!JS_IsArray(ctx, argv[0])) {
        THROW_EXCEPTION("arg points must be a array")
    }
    JSValue jslen = JS_GetPropertyStr(ctx, argv[0], "length") ;
    uint32_t plen ;
    if(0!=JS_ToUint32(ctx, &plen, jslen)) {
        JS_FreeValue(ctx,jslen);
        THROW_EXCEPTION("arg points is not a valid array")
    }
    JS_FreeValue(ctx,jslen);
    lv_point_t * points = malloc(plen * sizeof(lv_point_t));
    if(!points){
        THROW_EXCEPTION("out of memor?")
    }
    JSValue val ;
    int32_t tmp ;
    for(int i=0; i<plen; i++) {
        JSValue jspoint = JS_GetPropertyUint32(ctx, argv[0], i) ;
        JSARRAY_TO_POINT(jspoint, val, points[i], tmp)
        JS_FreeValue(ctx,jspoint);
    }

    lv_area_t * clip = JS_GetOpaque(argv[1],js_lv_area_class_id) ;
    if(!clip) {
        THROW_EXCEPTION("arg clip must a lv.Area object")
    }
    lv_draw_rect_dsc_t * dsc = JS_GetOpaque(argv[2],js_lv_draw_rect_dsc_class_id) ;
    if(!dsc) {
        THROW_EXCEPTION("arg dsc must a lv.DrawRectDsc object")
    }

    lv_draw_polygon(points, plen, clip, dsc) ;

    free(points) ;
    return JS_UNDEFINED ;
}






/**
 * @TODO 使用有序边表法
 */

#define MAX(a,b) ((a>b)? a: b)
#define MIN(a,b) ((a<b)? a: b)

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

static double coords_to_rad(lv_coord_t x, lv_coord_t y) {
    double rad = atan( y/(double)x ) ;
    // printf("x=%d, y=%d, rad=%.3f\n", x, y, rad) ;
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

static bool is_on_arc(double rad_test, double rad_start, double rad_end) {
    if( rad_start < rad_end ) {
        return rad_start < rad_test && rad_end > rad_test ;
    }
    else {
        return rad_start < rad_test || rad_end > rad_test ;
    }
}

typedef struct {
    be_gl_path_segment_t * segments ;
    lv_coord_t width ;
    lv_coord_t height ;
    size_t segment_cnt ;
} be_gl_path_t ;


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
    if(is_on_arc(t1, arc->rad1, arc->rad2)) {
        insert_interection(arc->cx + x, out_array, out_cnt) ;
    }
    if(is_on_arc(t2, arc->rad1, arc->rad2)) {
        insert_interection(arc->cx - x, out_array, out_cnt) ;
    }
}

inline static void draw_line(lv_coord_t x1, lv_coord_t x2, lv_coord_t ox, lv_coord_t oy, lv_area_t * clip, lv_color_t color, uint16_t * line_buff) {
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

inline static void draw_pixel(lv_coord_t x, lv_coord_t y, lv_coord_t ox, lv_coord_t oy, lv_area_t * clip, lv_color_t color, uint16_t * buff, lv_coord_t buff_width) {
    x+= ox;
    y+= oy;
    if( x<clip->x1 || x>clip->x2 || y<clip->y1 || y>clip->y2 ) {
        return ;
    }
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
                        draw_line(seg->x1, seg->x2, ox, oy, clip, color, buff+y*buff_width) ;
                    }
                    else {
                        draw_line(seg->x2, seg->x1, ox, oy, clip, color, buff+y*buff_width) ;
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
                draw_line(interactions[i], interactions[i+1], ox, oy, clip, color, buff+y*buff_width) ;
            }
            else {
                draw_line(interactions[i], interactions[i], ox, oy, clip, color, buff+y*buff_width) ;
            }
        }
    }
}

static JSClassID js_gl_path_class_id ;
static JSValue js_gl_path_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    be_gl_path_t * path = malloc(sizeof(be_gl_path_t)) ;
    memset(path, 0, sizeof(be_gl_path_t)) ;

    JSValue jsobj = JS_NewObjectClass(ctx, js_gl_path_class_id) ;
    JS_SetOpaque(jsobj, path) ;

    return jsobj ;
}
static void js_gl_path_finalizer(JSRuntime *rt, JSValue this_val){
    // printf("js_gl_path_finalizer()\n") ;
    be_gl_path_t * path = JS_GetOpaque(this_val, js_gl_path_class_id) ;
    be_gl_path_segment_t * seg = path->segments;
    while(seg) {
        be_gl_path_line_t * tmp = seg ;
        seg = seg->next ;
        free(tmp) ;
    }
    free(path) ;
}
static JSClassDef js_gl_path_class = {
    "lv.Path",
    .finalizer = js_gl_path_finalizer,
} ;

#define INCREASE_NUM(var, value) if(value>var) {var=value;}
static JSValue js_gl_path_add_line(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(4)

    be_gl_path_t * path = JS_GetOpaque(this_val, js_gl_path_class_id) ;
    if(!path) {
        THROW_EXCEPTION("arcTo must be called with a lv.Path object")
    }
    
    be_gl_path_line_t * line = malloc(sizeof(be_gl_path_line_t)) ;
    memset(line, 0, sizeof(be_gl_path_line_t)) ;
    line->segment.type = BE_GL_PATH_LINE ;

    if(path->segments){
        be_gl_path_segment_t * seg = path->segments ;
        for(; seg->next; seg=seg->next) {}
        seg->next = line ;
    }
    else {
        path->segments = line ;
    }

    ARGV_TO_INT32(0, x1)
    ARGV_TO_INT32(1, y1)
    ARGV_TO_INT32(2, x2)
    ARGV_TO_INT32(3, y2)

    if(y1<y2) {
        line->segment.x1 = x1 ;
        line->segment.y1 = y1 ;
        line->segment.x2 = x2 ;
        line->segment.y2 = y2 ;
    }
    else {
        line->segment.x1 = x2 ;
        line->segment.y1 = y2 ;
        line->segment.x2 = x1 ;
        line->segment.y2 = y1 ;
    }


    line->k = (float)(line->segment.x2-line->segment.x1) / (float)(line->segment.y2-line->segment.y1) ;
    // printf("k=%.3f\n", line->k) ;

    INCREASE_NUM(path->width, line->segment.x1)
    INCREASE_NUM(path->width, line->segment.x2)
    INCREASE_NUM(path->height, line->segment.y1)
    INCREASE_NUM(path->height, line->segment.y2)

    path->segment_cnt ++ ;

    return JS_UNDEFINED ;
}

static JSValue js_gl_path_add_arc(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(6)
    
    be_gl_path_t * path = JS_GetOpaque(this_val, js_gl_path_class_id) ;
    if(!path) {
        THROW_EXCEPTION("arcTo must be called with a lv.Path object")
    }

    be_gl_path_arc_t * arc = malloc(sizeof(be_gl_path_arc_t)) ;
    memset(arc, 0, sizeof(be_gl_path_arc_t)) ;
    arc->segment.type = BE_GL_PATH_ARC ;

    if(path->segments) {
        be_gl_path_segment_t * seg=path->segments ;
        for(; seg->next; seg=seg->next) {}
        seg->next = arc ;
    }
    else {
        path->segments = arc ;
    }

    ARGV_TO_INT32(0, x1)
    ARGV_TO_INT32(1, y1)
    ARGV_TO_INT32(2, x2)
    ARGV_TO_INT32(3, y2)

    int32_t val ;
    ARGV_TO_INT_VAR(4, arc->cx, val, JS_ToInt32)
    ARGV_TO_INT_VAR(5, arc->cy, val, JS_ToInt32)

    lv_coord_t _x = x1 - arc->cx ;
    lv_coord_t _y = y1 - arc->cy ;
    double rad1 = coords_to_rad( _x, _y ) ;
    double rad2 = coords_to_rad( x2 - arc->cx, y2 - arc->cy ) ;
    double radius = sqrt(_x*_x + _y*_y) ;

    arc->segment.x1 = x1 ;
    arc->segment.y1 = y1 ;
    arc->segment.x2 = x2 ;
    arc->segment.y2 = y2 ;
    arc->rad1 = rad1 ;
    arc->rad2 = rad2 ;
    arc->line_like = true ;
    arc->radius = radius + 0.5 ;
    
    // 拱底在圆弧上
    if( is_on_arc(HalfPI, arc->rad1, arc->rad2) ) {
        arc->y_upper = arc->cy + radius + 0.5 ;
        arc->line_like = false ;
    }
    else {
        arc->y_upper = MAX( arc->segment.y1, arc->segment.y2 ) ;
    }

    // 拱顶在圆弧上
    if( is_on_arc(PIHalf, arc->rad1, arc->rad2) ) {
        arc->y_lower = arc->cy - radius + 0.5 ;
        arc->line_like = false ;
    }
    else {
        arc->y_lower = MIN( arc->segment.y1, arc->segment.y2 ) ;
    }

    // dn2(arc->y_upper, arc->y_lower)

    
    INCREASE_NUM(path->width, arc->segment.x1)
    INCREASE_NUM(path->width, arc->segment.x2)
    INCREASE_NUM(path->height, arc->segment.y1)
    INCREASE_NUM(path->height, arc->segment.y2)


    path->segment_cnt ++ ;

    return JS_UNDEFINED ;
}

static JSValue js_gl_path_fill(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(5)
    lv_coord_t x, y ;
    int32_t tmp ;
    ARGV_TO_INT_VAR(0, x, tmp, JS_ToInt32)
    ARGV_TO_INT_VAR(1, y, tmp, JS_ToInt32)
    ARGV_TO_DOUBLE(2, zoom)

    lv_color_t color ;
    uint32_t tmp2 ;
    ARGV_TO_INT_VAR(3, color.full, tmp2, JS_ToUint32)

    lv_area_t * clip = JS_GetOpaque(argv[4],js_lv_area_class_id) ;
    if(!clip) {
        THROW_EXCEPTION("arg rect must a lv.Area object")
    }

    lv_disp_draw_buf_t * draw_buf = lv_disp_get_draw_buf(_lv_refr_get_disp_refreshing());
    
    be_gl_path_t * path = JS_GetOpaque(this_val, js_gl_path_class_id) ;
    if(!path) {
        THROW_EXCEPTION("arcTo must be called with a lv.Path object")
    }
    // dn2(path->width, path->height) ;

    x-= draw_buf->area.x1 ;
    y-= draw_buf->area.y1 ;

    lv_area_t loc_clip ;
    loc_clip.x1 = clip->x1 - draw_buf->area.x1 ;
    loc_clip.y1 = clip->y1 - draw_buf->area.y1 ;
    loc_clip.x2 = clip->x2 - draw_buf->area.x1 ;
    loc_clip.y2 = clip->y2 - draw_buf->area.y1 ;
    
    be_gl_path_fill(path, x, y, 0, color, &loc_clip, draw_buf->buf_act, draw_buf->area.x2-draw_buf->area.x1+1) ;

    return JS_UNDEFINED ;
}



static JSValue js_gl_path_test(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    be_gl_path_t * path = JS_GetOpaque(this_val, js_gl_path_class_id) ;
    if(!path) {
        THROW_EXCEPTION("arcTo must be called with a lv.Path object")
    }

    lv_coord_t y = 2 ;
    int32_t tmp ;
    ARGV_TO_INT_VAR(0, y, tmp, JS_ToInt32)
    
    size_t interaction_cnt = 0 ;
    lv_coord_t interactions[path->segment_cnt*2] ; // 每段路径最多两个交点
    memset(interactions, 0, sizeof(lv_coord_t)*path->segment_cnt) ;

    for( be_gl_path_segment_t* seg=path->segments; seg; seg=seg->next) {

        if(seg->type==BE_GL_PATH_LINE) {
            intersect_line(y, seg, interactions, &interaction_cnt) ;
        }
        else if(seg->type==BE_GL_PATH_ARC) {
        }
        else {
            printf("unknow path segment type:%d\n", seg->type) ;
            continue ;
        }
    }

    dn(interaction_cnt)
    for(size_t i=0; i<interaction_cnt; i+=2) {
        if(i<interaction_cnt-1){
            dn3(i, interactions[i], interactions[i+1])
        }
        else {
            dn2(i, interactions[i])
        }
    }
    // for(size_t i=0;i<interaction_cnt;i+=2) {
    //     if(i<interaction_cnt-1){
    //         dn3(y, interactions[i], interactions[i+1])
    //     }
    //     else {
    //         dn2(y, interactions[i])
    //     }
    // }


    return JS_UNDEFINED ;
}

static const JSCFunctionListEntry js_gl_path_proto_funcs[] = {
    JS_CFUNC_DEF("addLine", 0, js_gl_path_add_line),
    JS_CFUNC_DEF("addArc", 0, js_gl_path_add_arc),
    JS_CFUNC_DEF("fill", 0, js_gl_path_fill),
    JS_CFUNC_DEF("test", 0, js_gl_path_test),
} ;
void be_gl_init() {
    JS_NewClassID(&js_gl_path_class_id);   
}

void be_gl_require(JSContext *ctx, JSValue lvgl) {

    JS_SetPropertyStr(ctx, lvgl, "drawRect", JS_NewCFunction(ctx, js_lv_draw_rect, "drawRect", 1));
    JS_SetPropertyStr(ctx, lvgl, "drawPolygon", JS_NewCFunction(ctx, js_lv_draw_polygon, "drawPolygon", 1));
    JS_SetPropertyStr(ctx, lvgl, "drawArc", JS_NewCFunction(ctx, js_lv_draw_arc, "drawArc", 1));
    JS_SetPropertyStr(ctx, lvgl, "drawLine", JS_NewCFunction(ctx, js_lv_draw_line, "drawLine", 1));
    
    JS_SetPropertyStr(ctx, lvgl, "fillPath", JS_NewCFunction(ctx, js_lv_draw_line, "fillPath", 1));

    QJS_DEF_CLASS(gl_path, "Path", "lv.Path", JS_UNDEFINED, lvgl)

}
