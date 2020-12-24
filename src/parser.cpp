#include <ctype.h>

#include "parser.h"
#include "compare_funcs.h"

List l = NULL;
string file_con;
int ch = 0;
HashTable tf_ht = NULL;
HashTable idf_ht = NULL;

/* j_pairs list */
List json_con;

void del_j_pair(Pointer node)
{
    if (((j_pair *)node)->type == INTEGER)
    {
        delete ((j_pair *)node)->title;
        delete (int *)(((j_pair *)node)->value);
        delete (j_pair *)node;
    }
    else if (((j_pair *)node)->type == FLOAT)
    {
        delete ((j_pair *)node)->title;
        delete (float *)(((j_pair *)node)->value);
        delete (j_pair *)node;
    }
    else if (((j_pair *)node)->type == STRING)
    {
        delete ((j_pair *)node)->title;
        delete (string *)(((j_pair *)node)->value);
        delete (j_pair *)node;
    }
    else if (((j_pair *)node)->type == PAIRS)
    {
        delete ((j_pair *)node)->title;
        delete (j_pair *)(((j_pair *)node)->value);
        delete (j_pair *)node;
    }
    else if (((j_pair *)node)->type == TABLE)
    {
        delete ((j_pair *)node)->title;
        delete (List)(((j_pair *)node)->value);
        delete (j_pair *)node;
    }
}

void del_json_table_node(Pointer node)
{
    if (((json_table_node *)node)->type == INTEGER)
    {
        delete (int *)(((json_table_node *)node)->value);
        delete (json_table_node *)node;
    }
    else if (((json_table_node *)node)->type == FLOAT)
    {
        delete (float *)(((json_table_node *)node)->value);
        delete (json_table_node *)node;
    }
    else if (((json_table_node *)node)->type == STRING)
    {
        delete (string *)(((json_table_node *)node)->value);
        delete (json_table_node *)node;
    }
    else if (((json_table_node *)node)->type == PAIRS)
    {
        delete (int *)(((j_pair *)node)->value);
        delete (json_table_node *)node;
    }
    else if (((json_table_node *)node)->type == TABLE)
    {
        delete (json_table_node *)(((json_table_node *)node)->value);
        delete (json_table_node *)node;
    }
}

json_table_node *get_number()
{
    json_table_node *new_node = new json_table_node;
    int begin_of_number = ch;
    int end_of_number;
    int dots_counter = 0;
    for (; (file_con[ch] >= '0' && file_con[ch] <= '9') || file_con[ch] == '.'; ch++)
    {
        if (file_con[ch] == '.')
            dots_counter++;
    }

    if (dots_counter > 1 || (file_con[ch] != ' ' && file_con[ch] != '\t' && file_con[ch] != '\n' && file_con[ch] != ',' && file_con[ch] != ']'))
    {
        delete new_node;
        printf("get_number error\n");
        return NULL;
    }

    end_of_number = ch;
    string number = file_con.substr(begin_of_number, end_of_number - begin_of_number);

    if (dots_counter == 0)
    {
        new_node->type = INTEGER;
        new_node->value = new int;
        *(int *)(new_node->value) = stoi(number);
    }
    else
    {
        new_node->type = FLOAT;
        new_node->value = new float;
        *(float *)(new_node->value) = stof(number);
    }
    return new_node;
}

int end_of_string()
{
    for (; file_con[ch] != '\"'; ch++)
    {
        if (file_con[ch] == '.' || file_con[ch] == ',' || file_con[ch] == ';' || file_con[ch] == '?' || file_con[ch] == '\'' || file_con[ch] == '(' || file_con[ch] == ')')
            file_con[ch] = ' ';
        else if (file_con[ch] >= 'A' && file_con[ch] <= 'Z')
            file_con[ch] = file_con[ch] - ('A' - 'a');
        if (file_con[ch] == '\n')
            return 1;
    }
    int i = ch - 1;
    int counter = 0;
    while (file_con[i] == '\\')
    {
        counter++;
        i--;
    }
    if (counter % 2 == 0)
        return 0;
    ch++;
    return end_of_string();
}

/**
     * * pseudocode
     * 
     * find_word
     * if(word exists in tf_ht)
     *      tf_ht word counter++;
     * else
     *      tf_ht.insert(word)
     *      if(word exists in idf_ht)
     *          idf_ht word counter++
     *      else
     *          idf_ht.insert(word)
     **/
