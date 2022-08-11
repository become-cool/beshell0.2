#ifndef ONCE_H_DEBUG
#define ONCE_H_DEBUG

#include <stdio.h>

#define pf(...) printf(__VA_ARGS__) ;printf("\n") ;
#define dd printf("@%d\n", __LINE__) ;
#define df(msg) printf("@%d %s\n", __LINE__, msg) ;
#define dm(msg) printf("%s: %dKB\n", msg, esp_get_free_heap_size()/1024);
#define dp(p)   printf(#p"@%p\n", p) ;
#define ds(s)   printf(#s"=%s @%d\n", s, __LINE__) ;
#define dn(v)   printf(#v"=%d\n", v) ;
#define dn64(v)   printf(#v"=%lld\n", v) ;
#define dn2(v1,v2)              printf(#v1"=%d, "#v2"=%d\n", v1, v2) ;
#define dn3(v1,v2,v3)           printf(#v1"=%d, "#v2"=%d, "#v3"=%d\n", v1, v2, v3) ;
#define dn4(v1,v2,v3,v4)        printf(#v1"=%d, "#v2"=%d, "#v3"=%d, "#v4"=%d\n", v1, v2, v3, v4) ;
#define dn5(v1,v2,v3,v4,v5)     printf(#v1"=%d, "#v2"=%d, "#v3"=%d, "#v4"=%d, "#v5"=%d\n", v1, v2, v3, v4, v5) ;

#define dfunc   printf("%s()@%d\n", __FUNCTION__, __LINE__) ;

#define YES_OR_NO(exp) printf(#exp"? %s\n", (exp)? "yes": "no") ;
#define IS_NULL(exp) YES_OR_NO((exp)==NULL)

#endif
