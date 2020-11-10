#ifndef HASHTABLE
#define HASHTABLE

#include "avl_tree.h"


using namespace std;

#include "List.h"

#define  HT_SIZE 100


struct hashtable_node
{
    Pointer key;
    Pointer value;
};

typedef int (*hashFunc)(Pointer value);

class hashtable
{
    List table[HT_SIZE];
    DestroyFunc destroy_value; // function that distroys an element value
    CompareFunc compare;
    hashFunc hashfunction; 
public:
    hashtable(DestroyFunc destroy_value,hashFunc hashfunction,CompareFunc compare);
    ~hashtable();
    Pointer search(Pointer value);
    void insert(hashtable_node* value);
};

#endif