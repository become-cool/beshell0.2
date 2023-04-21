#include "be_lv_struct_wrapper.h"
#include "utils.h"
#include "cutils.h"
#include "lvgl.h"


// AUTO GENERATE CODE START [CLASS DEFINE] --------

// wrap lvgl struct lv_draw_rect_dsc_t
JSClassID js_lv_draw_rect_dsc_class_id ;
static JSValue js_lv_draw_rect_dsc_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    JSValue proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto)) {
        JS_FreeValue(ctx, proto) ;
        return JS_EXCEPTION ;
    }
    JSValue jsobj = JS_NewObjectProtoClass(ctx, proto, js_lv_draw_rect_dsc_class_id) ;
    JS_FreeValue(ctx, proto) ;

    lv_draw_rect_dsc_t * cvar = malloc(sizeof(lv_draw_rect_dsc_t)) ;
    lv_draw_rect_dsc_init(cvar) ;

    JS_SetOpaque(jsobj, cvar) ;
    return jsobj ;
}
static void js_lv_draw_rect_dsc_finalizer(JSRuntime *rt, JSValue this_val){
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(cvar) {
        // printf("free(lv_draw_rect_dsc_t*)\n") ;
        free(cvar) ;
    }
}
static JSClassDef js_lv_draw_rect_dsc_class = {
    "DrawRectDsc",
    .finalizer = js_lv_draw_rect_dsc_finalizer,
} ;
static JSValue js_lv_draw_rect_dsc_set_radius(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setRadius() must be call with a lv.DrawRectDsc object")
    }
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "radius", "setRadius") 
        } 
        cvar->radius = (lv_coord_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_radius(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setRadius() must be call with a lv.Radius object")
    }
    return JS_NewInt32(ctx, (int32_t)cvar->radius) ;
}
static JSValue js_lv_draw_rect_dsc_set_bg_color(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBgColor() must be call with a lv.DrawRectDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "bg_color", "setBgColor") 
        } 
        cvar->bg_color.full = num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_bg_color(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBgColor() must be call with a lv.BgColor object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->bg_color.full) ;
}
static JSValue js_lv_draw_rect_dsc_set_bg_grad_color(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBgGradColor() must be call with a lv.DrawRectDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "bg_grad_color", "setBgGradColor") 
        } 
        cvar->bg_grad_color.full = num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_bg_grad_color(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBgGradColor() must be call with a lv.BgGradColor object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->bg_grad_color.full) ;
}
static JSValue js_lv_draw_rect_dsc_set_bg_main_color_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBgMainColorStop() must be call with a lv.DrawRectDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "bg_main_color_stop", "setBgMainColorStop") 
        } 
        cvar->bg_main_color_stop = (uint8_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_bg_main_color_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBgMainColorStop() must be call with a lv.BgMainColorStop object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->bg_main_color_stop) ;
}
static JSValue js_lv_draw_rect_dsc_set_bg_grad_color_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBgGradColorStop() must be call with a lv.DrawRectDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "bg_grad_color_stop", "setBgGradColorStop") 
        } 
        cvar->bg_grad_color_stop = (uint8_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_bg_grad_color_stop(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBgGradColorStop() must be call with a lv.BgGradColorStop object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->bg_grad_color_stop) ;
}
static JSValue js_lv_draw_rect_dsc_set_bg_opa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBgOpa() must be call with a lv.DrawRectDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "bg_opa", "setBgOpa") 
        } 
        cvar->bg_opa = (lv_opa_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_bg_opa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBgOpa() must be call with a lv.BgOpa object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->bg_opa) ;
}
static JSValue js_lv_draw_rect_dsc_set_bg_img_recolor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBgImgRecolor() must be call with a lv.DrawRectDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "bg_img_recolor", "setBgImgRecolor") 
        } 
        cvar->bg_img_recolor.full = num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_bg_img_recolor(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBgImgRecolor() must be call with a lv.BgImgRecolor object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->bg_img_recolor.full) ;
}
static JSValue js_lv_draw_rect_dsc_set_bg_img_opa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBgImgOpa() must be call with a lv.DrawRectDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "bg_img_opa", "setBgImgOpa") 
        } 
        cvar->bg_img_opa = (lv_opa_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_bg_img_opa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBgImgOpa() must be call with a lv.BgImgOpa object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->bg_img_opa) ;
}
static JSValue js_lv_draw_rect_dsc_set_bg_img_recolor_opa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBgImgRecolorOpa() must be call with a lv.DrawRectDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "bg_img_recolor_opa", "setBgImgRecolorOpa") 
        } 
        cvar->bg_img_recolor_opa = (lv_opa_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_bg_img_recolor_opa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBgImgRecolorOpa() must be call with a lv.BgImgRecolorOpa object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->bg_img_recolor_opa) ;
}
static JSValue js_lv_draw_rect_dsc_set_bg_img_tiled(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBgImgTiled() must be call with a lv.DrawRectDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "bg_img_tiled", "setBgImgTiled") 
        } 
        cvar->bg_img_tiled = (uint8_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_bg_img_tiled(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBgImgTiled() must be call with a lv.BgImgTiled object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->bg_img_tiled) ;
}
static JSValue js_lv_draw_rect_dsc_set_border_color(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBorderColor() must be call with a lv.DrawRectDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "border_color", "setBorderColor") 
        } 
        cvar->border_color.full = num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_border_color(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBorderColor() must be call with a lv.BorderColor object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->border_color.full) ;
}
static JSValue js_lv_draw_rect_dsc_set_border_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBorderWidth() must be call with a lv.DrawRectDsc object")
    }
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "border_width", "setBorderWidth") 
        } 
        cvar->border_width = (lv_coord_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_border_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBorderWidth() must be call with a lv.BorderWidth object")
    }
    return JS_NewInt32(ctx, (int32_t)cvar->border_width) ;
}
static JSValue js_lv_draw_rect_dsc_set_border_opa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBorderOpa() must be call with a lv.DrawRectDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "border_opa", "setBorderOpa") 
        } 
        cvar->border_opa = (lv_opa_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_border_opa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBorderOpa() must be call with a lv.BorderOpa object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->border_opa) ;
}
static JSValue js_lv_draw_rect_dsc_set_border_post(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBorderPost() must be call with a lv.DrawRectDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "border_post", "setBorderPost") 
        } 
        cvar->border_post = (uint8_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_border_post(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBorderPost() must be call with a lv.BorderPost object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->border_post) ;
}
static JSValue js_lv_draw_rect_dsc_set_border_side(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBorderSide() must be call with a lv.DrawRectDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "border_side", "setBorderSide") 
        } 
        cvar->border_side = (lv_border_side_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_border_side(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setBorderSide() must be call with a lv.BorderSide object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->border_side) ;
}
static JSValue js_lv_draw_rect_dsc_set_outline_color(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setOutlineColor() must be call with a lv.DrawRectDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "outline_color", "setOutlineColor") 
        } 
        cvar->outline_color.full = num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_outline_color(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setOutlineColor() must be call with a lv.OutlineColor object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->outline_color.full) ;
}
static JSValue js_lv_draw_rect_dsc_set_outline_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setOutlineWidth() must be call with a lv.DrawRectDsc object")
    }
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "outline_width", "setOutlineWidth") 
        } 
        cvar->outline_width = (lv_coord_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_outline_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setOutlineWidth() must be call with a lv.OutlineWidth object")
    }
    return JS_NewInt32(ctx, (int32_t)cvar->outline_width) ;
}
static JSValue js_lv_draw_rect_dsc_set_outline_pad(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setOutlinePad() must be call with a lv.DrawRectDsc object")
    }
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "outline_pad", "setOutlinePad") 
        } 
        cvar->outline_pad = (lv_coord_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_outline_pad(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setOutlinePad() must be call with a lv.OutlinePad object")
    }
    return JS_NewInt32(ctx, (int32_t)cvar->outline_pad) ;
}
static JSValue js_lv_draw_rect_dsc_set_outline_opa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setOutlineOpa() must be call with a lv.DrawRectDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "outline_opa", "setOutlineOpa") 
        } 
        cvar->outline_opa = (lv_opa_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_outline_opa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setOutlineOpa() must be call with a lv.OutlineOpa object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->outline_opa) ;
}
static JSValue js_lv_draw_rect_dsc_set_shadow_color(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setShadowColor() must be call with a lv.DrawRectDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "shadow_color", "setShadowColor") 
        } 
        cvar->shadow_color.full = num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_shadow_color(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setShadowColor() must be call with a lv.ShadowColor object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->shadow_color.full) ;
}
static JSValue js_lv_draw_rect_dsc_set_shadow_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setShadowWidth() must be call with a lv.DrawRectDsc object")
    }
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "shadow_width", "setShadowWidth") 
        } 
        cvar->shadow_width = (lv_coord_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_shadow_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setShadowWidth() must be call with a lv.ShadowWidth object")
    }
    return JS_NewInt32(ctx, (int32_t)cvar->shadow_width) ;
}
static JSValue js_lv_draw_rect_dsc_set_shadow_ofs_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setShadowOfsX() must be call with a lv.DrawRectDsc object")
    }
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "shadow_ofs_x", "setShadowOfsX") 
        } 
        cvar->shadow_ofs_x = (lv_coord_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_shadow_ofs_x(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setShadowOfsX() must be call with a lv.ShadowOfsX object")
    }
    return JS_NewInt32(ctx, (int32_t)cvar->shadow_ofs_x) ;
}
static JSValue js_lv_draw_rect_dsc_set_shadow_ofs_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setShadowOfsY() must be call with a lv.DrawRectDsc object")
    }
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "shadow_ofs_y", "setShadowOfsY") 
        } 
        cvar->shadow_ofs_y = (lv_coord_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_shadow_ofs_y(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setShadowOfsY() must be call with a lv.ShadowOfsY object")
    }
    return JS_NewInt32(ctx, (int32_t)cvar->shadow_ofs_y) ;
}
static JSValue js_lv_draw_rect_dsc_set_shadow_spread(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setShadowSpread() must be call with a lv.DrawRectDsc object")
    }
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "shadow_spread", "setShadowSpread") 
        } 
        cvar->shadow_spread = (lv_coord_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_shadow_spread(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setShadowSpread() must be call with a lv.ShadowSpread object")
    }
    return JS_NewInt32(ctx, (int32_t)cvar->shadow_spread) ;
}
static JSValue js_lv_draw_rect_dsc_set_shadow_opa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setShadowOpa() must be call with a lv.DrawRectDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "shadow_opa", "setShadowOpa") 
        } 
        cvar->shadow_opa = (lv_opa_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_rect_dsc_get_shadow_opa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_rect_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_rect_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setShadowOpa() must be call with a lv.ShadowOpa object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->shadow_opa) ;
}

