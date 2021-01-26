#include "item.h"

int hashfunction_address(Pointer value,int buckets)
{
    return (long int)value % buckets;
}

int hashfunction(Pointer key,int buckets)
{
    string str = *(string *)key;
    uint hash = 15, M = 14;
    for (uint i = 0; i < str.size(); ++i)
        hash = M * hash + str[i];
    return hash % buckets;
}

void del_words(Pointer p)
{
    HashTable_Node ht_n=(HashTable_Node)p;
    string* s=(string*)ht_n->key;
    delete s;
    int *i=(int*)ht_n->value;
    delete i;
    delete ht_n;
}

item::item(string fldr, int i ): folder(fldr), id(i)
{
    array=NULL;
    common_and_uncommon.common = new list(NULL);
    common_and_uncommon.uncommon = new hashtable(100,NULL,hashfunction_address);
    common_and_uncommon.common->list_insert_next(LIST_BOF, this);
    words = new hashtable(50,del_words,hashfunction);

    // printf("Debug!!%s//%d   %d  \n",folder.c_str(),id,common_list->list_size());
}

item::~item()
{
    if (specs != NULL)
        delete specs;
    delete words;
    if(array)
        delete[] array;
}

connections item::get_common_and_uncommon()
{
    return common_and_uncommon;
}

int item::get_item_id()
{
    return id;
}

string item::get_item_folder()
{
    return folder;
}

string item::get_item_full_id()
{
    return folder + "//" + to_string(id);
}

void item::change_common_list(List new_list)
{
    common_and_uncommon.common = new_list;
}

void item::change_uncommon_ht(HashTable ht)
{
    // delete common_and_uncommon.uncommon;
    common_and_uncommon.uncommon = ht;
}

void item::set_spec_list(List spec)
{
    specs=spec;
}
List item::get_spec_list()
{
    return specs;
}
HashTable item::get_words_ht()
{
    return words;
}

void item::set_table(double* table)
{
    array=table;
}

double * item::get_bow_tfidf()
{
    return array;
}

