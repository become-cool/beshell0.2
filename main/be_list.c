#include "be_list.h"
#include <stdio.h>

void be_list_init(be_list_t * lst) {
    lst->head = NULL ;
    lst->tail = NULL ;
    lst->count = 0 ;
}

void be_list_append(be_list_t * lst, be_list_item_t * item) {
    
    item->prev = lst->tail;
    item->next = NULL;

    if(lst->tail) {
        lst->tail->next = item ;
    }
    lst->tail = item ;

    if(!lst->head) {
        lst->head = item ;
    }

    lst->count ++ ;
}


void be_list_remove(be_list_t * lst, be_list_item_t * item) {

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

    lst->count -- ;
}

bool be_list_is_empty(be_list_t * lst) {
    if( lst->head!=NULL && lst->tail!=NULL ){
        return 0 ;
    }
    if(lst->head!=NULL || lst->tail!=NULL) {
        printf("lst->head==NULL? %d; lst->tail==NULL? %d\n", lst->head==NULL?1:0, lst->tail==NULL?1:0 ) ;
    }
    return 1 ;
}


unsigned int be_list_recount(be_list_t * lst) {
    unsigned int cnt = 0 ;
    FOREACH_LIST(lst, item) {
        cnt ++ ;
    }
    lst->count = cnt ;
    return cnt ;
}
