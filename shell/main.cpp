#include <iostream>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include "item.h"
#include "parser.h"
#include "compare_funcs.h"
#include "../include/hashtable.h"

void foo(List main_l, List to_del_l)
{
    item *tempItem;
    ListNode tempNode = to_del_l->list_first();
    do
    {
        tempItem = (item *)to_del_l->list_node_value(tempNode);
        tempItem->change_common_list(main_l);
        main_l->list_insert_next(main_l->list_last(), tempItem);
        tempNode = to_del_l->list_next(tempNode);
    } while (tempNode != NULL);

    delete to_del_l;
}

void print_list(List list_to_print)
{
    printf("%p\n", list_to_print);
    item *tempItem;
    ListNode tempNode = list_to_print->list_first();
    do
    {
        // printf("%p\n",tempNode);
        tempItem = (item *)list_to_print->list_node_value(tempNode);
        printf("%s->", tempItem->get_item_full_id().c_str());
        tempNode = tempNode->next;
    } while (tempNode != NULL);
    printf("NULL\n");
}

int list_cmp(Pointer l1, Pointer l2)
{
    if (l1 == l2)
        return 0;
    else
        return 1;
}



void print_commons(List visited_lists, List list_to_visit, FILE *output_file)
{
    if (visited_lists->list_find(list_to_visit, list_cmp))
        return;

    ListNode nodeOne = list_to_visit->list_first();
    item *itemOne = (item *)list_to_visit->list_node_value(nodeOne);
    ListNode nodeTwo = nodeOne;
    item *itemTwo = itemOne;
    while (nodeOne != list_to_visit->list_last())
    {
        nodeTwo=list_to_visit->list_next(nodeTwo);
        itemTwo=(item*)list_to_visit->list_node_value(nodeTwo);
        fprintf(output_file,"%s,%s\n",itemOne->get_item_full_id().c_str(),itemTwo->get_item_full_id().c_str());
        if(nodeTwo==list_to_visit->list_last())
        {
            nodeOne = list_to_visit->list_next(nodeOne);
            itemOne = (item *)list_to_visit->list_node_value(nodeOne);
            nodeTwo = nodeOne;
            itemTwo = itemOne;
        }
    }
    visited_lists->list_insert_next(visited_lists->list_last(), list_to_visit);
}

/* returns 0 for success or 1 for failure */
int read_files(string folder, HashTable htable) // folder is inner folder (ex. ebay.com) ./data/2013_camera_specs/buy.net/4233.json
{
    DIR *dir;
    dirent *dir_item;

    string name;
    int id;

    avl_tree* tree = new avl_tree();
    HashTable_Node ht_n = new hashtable_node();
    ht_n->key = new string(folder);
    ht_n->value = tree;

    dir = opendir(folder.c_str());
    dir_item = readdir(dir);
    item* it;
    List spec_list;

    while (dir_item != NULL) // for every file in dir
    {
        if (dir_item->d_type == DT_DIR) // skip . and ..
        {
            dir_item = readdir(dir);
            continue;
        }
        name = dir_item->d_name;

        spec_list = parse(name);
        if (spec_list==NULL)
        {
            delete ht_n->key;
            delete ht_n;
            delete tree;
            return 1;
        }

        it = new item(folder, atoi(name.c_str()));
        //insert speps into item
        it->set_specs(spec_list);

        //insert item into tree
        tree->insert(it, cmp_avl_insert);

        //insert item into our database
    }
    htable->insert(ht_n);
    return 0;
}

int main(int argc, char const *argv[])
{

    item a("a", 10);
    item b("b", 20);
    item c("c", 30);
    item d("d", 40);
    foo(a.get_common_list(), b.get_common_list());
    foo(c.get_common_list(), d.get_common_list());
    printf("\n\na\n");
    print_list(a.get_common_list());
    printf("\nb\n");
    print_list(b.get_common_list());
    printf("\nc\n");
    print_list(c.get_common_list());
    printf("\nd\n");
    print_list(d.get_common_list());

    foo(a.get_common_list(), c.get_common_list());

    printf("\n\na\n");
    print_list(a.get_common_list());
    printf("\nb\n");
    print_list(b.get_common_list());
    printf("\nc\n");
    print_list(c.get_common_list());
    printf("\nd\n");
    print_list(d.get_common_list());

    printf("\n\n");
    List visited_lists = new list(NULL);
    print_commons(visited_lists, a.get_common_list(), stdout);
    printf("\n\n");
    print_commons(visited_lists, b.get_common_list(), stdout);
    printf("\n\n");
    print_commons(visited_lists, c.get_common_list(), stdout);
    printf("\n\n");
    print_commons(visited_lists, d.get_common_list(), stdout);

    return 0;
}
