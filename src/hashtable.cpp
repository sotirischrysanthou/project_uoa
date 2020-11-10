#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>


#include "../include/hashtable.h"

using namespace std;

hashtable::hashtable(DestroyFunc destroy_value,hashFunc hashfunction,CompareFunc compare)
{
    this->destroy_value=destroy_value;
    this->hashfunction=hashfunction;
    this->compare=compare;

    for (int i = 0; i < HT_SIZE; i++)
    {
        table[i]=new list(destroy_value);
    }
}

hashtable::~hashtable()
{
    for (int i = 0; i < HT_SIZE; i++)
    {
        delete table[i];
    }
}

Pointer hashtable::search(Pointer key)
{
    List chain=table[hashfunction(key)];
    return chain->list_find(key,compare);
}

void hashtable::insert(hashtable_node* value)
{   
    List chain=table[hashfunction(value->key)];
    chain->list_insert_next(chain->list_last(),(Pointer)value);
}