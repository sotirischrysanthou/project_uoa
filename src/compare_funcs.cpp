#include "compare_funcs.h"
#include "item.h"
#include "../include/hashtable.h"

using namespace std;

// to_find : int*      value : item*
int cmp_avl_search(Pointer to_find,Pointer value)
{
    return (*(int*)to_find)-((item*)value)->get_item_id();
}

// to_insert:item*     value : item*
int cmp_avl_insert(Pointer to_insert,Pointer value)
{
    return ((item*)to_insert)->get_item_id()-((item*)value)->get_item_id();
}

// to_find: string     value : hashtable_node
int cmp_hashtable_search(Pointer to_find,Pointer value)
{
    return ((string*)to_find)->compare(*(string*)(((hashtable_node*)value)->key));
}

// id_to_find: int     ht_node: hashtable_node
int cmp_hashtable_search_item(Pointer id_to_find, Pointer ht_node)
{
    return (*(int *)(id_to_find)) - (*(int*)(((HashTable_Node)ht_node)->key));
}

// a: mem_address      b: hash_table_node
int cmp_hashtable_search_address(Pointer a,Pointer b)
{
    return (long int)a-(long int)(((HashTable_Node)b)->value);
}