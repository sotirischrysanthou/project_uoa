#ifndef PARSER
#define PARSER

#include "common_types.h"
#include "List.h"

#define PAIRS 'p'
#define TABLE 't'
#define FLOAT 'f'
#define INTEGER 'i'
#define STRING 's'

struct j_pair
{
    char type;
    Pointer value;
    string title;
};

struct json_table_node
{
    char type;
    Pointer value;
};

json_table_node *get_number();
string get_string();
j_pair *json_pair();
List create_jList_of_pairs();
json_table_node *json_table();
List create_jTable();
List parse(string filename);

#endif