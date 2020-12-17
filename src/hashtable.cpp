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
    size = 0;
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
    HashTable_Node temp = (HashTable_Node)(chain->list_find(key, compare));

    return temp == NULL ? NULL : temp->value;
}

void hashtable::insert(hashtable_node *value)
{
    List chain = table[hashfunction(value->key)];
    chain->list_insert_next(chain->list_last(), (Pointer)value);
    size++;
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

List hashtable::return_ht_nodes()
{
    List l = new list(NULL);
    HashTable_Node temp;
    for (int i = 0; i < HT_SIZE; i++)
    {
        ListNode tempNode = table[i]->list_first();
        while (tempNode != NULL)
        {
            temp = (HashTable_Node)(table[i]->list_node_value(tempNode));
            l->list_insert_next(l->list_last(), temp);
            tempNode = table[i]->list_next(tempNode);
        }
    }
    return l;
}

int cmp(Pointer a, Pointer b)
{
    return ((ListNode)b)->next->value == a;
}

void hashtable::remove(Pointer key)
{
    List chain = table[hashfunction(key)];
    if(chain->list_size()==0)
    {
        printf("nooooooooooot\n");
        return;
    }
    ListNode tempNode = chain->list_first();
    if (tempNode->value == key)
    {
        chain->list_remove_next(NULL);
        size--;
    }
    else
    {
        while (tempNode->next != NULL && ((HashTable_Node)(tempNode->next->value))->key != key)
            tempNode = tempNode->next;
        if (tempNode->next != NULL)
        {
            chain->list_remove_next(tempNode);
            size--;
        }
    }
}

int hashtable::ht_size()
{
    return size;
}