static const JSCFunctionListEntry js_lv_draw_rect_dsc_proto_funcs[] = {
    JS_CFUNC_DEF("setRadius", 0, js_lv_draw_rect_dsc_set_radius),
    JS_CFUNC_DEF("setBgColor", 0, js_lv_draw_rect_dsc_set_bg_color),
    JS_CFUNC_DEF("setBgGradColor", 0, js_lv_draw_rect_dsc_set_bg_grad_color),
    JS_CFUNC_DEF("setBgMainColorStop", 0, js_lv_draw_rect_dsc_set_bg_main_color_stop),
    JS_CFUNC_DEF("setBgGradColorStop", 0, js_lv_draw_rect_dsc_set_bg_grad_color_stop),
    JS_CFUNC_DEF("setBgOpa", 0, js_lv_draw_rect_dsc_set_bg_opa),
    JS_CFUNC_DEF("setBgImgRecolor", 0, js_lv_draw_rect_dsc_set_bg_img_recolor),
    JS_CFUNC_DEF("setBgImgOpa", 0, js_lv_draw_rect_dsc_set_bg_img_opa),
    JS_CFUNC_DEF("setBgImgRecolorOpa", 0, js_lv_draw_rect_dsc_set_bg_img_recolor_opa),
    JS_CFUNC_DEF("setBgImgTiled", 0, js_lv_draw_rect_dsc_set_bg_img_tiled),
    JS_CFUNC_DEF("setBorderColor", 0, js_lv_draw_rect_dsc_set_border_color),
    JS_CFUNC_DEF("setBorderWidth", 0, js_lv_draw_rect_dsc_set_border_width),
    JS_CFUNC_DEF("setBorderOpa", 0, js_lv_draw_rect_dsc_set_border_opa),
    JS_CFUNC_DEF("setBorderPost", 0, js_lv_draw_rect_dsc_set_border_post),
    JS_CFUNC_DEF("setBorderSide", 0, js_lv_draw_rect_dsc_set_border_side),
    JS_CFUNC_DEF("setOutlineColor", 0, js_lv_draw_rect_dsc_set_outline_color),
    JS_CFUNC_DEF("setOutlineWidth", 0, js_lv_draw_rect_dsc_set_outline_width),
    JS_CFUNC_DEF("setOutlinePad", 0, js_lv_draw_rect_dsc_set_outline_pad),
    JS_CFUNC_DEF("setOutlineOpa", 0, js_lv_draw_rect_dsc_set_outline_opa),
    JS_CFUNC_DEF("setShadowColor", 0, js_lv_draw_rect_dsc_set_shadow_color),
    JS_CFUNC_DEF("setShadowWidth", 0, js_lv_draw_rect_dsc_set_shadow_width),
    JS_CFUNC_DEF("setShadowOfsX", 0, js_lv_draw_rect_dsc_set_shadow_ofs_x),
    JS_CFUNC_DEF("setShadowOfsY", 0, js_lv_draw_rect_dsc_set_shadow_ofs_y),
    JS_CFUNC_DEF("setShadowSpread", 0, js_lv_draw_rect_dsc_set_shadow_spread),
    JS_CFUNC_DEF("setShadowOpa", 0, js_lv_draw_rect_dsc_set_shadow_opa),

    JS_CFUNC_DEF("radius", 0, js_lv_draw_rect_dsc_get_radius),
    JS_CFUNC_DEF("bgColor", 0, js_lv_draw_rect_dsc_get_bg_color),
    JS_CFUNC_DEF("bgGradColor", 0, js_lv_draw_rect_dsc_get_bg_grad_color),
    JS_CFUNC_DEF("bgMainColorStop", 0, js_lv_draw_rect_dsc_get_bg_main_color_stop),
    JS_CFUNC_DEF("bgGradColorStop", 0, js_lv_draw_rect_dsc_get_bg_grad_color_stop),
    JS_CFUNC_DEF("bgOpa", 0, js_lv_draw_rect_dsc_get_bg_opa),
    JS_CFUNC_DEF("bgImgRecolor", 0, js_lv_draw_rect_dsc_get_bg_img_recolor),
    JS_CFUNC_DEF("bgImgOpa", 0, js_lv_draw_rect_dsc_get_bg_img_opa),
    JS_CFUNC_DEF("bgImgRecolorOpa", 0, js_lv_draw_rect_dsc_get_bg_img_recolor_opa),
    JS_CFUNC_DEF("bgImgTiled", 0, js_lv_draw_rect_dsc_get_bg_img_tiled),
    JS_CFUNC_DEF("borderColor", 0, js_lv_draw_rect_dsc_get_border_color),
    JS_CFUNC_DEF("borderWidth", 0, js_lv_draw_rect_dsc_get_border_width),
    JS_CFUNC_DEF("borderOpa", 0, js_lv_draw_rect_dsc_get_border_opa),
    JS_CFUNC_DEF("borderPost", 0, js_lv_draw_rect_dsc_get_border_post),
    JS_CFUNC_DEF("borderSide", 0, js_lv_draw_rect_dsc_get_border_side),
    JS_CFUNC_DEF("outlineColor", 0, js_lv_draw_rect_dsc_get_outline_color),
    JS_CFUNC_DEF("outlineWidth", 0, js_lv_draw_rect_dsc_get_outline_width),
    JS_CFUNC_DEF("outlinePad", 0, js_lv_draw_rect_dsc_get_outline_pad),
    JS_CFUNC_DEF("outlineOpa", 0, js_lv_draw_rect_dsc_get_outline_opa),
    JS_CFUNC_DEF("shadowColor", 0, js_lv_draw_rect_dsc_get_shadow_color),
    JS_CFUNC_DEF("shadowWidth", 0, js_lv_draw_rect_dsc_get_shadow_width),
    JS_CFUNC_DEF("shadowOfsX", 0, js_lv_draw_rect_dsc_get_shadow_ofs_x),
    JS_CFUNC_DEF("shadowOfsY", 0, js_lv_draw_rect_dsc_get_shadow_ofs_y),
    JS_CFUNC_DEF("shadowSpread", 0, js_lv_draw_rect_dsc_get_shadow_spread),
    JS_CFUNC_DEF("shadowOpa", 0, js_lv_draw_rect_dsc_get_shadow_opa),

} ;


