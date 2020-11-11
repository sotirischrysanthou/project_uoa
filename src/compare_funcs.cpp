#include "compare_funcs.h"
#include "../include/hashtable.h"
#include "item.h"

// to_find : int*      value : item*
int cmp_avl_search(Pointer to_find,Pointer value)
{
    return (*(int*)to_find)-((item*)value)->get_item_id();
}

// to_insert:item*     value : itme*
int cmp_avl_insert(Pointer to_insert,Pointer value)
{
    return ((item*)to_insert)->get_item_id()-((item*)value)->get_item_id();
}

// to_find: string     value : hashtable_node
int cmp_hashtable_search(Pointer to_find,Pointer value)
{
    ((string*)to_find)->compare(*(string*)(((hashtable_node*)value)->key));
}