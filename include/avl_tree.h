#pragma once

#ifndef AVL_TREE
#define AVL_TREE


#include "common_types.h"

using namespace std;
typedef struct avl_node* AvlNode;

struct avl_node
{
    Pointer value;
    AvlNode left;
    AvlNode right;
};

class avl_tree
{
    AvlNode root;
    int get_height(AvlNode node);
    int difference(AvlNode node);
    AvlNode right_right(AvlNode parent);
    AvlNode left_left(AvlNode parent);
    AvlNode left_right(AvlNode parent);
    AvlNode right_left(AvlNode parent);
    AvlNode balance(AvlNode node);
    AvlNode rec_insert(AvlNode curr, Pointer value, CompareFunc cmp);
    void inorder(AvlNode temp);
    void del(AvlNode temp);
    Pointer rec_search(AvlNode node, Pointer value, CompareFunc cmp);


public:
    void insert(Pointer value,CompareFunc cmp);
    Pointer search(Pointer value, CompareFunc cmp);
    avl_tree()
    {
        root=NULL;
    }
    ~avl_tree()
    {
        del(root); /* a recursive delete function is called */
    }
};


#endif