// wrap lvgl struct lv_draw_arc_dsc_t
JSClassID js_lv_draw_arc_dsc_class_id ;
static JSValue js_lv_draw_arc_dsc_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    JSValue proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto)) {
        JS_FreeValue(ctx, proto) ;
        return JS_EXCEPTION ;
    }
    JSValue jsobj = JS_NewObjectProtoClass(ctx, proto, js_lv_draw_arc_dsc_class_id) ;
    JS_FreeValue(ctx, proto) ;

    lv_draw_arc_dsc_t * cvar = malloc(sizeof(lv_draw_arc_dsc_t)) ;
    lv_draw_arc_dsc_init(cvar) ;

    JS_SetOpaque(jsobj, cvar) ;
    return jsobj ;
}
static void js_lv_draw_arc_dsc_finalizer(JSRuntime *rt, JSValue this_val){
    lv_draw_arc_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_arc_dsc_class_id) ;
    if(cvar) {
        // printf("free(lv_draw_arc_dsc_t*)\n") ;
        free(cvar) ;
    }
}
static JSClassDef js_lv_draw_arc_dsc_class = {
    "DrawArcDsc",
    .finalizer = js_lv_draw_arc_dsc_finalizer,
} ;
static JSValue js_lv_draw_arc_dsc_set_color(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_arc_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_arc_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setColor() must be call with a lv.DrawArcDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "color", "setColor") 
        } 
        cvar->color.full = num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_arc_dsc_get_color(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_arc_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_arc_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setColor() must be call with a lv.Color object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->color.full) ;
}
static JSValue js_lv_draw_arc_dsc_set_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_arc_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_arc_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setWidth() must be call with a lv.DrawArcDsc object")
    }
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "width", "setWidth") 
        } 
        cvar->width = (lv_coord_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_arc_dsc_get_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_arc_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_arc_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setWidth() must be call with a lv.Width object")
    }
    return JS_NewInt32(ctx, (int32_t)cvar->width) ;
}
static JSValue js_lv_draw_arc_dsc_set_opa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_arc_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_arc_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setOpa() must be call with a lv.DrawArcDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "opa", "setOpa") 
        } 
        cvar->opa = (lv_opa_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_arc_dsc_get_opa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_arc_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_arc_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setOpa() must be call with a lv.Opa object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->opa) ;
}
static JSValue js_lv_draw_arc_dsc_set_rounded(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_arc_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_arc_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setRounded() must be call with a lv.DrawArcDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "rounded", "setRounded") 
        } 
        cvar->rounded = (uint8_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_arc_dsc_get_rounded(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_arc_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_arc_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setRounded() must be call with a lv.Rounded object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->rounded) ;
}

