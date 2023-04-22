#ifndef __H_LIST__
#define __H_LIST__

#include <stdlib.h>
#include <stdbool.h>

#define dd printf("@%d\n",__LINE__);

typedef struct list_item {
    struct list_item * prev ;
    struct list_item * next ;
} list_item_t;

typedef struct {
    list_item_t * head ;
    list_item_t * tail ;
    size_t count ;
} list_t;

void list_init(list_t * lst) ;
void list_append(list_t * lst, list_item_t * item) ;
void list_remove(list_t * lst, list_item_t * item) ;
bool list_is_empty(list_t * lst) ;
unsigned int list_recount(list_t * lst) ;


#define FOREACH_LIST(lst, item) for(list_item_t * item=(lst)->head; item!=NULL; item=item->next)
#define FOREACH_TYPE_LIST(lst, type, item) for(type * item=(type*)(lst)->head; item!=NULL; item=((list_item_t*)item)->next)

#endif