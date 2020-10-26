#include <iostream>
#include <cstdio>
#include <sstream>
#include <string>

#include "avl_tree.h"
#include "list.h"
#include "hashtable.h"

using namespace std;

int hashtable::hash(string str)
{
    unsigned int sum=0;
    int res;
    for(int i=0;i<str.length();i++)
        sum+=int(str[i])*i/(i+10);
    res = sum%size;
    return res;
}

avl_tree* hashtable::find_tree(string str)
{
    int i;
    avl_tree *tree;
    bucket* buck;
    int h = hash(str);
    if (ht[h].get_count()==0)
        return NULL;
    list_node *curr = ht[h].get_head();
    while (curr!=NULL)
    {
        //buck = (bucket*)ht[h].get_data(curr);
        buck = (bucket*)curr->data;
        for(i=0;i<buck->count;i++)
        {
            if(buck->items[i].key.compare(str)==0)
                return &(buck->items[i].tree);
        }
    curr=curr->next;
    }
    return NULL;
}

void hashtable::insert(list_node *rec_l_node, string str, int i) /* i=0 for entries, i=1 for exits */
{
    /* find tree */
    avl_tree *tree = find_tree(str);
    if (tree!=NULL) /* tree exists */
    {
        tree->ins(rec_l_node, i);
        return;
    }
    int h;
    record* rec = (record*)rec_l_node->data;
    h = hash(str);
    if(ht[h].get_count()==0) /* no buckets here */
    {
        bucket *buck;
        buck = new bucket(b_max);
        buck->items[0].key = str;
        buck->items[0].tree.ins(rec_l_node, i);
        buck->count++;
        ht[h].insert(buck);
    }
    else
    {
        bucket* last = (bucket*)ht[h].get_tail()->data;
        if(last->count==b_max) /* last bucket is full */
        {
            bucket *buck;
            buck = new bucket(b_max);
            buck->items[0].key = str;
            buck->items[0].tree.ins(rec_l_node, i);
            buck->count++;
            ht[h].insert(buck);
        }
        else /* room in last bucket */
        {
            last->items[last->count].key = str;
            last->items[last->count].tree.ins(rec_l_node, i);
            last->count++;
        }
    }
}

int hashtable::search(int i, string disease, string from, string to, string country)
{
    int indicator;
    avl_tree *tree;
    if (i==0 || i==1 ||i==4)
    {
        /* all diseases */
        list_node *curr;
        bucket *buck;
        int j,k;
        int total=0;
        for (j=0;j<size;j++) /* all hashtable cells */
        {
            curr = ht[j].get_head();
            while (curr!=NULL) /* all buckets */
            {
                buck = (bucket*)curr->data;
                for(k=0;k<buck->count;k++) /* all bucket items */
                {
                    tree = &(buck->items[k].tree);
                    switch (i)
                    {
                    case 0: /* all patients */
                        indicator = 0;
                        // cout <<buck->items[k].key<<" ";
                        break;
                    case 1: /* patients from to */
                        indicator = 1;
                        // cout <<buck->items[k].key<<" ";
                        break;
                    case 4: /* patients still in hospital */
                        indicator = 3;
                        // cout <<buck->items[k].key<<" ";
                        break;
                    default:
                        break;
                    }
                    total+=tree->search(indicator, from, to, country);
                }
                curr = curr->next;
            }

        }
        return total;
    }
    else if (i==2 || i==3 || i==5)
    {
        /* find disease */
        tree = find_tree(disease);
        if(tree==NULL)
            return -1;
        switch (i)
        {
        case 2: /* for disease patients from to */
            indicator = 1;
            // cout <<disease<<" ";
            break;
        case 3: /* for disease patients from to in country */
            indicator = 2;
            // cout <<disease<<" ";
            break;
        case 5: /* for disease patients still in hospital */
            indicator = 3;
            // cout <<disease<<" ";
            break;
        default:
            break;
        }
        return tree->search(indicator, from, to, country);
    }
}

string hashtable::gettopk(int k, string disease, string country, string from, string to)
{
    avl_tree *tree = find_tree(disease);
    if(tree==NULL)
    {
        string msg="ERROR";
        return msg;
    }
    return tree->getk(k, country, from, to);
}

/* return the string for numPatientAdmissions and numPatientDischarges */
string hashtable::getAdmDis(string disease, string from, string to, string country)
{
    string result;
    avl_tree *tree = find_tree(disease);
    if(tree==NULL)
    {
        result="ERROR";
        return result;
    }
    result=tree->searchAD(from, to, country);
    return result;
}