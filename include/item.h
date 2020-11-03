#include "List.h"
using namespace std;

int itemHashFunc(Pointer item,int TABLE_SIZE)
{

}

class item
{
    string folder;
    int id;
    List common_list;
    List specs;
public:
    item(string fldr, int i);
    List get_common_list();
    string get_item_id();
    void change_common_list(List new_list);
};