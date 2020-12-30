#ifndef PARSER
#define PARSER

#include <string>
#include "common_types.h"
#include "List.h"
#include "hashtable.h"


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
string get_string(bool flag);
/* parse a pair node ex. { >>"A" : a<<, "B" : b } */
j_pair *json_pair();
/* parse pairs ex. { "A" : a, "B" : b } */
List create_jList_of_pairs();
/* parse a table node ex. [a, >>b<<, c] */
json_table_node *json_table();
/* parse tables ex. [a, b, c] */
List create_jTable();
/* parse the file and insert all specs into list that is returned
   depending on whether any of the hashtables are given, tfs and the vocabulary are formed */
List parse(string filename, HashTable returned_tf=NULL, HashTable receved_idf=NULL);
/* print the content of a pair
   to print a whole json file, you simply print the outer pair and the content is also printed */
void print_json(List l);
#endif