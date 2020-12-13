#include "item.h"

int hashfunction_address(Pointer value)
{
    return (long int)value % HT_SIZE;
}

item::item(string fldr, int i, List spec_list) : folder(fldr), id(i), specs(spec_list)
{

    common_and_uncommon.common = new list(NULL);
    common_and_uncommon.uncommon = new hashtable(NULL,hashfunction_address);
    common_and_uncommon.common->list_insert_next(LIST_BOF, this);

    // printf("Debug!!%s//%d   %d  \n",folder.c_str(),id,common_list->list_size());
}

item::~item()
{
    if (specs != NULL)
        delete specs;
    // delete common_and_uncommon.uncommon;
}

List item::get_common_list()
{
    return common_and_uncommon.common;
}
HashTable item::get_uncommon_list()
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
