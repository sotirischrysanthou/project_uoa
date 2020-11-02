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
        table[i]=new list(NULL);
    }
}

Pointer hashtable::search(Pointer value)
{
    List chain=table[hashfunction(value)];
    return chain->list_find(value,compare);
}

void hashtable::insert(Pointer value)
{   
    List chain=table[hashfunction(value)];
    chain->list_insert_next(chain->list_last(),value);
}