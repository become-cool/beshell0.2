#ifndef __H_LIST__
#define __H_LIST__

#include <stdlib.h>
#include <stdbool.h>

#define dd printf("@%d\n",__LINE__);

typedef struct list_item {
    struct list_item * prev ;
    struct list_item * next ;
} be_list_item_t;

typedef struct {
    be_list_item_t * head ;
    be_list_item_t * tail ;
    size_t count ;
} be_list_t;

void be_list_init(be_list_t * lst) ;
void be_list_append(be_list_t * lst, be_list_item_t * item) ;
void be_list_remove(be_list_t * lst, be_list_item_t * item) ;
bool be_list_is_empty(be_list_t * lst) ;
unsigned int be_list_recount(be_list_t * lst) ;


#define FOREACH_LIST(lst, item) for(be_list_item_t * item=(lst)->head; item!=NULL; item=item->next)
#define FOREACH_TYPE_LIST(lst, type, item) for(type * item=(type*)(lst)->head; item!=NULL; item=((be_list_item_t*)item)->next)

#endif