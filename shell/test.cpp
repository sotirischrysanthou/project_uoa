#include <acutest.h>
#include "functions.h"

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
    if(!TEST_CHECK(lines_counter("./output_test_medium.txt")==3582))
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
    if(!TEST_CHECK(lines_counter("./output_test_large.txt")==42535))
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
   { "test_medium", test_medium },
   { "test_large", test_large },
   { "test_parse", test_parse },
   { NULL, NULL }
};