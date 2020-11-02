#ifndef HASHTABLE
#define HASHTABLE

#include "avl_tree.h"


using namespace std;

#include "List.h"

#define  HT_SIZE 100

class hashtable
{
    List table[HT_SIZE];
    DestroyFunc destroy_value; // function that distroys an element value
    CompareFunc compare;
    hashFunc hashfunction; 
public:
    hashtable(DestroyFunc destroy_value,hashFunc hashfunction,CompareFunc compare);
    Pointer search(Pointer value);
    void insert(Pointer value);
};

#endif