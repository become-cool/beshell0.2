#ifndef __H_LIST__
#define __H_LIST__

#include <stdlib.h>

typedef struct list_item {
    struct list_item * prev ;
    struct list_item * next ;
    void * data ;
} list_item_t;

typedef struct {
    list_item_t * head ;
    list_item_t * tail ;
} list_t;

void list_init(list_t * lst) ;
void list_append(list_t * lst, void * data) ;
char list_remove(list_t * lst, void * data) ;
char list_is_empty(list_t * lst) ;
unsigned int list_count(list_t * lst) ;

list_item_t * list_find(list_t * lst, void * data) ;

#define FOREACH_LIST(lst, item) for(list_item_t * item=(lst)->head; item!=NULL; item=item->next) 

#endif