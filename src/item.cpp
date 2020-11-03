#include "item.h"

item::item(string fldr, int i):folder(fldr), id(i)
{
    common_list=new list(NULL);
    common_list->list_insert_next(LIST_BOF,this);
    
    specs=new list(NULL);

    printf("Debug!!%s//%d   %d  \n",folder.c_str(),id,common_list->list_size());
}

List item::get_common_list()
{
    return common_list;
}

string item::get_item_id()
{
    return folder+to_string(id);
}

void item::change_common_list(List new_list)
{
    common_list=new_list;
}