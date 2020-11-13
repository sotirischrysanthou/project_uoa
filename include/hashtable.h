#ifndef HASHTABLE
#define HASHTABLE

#include "avl_tree.h"


using namespace std;

#include "List.h"

#define  HT_SIZE 10



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
public:
    hashtable(DestroyFunc destroy_value,hashFunc hashfunction);
    ~hashtable();
    Pointer search(Pointer value,CompareFunc compare);
    void insert(hashtable_node* value);
};

#endif