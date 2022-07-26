#include "driver_camera.h"
#include "module_metadata.h"
#include "utils.h"
#include "esp_camera.h"
#include "esp_http_server.h"
#include <string.h>
#include "img_converters.h"

#define PART_BOUNDARY "123456789000000000000987654321"
static const char *_STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char *_STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char *_STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\nX-Timestamp: %d.%06d\r\n\r\n";


bool inited = false ;

bool driver_camera_has_inited() {
    return inited ;
}


httpd_handle_t stream_httpd = NULL;
static esp_err_t web_camera_stream(httpd_req_t *req) {

    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_hdr(req, "X-Framerate", "60");

    if(!inited) {
        httpd_resp_set_status(req, "404") ;
        return httpd_resp_send(req, "Camera device not setuped", 5);
    }

    esp_err_t res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);

    char *part_buf[128];
    camera_fb_t * fb = NULL ;
    size_t hlen = 0 ;

    while (true) {

        fb = esp_camera_fb_get();
        if (!fb) {
            httpd_resp_send(req, "Camera device not setuped", 5);
            break ;
        }

        res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
        if(res!=ESP_OK) {
            break ;
        }
        
        hlen = snprintf((char *)part_buf, 128, _STREAM_PART, fb->len, fb->timestamp.tv_sec, fb->timestamp.tv_usec);
        res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
        if(res!=ESP_OK) {
            break ;
        }

        res = httpd_resp_send_chunk(req, (const char *)fb->buf, fb->len);
        if(res!=ESP_OK) {
            break ;
        }
        
        esp_camera_fb_return(fb);
        fb = NULL ;
    }

    if(fb) {
        esp_camera_fb_return(fb);
    }

    return ESP_OK;
}

static void web_camera_init() {
    
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 8;
    config.server_port = 8019 ;
    config.ctrl_port += 8019 ;
    config.core_id = 1 ;

    httpd_uri_t stream_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = web_camera_stream,
        .user_ctx = NULL};

    if (httpd_start(&stream_httpd, &config) != ESP_OK) {
        printf("start camera stream server faild\n") ;
        return ;
    }
    
    httpd_register_uri_handler(stream_httpd, &stream_uri);
}



static JSValue js_camera_has_setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    return inited? JS_TRUE: JS_FALSE ;
}

