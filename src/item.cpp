#include "item.h"

item::item(string fldr, int i, List spec_list) : folder(fldr), id(i), specs(spec_list)
{

    common_and_uncommon.common = new list(NULL);
    common_and_uncommon.uncommon = new list(NULL);
    common_and_uncommon.common->list_insert_next(LIST_BOF, this);

    // printf("Debug!!%s//%d   %d  \n",folder.c_str(),id,common_list->list_size());
}

item::~item()
{
    //delete common_list;
    if (specs != NULL)
        delete specs;
}

List item::get_common_list()
{
    return common_and_uncommon.common;
}
List item::get_uncommon_list()
{
    return common_and_uncommon.uncommon;
}

connections item::get_common_and_uncommon()
{
    return common_and_uncommon;
}

int item::get_item_id()
{
    return id;
}

string item::get_item_folder()
{
    return folder;
}

string item::get_item_full_id()
{
    return folder + "//" + to_string(id);
}

void item::change_common_list(List new_list)
{
    common_and_uncommon.common = new_list;
}
