#include <acutest.h>
#include "functions.h"


void test_list(void)
{
    List l = new list(NULL);
    int i=10;
    l->list_insert_next(NULL,&i);
    if(!TEST_CHECK(l->list_size()==1))
        TEST_MSG("incorrect list size");
    if(!TEST_CHECK(*(int*)(l->list_first()->value)==10))
        TEST_MSG("incorrect list_node value");
    delete l;
}

void test_ht(void)
{
    HashTable ht = new hashtable(10,NULL,hashfunction);
    string strk = "mykey";
    string strv = "myvalue";
    HashTable_Node htn = new hashtable_node;
    htn->key=&strk;
    htn->value=&strv;
    ht->insert(htn);
    if(!TEST_CHECK(ht->ht_size()==1))
        TEST_MSG("incorrect hashtable size");
    string strr = *(string*)(ht->search(&strk, cmp_hashtable_search));
    if(!TEST_CHECK(strr.compare(strv)==0))
        TEST_MSG("incorrect hashtable_node value");
}

void test_small(void)
{
    HashTable HT = new hashtable(10,del_main_ht, hashfunction);
    HashTable IDF = NULL;
    // IDF = new hashtable(500,NULL, hashfunction);
    int c = read_all_folders("./data/test_small_data", HT, IDF);
    if (!TEST_CHECK(HT != NULL))
        TEST_MSG("reading folders failed");
    read_csv("./data/sigmod_small_labelled_dataset.csv", HT);
    // set_Bow_or_TfIdf(HT, IDF, c, 1);
    FILE *output_file = fopen("./output_test_small.txt", "w");
    print_all(HT, output_file);
    fclose(output_file);
    if(!TEST_CHECK(lines_counter("./output_test_small.txt")==28))
        TEST_MSG("incorrect output");
    delete HT;
    // delete IDF;
    c++;
}

void test_medium(void)
{
    HashTable HT = new hashtable(10,del_main_ht, hashfunction);
    HashTable IDF = NULL;
    // IDF = new hashtable(500,NULL, hashfunction);
    int c = read_all_folders("./data/2013_camera_specs", HT, IDF);
    if (!TEST_CHECK(HT != NULL))
        TEST_MSG("reading folders failed");
    read_csv("./data/sigmod_medium_labelled_dataset.csv", HT);
    // set_Bow_or_TfIdf(HT, IDF, c, 1);
    FILE *output_file = fopen("./output_test_medium.txt", "w");
    print_all(HT, output_file);
    fclose(output_file);
    if(!TEST_CHECK(lines_counter("./output_test_medium.txt")==46665))
        TEST_MSG("incorrect output");
    delete HT;
    // delete IDF;
    c++;
}

void test_large(void)
{
    HashTable HT = new hashtable(10,del_main_ht, hashfunction);
    HashTable IDF = NULL;
    // IDF = new hashtable(500,NULL, hashfunction);
    int c = read_all_folders("./data/2013_camera_specs", HT, IDF);
    if (TEST_CHECK(HT != NULL))
        TEST_MSG("reading folders failed");
    // set_Bow_or_TfIdf(HT, IDF, c, 1);
    read_csv("./data/sigmod_large_labelled_dataset.csv", HT);
    FILE *output_file = fopen("./output_test_large.txt", "w");
    print_all(HT, output_file);
    fclose(output_file);
    if(!TEST_CHECK(lines_counter("./output_test_large.txt")==341929))
        TEST_MSG("incorrect output");
    delete HT;
    // delete IDF;
    c++;
}
void test_parse(void)
{
    List res;
    printf("Printing buy.net/4266.json in parsed form. Open file to check content.\n");
    res = parse("./data/2013_camera_specs/buy.net/4266.json");
    print_json(res);
}

TEST_LIST = {
   { "test_list", test_list },
   { "test_ht", test_ht },
   { "test_small", test_small },
   { "test_medium", test_medium },
   { "test_large", test_large },
   { "test_parse", test_parse },
   { NULL, NULL }
};