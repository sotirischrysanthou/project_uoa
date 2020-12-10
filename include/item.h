#ifndef ITEM
#define ITEM
#include "List.h"
#include <string>
using namespace std;


class item
{
    string folder; /* ex. "buy.net" */
    int id; /* ex. 4233 */
    List common_list; /* a list of all common items */
    List specs; /* a list of all of the item's specs, parsed from the json file */
public:
    item(string fldr, int i, List spec_list);
    ~item();
    List get_common_list();
    int get_item_id();
    string get_item_folder();
    string get_item_full_id();
    void change_common_list(List new_list);

};

#endif