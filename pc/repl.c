#include "repl.h"
#include "utils.h"
#include <string.h>
#include "beshell.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

void js_dump_err(JSContext *ctx, JSValueConst val) {
    const char * str = JS_ToCString(ctx, val);
    if (str) {
		printf(str);
        JS_FreeCString(ctx, str);
    } else {
        printf("[exception]\n");
    }
    fflush(stdout);
}
void echo_error(JSContext * ctx) {
    JSValue exception_val = JS_GetException(ctx);
    bool is_error = JS_IsError(ctx, exception_val);
    js_dump_err(ctx, exception_val);
    if (is_error) {
        JSValue val = JS_GetPropertyStr(ctx, exception_val, "stack");
        if (!JS_IsUndefined(val)) {
            js_dump_err(ctx, val);
        }
        JS_FreeValue(ctx, val);
    }
    JS_FreeValue(ctx, exception_val);
}



#define MAXBYTES 102400

fd_set readfds;
struct timeval tv;
int fd_stdin ;
char buf[MAXBYTES] ;


JSValue js_stdin_callback ;


JSValue js_proc_set_stdin_callback(JSContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv) {
    CHECK_ARGC(1)
    if( !JS_IsFunction(ctx, argv[0]) ) {
        THROW_EXCEPTION("arg callback must be a function")
    }
    JS_DupValue(ctx, argv[0]) ;
    js_stdin_callback = argv[0] ;
    return JS_UNDEFINED ;
}

void be_module_repl_init() {
    js_stdin_callback = JS_UNDEFINED ;
    fd_stdin = fileno(stdin);
}

void be_module_repl_reset(JSContext * ctx) {
    JS_FreeValue(ctx, js_stdin_callback) ;
    js_stdin_callback = JS_UNDEFINED ;
}
void be_module_repl_require(JSContext * ctx) {
    JSValue jsproc = js_get_glob_prop(ctx, 1, "process") ;
    JS_SetPropertyStr(ctx, jsproc, "setStdinCallback", JS_NewCFunction(ctx, js_proc_set_stdin_callback, "setStdinCallback", 1));

    pid_t pid = getpid() ;    
    JS_SetPropertyStr(ctx, jsproc, "pid", JS_NewInt32(ctx, pid));


    JS_FreeValue(ctx, jsproc) ;
}

void be_module_repl_loop(JSContext * ctx) {

    FD_ZERO(&readfds);
    FD_SET(fileno(stdin), &readfds);

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    size_t num_readable = select(fd_stdin + 1, &readfds, NULL, NULL, &tv);
    if(errno==EINTR) {
        num_readable = select(fd_stdin + 1, &readfds, NULL, NULL, &tv);
    }
    if (num_readable == -1) {
        // fprintf(stderr, "\nError in select : %s\n", strerror(errno));
    }
    if (num_readable == 0) {
        return ;
    } else {
        size_t num_bytes = read(fd_stdin, buf, MAXBYTES);
        if (num_bytes < 0) {
                fprintf(stderr, "\nError on read : %s\n", strerror(errno));
                exit(1);
        }

        if( JS_IsFunction(ctx, js_stdin_callback) ) {
            JSValue jsstr = JS_NewStringLen(ctx, buf, num_bytes) ;
            MAKE_ARGV1(jsargv, jsstr);

            JSValue ret = JS_Call(ctx, js_stdin_callback, JS_UNDEFINED, 1, jsargv) ;
            JS_FreeValue(ctx, ret) ;
            free(jsargv) ;
            JS_FreeValue(ctx, jsstr) ;
        }
    }
}
