#ifndef AVL_TREE
#define AVL_TREE

#include "list.h"

using namespace std;

struct avl_node
{
    time_t data;
    List records;
    avl_node *left;
    avl_node *right;
    avl_node(time_t d, list_node* l)
    {
        data=d;
        left=NULL;
        right=NULL;
        records.insert(l->data);
    }
};

class avl_tree
{
    avl_node *root;
    int get_height(avl_node *node);
    int difference(avl_node *node);
    avl_node *right_right(avl_node *parent);
    avl_node *left_left(avl_node *parent);
    avl_node *left_right(avl_node *parent);
    avl_node *right_left(avl_node *parent);
    avl_node *balance(avl_node *node);
    avl_node *insert(avl_node *curr, time_t value, list_node *l_node);
    void inorder(avl_node *temp);
    void del(avl_node *temp);
    time_t get_last(avl_node *node);
    int rec_search(avl_node *node, time_t from, time_t to, int ex, string country, int *table=NULL);
    void rec_searchAD(List *l,avl_node *node, time_t from, time_t to);

public:
    void ins(list_node *l_node, int i);
    int search(int i, string from, string to, string country="");
    string searchAD(string from, string to, string country="");
    string getk(int k, string country, string from, string to);
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