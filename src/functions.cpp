#include "functions.h"

void same_prod(List main_l, List to_del_l)
{
    if (main_l == to_del_l)
        return;
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

void destroy_item(Pointer value)
{
    delete (item *)value;
}

void del_item_ht(Pointer value)
{
    delete (int*)((HashTable_Node)value)->key;
    delete (item *)((HashTable_Node)value)->value;
    delete (HashTable_Node)value;
}

int hashfunction_int(Pointer key)
{
    return (*(int *)key) % HT_SIZE;
}

void read_files(string main_folder, string folder, HashTable htable) // folder is inner folder (ex. ebay.com) ./data/2013_camera_specs/buy.net/4233.json
{
    DIR *dir;
    dirent *dir_item;

    string name;

    // avl_tree *tree = new avl_tree(destroy_item);
    HashTable ht_items = new hashtable(del_item_ht, hashfunction_int);
    HashTable_Node ht_n = new hashtable_node();
    ht_n->key = new string(folder);
    ht_n->value = ht_items;

    dir = opendir((main_folder + "/" + folder).c_str());
    dir_item = readdir(dir);
    item *it;
    List spec_list=NULL;
    int counter = 0;
    while (dir_item != NULL) // for every file in dir
    {
        if (dir_item->d_type == DT_DIR) // skip "." and ".."
        {
            dir_item = readdir(dir);
            continue;
        }
        name = dir_item->d_name;
        spec_list = parse(main_folder + "/" + folder + "/" + dir_item->d_name);
        it = new item(folder, atoi(name.erase(name.size() - 5).c_str()), spec_list);
        clock_t t;
        HashTable_Node ht_item_n = new hashtable_node();
        ht_item_n->key = new int(it->get_item_id());
        ht_item_n->value = it;
        ht_items->insert(ht_item_n);

        dir_item = readdir(dir);
    }
    htable->insert(ht_n);
    closedir(dir);
}

int hashfunction(Pointer key)
{
    string str = *(string *)key;
    uint hash = 15, M = 14;
    for (uint i = 0; i < str.size(); ++i)
        hash = M * hash + str[i];
    return hash % HT_SIZE;
}

void del_main_ht(Pointer value)
{
    delete (string *)((HashTable_Node)value)->key;
    delete (HashTable)((HashTable_Node)value)->value;
    delete (HashTable_Node)value;
}

HashTable read_all_folders(string dir_name)
{
    HashTable ht = new hashtable(del_main_ht, hashfunction);
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
        read_files(dir_name, de->d_name, ht);
    }
    closedir(dr);
    return ht;
}



void read_csv(string filename, HashTable ht)
{
    AvlTree tree;
    HashTable ht_items;
    item *it1, *it2;
    string name1, name2, line;
    int num1, num2, similar, comma1, comma2, slash1, slash2;
    size_t buffer_size = 0;
    char *buffer = NULL;
    FILE *stream = fopen(filename.c_str(), "r");
    if (!stream)
    {
        printf("Error reading file: %s\n", filename.c_str());
        return;
    }
    getline(&buffer, &buffer_size, stream);
    int c = 0;
    while (getline(&buffer, &buffer_size, stream) != -1)
    {
        line = buffer;
        slash1 = line.find("//");
        comma1 = line.find(',', slash1);
        slash2 = line.find("//", comma1);
        comma2 = line.find(',', slash2);

        name1 = line.substr(0, slash1);
        num1 = stoi(line.substr(slash1 + 2, comma1 - slash1 + 2));
        name2 = line.substr(comma1 + 1, slash2 - comma1 + 1 - 2);
        num2 = stoi(line.substr(slash2 + 2, comma2 - slash2 + 2));
        similar = stoi(line.substr(comma2 + 1));

        // printf("%-25s // %-10d ---- %-25s // %-5d ---- %d\n",name1.c_str(), num1, name2.c_str(), num2, similar);

        if (similar)
        {
            ht_items = (HashTable)ht->search(&name1, cmp_hashtable_search);
            it1 = (item *)ht_items->search(&num1, cmp_hashtable_search_item);
            ht_items = (HashTable)ht->search(&name2, cmp_hashtable_search);
            it2 = (item *)ht_items->search(&num2, cmp_hashtable_search_item);

            // printf("item1: %s  ------------ item2: %s \n", it1->get_item_full_id().c_str(), it2->get_item_full_id().c_str());

            same_prod(it1->get_common_list(), it2->get_common_list());
        }
    }
    free(buffer);
    fclose(stream);
}

List visited_lists;
FILE *output;

void destroy_list(Pointer value)
{
    delete (List)value;
}

void print_all_commons(Pointer value)
{
    // printf("%s\n",((item*)value)->get_item_full_id().c_str());
    print_commons(visited_lists, ((item *)value)->get_common_list(), output);
}

void print_all(HashTable ht, FILE *output_file)
{
    visited_lists = new list(destroy_list);
    output = output_file;
    List hts = ht->return_list();
    ListNode tempNode = hts->list_first();
    while (tempNode != NULL)
    {
        List items_list = ((HashTable)tempNode->value)->return_list();
        ListNode tempNode1 = items_list->list_first();
        while (tempNode1 != NULL)
        {
            print_all_commons(tempNode1->value);
            tempNode1 = items_list->list_next(tempNode1);
        }
        tempNode = hts->list_next(tempNode);
        delete items_list;
    }
    delete hts;
    delete visited_lists;
}
