#include <iostream>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "item.h"
#include "parser.h"
#include "compare_funcs.h"
#include "hashtable.h"
#include "compare_funcs.h"

/* Create a connection between 2 items, using their similar lists */
void similar_items(item A,item B);
/* print a list of every item in a similar list */
void print_list(List list_to_print);
/* print all items common with the given list_to_visit (basically prints a clique) */
void print_commons(HashTable visited_lists, List list_to_visit, FILE *output_file);
/* read all folders and files and input all items and specs into a hashtable */
int read_all_folders(string dir_name, HashTable ht, HashTable tf_idf);
/* read a csv and create all similar connections between the items */
void read_csv(string filename, HashTable ht);
/* print all similar pairs (project1 output) */
void print_all(HashTable ht,FILE *output_file=stdout);
/* set the values for BoW (0) or Tf-Idf (1) */
void set_Bow_or_TfIdf(HashTable ht, HashTable tfidf, int Item_count, bool flag);
/* destroy function */
void del_main_ht(Pointer value);
/* count and return the number of lines in a file */
int lines_counter(const char *filename);
/* training function using "filename" as the training csv. repetitions have 1 as default */
double *train(string filename, HashTable ht, HashTable idf, int reps=1);
/* testing function */
void test(string filename, HashTable ht, double *W, int idf_size, bool validation=0);

