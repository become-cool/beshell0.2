#include "utils.h"
#include <time.h>
#include<stdarg.h>

uint64_t gettime() {
	struct timespec tm = {0, 0};
	clock_gettime(CLOCK_REALTIME, &tm);
	return tm.tv_nsec/1000/1000 + tm.tv_sec*1000 ;
}


char * mallocf(char * format, ...) {

	char * str = NULL ;

    va_list argptr;
    va_start(argptr, format);

	size_t len = vsnprintf(NULL, 0, format, argptr) ;
	if(len) {
		str = malloc(len+1) ;
		if(str) {
			vsnprintf(str, len+1, format,argptr ) ;
		}
	}

	va_end(argptr);
	return str ;

}