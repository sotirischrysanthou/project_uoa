#ifndef HASHTABLE
#define HASHTABLE

#include "avl_tree.h"
#include "list.h"

using namespace std;

struct bucket_item
{
    string key;
    avl_tree tree;
};

struct bucket
{
    int count;
    bucket_item *items;
    bucket(int num)
    {
        count = 0;
        items = new bucket_item[num];
    }
    ~bucket()
    {
        delete[] items;
    }
};

class hashtable
{
    List *ht;
    int size;
    int b_max;
    
    public:
    int hash(string str);
    avl_tree* find_tree(string str);
    void insert(list_node* rec_l_node, string str, int i);
    int search(int i, string disease="", string from="", string to="", string country="");
    string gettopk(int k, string disease, string country, string from, string to);
    string getAdmDis(string disease, string from, string to, string country="");
    hashtable(int num, int b_num)
    {
        size=num;
        b_max=b_num/(sizeof(string)+sizeof(avl_tree));
        ht=new List[size];
    }
    ~hashtable()
    {
        delete[] ht;
    }
};

#endif