#ifndef ITEM
#define ITEM
#include "List.h"
#include "hashtable.h"
#include <string>
using namespace std;

int hashfunction_address(Pointer value);
int hashfunction(Pointer key);

struct connections
{
    List common;
    HashTable uncommon;
};


class item
{
    string folder;  /* ex. "buy.net" */
    int id;         /* ex. 4233 */
    // List common_list; /* a list of all common items */
    connections common_and_uncommon;
    List specs; /* a list of all of the item's specs, parsed from the json file */
    HashTable words;
    int *array_BoW;
    float *array_TF_IDF;
public:
    item(string fldr, int i);
    ~item();
    /* Returns a list of items */

    /* Reterns a list of lists that contain items(list of common lists of uncommon items) */
    connections get_common_and_uncommon();
    int get_item_id();
    string get_item_folder();
    string get_item_full_id();
    void change_common_list(List new_list);
    void change_uncommon_ht(HashTable ht);
    void set_spec_list(List spec);
    List get_spec_list();
    HashTable get_words_ht();
    void set_tables(int *bow, float* tfidf);
    int * get_bow();
    float * get_tfidf();

};

#endif