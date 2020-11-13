#include "functions.h"

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
        nodeTwo = list_to_visit->list_next(nodeTwo);
        itemTwo = (item *)list_to_visit->list_node_value(nodeTwo);
        fprintf(output_file, "%s,%s\n", itemOne->get_item_full_id().c_str(), itemTwo->get_item_full_id().c_str());
        if (nodeTwo == list_to_visit->list_last())
        {
            nodeOne = list_to_visit->list_next(nodeOne);
            itemOne = (item *)list_to_visit->list_node_value(nodeOne);
            nodeTwo = nodeOne;
            itemTwo = itemOne;
        }
    }
    visited_lists->list_insert_next(visited_lists->list_last(), list_to_visit);
}

void read_files(string main_folder,string folder, HashTable htable) // folder is inner folder (ex. ebay.com) ./data/2013_camera_specs/buy.net/4233.json
{
    DIR *dir;
    dirent *dir_item;

    string name;

    avl_tree* tree = new avl_tree();
    HashTable_Node ht_n = new hashtable_node();
    ht_n->key = new string(folder);
    ht_n->value = tree;

    dir = opendir((main_folder+"/"+folder).c_str());
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
        spec_list = parse(main_folder+"/"+folder+"/"+dir_item->d_name);
        
        it = new item(folder, atoi(name.erase(name.size()-5).c_str()),spec_list);
        
        //insert item into tree
        tree->insert(it, cmp_avl_insert);

        //insert item into our database

        dir_item = readdir(dir);
    }
    htable->insert(ht_n);
}

int hashfunction(Pointer key)
{
    string str=*(string*)key;
    uint hash = 15, M = 14;
    for (uint i = 0; i < str.size(); ++i)
        hash = M * hash + str[i];
    return hash % HT_SIZE;
}

void destroy(Pointer value)
{
    delete (HashTable_Node)value;
}

HashTable read_all_folders(string dir_name)
{
    HashTable ht=new hashtable(destroy,hashfunction);
    struct dirent *de;
    DIR *dr = opendir(dir_name.c_str());
    if (!dr)
    {
        perror(dir_name.c_str());
        return NULL;
    }
    while ((de = readdir(dr)) != NULL)
    {
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
            continue;
        read_files(dir_name,de->d_name,ht);
    }
    return ht;
}

void read_csv()
{}