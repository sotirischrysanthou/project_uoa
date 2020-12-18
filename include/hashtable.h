#pragma once
#ifndef HASHTABLE
#define HASHTABLE


#include <string>
#include "List.h"

#define  HT_SIZE 1000




typedef int (*hashFunc)(Pointer value);

typedef class hashtable* HashTable;
typedef struct hashtable_node* HashTable_Node;

struct hashtable_node
{
    Pointer key;
    Pointer value;
};

class hashtable
{
    List table[HT_SIZE];
    DestroyFunc destroy_value; // function that distroys an element value
    hashFunc hashfunction;
    int size;
public:
    hashtable(DestroyFunc destroy_value,hashFunc hashfunction);
    ~hashtable();
    Pointer search(Pointer value,CompareFunc compare);
    void insert(hashtable_node* value);
    List return_list();
    List return_ht_nodes();
    void remove(Pointer key);
    int ht_size();
};

#endif