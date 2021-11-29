#include "list.h"
#include <stdio.h>

void list_init(list_t * lst) {
    lst->head = NULL ;
    lst->tail = NULL ;
}

void _list_append(list_t * lst, list_item_t * item) {
    
    item->prev = lst->tail;
    item->next = NULL;

    if(lst->tail) {
        lst->tail->next = item ;
    }
    lst->tail = item ;

    if(!lst->head) {
        lst->head = item ;
    }
}

void list_append(list_t * lst, void * data) {
    list_item_t * item = malloc(sizeof(list_item_t)) ;
    if(!item) {
        return NULL ;
    }
    item->data = data ;
    _list_append(lst, item) ;
}


void _list_remove(list_t * lst, list_item_t * item) {

    if(item->prev) {
        item->prev->next = item->next ;
    }
    if(item->next) {
        item->next->prev = item->prev ;
    }

    if(lst->head==item) {
        lst->head = item->next ;
    }
    if(lst->tail==item) {
        lst->tail = item->prev ;
    }

    item->next = NULL ;
    item->prev = NULL ;
    
    free(item) ;
}

char list_remove(list_t * lst, void * data) {
    list_item_t * item = list_find(lst, data) ;
    if(item==NULL){
        return 0 ;
    }
    _list_remove(lst, item) ;
    return 1 ;
}


list_item_t * list_find(list_t * lst, void * data) {
    FOREACH_LIST(lst, item) {
        if(item->data == data) {
            return item ;
        }
    }
    return NULL ;
}

char list_is_empty(list_t * lst) {
    if( lst->head!=NULL && lst->tail!=NULL ){
        return 0 ;
    }
    if(lst->head!=NULL || lst->tail!=NULL) {
        printf("lst->head==NULL? %d; lst->tail==NULL? %d\n", lst->head==NULL?1:0, lst->tail==NULL?1:0 ) ;
    }
    return 1 ;
}


unsigned int list_count(list_t * lst) {
    unsigned int cnt = 0 ;
    FOREACH_LIST(lst, item) {
        cnt ++ ;
    }
    return cnt ;
}
