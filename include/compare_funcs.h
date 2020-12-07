#pragma once

#include "common_types.h"


int cmp_avl_search(Pointer to_find,Pointer value);
int cmp_avl_insert(Pointer to_insert,Pointer value);

int cmp_hashtable_search(Pointer to_find,Pointer value);
int cmp_hashtable_search_item(Pointer id_to_find, Pointer ht_node)