void tf_ifd_hts_insert(int begin, int end)
{
    int begin_of_word = begin;
    int end_of_word;
    string *word;
    int *counter = NULL;
    for (int i = begin; i <= end; i++)
    {
        if (isdigit(file_con[begin_of_word]))
        {
            if (file_con[i] == ' ')
                begin_of_word = i + 1;
        }

        else if (file_con[i] == ' ' || file_con[i] == '\"')
        {
            end_of_word = i;
            if (end_of_word - begin_of_word < 3)
            {
                begin_of_word = i + 1;
                continue;
            }
            word = new string(file_con.substr(begin_of_word, end_of_word - begin_of_word));
            counter = (int *)tf_ht->search(word, cmp_hashtable_search);
            if (counter)
                (*counter)++;
            else
            {
                counter = new int(1);
                HashTable_Node ht_n = new hashtable_node;
                ht_n->key = word;
                ht_n->value = counter;
                tf_ht->insert(ht_n);
                    counter = (int *)idf_ht->search(word, cmp_hashtable_search);
                    if (counter)
                        counter[1]++;
                    else
                    {
                        counter = new int[2];
                        counter[0] = idf_ht->ht_size();
                        counter[1] = 1;
                        
                        ht_n = new hashtable_node;
                        ht_n->key = word;
                        ht_n->value = counter;
                        idf_ht->insert(ht_n);
                    }
                
            }
            begin_of_word = i + 1;
        }
    }
}

string get_string(bool flag)
{
    int begin = ch, end;

    if (end_of_string())
    {
        printf("get_string error\n");
        return "ERROR_PARSING_STRING";
    }

    end = ch;
    ch++;

    if (flag && tf_ht && idf_ht)
        tf_ifd_hts_insert(begin, end);

    return file_con.substr(begin, end - begin);
}

j_pair *json_pair()
{
    j_pair *new_pair;

    ch++;
    new_pair = new j_pair;
    new_pair->type = PAIRS;
    new_pair->title = new string(get_string(0));
    if (new_pair->title->compare("ERROR_PARSING_STRING") == 0)
    {
        delete new_pair;
        return NULL;
    }

    for (; file_con[ch] != ':'; ch++)
    {
        if (file_con[ch] != ' ' && file_con[ch] != '\t' && file_con[ch] != '\n')
        {
            delete new_pair;
            printf("json_pair error1\n");
            return NULL;
        }
    }
    ch++;
    for (; !(file_con[ch] >= '0' && file_con[ch] <= '9') && file_con[ch] != '\"' && file_con[ch] != '[' && file_con[ch] != '{'; ch++)
    {
        if (file_con[ch] != ' ' && file_con[ch] != '\t' && file_con[ch] != '\n')
        {
            delete new_pair;
            printf("json_pair error2\n");
            return NULL;
        }
    }

    /* int and floats */
    if (file_con[ch] >= '0' && file_con[ch] <= '9')
    {
        json_table_node *tNode = get_number();
        if (tNode == NULL)
        {
            delete new_pair;
            return NULL;
        }
        new_pair->type = tNode->type;
        new_pair->value = tNode->value;
    }
    /* string */
    else if (file_con[ch] == '\"')
    {
        new_pair->type = STRING;
        ch++;
        new_pair->value = new string(get_string(1));
        if ((*(string *)(new_pair->value)).compare("ERROR_PARSING_STRING") == 0)
        {
            delete (string *)(new_pair->value);
            delete new_pair;
            return NULL;
        }
    }
    /* pairs */
    else if (file_con[ch] == '{')
    {
        new_pair->type = PAIRS;
        new_pair->value = create_jList_of_pairs();
        if (new_pair->value == NULL)
        {
            delete new_pair;
            return NULL;
        }
    }
    /* table */
    else if (file_con[ch] == '[')
    {
        new_pair->type = TABLE;
        new_pair->value = create_jTable();
        if (new_pair->value == NULL)
        {
            delete new_pair;
            return NULL;
        }
    }

    return new_pair;
}

List create_jList_of_pairs()
{
    List pList = new list(del_j_pair);
    while (file_con[ch] != '}')
    {
        ch++;
        for (; file_con[ch] != '"' && file_con[ch] != '}'; ch++)
        {
            if (file_con[ch] != ' ' && file_con[ch] != '\t' && file_con[ch] != '\n')
            {
                delete pList;
                printf("create_jList_of_pairs error1\n");
                return NULL;
            }
        }
        if (file_con[ch] == '}')
            break;
        j_pair *temp = json_pair();
        if (temp == NULL)
        {
            delete pList;
            return NULL;
        }
        pList->list_insert_next(pList->list_last(), temp);
        for (; file_con[ch] != ',' && file_con[ch] != '}'; ch++)
        {
            if (file_con[ch] != ' ' && file_con[ch] != '\t' && file_con[ch] != '\n')
            {
                delete pList;
                printf("create_jList_of_pairs error2\n");
                return NULL;
            }
        }
    }
    ch++;
    return pList;
}

