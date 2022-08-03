#ifndef ONCE_H_DEBUG
#define ONCE_H_DEBUG

#include <stdio.h>

#define PF(...) printf(__VA_ARGS__) ;printf("\n") ;
#define DD printf("@%d\n", __LINE__) ;
#define DF(msg) printf("@%d %s\n", __LINE__, msg) ;
#define DM(msg) printf("%s: %dKB\n", msg, esp_get_free_heap_size()/1024);
#define DP(p)   printf(#p"@%p\n", p) ;
#define DS(s)   printf(#s"=%s\n", s) ;
#define DN(v)   printf(#v"=%d\n", v) ;
#define DN64(v)   printf(#v"=%lld\n", v) ;
#define DN2(v1,v2)              printf(#v1"=%d, "#v2"=%d\n", v1, v2) ;
#define DN3(v1,v2,v3)           printf(#v1"=%d, "#v2"=%d, "#v3"=%d\n", v1, v2, v3) ;
#define DN4(v1,v2,v3,v4)        printf(#v1"=%d, "#v2"=%d, "#v3"=%d, "#v4"=%d\n", v1, v2, v3, v4) ;
#define DN5(v1,v2,v3,v4,v5)     printf(#v1"=%d, "#v2"=%d, "#v3"=%d, "#v4"=%d, "#v5"=%d\n", v1, v2, v3, v4, v5) ;

#define DFUNC   printf("%s()@%d\n", __FUNCTION__, __LINE__) ;

#define YES_OR_NO(exp) printf(#exp"? %s\n", (exp)? "yes": "no") ;
#define IS_NULL(exp) YES_OR_NO((exp)==NULL)

#endif
