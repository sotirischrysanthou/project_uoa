#ifndef ITEM
#define ITEM
#include "List.h"
#include "hashtable.h"
#include <string>
using namespace std;

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
public:
    item(string fldr, int i, List spec_list);
    ~item();
    /* Returns a list of items */
    List get_common_list();
    /* Reterns a list of lists that contain items(list of common lists of uncommon items) */
    HashTable get_uncommon_list();
    connections get_common_and_uncommon();
    int get_item_id();
    string get_item_folder();
    string get_item_full_id();
    void change_common_list(List new_list);

};

#endif