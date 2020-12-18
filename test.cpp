#include <stdio.h>
#include <string>

using namespace std;

int hashfunction_address(void *value)
{
    long int a = (long int)&value;
    return a % 10;
}

// a: mem_address      b: hash_table_node
int cmp_hashtable_search_address(void *a, void *b)
{
    return (a == b) ? 0 : 1;
}

int main()
{
    string str;
    str = "sdf.E..G.seESD'gsS;;EGSEge,,sge,gegSEGSEGAWH'Hsee'.tsESg;tsETSE,gS.Etseg.";
    int i;
    for (i = 0; i < str.size(); i++)
    {
        if (str[i] == '.'||str[i] == ','||str[i] == ';'||str[i] == '\''||str[i] == '?')
            // str[i] = ' ';                                /* replace with ' ' */
        {    str=str.substr(0,i)+str.substr(i+1); i--; }    /* remove comletely */
        else if (str[i]>='A'&&str[i]<='Z')
            str[i]=str[i]-('A'-'a');
        printf("%s ~~~~ %d   %c\n", str.c_str(), i, str[i]);
    }
}