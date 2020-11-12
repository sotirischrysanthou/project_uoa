#include "parser.h"

List l = NULL;
string file_con;
int ch = 0;
int pos;

/* j_pairs list */
List json_con;

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

string get_string()
{
    int begin = ch, end;

    for (; !(file_con[ch] == '\"' && (file_con[ch - 1] != '\\' || (file_con[ch - 1] == '\\' && file_con[ch - 2] == '\\'))); ch++)
    {
        if (file_con[ch] == '\n')
        {
            printf("get_string error\n");
            return "ERROR_PARSING_STRING";
        }
    }
    end = ch;
    ch++;
    return file_con.substr(begin, end - begin);
}

j_pair *json_pair()
{
    j_pair *new_pair;

    ch++;
    new_pair = new j_pair;
    new_pair->title = get_string();
    if (new_pair->title.compare("ERROR_PARSING_STRING")==0)
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
        if (tNode==NULL)
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
        new_pair->value = new string(get_string());
        if ((*(string*)(new_pair->value)).compare("ERROR_PARSING_STRING")==0)
        {
            delete new_pair->value;
            delete new_pair;
            return NULL;
        }
    }
    /* pairs */
    else if (file_con[ch] == '{')
    {
        new_pair->type = PAIRS;
        new_pair->value = create_jList_of_pairs();
        if(new_pair->value==NULL)
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
        if (new_pair->value==NULL)
        {
            delete new_pair;
            return NULL;
        }
    }

    return new_pair;
}

List create_jList_of_pairs()
{
    List pList = new list(NULL);
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
        if(temp==NULL)
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
    json_table_node *new_node=new json_table_node;

    if (file_con[ch] >= '0' && file_con[ch] <= '9')
    {
        new_node = get_number();
        if(new_node==NULL)
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
        new_node->value = new string(get_string());
        if ((*(string*)new_node->value).compare("ERROR_PARSING_STRING")==0)
        {
            delete new_node->value;
            delete new_node;
            return NULL;
        }
    }
    /* pairs */
    else if (file_con[ch] == '{')
    {
        new_node->type = PAIRS;
        new_node->value = create_jList_of_pairs();
        if(new_node->value==NULL)
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
        if (new_node->value==NULL)
        {
            delete new_node;
            return NULL;
        }
    }

    return new_node;
}

List create_jTable()
{
    List tList = new list(NULL);
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
        json_table_node *temp=json_table();
        if(temp==NULL)
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

List parse(char *filename)
{
    l = new list(NULL);
    FILE *stream = fopen(filename, "r");
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
    file_con = buffer;
    json_con = create_jList_of_pairs();
    if(json_con==NULL)
    {
        delete l;
        return NULL;
    }
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
        printf("%s: ", (jpair->title.c_str()));
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
        table=(json_table_node*)l->list_node_value(lnode);
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

// int main()
// {
//     List json;
//     json = parse("j.json");
//     if(json==NULL)
//     {
//         printf("file dropped\n");
//         return 0;
//     }
//     printf("size of list: %d\n", json->list_size());
//     print_pair(json);
//     return 0;
// }