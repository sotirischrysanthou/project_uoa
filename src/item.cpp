#include "item.h"

item::item(string item_id):id(item_id)
{
    common_list=new list(NULL);
    common_list->list_insert_next(LIST_BOF,this);
    printf("Debug!!%s   %d  \n",id.c_str(),common_list->list_size());
}

List item::get_common_list()
{
    return common_list;
}

string item::get_item_id()
{
    return id;
}

void item::change_common_list(List new_list)
{
    common_list=new_list;
}