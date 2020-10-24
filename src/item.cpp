#include "item.h"

item::item()
{
    common_list=new list(NULL);
    common_list->list_insert_next(LIST_BOF,this);
}

List item::get_common_list()
{
    return common_list;
}