/*
@brief 
@param pins
@param options

format:
yuv422
grayscale
rgb565
jpeg

size:
QVGA    320 x 240
CIF     352 x 288
VGA     640 x 480
SVGA    800 x 600
XGA     1024 x 768
SXGA    1280 x 1024
UXGA    1600 x 1200
*/
static JSValue js_camera_setup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {

    if(inited) {
        THROW_EXCEPTION("camera device has setuped.")
    }

    CHECK_ARGC(1)
    if( !JS_IsObject(argv[0]) ) {
        THROW_EXCEPTION("param pins must be a object")
    }
    
    camera_config_t config ;
    memset(&config, 0, sizeof(camera_config_t));

    ASSIGN_INT_PROP(argv[0], "d0", config.pin_d0, error)
    ASSIGN_INT_PROP(argv[0], "d1", config.pin_d1, error)
    ASSIGN_INT_PROP(argv[0], "d2", config.pin_d2, error)
    ASSIGN_INT_PROP(argv[0], "d3", config.pin_d3, error)
    ASSIGN_INT_PROP(argv[0], "d4", config.pin_d4, error)
    ASSIGN_INT_PROP(argv[0], "d5", config.pin_d5, error)
    ASSIGN_INT_PROP(argv[0], "d6", config.pin_d6, error)
    ASSIGN_INT_PROP(argv[0], "d7", config.pin_d7, error)
    ASSIGN_INT_PROP(argv[0], "xclk", config.pin_xclk, error)
    ASSIGN_INT_PROP(argv[0], "pclk", config.pin_pclk, error)
    ASSIGN_INT_PROP(argv[0], "vsync", config.pin_vsync, error)
    ASSIGN_INT_PROP(argv[0], "href", config.pin_href, error)
    ASSIGN_INT_PROP(argv[0], "sda", config.pin_sscb_sda, error)
    ASSIGN_INT_PROP(argv[0], "scl", config.pin_sscb_scl, error)
    ASSIGN_INT_PROP_DEFAULT(argv[0], "pwdn", config.pin_pwdn, -1, error)
    ASSIGN_INT_PROP_DEFAULT(argv[0], "reset", config.pin_reset, -1, error)

    ASSIGN_INT_PROP_DEFAULT(argv[0], "ledc_channel", config.ledc_channel, 1, error)
    ASSIGN_INT_PROP_DEFAULT(argv[0], "ledc_timer", config.ledc_timer, 1, error)
    ASSIGN_INT_PROP_DEFAULT(argv[0], "jpeg_quality", config.jpeg_quality, 10, error)
    ASSIGN_INT_PROP_DEFAULT(argv[0], "fb_count", config.fb_count, 2, error)
    ASSIGN_INT_PROP_DEFAULT(argv[0], "xclk_freq", config.xclk_freq_hz, 20000000, error)
    
    JSValue format = JS_GetPropertyStr(ctx, argv[0], "format") ;
    if( format==JS_UNDEFINED ) {
        config.pixel_format = PIXFORMAT_JPEG ;
    }
    else {
        char * cformat = (char *)JS_ToCString(ctx, format) ;
        JS_FreeValue(ctx,format) ;

        if(strcmp(cformat, "jpeg")==0) {
            config.pixel_format = PIXFORMAT_JPEG ;
        }
        else if(strcmp(cformat, "rgb565")==0) {
            config.pixel_format = PIXFORMAT_RGB565 ;
        }
        else if(strcmp(cformat, "yuv422")==0) {
            config.pixel_format = PIXFORMAT_YUV422 ;
        }
        else if(strcmp(cformat, "grayscale")==0) {
            config.pixel_format = PIXFORMAT_GRAYSCALE ;
        }
        else {
            THROW_EXCEPTION("unknow options.format value")
        }
        JS_FreeCString(ctx, cformat) ;
    }


    JSValue size = JS_GetPropertyStr(ctx, argv[0], "size") ;
    if( format==JS_UNDEFINED ) {
        config.frame_size = PIXFORMAT_JPEG ;
    }
    else {
        char * csize = (char *)JS_ToCString(ctx, size) ;
        JS_FreeValue(ctx,size) ;

        if(strcmp(csize, "QVGA")==0) {
            config.frame_size = FRAMESIZE_QVGA ;
        }
        else if(strcmp(csize, "CIF")==0) {
            config.frame_size = FRAMESIZE_CIF ;
        }
        else if(strcmp(csize, "VGA")==0) {
            config.frame_size = FRAMESIZE_VGA ;
        }
        else if(strcmp(csize, "SVGA")==0) {
            config.frame_size = FRAMESIZE_SVGA ;
        }
        else if(strcmp(csize, "XGA")==0) {
            config.frame_size = FRAMESIZE_XGA ;
        }
        else if(strcmp(csize, "SXGA")==0) {
            config.frame_size = FRAMESIZE_SXGA ;
        }
        else if(strcmp(csize, "UXGA")==0) {
            config.frame_size = FRAMESIZE_UXGA ;
        }
        else {
            THROW_EXCEPTION("unknow options.size value")
        }
        JS_FreeCString(ctx, csize) ;
    }

    config.fb_location = CAMERA_FB_IN_PSRAM ;
    config.grab_mode = 0 ;

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        THROW_EXCEPTION("Camera Init Failed, %s (%d)", esp_err_to_name(err), err)
    }

    inited = true ;

    // camera_fb_t * fb = esp_camera_fb_get();
    // dn2(fb->width, fb->height)
    // dn2(fb->format, fb->len)    
    // esp_camera_fb_return(fb);

    return JS_TRUE ;

error:
    return JS_EXCEPTION ;
}

static JSValue js_camera_unsetup(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    if(!inited) {
        THROW_EXCEPTION("camera device not setup yet.")
    }
    if(esp_camera_deinit()==ESP_OK) {
        inited = false ;
        return JS_TRUE ;
    }
    else {
        return JS_FALSE ;
    }
}

