#ifndef PARSER
#define PARSER

#include <string>
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
    string *title;
};

struct json_table_node
{
    char type;
    Pointer value;
};

/* parse ints, floats */
json_table_node *get_number();
/* parse strings */
string get_string();
/* parse pairs ex. { "A" : a, "B" : b } */
j_pair *json_pair();
List create_jList_of_pairs();
/* parse tables ex. [a, b, c] */
json_table_node *json_table();
List create_jTable();
/* parse the file and insert all specs into list that is returned */
List parse(string filename);

#endif