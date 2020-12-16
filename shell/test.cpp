#include <acutest.h>
#include "functions.h"

int lines_counter(const char *filename)
{
    FILE *fp; 
    int count = 0;
    char c;

    fp = fopen(filename, "r"); 
  
    if (fp == NULL) 
        return 0; 
  
    for (c = getc(fp); c != EOF; c = getc(fp)) 
        if (c == '\n')
            count = count + 1; 
  
    fclose(fp); 

    return count; 
}

void test_medium(void)
{
    HashTable HT = NULL;
    HashTable IDF = NULL;
    int c = read_all_folders("./data/2013_camera_specs", HT, IDF);
    if (!TEST_CHECK(HT != NULL))
        TEST_MSG("reading folders failed");
    read_csv("./data/sigmod_medium_labelled_dataset.csv", HT);
    FILE *output_file = fopen("./output_test_medium.txt", "w");
    print_all(HT, output_file);
    fclose(output_file);
    if(!TEST_CHECK(lines_counter("./output_test_medium.txt")==3582))
        TEST_MSG("incorrect output");
    delete HT;
}

void test_large(void)
{
    HashTable HT = NULL;
    HashTable IDF = NULL;
    int c = read_all_folders("./data/2013_camera_specs", HT, IDF);
    if (TEST_CHECK(HT != NULL))
        TEST_MSG("reading folders failed");
    read_csv("./data/sigmod_large_labelled_dataset.csv", HT);
    FILE *output_file = fopen("./output_test_large.txt", "w");
    print_all(HT, output_file);
    fclose(output_file);
    if(!TEST_CHECK(lines_counter("./output_test_large.txt")==42535))
        TEST_MSG("incorrect output");
    delete HT;
}

TEST_LIST = {
   { "test_medium", test_medium },
   { "test_large", test_large },
   { NULL, NULL }
};