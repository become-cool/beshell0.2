#ifndef __H_REMOTE_VIDEO_STREAM__
#define __H_REMOTE_VIDEO_STREAM__

#include "quickjs-libc.h"


void be_remote_video_stream_init() ;
void be_remote_video_stream_require(JSContext *ctx,JSValue jspkg) ;
void be_remote_video_stream_reset(JSContext *ctx) ;

#endif