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
        /*
        ! ERROR 
        */
        printf("error1\n"); 
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
            /* 
            !ERROR
            */
            printf("error2\n");
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

    for (; file_con[ch] != ':'; ch++)
    {
        if (file_con[ch] != ' ' && file_con[ch] != '\t' && file_con[ch] != '\n')
        {
            /* 
            ! ERROR
            */
            printf("error3\n");
        }
    }
    ch++;
    for (; !(file_con[ch] >= '0' && file_con[ch] <= '9') && file_con[ch] != '\"' && file_con[ch] != '[' && file_con[ch] != '{'; ch++)
    {
        if (file_con[ch] != ' ' && file_con[ch] != '\t' && file_con[ch] != '\n')
        {
            /* 
            ! ERROR
            */
            printf("error4\n");
        }
    }

    /* int and floats */
    if (file_con[ch] >= '0' && file_con[ch] <= '9')
    {
        json_table_node *tNode = get_number();
        new_pair->type = tNode->type;
        new_pair->value = tNode->value;
    }
    /* string */
    else if (file_con[ch] == '\"')
    {
        new_pair->type = STRING;
        ch++;
        new_pair->value = new string(get_string());
    }
    /* pairs */
    else if (file_con[ch] == '{')
    {
        new_pair->type = PAIRS;
        new_pair->value = create_jList_of_pairs();
    }
    /* table */
    else if (file_con[ch] == '[')
    {
        new_pair->type = TABLE;
        new_pair->value = create_jTable();
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
                /* 
                ! ERROR
                */
                printf("error5\n");
            }
        }
        if (file_con[ch] == '}')
            break;
        pList->list_insert_next(pList->list_last(), json_pair());
        for (; file_con[ch] != ',' && file_con[ch] != '}'; ch++)
        {
            if (file_con[ch] != ' ' && file_con[ch] != '\t' && file_con[ch] != '\n')
            {
                /* 
                ! ERROR
                */
                printf("error6\n");
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
    }
    /* string */
    else if (file_con[ch] == '\"')
    {
        new_node->type = STRING;
        ch++;
        new_node->value = new string(get_string());
    }
    /* pairs */
    else if (file_con[ch] == '{')
    {
        new_node->type = PAIRS;
        new_node->value = create_jList_of_pairs();
    }
    /* table */
    else if (file_con[ch] == '[')
    {
        new_node->type = TABLE;
        new_node->value = create_jTable();
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
                /* 
                ! ERROR
                */
                printf("error7\n");
            }
        }
        if (file_con[ch] == ']')
            break;
        tList->list_insert_next(tList->list_last(), json_table());
        for (; file_con[ch] != ',' && file_con[ch] != ']'; ch++)
        {
            if (file_con[ch] != ' ' && file_con[ch] != '\t' && file_con[ch] != '\n')
            {
                /* 
                ! ERROR
                */
                printf("error8\n");
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
    return json_con;
}
