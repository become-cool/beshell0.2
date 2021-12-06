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


void repl_init() {
    fd_stdin = fileno(stdin);
}

void repl_loop(JSContext * ctx) {

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
        if(num_bytes<MAXBYTES) {
            buf[num_bytes] = 0 ;
        }
        JSValue ret = JS_Eval(ctx, buf, num_bytes, "REPL", JS_EVAL_TYPE_GLOBAL) ;
        if(JS_IsException(ret)) {
            echo_error(ctx) ;
        }

        else {
            char * retstr = JS_ToCString(ctx, ret) ;
            printf("%s\n", retstr) ;
            JS_FreeCString(ctx, retstr) ;
        }

        JS_FreeValue(ctx, ret) ;
    }
}
