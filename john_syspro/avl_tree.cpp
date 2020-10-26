#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>

#include "avl_tree.h"
#include "list.h"

using namespace std;

int Max(int a, int b)
{
    if(a>b)
        return a;
    return b;
}

typedef struct tm tm;

/* gives the height of a node */
int avl_tree::get_height(avl_node *node)
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
int avl_tree::difference(avl_node *node)
{
    int left_height, right_height, balance_factor;
    left_height = get_height(node->left);
    right_height = get_height(node->right);
    balance_factor = left_height - right_height;
    return balance_factor;
}

/* right - right rotation */
avl_node *avl_tree::right_right(avl_node *parent)
{
    avl_node *temp;
    temp = parent->right;
    parent->right = temp->left;
    temp->left = parent;
    return temp;
}

/* left - left rotation */
avl_node *avl_tree::left_left(avl_node *parent)
{
    avl_node *temp;
    temp = parent->left;
    parent->left = temp->right;
    temp->right = parent;
    return temp;
}

/* left - right rotation (right-right on left then left-left on parent) */
avl_node *avl_tree::left_right(avl_node *parent)
{
    avl_node *temp;
    temp = parent->left;
    parent->left = right_right(temp);
    return left_left(parent);
}

/* right - left rotation (left-left on right then right-right on parent) */
avl_node *avl_tree::right_left(avl_node *parent)
{
    avl_node *temp;
    temp = parent->right;
    parent->right = left_left(temp);
    return right_right(parent);
}

/* balance the tree */
avl_node *avl_tree::balance(avl_node *node)
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

avl_node *avl_tree::insert(avl_node *curr, time_t value, list_node *l_node)
{
    if (curr == NULL) /* the node doesn't exist */
    {
        /* a new node is allocated */
        curr = new avl_node(value, l_node);
        return curr;
    }
    else if (value == curr->data) /* the node was found */
    {
        /* insertion in the List that every node has */
        curr->records.insert(l_node->data);
    }
    else if (value < curr->data)
    {
        /* insert left and balance */
        curr->left = insert(curr->left, value, l_node);
        curr = balance(curr);
    }
    else if (value > curr->data)
    {
        /* insert right and balance */
        curr->right = insert(curr->right, value, l_node);
        curr = balance(curr);
    }
    return curr;
}

/* calls insert on the root */
void avl_tree::ins(list_node *l_node, int i) /* i=0 for entries, i=1 for exits */
{
    time_t value;
    record* r;
    r=(record*)l_node->data;
    if (i==0)
        value = r->entryDate;
    else
        value = r->exitDate;
    root = insert(root, value, l_node);
}

void avl_tree::del(avl_node *temp) {
   if (temp == NULL)
      return;
    del(temp->left);
    del(temp->right);
    delete temp;
}

/* get the last (largest) value in the tree */
time_t avl_tree::get_last(avl_node *node)
{
    if (node->right==NULL)
        return node->data;
    return get_last(node->right);
}

/* ex=0 for all patients, ex=1 for current patients */
int avl_tree::rec_search(avl_node *node, time_t from, time_t to, int ex, string country, int* table)
{
    int sum = 0;
    if (node!=NULL)
    {
        if (node->data > from)
            sum += rec_search(node->left, from, to, ex, country, table);
        if (node->data >= from && node->data <= to)
        {
            /* if ex=1, we are looking for current patients */
            if (ex==1)
            {
                list_node *curr = node->records.get_head();
                while (curr!=NULL)
                {
                    if (((record*)curr->data)->exitDate==0)
                        sum++;
                    curr = curr->next;
                }
            }
            else if (country.length()>0)
            {
                list_node *curr = node->records.get_head();
                while (curr!=NULL)
                {
                    if (((record*)curr->data)->country.compare(country)==0)
                    {
                        sum++;
                        if(table!=NULL)
                        {
                            if(((record*)curr->data)->age<=20)
                                table[0]++;
                            else if(((record*)curr->data)->age<=40)
                                table[1]++;
                            else if(((record*)curr->data)->age<=40)
                                table[2]++;
                            else
                                table[3]++;
                        }
                    }
                    curr = curr->next;
                }
            }
            else
            {
                sum+=node->records.get_count();
            }
        }
        if (node->data < to)
            sum += rec_search(node->right, from, to, ex, country, table);
    }
    return sum;
}

/* i is the indicator */
int avl_tree::search(int i, string from, string to, string country)
{
    int ex=0;
    time_t f, t;
    switch (i)
    {
    case 0: /* find all patients */
        f=0;
        t=get_last(root);
        country="";
        break;
    case 1: /* find all patients from f to t */
        f=DMYtoTIME(from);
        t=DMYtoTIME(to);
        country="";
        break;
    case 2: /* find all patients from f to t in a specific country */
        f=DMYtoTIME(from);
        t=DMYtoTIME(to);
        break;
    case 3: /* find all patients that are still in a hospital */
        f=0;
        t=get_last(root);
        ex=1;
        country="";
        break;
    default:
        return -1;
        break;
    }
    return rec_search(root, f, t, ex, country);
}

// int rec_getk(int k, string country, string from, string to, int* table)
// {}

string avl_tree::getk(int k, string country, string from, string to)
{
    string result="";
    int table[4];
    int i,j,top,pos;
    for (i = 0; i < 4; i++)
        table[i]=0;
    time_t f = DMYtoTIME(from);
    time_t t = DMYtoTIME(to);
    int total = rec_search(root, f, t, 0, country, table);
    for (i = 0; i < 4; i++)
        table[i]=table[i]*100/total;
    for(i=0;i<k && i<4;i++)
    {
        top=-1;
        for(j=0;j<4;j++)
            if(table[j]>top)
            {
                pos=j;
                top=table[j];
            }
        if(pos==0)
            result+="0-20: "+to_string(top)+"%\n";
        else if(pos==1)
            result+="21-40: "+to_string(top)+"%\n";
        else if(pos==2)
            result+="41-60: "+to_string(top)+"%\n";
        else
            result+="60+: "+to_string(top)+"%\n";
        table[pos]=-1;
    }
    // cout<<result;
    return result;

}

void avl_tree::rec_searchAD(List* l,avl_node *node, time_t from, time_t to)
{
    if(node!=NULL)
    {
        if(node->data>from)
            rec_searchAD(l, node->left, from, to);
        if(node->data>=from && node->data<=to)
        {
            list_node *curr=node->records.get_head();
            while(curr!=NULL)
            {
                l->insertAD(((record*)curr->data)->country);
                curr=curr->next;
            }
        }
        if(node->data<to)
            rec_searchAD(l, node->right, from, to);
    }
}

string avl_tree::searchAD(string from, string to, string country)
{
    string result="";
    List l;
    rec_searchAD(&l, root, DMYtoTIME(from), DMYtoTIME(to));
    list_node *curr=l.get_head();
    while(curr!=NULL)
    {
        if(country.length()!=0)
        {
            if(country.compare(((CountryCount*)curr->data)->country)==0)
            {
                result = result + ((CountryCount*)curr->data)->country + ' ' + to_string(((CountryCount*)curr->data)->count) + '\n';
            }
        }
        else
        {
            result = result + ((CountryCount*)curr->data)->country + ' ' + to_string(((CountryCount*)curr->data)->count) + '\n';
        }
        
        curr=curr->next;
    }
    return result;
}