static const JSCFunctionListEntry js_lv_draw_arc_dsc_proto_funcs[] = {
    JS_CFUNC_DEF("setColor", 0, js_lv_draw_arc_dsc_set_color),
    JS_CFUNC_DEF("setWidth", 0, js_lv_draw_arc_dsc_set_width),
    JS_CFUNC_DEF("setOpa", 0, js_lv_draw_arc_dsc_set_opa),
    JS_CFUNC_DEF("setRounded", 0, js_lv_draw_arc_dsc_set_rounded),

    JS_CFUNC_DEF("color", 0, js_lv_draw_arc_dsc_get_color),
    JS_CFUNC_DEF("width", 0, js_lv_draw_arc_dsc_get_width),
    JS_CFUNC_DEF("opa", 0, js_lv_draw_arc_dsc_get_opa),
    JS_CFUNC_DEF("rounded", 0, js_lv_draw_arc_dsc_get_rounded),

} ;


// wrap lvgl struct lv_draw_line_dsc_t
JSClassID js_lv_draw_line_dsc_class_id ;
static JSValue js_lv_draw_line_dsc_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    JSValue proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto)) {
        JS_FreeValue(ctx, proto) ;
        return JS_EXCEPTION ;
    }
    JSValue jsobj = JS_NewObjectProtoClass(ctx, proto, js_lv_draw_line_dsc_class_id) ;
    JS_FreeValue(ctx, proto) ;

    lv_draw_line_dsc_t * cvar = malloc(sizeof(lv_draw_line_dsc_t)) ;
    lv_draw_line_dsc_init(cvar) ;

    JS_SetOpaque(jsobj, cvar) ;
    return jsobj ;
}
static void js_lv_draw_line_dsc_finalizer(JSRuntime *rt, JSValue this_val){
    lv_draw_line_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_line_dsc_class_id) ;
    if(cvar) {
        // printf("free(lv_draw_line_dsc_t*)\n") ;
        free(cvar) ;
    }
}
static JSClassDef js_lv_draw_line_dsc_class = {
    "DrawLineDsc",
    .finalizer = js_lv_draw_line_dsc_finalizer,
} ;
static JSValue js_lv_draw_line_dsc_set_color(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_line_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_line_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setColor() must be call with a lv.DrawLineDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "color", "setColor") 
        } 
        cvar->color.full = num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_line_dsc_get_color(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_line_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_line_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setColor() must be call with a lv.Color object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->color.full) ;
}
static JSValue js_lv_draw_line_dsc_set_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_line_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_line_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setWidth() must be call with a lv.DrawLineDsc object")
    }
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "width", "setWidth") 
        } 
        cvar->width = (lv_coord_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_line_dsc_get_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_line_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_line_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setWidth() must be call with a lv.Width object")
    }
    return JS_NewInt32(ctx, (int32_t)cvar->width) ;
}
static JSValue js_lv_draw_line_dsc_set_dash_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_line_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_line_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setDashWidth() must be call with a lv.DrawLineDsc object")
    }
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "dash_width", "setDashWidth") 
        } 
        cvar->dash_width = (lv_coord_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_line_dsc_get_dash_width(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_line_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_line_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setDashWidth() must be call with a lv.DashWidth object")
    }
    return JS_NewInt32(ctx, (int32_t)cvar->dash_width) ;
}
static JSValue js_lv_draw_line_dsc_set_dash_gap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_line_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_line_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setDashGap() must be call with a lv.DrawLineDsc object")
    }
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "dash_gap", "setDashGap") 
        } 
        cvar->dash_gap = (lv_coord_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_line_dsc_get_dash_gap(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_line_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_line_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setDashGap() must be call with a lv.DashGap object")
    }
    return JS_NewInt32(ctx, (int32_t)cvar->dash_gap) ;
}
static JSValue js_lv_draw_line_dsc_set_opa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_line_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_line_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setOpa() must be call with a lv.DrawLineDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "opa", "setOpa") 
        } 
        cvar->opa = (lv_opa_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_line_dsc_get_opa(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_line_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_line_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setOpa() must be call with a lv.Opa object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->opa) ;
}
static JSValue js_lv_draw_line_dsc_set_round_start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_line_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_line_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setRoundStart() must be call with a lv.DrawLineDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "round_start", "setRoundStart") 
        } 
        cvar->round_start = (uint8_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_line_dsc_get_round_start(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_line_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_line_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setRoundStart() must be call with a lv.RoundStart object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->round_start) ;
}
static JSValue js_lv_draw_line_dsc_set_round_end(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_line_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_line_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setRoundEnd() must be call with a lv.DrawLineDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "round_end", "setRoundEnd") 
        } 
        cvar->round_end = (uint8_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_line_dsc_get_round_end(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_line_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_line_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setRoundEnd() must be call with a lv.RoundEnd object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->round_end) ;
}
static JSValue js_lv_draw_line_dsc_set_raw_end(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_draw_line_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_line_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setRawEnd() must be call with a lv.DrawLineDsc object")
    }
    {
        uint32_t num ;
        if( JS_ToUint32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "raw_end", "setRawEnd") 
        } 
        cvar->raw_end = (uint8_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_draw_line_dsc_get_raw_end(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_draw_line_dsc_t * cvar = JS_GetOpaque(this_val, js_lv_draw_line_dsc_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setRawEnd() must be call with a lv.RawEnd object")
    }
    return JS_NewUint32(ctx, (uint32_t)cvar->raw_end) ;
}

static const JSCFunctionListEntry js_lv_draw_line_dsc_proto_funcs[] = {
    JS_CFUNC_DEF("setColor", 0, js_lv_draw_line_dsc_set_color),
    JS_CFUNC_DEF("setWidth", 0, js_lv_draw_line_dsc_set_width),
    JS_CFUNC_DEF("setDashWidth", 0, js_lv_draw_line_dsc_set_dash_width),
    JS_CFUNC_DEF("setDashGap", 0, js_lv_draw_line_dsc_set_dash_gap),
    JS_CFUNC_DEF("setOpa", 0, js_lv_draw_line_dsc_set_opa),
    JS_CFUNC_DEF("setRoundStart", 0, js_lv_draw_line_dsc_set_round_start),
    JS_CFUNC_DEF("setRoundEnd", 0, js_lv_draw_line_dsc_set_round_end),
    JS_CFUNC_DEF("setRawEnd", 0, js_lv_draw_line_dsc_set_raw_end),

    JS_CFUNC_DEF("color", 0, js_lv_draw_line_dsc_get_color),
    JS_CFUNC_DEF("width", 0, js_lv_draw_line_dsc_get_width),
    JS_CFUNC_DEF("dashWidth", 0, js_lv_draw_line_dsc_get_dash_width),
    JS_CFUNC_DEF("dashGap", 0, js_lv_draw_line_dsc_get_dash_gap),
    JS_CFUNC_DEF("opa", 0, js_lv_draw_line_dsc_get_opa),
    JS_CFUNC_DEF("roundStart", 0, js_lv_draw_line_dsc_get_round_start),
    JS_CFUNC_DEF("roundEnd", 0, js_lv_draw_line_dsc_get_round_end),
    JS_CFUNC_DEF("rawEnd", 0, js_lv_draw_line_dsc_get_raw_end),

} ;


// wrap lvgl struct lv_area_t
JSClassID js_lv_area_class_id ;
static JSValue js_lv_area_constructor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv){
    JSValue proto = JS_GetPropertyStr(ctx, new_target, "prototype");
    if (JS_IsException(proto)) {
        JS_FreeValue(ctx, proto) ;
        return JS_EXCEPTION ;
    }
    JSValue jsobj = JS_NewObjectProtoClass(ctx, proto, js_lv_area_class_id) ;
    JS_FreeValue(ctx, proto) ;

    lv_area_t * cvar = malloc(sizeof(lv_area_t)) ;

    CHECK_ARGC(4)
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "x1", "lv.Area") 
        } 
        cvar->x1 = (lv_coord_t)num ; 
    } 
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[1])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "y1", "lv.Area") 
        } 
        cvar->y1 = (lv_coord_t)num ; 
    } 
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[2])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "x2", "lv.Area") 
        } 
        cvar->x2 = (lv_coord_t)num ; 
    } 
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[3])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "y2", "lv.Area") 
        } 
        cvar->y2 = (lv_coord_t)num ; 
    } 

    JS_SetOpaque(jsobj, cvar) ;
    return jsobj ;
}
static void js_lv_area_finalizer(JSRuntime *rt, JSValue this_val){
    lv_area_t * cvar = JS_GetOpaque(this_val, js_lv_area_class_id) ;
    if(cvar) {
        // printf("free(lv_area_t*)\n") ;
        free(cvar) ;
    }
}
static JSClassDef js_lv_area_class = {
    "Area",
    .finalizer = js_lv_area_finalizer,
} ;
static JSValue js_lv_area_set_x1(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_area_t * cvar = JS_GetOpaque(this_val, js_lv_area_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setX1() must be call with a lv.Area object")
    }
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "x1", "setX1") 
        } 
        cvar->x1 = (lv_coord_t)num ; 
    }

    return JS_UNDEFINED ;
}
static JSValue js_lv_area_get_x1(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_area_t * cvar = JS_GetOpaque(this_val, js_lv_area_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setX1() must be call with a lv.X1 object")
    }
    return JS_NewInt32(ctx, (int32_t)cvar->x1) ;
}
static JSValue js_lv_area_set_y1(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_area_t * cvar = JS_GetOpaque(this_val, js_lv_area_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setY1() must be call with a lv.Area object")
    }
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "y1", "setY1") 
        } 
        cvar->y1 = (lv_coord_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_area_get_y1(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_area_t * cvar = JS_GetOpaque(this_val, js_lv_area_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setY1() must be call with a lv.Y1 object")
    }
    return JS_NewInt32(ctx, (int32_t)cvar->y1) ;
}
static JSValue js_lv_area_set_x2(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_area_t * cvar = JS_GetOpaque(this_val, js_lv_area_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setX2() must be call with a lv.Area object")
    }
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "x2", "setX2") 
        } 
        cvar->x2 = (lv_coord_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_area_get_x2(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_area_t * cvar = JS_GetOpaque(this_val, js_lv_area_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setX2() must be call with a lv.X2 object")
    }
    return JS_NewInt32(ctx, (int32_t)cvar->x2) ;
}
static JSValue js_lv_area_set_y2(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    lv_area_t * cvar = JS_GetOpaque(this_val, js_lv_area_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setY2() must be call with a lv.Area object")
    }
    {
        int32_t num ;
        if( JS_ToInt32(ctx, &num, argv[0])!=0 ) {
            THROW_EXCEPTION("arg %s of %s() is invalid", "y2", "setY2") 
        } 
        cvar->y2 = (lv_coord_t)num ; 
    } 

    return JS_UNDEFINED ;
}
static JSValue js_lv_area_get_y2(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    lv_area_t * cvar = JS_GetOpaque(this_val, js_lv_area_class_id) ;
    if(!cvar) {
        THROW_EXCEPTION("setY2() must be call with a lv.Y2 object")
    }
    return JS_NewInt32(ctx, (int32_t)cvar->y2) ;
}

static const JSCFunctionListEntry js_lv_area_proto_funcs[] = {
    JS_CFUNC_DEF("setX1", 0, js_lv_area_set_x1),
    JS_CFUNC_DEF("setY1", 0, js_lv_area_set_y1),
    JS_CFUNC_DEF("setX2", 0, js_lv_area_set_x2),
    JS_CFUNC_DEF("setY2", 0, js_lv_area_set_y2),

    JS_CFUNC_DEF("x1", 0, js_lv_area_get_x1),
    JS_CFUNC_DEF("y1", 0, js_lv_area_get_y1),
    JS_CFUNC_DEF("x2", 0, js_lv_area_get_x2),
    JS_CFUNC_DEF("y2", 0, js_lv_area_get_y2),

} ;

// AUTO GENERATE CODE END [CLASS DEFINE] --------


void be_lv_structs_init() {
// AUTO GENERATE CODE START [CLASS ID INIT] --------
    JS_NewClassID(&js_lv_draw_rect_dsc_class_id);
    JS_NewClassID(&js_lv_draw_arc_dsc_class_id);
    JS_NewClassID(&js_lv_draw_line_dsc_class_id);
    JS_NewClassID(&js_lv_area_class_id);
// AUTO GENERATE CODE END [CLASS ID INIT] --------
}

#include "psram.h"

void be_lv_structs_require(JSContext *ctx, JSValue lvgl) {

// AUTO GENERATE CODE START [CLASS REQUIRE] --------
    qjs_def_class(ctx, "DrawRectDsc", js_lv_draw_rect_dsc_class_id, &js_lv_draw_rect_dsc_class
                , "lvgl.DrawRectDsc", js_lv_draw_rect_dsc_constructor, js_lv_draw_rect_dsc_proto_funcs, countof(js_lv_draw_rect_dsc_proto_funcs), JS_UNDEFINED, lvgl) ;

    qjs_def_class(ctx, "DrawArcDsc", js_lv_draw_arc_dsc_class_id, &js_lv_draw_arc_dsc_class
                , "lvgl.DrawArcDsc", js_lv_draw_arc_dsc_constructor, js_lv_draw_arc_dsc_proto_funcs, countof(js_lv_draw_arc_dsc_proto_funcs), JS_UNDEFINED, lvgl) ;

    qjs_def_class(ctx, "DrawLineDsc", js_lv_draw_line_dsc_class_id, &js_lv_draw_line_dsc_class
                , "lvgl.DrawLineDsc", js_lv_draw_line_dsc_constructor, js_lv_draw_line_dsc_proto_funcs, countof(js_lv_draw_line_dsc_proto_funcs), JS_UNDEFINED, lvgl) ;

    qjs_def_class(ctx, "Area", js_lv_area_class_id, &js_lv_area_class
                , "lvgl.Area", js_lv_area_constructor, js_lv_area_proto_funcs, countof(js_lv_area_proto_funcs), JS_UNDEFINED, lvgl) ;

// AUTO GENERATE CODE END [CLASS REQUIRE] --------
}
