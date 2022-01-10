#include "be_gl.h"
#include "be_gl_js.h"
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
    double rad1 = be_gl_coords_to_rad( _x, _y ) ;
    double rad2 = be_gl_coords_to_rad( x2 - arc->cx, y2 - arc->cy ) ;
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
    if( be_gl_is_on_arc(HalfPI, arc->rad1, arc->rad2) ) {
        arc->y_upper = arc->cy + radius + 0.5 ;
        arc->line_like = false ;
    }
    else {
        arc->y_upper = MAX( arc->segment.y1, arc->segment.y2 ) ;
    }

    // 拱顶在圆弧上
    if( be_gl_is_on_arc(PIHalf, arc->rad1, arc->rad2) ) {
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

static JSValue js_gl_path_draw(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv, int draw_method) {
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
    
    if(draw_method==1) {
        be_gl_path_fill(path, x, y, 0, color, &loc_clip, draw_buf->buf_act, draw_buf->area.x2-draw_buf->area.x1+1) ;
    }
    else if(draw_method==2) {
        be_gl_path_stroke(path, x, y, 0, color, &loc_clip, draw_buf->buf_act, draw_buf->area.x2-draw_buf->area.x1+1) ;
    }

    return JS_UNDEFINED ;
}


static JSValue js_gl_path_test(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    // CHECK_ARGC(1)
    // be_gl_path_t * path = JS_GetOpaque(this_val, js_gl_path_class_id) ;
    // if(!path) {
    //     THROW_EXCEPTION("arcTo must be called with a lv.Path object")
    // }

    // lv_coord_t y = 2 ;
    // int32_t tmp ;
    // ARGV_TO_INT_VAR(0, y, tmp, JS_ToInt32)
    
    // size_t interaction_cnt = 0 ;
    // lv_coord_t interactions[path->segment_cnt*2] ; // 每段路径最多两个交点
    // memset(interactions, 0, sizeof(lv_coord_t)*path->segment_cnt) ;

    // for( be_gl_path_segment_t* seg=path->segments; seg; seg=seg->next) {

    //     if(seg->type==BE_GL_PATH_LINE) {
    //         intersect_line(y, seg, interactions, &interaction_cnt) ;
    //     }
    //     else if(seg->type==BE_GL_PATH_ARC) {
    //     }
    //     else {
    //         printf("unknow path segment type:%d\n", seg->type) ;
    //         continue ;
    //     }
    // }

    // dn(interaction_cnt)
    // for(size_t i=0; i<interaction_cnt; i+=2) {
    //     if(i<interaction_cnt-1){
    //         dn3(i, interactions[i], interactions[i+1])
    //     }
    //     else {
    //         dn2(i, interactions[i])
    //     }
    // }
    // // for(size_t i=0;i<interaction_cnt;i+=2) {
    // //     if(i<interaction_cnt-1){
    // //         dn3(y, interactions[i], interactions[i+1])
    // //     }
    // //     else {
    // //         dn2(y, interactions[i])
    // //     }
    // // }


    return JS_UNDEFINED ;
}

static const JSCFunctionListEntry js_gl_path_proto_funcs[] = {
    JS_CFUNC_DEF("addLine", 4, js_gl_path_add_line),
    JS_CFUNC_DEF("addArc", 6, js_gl_path_add_arc),
    JS_CFUNC_MAGIC_DEF("fill", 5, js_gl_path_draw, 1),
    JS_CFUNC_MAGIC_DEF("stroke", 5, js_gl_path_draw, 2),
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
