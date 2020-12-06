#include "item.h"

item::item(string fldr, int i,List spec_list):folder(fldr), id(i) ,specs(spec_list)
{

    common_list=new list(NULL);
    common_list->list_insert_next(LIST_BOF,this);

    
    // printf("Debug!!%s//%d   %d  \n",folder.c_str(),id,common_list->list_size());
}

item::~item()
{   
    //delete common_list;
    if(specs!=NULL)
        delete specs;
}

List item::get_common_list()
{
    return common_list;
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
    return folder+"//"+to_string(id);
}

void item::change_common_list(List new_list)
{
    common_list=new_list;
}

