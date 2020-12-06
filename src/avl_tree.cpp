#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>

#include "avl_tree.h"

using namespace std;

int Max(int a, int b)
{
    if(a>b)
        return a;
    return b;
}

typedef struct tm tm;

/* gives the height of a node */
int avl_tree::get_height(AvlNode node)
{
    int left_height, right_height, height;
    int total = 0;
    if (node != NULL)
    {
        left_height = get_height(node->left);
        right_height = get_height(node->right);
        height = Max(left_height, right_height);
        total = height + 1;
    }
    return total;
}

/* gives the total difference of a node's children */
int avl_tree::difference(AvlNode node)
{
    int left_height, right_height, balance_factor;
    left_height = get_height(node->left);
    right_height = get_height(node->right);
    balance_factor = left_height - right_height;
    return balance_factor;
}

/* right - right rotation */
AvlNode avl_tree::right_right(AvlNode parent)
{
    AvlNode temp;
    temp = parent->right;
    parent->right = temp->left;
    temp->left = parent;
    return temp;
}

/* left - left rotation */
AvlNode avl_tree::left_left(AvlNode parent)
{
    AvlNode temp;
    temp = parent->left;
    parent->left = temp->right;
    temp->right = parent;
    return temp;
}

/* left - right rotation (right-right on left then left-left on parent) */
AvlNode avl_tree::left_right(AvlNode parent)
{
    AvlNode temp;
    temp = parent->left;
    parent->left = right_right(temp);
    return left_left(parent);
}

/* right - left rotation (left-left on right then right-right on parent) */
AvlNode avl_tree::right_left(AvlNode parent)
{
    AvlNode temp;
    temp = parent->right;
    parent->right = left_left(temp);
    return right_right(parent);
}

/* balance the tree */
AvlNode avl_tree::balance(AvlNode node)
{
    int bal_factor = difference(node);
    if (bal_factor > 1) /* not balanced on the left */
    {
        if (difference(node->left) > 0) /* not balanced on the left again */
            node = left_left(node);
        else
            node = left_right(node);
    }
    else if (bal_factor < -1) /* not balanced on the right */
    {
        if (difference(node->right) > 0) /* not balanced on the left this time */
            node = right_left(node);
        else
            node = right_right(node);
    }
    return node;
}

AvlNode avl_tree::rec_insert(AvlNode curr, Pointer value,CompareFunc cmp)
{
    if (curr == NULL) /* the node doesn't exist */
    {
        /* a new node is allocated */
        curr = new avl_node;
        curr->left=NULL;
        curr->right=NULL;
        curr->value=value;
        return curr;
    }
    else if (cmp(value,curr->value)<=0)
    {
        /* insert left and balance */
        curr->left = rec_insert(curr->left, value, cmp);
        curr = balance(curr);
    }
    else
    {
        /* insert right and balance */
        curr->right = rec_insert(curr->right, value, cmp);
        curr = balance(curr);
    }
    return curr;
}

/* calls insert on the root */
void avl_tree::insert(Pointer value,CompareFunc cmp)
{
    root = rec_insert(root, value,cmp);
}

void avl_tree::del(AvlNode temp) {
   if (temp == NULL)
      return;
    del(temp->left);
    del(temp->right);
    destroy_value(temp->value);
    delete temp;
}



Pointer avl_tree::rec_search(AvlNode node, Pointer value, CompareFunc cmp)
{
    if(node==NULL)
        return NULL;
    else if(cmp(value,node->value)==0)
        return node->value;
    else if(cmp(value,node->value)<0)
        return rec_search(node->left,value,cmp);
    else
        return rec_search(node->right,value,cmp);
}

Pointer avl_tree::search(Pointer value, CompareFunc cmp)
{
    return rec_search(root, value, cmp);
}

void rec_inorder(AvlNode node,GeneralFunc func)
{
    if(node==NULL)
        return;
    rec_inorder(node->left,func);
    func(node->value);
    rec_inorder(node->right,func);
}

void avl_tree::inorder(GeneralFunc func)
{
    rec_inorder(root,func);
}

