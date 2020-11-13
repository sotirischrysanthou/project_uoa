#include "List.h"
using namespace std;

// int itemHashFunc(Pointer item,int TABLE_SIZE)
// {

// }

// struct spec
// {
//     string s_name;
//     char info_type;
//     string s_info;
// };

class item
{
    string folder;
    int id;
    List common_list;
    List specs;
public:
    item(string fldr, int i, List spec_list);
    List get_common_list();
    int get_item_id();
    string get_item_folder();
    string get_item_full_id();
    void change_common_list(List new_list);

};