static JSValue js_camera_capture(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(0)
    
    if(!inited) {
        THROW_EXCEPTION("camera device not setup yet.")
    }

    camera_fb_t * fb = esp_camera_fb_get();
    if(fb==NULL) {
        return JS_NULL ;
    }

    uint8_t * buff = malloc(fb->len) ;
    if(!buff) {
        esp_camera_fb_return(fb);
        THROW_EXCEPTION("out of memory") ;
    }
    memcpy(buff, fb->buf, fb->len) ;
    JSValue arrbuff = JS_NewArrayBuffer(ctx, buff, fb->len, freeArrayBuffer, NULL, false) ;

    JS_SetPropertyStr(ctx, arrbuff, "width", JS_NewInt32(ctx, fb->width) ) ;
    JS_SetPropertyStr(ctx, arrbuff, "height", JS_NewInt32(ctx, fb->height) ) ;
    JS_SetPropertyStr(ctx, arrbuff, "format", JS_NewInt32(ctx, fb->format ) ) ;
    
    esp_camera_fb_return(fb);
    return arrbuff ;
}

static JSValue js_camera_jpeg2rgb888(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(2)

    // JSValue propWidth = JS_GetPropertyStr(ctx, argv[0], "width") ;
    // JSValue propHeight = JS_GetPropertyStr(ctx, argv[0], "height") ;

    // int width = 0;
    // int height = 0;
    // if( JS_ToInt32(ctx, &width, propWidth)!=0 || JS_ToInt32(ctx, &height, propHeight)!=0 ){
    //     THROW_EXCEPTION("arg frame is not a valid frame object, missing width or height property.")
    // }
    // dn2(width, height)

    ARGV_TO_ARRAYBUFFER(0, jpgbuf, jpglen) 
    ARGV_TO_ARRAYBUFFER(1, rawbuf, rawlen) 

    return fmt2rgb888(jpgbuf, jpglen, PIXFORMAT_JPEG, rawbuf)? JS_TRUE : JS_FALSE;
}

struct mg_connection * stream_client = NULL ;
int64_t last_stream_tick = 0 ;

bool be_module_driver_camera_response(struct mg_connection *c, int ev, void *ev_data){

    if(ev==MG_EV_CLOSE) {
        // 推流接收客户端已断开
        if(c==stream_client) {
            printf("camera stream client closed.\n") ;
            stream_client = NULL ;
            return false ; // <- 可继续处理
        }
    }

    else if (ev == MG_EV_HTTP_MSG) {

        struct mg_http_message *hm = (struct mg_http_message *)ev_data;

        if (mg_http_match_uri(hm, "/camera/capture")) {
            if(!inited) {
                mg_http_reply(c, 404, "", "%s", "Camera device not setuped");
                return true ;
            }
            camera_fb_t * fb = esp_camera_fb_get();
            if (!fb) {
                mg_http_reply(c, 500, "", "%s", "Camera capture failed");
                return true ;
            }

            mg_printf(c, "HTTP/1.1 200 OK\r\n") ;
            mg_printf(c, "Content-Type: image/jpeg\r\n") ;
            mg_printf(c, "Content-Disposition: inline; filename=picture.jpg\r\n") ;
            mg_printf(c, "Content-Length: %d\r\n\r\n", fb->len);

            mg_send(c, fb->buf, fb->len);

            esp_camera_fb_return(fb);

            return true ;
        }
    }

    return false ;
}


void be_module_driver_camera_init() {
    inited = false ;
    web_camera_init() ;
}

void be_module_driver_camera_require(JSContext *ctx, JSValue driver) {

    JSValue camera = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, driver, "camera", camera);

    JS_SetPropertyStr(ctx, camera, "setup", JS_NewCFunction(ctx, js_camera_setup, "setup", 1));
    JS_SetPropertyStr(ctx, camera, "unsetup", JS_NewCFunction(ctx, js_camera_unsetup, "unsetup", 1));
    JS_SetPropertyStr(ctx, camera, "hasSetup", JS_NewCFunction(ctx, js_camera_has_setup, "hasSetup", 1));
    JS_SetPropertyStr(ctx, camera, "capture", JS_NewCFunction(ctx, js_camera_capture, "capture", 1));
}


void be_module_driver_camera_reset(JSContext *ctx) {
    if(inited) {
        esp_camera_deinit() ;
        inited = false ;
    }
}

