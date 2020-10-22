#pragma once 

#include "common_types.h"

#define LIST_BOF (ListNode)0
#define LIST_EOF (ListNode)0


typedef class list* List;
typedef struct list_node* ListNode;

struct list_node
{
	ListNode next; // pointer to next node
	Pointer value;
};


class list
{
	ListNode dummy;			   // before the first node of list
	ListNode last;			   // pointer to the last node, or to dummy if list is empty
	int size;				   // size of list, list_size() will be Ο(1)
	DestroyFunc destroy_value; // function that distroys an element value
public:
    // Creates and return a new list.
    // If destroy_value != NULL, the it's calling destroy_value(value)
    // every time that we want to remove an element.
	list(DestroyFunc destroy_value);
    
    // Frees the list
    ~list();

    // Returns the number of list's elements.
    int list_size();

    // Adds new_node next to node, or at the bigin of list if node == LIST_BOF.
    void list_insert_next(ListNode node, Pointer value);
    
    // Removes next node of node, of first node if node == LIST_BOF.
    void list_remove_next(ListNode node);
    
    
    // Returns the first node that it's content is equal with value
    // orNULL if doesn't exist
    Pointer list_find(Pointer value, CompareFunc compare);



    // Traversing the list /////////////////////////////////////////////

    
    // Returns the first or LIST_BOF if list is Empty
    ListNode list_first();
    
    // Returns the last or LIST_BOF if list is Empty
    ListNode list_last();
    
    // Returns node's next or LIST_EOF if node is the last one
    ListNode list_next(ListNode node);
    
    // Returns node's content
    Pointer list_node_value(ListNode node);
    
    // Returns node's content that is equal with value
    ListNode list_find_node(Pointer value, CompareFunc compare);
};


