#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>

#include "../include/hashtable.h"

using namespace std;

hashtable::hashtable(DestroyFunc destroy_value, hashFunc hashfunction)
{
    this->destroy_value = destroy_value;
    this->hashfunction = hashfunction;

    for (int i = 0; i < HT_SIZE; i++)
    {
        table[i] = new list(destroy_value);
    }
}

hashtable::~hashtable()
{
    for (int i = 0; i < HT_SIZE; i++)
    {
        delete table[i];
    }
}

Pointer hashtable::search(Pointer key, CompareFunc compare)
{
    List chain = table[hashfunction(key)];
    HashTable_Node temp = (HashTable_Node)chain->list_find(key, compare);
    return temp->value;
}

void hashtable::insert(hashtable_node *value)
{
    List chain = table[hashfunction(value->key)];
    chain->list_insert_next(chain->list_last(), (Pointer)value);
}

List hashtable::return_list()
{
    List l = new list(NULL); 
    HashTable_Node temp;
    for (int i = 0; i < HT_SIZE; i++)
    {
        ListNode tempNode = table[i]->list_first();
        while (tempNode != NULL)
        {
            temp = (HashTable_Node)(table[i]->list_node_value(tempNode));
            l->list_insert_next(l->list_last(), temp->value);
            tempNode = table[i]->list_next(tempNode);
        }
    }
    return l;
}