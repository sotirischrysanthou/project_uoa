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


void foo(List main_l, List to_del_l);
void print_list(List list_to_print);
void print_commons(List visited_lists, List list_to_visit, FILE *output_file);
HashTable read_all_folders(string dir_name);
void read_csv(string filename, HashTable ht);
