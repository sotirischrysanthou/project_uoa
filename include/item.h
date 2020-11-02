#include "List.h"
using namespace std;

int itemHashFunc(Pointer item,int TABLE_SIZE)
{

}

class item
{
    string id;
    List common_list;
public:
    item(string item_id);
    List get_common_list();
    string get_item_id();
    void change_common_list(List new_list);
};