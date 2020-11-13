#include <iostream>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "item.h"
#include "parser.h"
#include "compare_funcs.h"
#include "../include/hashtable.h"
#include "compare_funcs.h"



int main(int argc, char const *argv[])
{

    item a("a", 10,NULL);
    item b("b", 20,NULL);
    item c("c", 30,NULL);
    item d("d", 40,NULL);
    foo(a.get_common_list(), b.get_common_list());
    foo(c.get_common_list(), d.get_common_list());
    printf("\n\na\n");
    print_list(a.get_common_list());
    printf("\nb\n");
    print_list(b.get_common_list());
    printf("\nc\n");
    print_list(c.get_common_list());
    printf("\nd\n");
    print_list(d.get_common_list());

    foo(a.get_common_list(), c.get_common_list());

    printf("\n\na\n");
    print_list(a.get_common_list());
    printf("\nb\n");
    print_list(b.get_common_list());
    printf("\nc\n");
    print_list(c.get_common_list());
    printf("\nd\n");
    print_list(d.get_common_list());

    printf("\n\n");
    List visited_lists = new list(NULL);
    print_commons(visited_lists, a.get_common_list(), stdout);
    printf("\n\n");
    print_commons(visited_lists, b.get_common_list(), stdout);
    printf("\n\n");
    print_commons(visited_lists, c.get_common_list(), stdout);
    printf("\n\n");
    print_commons(visited_lists, d.get_common_list(), stdout);

    HashTable HT=read_all_folders("./data/2013_camera_specs");
    delete HT;
    return 0;
}
