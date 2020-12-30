
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "List.h"
#include"item.h"



list::list(DestroyFunc destroy_value)
{
	// Create stuct
	size = 0;
	this->destroy_value = destroy_value;

	dummy = new list_node;
	dummy->next = NULL;

	last = dummy;
}


list::~list()
{
	ListNode node = dummy;
	while (node != NULL)
	{
		ListNode next = node->next;

		// call destroy_value, if exists
		if (node != dummy && destroy_value != NULL)
		{
			destroy_value(node->value);
		}
		delete node;
		node = next;
	}
}


int list::list_size()
{
	return size;
}

void list::list_insert_next(ListNode node, Pointer value)
{
	// if the node is NULL just insert next to dummy node
	if (node == NULL)
	{
		node = dummy;
	}
	// Creation of new node
	ListNode new_node = new list_node;
	new_node->value = value;

	new_node->next = node->next;
	node->next = new_node;

	// Update size & last
	size++;
	if (last == node)
		last = new_node;
}

void list::list_remove_next(ListNode node)
{
	// if node is NULL just delete dummy->next!
	if (node == NULL)
		node = dummy;

	ListNode removed = node->next;
	assert(removed != NULL); 

	if (destroy_value != NULL)
		destroy_value(removed->value);

	node->next = removed->next;

	free(removed);

	// Update size & last
	size--;
	if (last == removed)
		last = node;
}

Pointer list::list_find(Pointer value, CompareFunc compare)
{
	ListNode node = this->list_find_node(value, compare);
	return node == NULL ? NULL : node->value;
}


// Traversing the list /////////////////////////////////////////////

ListNode list::list_first()
{
	// first is dummy's next.

	return dummy->next;
}

ListNode list::list_last()
{

	if (last == dummy)
		return LIST_EOF; // list empty
	else
		return last;
}

ListNode list::list_next(ListNode node)
{
	assert(node != NULL);
	return node->next;
}

Pointer list::list_node_value(ListNode node)
{
	assert(node != NULL);
	return node->value;
}

ListNode list::list_find_node(Pointer value, CompareFunc compare)
{
	for (ListNode node = dummy->next; node != NULL; node = node->next)
		if (compare(value, node->value) == 0)
			return node; 

	return NULL;
}