json_table_node *json_table()
{
    json_table_node *new_node = new json_table_node;

    if (file_con[ch] >= '0' && file_con[ch] <= '9')
    {
        new_node = get_number();
        if (new_node == NULL)
        {
            delete new_node;
            return NULL;
        }
    }
    /* string */
    else if (file_con[ch] == '\"')
    {
        new_node->type = STRING;
        ch++;
        new_node->value = new string(get_string(1));
        if ((*(string *)new_node->value).compare("ERROR_PARSING_STRING") == 0)
        {
            delete (string *)(new_node->value);
            delete new_node;
            return NULL;
        }
    }
    /* pairs */
    else if (file_con[ch] == '{')
    {
        new_node->type = PAIRS;
        new_node->value = create_jList_of_pairs();
        if (new_node->value == NULL)
        {
            delete new_node;
            return NULL;
        }
    }
    /* table */
    else if (file_con[ch] == '[')
    {
        new_node->type = TABLE;
        new_node->value = create_jTable();
        if (new_node->value == NULL)
        {
            delete new_node;
            return NULL;
        }
    }

    return new_node;
}

List create_jTable()
{
    List tList = new list(del_json_table_node);
    while (file_con[ch] != ']')
    {
        ch++;
        for (; !(file_con[ch] >= '0' && file_con[ch] <= '9') && file_con[ch] != '\"' && file_con[ch] != '[' && file_con[ch] != '{' && file_con[ch] != ']'; ch++)
        {
            if (file_con[ch] != ' ' && file_con[ch] != '\t' && file_con[ch] != '\n')
            {
                delete tList;
                printf("create_jTable error1\n");
                return NULL;
            }
        }
        if (file_con[ch] == ']')
            break;
        json_table_node *temp = json_table();
        if (temp == NULL)
        {
            delete tList;
            return NULL;
        }
        tList->list_insert_next(tList->list_last(), temp);
        for (; file_con[ch] != ',' && file_con[ch] != ']'; ch++)
        {
            if (file_con[ch] != ' ' && file_con[ch] != '\t' && file_con[ch] != '\n')
            {
                delete tList;
                printf("create_jTable error2\n");
                return NULL;
            }
        }
    }
    ch++;
    return tList;
}

List parse(string filename, HashTable returned_tf, HashTable receved_idf)
{
    FILE *stream = NULL;
    stream = fopen(filename.c_str(), "r");
    char *buffer = NULL;
    long length;
    fseek(stream, 0, SEEK_END);
    length = ftell(stream);
    fseek(stream, 0, SEEK_SET);
    buffer = (char *)malloc(length);
    if (buffer)
    {
        fread(buffer, 1, length, stream);
    }

    idf_ht = receved_idf;
    tf_ht = returned_tf;

    file_con = buffer;
    ch = 0;
    json_con = NULL;
    json_con = create_jList_of_pairs();
    free(buffer);
    fclose(stream);
    return json_con;
}

/* ----------PRINTS----------- */

void print_table(List l);

void print_pair(List l)
{
    j_pair *jpair;
    ListNode lnode = l->list_first();
    while (lnode != NULL)
    {
        jpair = (j_pair *)lnode->value;
        printf("%s: ", (jpair->title->c_str()));
        switch (jpair->type)
        {
        case PAIRS:
            print_pair((List)(jpair->value));
            break;
        case TABLE:
            print_table((List)(jpair->value));
            break;
        case STRING:
            printf("%s\n", (*(string *)(jpair->value)).c_str());
            break;
        case INTEGER:
            printf("%i\n", (*(int *)(jpair->value)));
            break;
        case FLOAT:
            printf("%f\n", (*(float *)(jpair->value)));
            break;
        default:
            printf("\n");
            break;
        }
        lnode = l->list_next(lnode);
    }
}

void print_table(List l)
{
    json_table_node *table;
    ListNode lnode = l->list_first();
    while (lnode != NULL)
    {
        table = (json_table_node *)l->list_node_value(lnode);
        switch (table->type)
        {
        case PAIRS:
            print_pair((List)(table->value));
            break;
        case TABLE:
            print_table((List)(table->value));
            break;
        case STRING:
            printf("%s\n", (*(string *)(table->value)).c_str());
            break;
        case INTEGER:
            printf("%i\n", (*(int *)(table->value)));
            break;
        case FLOAT:
            printf("%f\n", (*(float *)(table->value)));
            break;
        default:
            printf("\n");
            break;
        }

        lnode = l->list_next(lnode);
    }
}
